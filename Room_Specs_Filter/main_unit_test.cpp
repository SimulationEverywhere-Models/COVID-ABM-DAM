//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header 
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include "../atomics/subnet.hpp"
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;
using TIME = NDTime;

/***** (1) *****/
//define input and output ports for couples model
//struct top_out: public out_port<room_in>{};
struct in_PersonID : public cadmium::in_port<long>{};
struct in_people_in_room : public cadmium::in_port<int>{};
struct in_room_size : public cadmium::in_port<float>{};
struct in_viral_particles : public cadmium::in_port<float>{};
struct in_RoomID : public cadmium::in_port<long>{};

struct out_PersonID : public cadmium::in_port<long>{};
struct out_people_in_room : public cadmium::in_port<int>{};
struct out_room_size : public cadmium::in_port<float>{};
struct out_viral_particles : public cadmium::in_port<float>{};
struct out_RoomID : public cadmium::in_port<long>{};

/***** (2) *****/
//Input Reader atomic model declaration
template<typename T>
class InputReader_room_in : public iestream_input<room_in, T> {
 public:
 InputReader_room_in () = default;
 InputReader_room_in (const char* file_path) : iestream_input<room_in,T> (file_path) {}
};

/***** (3) *****/
int main(){
 //Input Reader atomic model instantiation
 const char * i_input_data = "../input_data/subnet_input_test.txt";
 shared_ptr<dynamic::modeling::model> input_reader;
 input_reader = dynamic::translate::make_dynamic_atomic_model
 <InputReader_room_in, TIME, const char*>("input_reader", move(i_input_data));

 /***** (4) *****/
 //Subnet atomic model instantiation
 shared_ptr<dynamic::modeling::model> subnet1;
 subnet1 = dynamic::translate::make_dynamic_atomic_model<Subnet, TIME>("subnet1");

 /***** (5) *****/
 //TOP MODEL
 dynamic::modeling::Ports iports_TOP;
 iports_TOP = {};
 dynamic::modeling::Ports oports_TOP;
 oports_TOP = {typeid(top_out)};
 dynamic::modeling::Models submodels_TOP;
 submodels_TOP = {input_reader, subnet1};
 dynamic::modeling::EICs eics_TOP;
 eics_TOP = {}; 
 dynamic::modeling::EOCs eocs_TOP;
 eocs_TOP = {
 dynamic::translate::make_EOC<Subnet_defs::out,top_out>("subnet1")
 };
 dynamic::modeling::ICs ics_TOP;
 ics_TOP = {
 dynamic::translate::make_IC<iestream_input_defs<room_out>::out,Subnet_defs::in>(
 "input_reader","subnet1")
 };
 shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
 TOP = make_shared<dynamic::modeling::coupled<TIME>>(
 "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
 );

 /***** (6) *****/
 //Loggers
 static ofstream out_messages("../simulation_results/subnet_test_output_messages.txt");
 struct oss_sink_messages{
 static ostream& sink(){
 return out_messages;
 }
 };
 static ofstream out_state("../simulation_results/subnet_test_output_state.txt");
 struct oss_sink_state{
 static ostream& sink(){
 return out_state;
 }};

 using state = logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>,oss_sink_state>;
 using log_messages = logger::logger<logger::logger_messages,dynamic::logger::formatter<TIME>, oss_sink_messages>;
 using global_time_mes = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_messages>;
 using global_time_sta = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_state>;
 using logger_top = logger::multilogger<state, log_messages, global_time_mes,global_time_sta>;

 /***** (7) *****/
 //Runner call
 dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
 r.run_until(NDTime("00:05:0:000"));
 return 0;
} 