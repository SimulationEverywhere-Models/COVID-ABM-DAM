//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "Health_Status_Filter.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct inp_in : public cadmium::in_port<health_status>{};


/***** Define output ports for coupled model *****/
struct outp_out : public cadmium::out_port<bool>{};


/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Int : public iestream_input<string,T> {
    public:
        InputReader_Int () = default;
        InputReader_Int (const char* file_path) : iestream_input<string,T>(file_path) {}
};
template<typename T>
class InputReader_Int1 : public iestream_input<string,T> {
    public:
        InputReader_Int1 () = default;
        InputReader_Int1 (const char* file_path) : iestream_input<string,T>(file_path) {}
};

int main(){


    /****** Input Reader atomic models instantiation *******************/
    const char * i_input_data_control = "HealthStatusFilter_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_con = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader_con" , move(i_input_data_control));
	

    /****** HealthStatusFilter atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> HealthStatusFilter1 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME>("HealthStatusFilter1");

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(IsSick_out)};
    dynamic::modeling::Models submodels_TOP = {input_reader_con, HealthStatusFilter1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<HealthStatusFilter_defs::IsSick,IsSick_out>("HealthStatusFilter1"),

    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<string>::out,HealthStatusFilter_defs::HealthStatus>("input_reader_con","HealthStatusFilter1"),

    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    static ofstream out_messages(HealthStatusFilter_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("HealthStatusFilter_test_output_state.txt");
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