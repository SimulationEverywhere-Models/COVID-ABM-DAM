//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

#include "../data/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/Health_Status_Filter.hpp"

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
struct inp_in : public cadmium::in_port<health_status>{};


/***** Define output ports for coupled model *****/
struct outp_out : public cadmium::out_port<bool>{};


/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_health_status : public iestream_input<health_status,T> {
    public:
        InputReader_health_status () = default;
        InputReader_health_status (const char* file_path) : iestream_input<health_status,T> (file_path) {}
};


int main(){


    long student_id = 4;

    /****** Input Reader atomic models instantiation *******************/
    const char * i_input_data = "../input_data/Health_Status_filter_input.txt";
    shared_ptr<dynamic::modeling::model> input_reader;
    input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_health_status, TIME, const char* >("input_reader" , move(i_input_data));
	

    /****** HealthStatusFilter atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> HealthStatusFilter1 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long>("HealthStatusFilter", std::move(student_id));

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_out)};
    dynamic::modeling::Models submodels_TOP = {input_reader, HealthStatusFilter1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<Health_Status_Filter_Ports::health_out,outp_out>("HealthStatusFilter")

    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<health_status>::out,Health_Status_Filter_Ports::health_in>("input_reader","HealthStatusFilter")

    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/HealthStatusFilter_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/HealthStatusFilter_test_output_state.txt");
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
    r.run_until_passivate();
    return 0;
};