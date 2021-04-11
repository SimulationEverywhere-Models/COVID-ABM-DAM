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

#include "../data/tinyXML/tinyxml.h"
#include "../data/tinyXML/tinystr.h"

#include "../data/tinyXML/tinyxml.cpp"
#include "../data/tinyXML/tinyxmlerror.cpp"
#include "../data/tinyXML/tinyxmlparser.cpp"
#include "../data/tinyXML/tinystr.cpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/BehaviourRules.hpp"

//C++ libraries
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct inp_in : public cadmium::in_port<bool>{};
struct inp_in1 : public cadmium::in_port<room_specs>{};

/***** Define output ports for coupled model *****/
struct outp_out : public cadmium::out_port<person_node>{};

/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_health : public iestream_input<bool,T> {
    public:
        InputReader_health () = default;
        InputReader_health(const char* file_path) : iestream_input<bool,T>(file_path) {} 
};

template<typename T>
class InputReader_room : public iestream_input<room_specs, T>{
    public:
        InputReader_room () = default;
        InputReader_room(const char* file_path) : iestream_input<room_specs, T>(file_path){}
};

int main(){

    DecisionMakerBehaviour person;
    person.load("../data/Student1.xml"); // Include constructor path to the XML file with Sean's load method
    //long in_person;

    /****** Input Reader atomic models instantiation *******************/
    const char * i_input_health = "../input_data/BehaviourRules_health_status_input.txt";
    shared_ptr<dynamic::modeling::model> input_reader_health = dynamic::translate::make_dynamic_atomic_model<InputReader_health, TIME, const char* >("input_reader_health" , move(i_input_health));
	const char * i_input_room = "../input_data/BehaviourRules_room_specs_input.txt";
	shared_ptr<dynamic::modeling::model> input_reader_room = dynamic::translate::make_dynamic_atomic_model<InputReader_room, TIME, const char* >("input_reader_room" , move(i_input_room));

    /****** atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> BehaviourRules1 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME > ("BehaviourRules", "../data/Student1.xml", TIME("01:30:00:00"), TIME("02:00:00:00"));

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_out)};
    dynamic::modeling::Models submodels_TOP = {input_reader_health, input_reader_room, BehaviourRules1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("BehaviourRules")

    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<bool>::out,BehaviourRules_Ports::BehaviourRulesInHealth>("input_reader_health","BehaviourRules"),
		dynamic::translate::make_IC<iestream_input_defs<room_specs>::out,BehaviourRules_Ports::BehaviourRulesInRoom>("input_reader_room","BehaviourRules")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/BehaviourRules_test_output_messages.txt"); // adjust file directory here
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/BehaviourRules_test_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("20:00:00:000"));
    return 0;
}
