//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header 
#include <NDTime.hpp>

#include "../data/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/room.hpp"
#include "../atomics/room_entering.hpp"
#include "../atomics/room_leaving.hpp"

//C++ libraries
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;
using TIME = NDTime;

//define input port for coupled model
struct inp_in_entering : public cadmium::in_port<person_node>{};
struct inp_in_leaving : public cadmium::in_port<person_node>{};

//define output port for coupled model
struct outp_out : public cadmium::out_port<room_specs>{};


//Input Reader atomic model declaration
template<typename T>
class InputReader_room_model : public iestream_input<person_node, T> {
    public:
        InputReader_room_model () = default;
        InputReader_room_model (const char* file_path) : iestream_input<person_node,T> (file_path) {}
};


int main(){

   string room_ID = "4th_Mackenzie";

    //Input Reader atomic model instantiation
    const char * i_input_data1 = "../input_data/room_input_entering.txt";
    shared_ptr<dynamic::modeling::model> input_reader1;
    input_reader1 = dynamic::translate::make_dynamic_atomic_model<InputReader_room_model, TIME, const char*>("input_reader1", move(i_input_data1));

     const char * i_input_data2 = "../input_data/room_input_leaving.txt";
    shared_ptr<dynamic::modeling::model> input_reader2;
    input_reader2 = dynamic::translate::make_dynamic_atomic_model<InputReader_room_model, TIME, const char*>("input_reader2", move(i_input_data2));

    //Room atomic model instantiation
    shared_ptr<dynamic::modeling::model> RoomModel1= dynamic::translate::make_dynamic_atomic_model<Room_Model, TIME, string, long>("room_model", move(room_ID), 100);

    /****** Room Entering Filter ***************/
     shared_ptr<dynamic::modeling::model> room_entering1 = dynamic::translate::make_dynamic_atomic_model<room_entering, TIME, string>("room_entering", "4th_Mackenzie");

     /****** Room Leaving Filter ***************/
    shared_ptr<dynamic::modeling::model> room_leaving1 = dynamic::translate::make_dynamic_atomic_model<room_leaving, TIME, string> ("room_leaving", "4th_Mackenzie");


    /************ ROOM FILTER MODEL *****************/
    dynamic::modeling::Ports iports_ROOM = {typeid(inp_in_leaving),typeid(inp_in_entering)};
    dynamic::modeling::Ports oports_ROOM = {typeid(outp_out)};
    dynamic::modeling::Models submodels_ROOM = {RoomModel1, room_entering1, room_leaving1};
    dynamic::modeling::EICs eics_ROOM = {
        dynamic::translate::make_EIC<inp_in_leaving, room_leaving_ports::room_leaving_in>("room_leaving"),
        dynamic::translate::make_EIC<inp_in_entering, room_entering_ports::room_entering_in>("room_entering")
    };
    dynamic::modeling::EOCs eocs_ROOM = {
        dynamic::translate::make_EOC<Room_Model_Ports::room_model_out, outp_out>("room_model")
    };
    dynamic::modeling::ICs ics_ROOM = {
        dynamic::translate::make_IC<room_leaving_ports::room_leaving_out, Room_Model_Ports::room_model_in_leaving>("room_leaving", "room_model"),
        dynamic::translate::make_IC<room_entering_ports::room_entering_out, Room_Model_Ports::room_model_in_entering>("room_entering","room_model")
    };
     shared_ptr<dynamic::modeling::coupled<TIME>> ROOM;
    ROOM = make_shared<dynamic::modeling::coupled<TIME>>(
        "ROOM", submodels_ROOM, iports_ROOM, oports_ROOM, eics_ROOM, eocs_ROOM, ics_ROOM
    );

    //TOP MODEL
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_out)};
    dynamic::modeling::Models submodels_TOP= {input_reader1, input_reader2, ROOM};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP{
        dynamic::translate::make_EOC<outp_out,outp_out>("ROOM")
        
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<person_node>::out,inp_in_leaving>("input_reader2","ROOM"),
        dynamic::translate::make_IC<iestream_input_defs<person_node>::out,inp_in_entering>("input_reader1","ROOM")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
    "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    //Loggers
    static ofstream out_messages("../simulation_results/RoomModel_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
        return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/RoomModel_test_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){
        return out_state;
    }};

    using state = logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>,oss_sink_state>;
    using log_messages = logger::logger<logger::logger_messages,dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_state>;
    using logger_top = logger::multilogger<state, log_messages, global_time_mes,global_time_sta>;


    //Runner call
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until_passivate();
    return 0;
}
