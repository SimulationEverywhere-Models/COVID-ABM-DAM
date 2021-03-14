//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

#include "data/tinyXML/tinyxml.h"
#include "data/tinyXML/tinystr.h"

#include "data/tinyXML/tinyxml.cpp"
#include "data/tinyXML/tinyxmlerror.cpp"
#include "data/tinyXML/tinyxmlparser.cpp"
#include "data/tinyXML/tinystr.cpp"

//Messages structures
#include "data/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "atomics/Health_Status_Filter.hpp"
#include "atomics/Room_Specs_Filter.hpp"
#include "atomics/BehaviourRules.hpp"
#include "atomics/room.hpp"

//C++ headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct inp_in1 : public cadmium::in_port<health_status>{}; //change variable names 
struct inp_in2 : public cadmium::in_port<room_specs>{};
struct inp_SIM : public cadmium::in_port<health_status>{};
// add another input for the generator

/***** Define output ports for coupled model *****/
struct outp_out : public cadmium::out_port<person_node>{};


/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_health : public iestream_input<health_status, T> {
    public:
        InputReader_health() = default;
        InputReader_health(const char* file_path) : iestream_input<health_status,T> (file_path) {}
};

template<typename T>
class InputReader_room : public iestream_input<room_specs, T> {
    public:
        InputReader_room() = default;
        InputReader_room(const char* file_path) : iestream_input<room_specs,T> (file_path) {}
};


int main(){
    
    DecisionMakerBehaviour person1;
    DecisionMakerBehaviour person2;
    DecisionMakerBehaviour person3;
    DecisionMakerBehaviour person4;
    DecisionMakerBehaviour person5;

    person1.load("../data/Student1.xml");
    person2.load("../data/XML2.xml");
    person3.load("../data/XML3.xml");
    person4.load("../data/XML4.xml");
    person5.load("../data/XML5.xml");

    /****** Input Reader atomic model instantiation *******************/
    const char * i_input_data_health = "../input_data/health_generator.txt";
    shared_ptr<dynamic::modeling::model> input_reader_health = dynamic::translate::make_dynamic_atomic_model<InputReader_health, TIME, const char* >("input_reader_health" , move(i_input_data_health));
    
    /****** Health Status atomic model instantiation *****************/
    shared_ptr<dynamic::modeling::model> health_status1 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long&>("health_status1", person1.iD); //person1.id 
    shared_ptr<dynamic::modeling::model> health_status2 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long&>("health_status2", person2.iD);
    shared_ptr<dynamic::modeling::model> health_status3 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long&>("health_status3", person3.iD);
    shared_ptr<dynamic::modeling::model> health_status4 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long&>("health_status4", person4.iD);
    shared_ptr<dynamic::modeling::model> health_status5 = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long&>("health_status5", person5.iD);

    /****** Room Specs atomic model instantiation *****************/
    shared_ptr<dynamic::modeling::model> room_specs1 = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long&>("room_specs1", person1.iD);
    shared_ptr<dynamic::modeling::model> room_specs2 = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long&>("room_specs2", person2.iD);
    shared_ptr<dynamic::modeling::model> room_specs3 = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long&>("room_specs3", person3.iD);
    shared_ptr<dynamic::modeling::model> room_specs4 = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long&>("room_specs4", person4.iD);
    shared_ptr<dynamic::modeling::model> room_specs5 = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long&>("room_specs5", person5.iD);

    /****** Behaviour Rules atomic model instantiation *****************/
    shared_ptr<dynamic::modeling::model> behaviour_rules1 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>("behaviour_rules1", "../data/Student1.xml", TIME("01:30:00:00"), TIME("01:50:00:00"));
    shared_ptr<dynamic::modeling::model> behaviour_rules2 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>("behaviour_rules2", "../data/XML2.xml", TIME("01:00:00:00"), TIME("00:50:00:00"));
    shared_ptr<dynamic::modeling::model> behaviour_rules3 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>("behaviour_rules3", "../data/XML3.xml", TIME("01:40:00:00"), TIME("02:50:00:00"));
    shared_ptr<dynamic::modeling::model> behaviour_rules4 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>("behaviour_rules4", "../data/XML4.xml", TIME("02:20:00:00"), TIME("01:00:00:00"));
    shared_ptr<dynamic::modeling::model> behaviour_rules5 = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>("behaviour_rules5", "../data/XML5.xml", TIME("01:30:00:00"), TIME("01:30:00:00"));

    /****** Room Model atomic model instantiation **************/
    shared_ptr<dynamic::modeling::model> room_model = dynamic::translate::make_dynamic_atomic_model<Room_Model, TIME, string, long>("room_model", "4th_Mackenzie", 100);

    /*********** NODE 1 COUPLED MODEL *************/
    dynamic::modeling::Ports iports_node1 = {typeid(inp_in1),typeid(inp_in2)};
    dynamic::modeling::Ports oports_node1 = {typeid(outp_out)};
    dynamic::modeling::Models submodels_node1 = {health_status1, room_specs1, behaviour_rules1}; 
    dynamic::modeling::EICs eics_node1 = {
        dynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>("health_status1"), 
        dynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>("room_specs1")
    };
    dynamic::modeling::EOCs eocs_node1 = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("behaviour_rules1")
    };
    dynamic::modeling::ICs ics_node1 = {
        dynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>("health_status1","behaviour_rules1"), 
        dynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>("room_specs1","behaviour_rules1"),
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> NODE1;
    NODE1 = make_shared<dynamic::modeling::coupled<TIME>>(
        "NODE1", submodels_node1, iports_node1, oports_node1, eics_node1, eocs_node1, ics_node1
    );

    /*********** NODE 2 COUPLED MODEL *************/
    dynamic::modeling::Ports iports_node2 = {typeid(inp_in1),typeid(inp_in2)};
    dynamic::modeling::Ports oports_node2 = {typeid(outp_out)};
    dynamic::modeling::Models submodels_node2 = {health_status2, room_specs2, behaviour_rules2}; 
    dynamic::modeling::EICs eics_node2 = {
        dynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>("health_status2"), 
        dynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>("room_specs2")
    };
    dynamic::modeling::EOCs eocs_node2 = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("behaviour_rules2")
    };
    dynamic::modeling::ICs ics_node2 = {
        dynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>("health_status2","behaviour_rules2"), 
        dynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>("room_specs2","behaviour_rules2"),
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> NODE2;
    NODE1 = make_shared<dynamic::modeling::coupled<TIME>>(
        "NODE2", submodels_node2, iports_node2, oports_node2, eics_node2, eocs_node2, ics_node2
    );

    /*********** NODE 3 COUPLED MODEL *************/
    dynamic::modeling::Ports iports_node3 = {typeid(inp_in1),typeid(inp_in2)};
    dynamic::modeling::Ports oports_node3 = {typeid(outp_out)};
    dynamic::modeling::Models submodels_node3 = {health_status3, room_specs3, behaviour_rules3}; 
    dynamic::modeling::EICs eics_node3 = {
        dynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>("health_status3"), 
        dynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>("room_specs3")
    };
    dynamic::modeling::EOCs eocs_node3 = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("behaviour_rules3")
    };
    dynamic::modeling::ICs ics_node3 = {
        dynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>("health_status3","behaviour_rules3"), 
        dynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>("room_specs3","behaviour_rules3"),
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> NODE3;
    NODE3 = make_shared<dynamic::modeling::coupled<TIME>>(
        "NODE3", submodels_node3, iports_node3, oports_node3, eics_node3, eocs_node3, ics_node3
    );

    /*********** NODE 4 COUPLED MODEL *************/
    dynamic::modeling::Ports iports_node4 = {typeid(inp_in1),typeid(inp_in2)};
    dynamic::modeling::Ports oports_node4 = {typeid(outp_out)};
    dynamic::modeling::Models submodels_node4 = {health_status4, room_specs4, behaviour_rules4};  
    dynamic::modeling::EICs eics_node4 = {
        dynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>("health_status4"), 
        dynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>("room_specs4")
    };
    dynamic::modeling::EOCs eocs_node4 = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("behaviour_rules4")
    };
    dynamic::modeling::ICs ics_node4 = {
        dynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>("health_status4","behaviour_rules4"), 
        dynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>("room_specs4","behaviour_rules4"),
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> NODE4;
    NODE4 = make_shared<dynamic::modeling::coupled<TIME>>(
        "NODE4", submodels_node4, iports_node4, oports_node4, eics_node4, eocs_node4, ics_node4
    );

    /*********** NODE 5 COUPLED MODEL *************/
    dynamic::modeling::Ports iports_node5 = {typeid(inp_in1),typeid(inp_in2)};
    dynamic::modeling::Ports oports_node5 = {typeid(outp_out)};
    dynamic::modeling::Models submodels_node5 = {health_status5, room_specs5, behaviour_rules5}; 
    dynamic::modeling::EICs eics_node5 = {
        dynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>("health_status5"), 
        dynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>("room_specs5")
    };
    dynamic::modeling::EOCs eocs_node5 = {
        dynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>("behaviour_rules5")
    };
    dynamic::modeling::ICs ics_node5 = {
        dynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>("health_status5","behaviour_rules5"), 
        dynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>("room_specs5","behaviour_rules5"),
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> NODE5;
    NODE5 = make_shared<dynamic::modeling::coupled<TIME>>(
        "NODE5", submodels_node5, iports_node5, oports_node5, eics_node5, eocs_node5, ics_node5
    );

    /********* SIMULATOR MODEL *********/
    dynamic::modeling::Ports iports_SIM = {typeid(inp_SIM)}; // sim input
    dynamic::modeling::Ports oports_SIM = {typeid(outp_out)};
    dynamic::modeling::Models submodels_SIM = {room_model, NODE1, NODE2, NODE3, NODE4, NODE5};
    dynamic::modeling::EICs eics_SIM = {
        dynamic::translate::make_EIC<inp_SIM, Health_Status_Filter_Ports::health_in>("NODE1"),
        dynamic::translate::make_EIC<inp_SIM, Health_Status_Filter_Ports::health_in>("NODE2"),
        dynamic::translate::make_EIC<inp_SIM, Health_Status_Filter_Ports::health_in>("NODE3"),
        dynamic::translate::make_EIC<inp_SIM, Health_Status_Filter_Ports::health_in>("NODE4"),
        dynamic::translate::make_EIC<inp_SIM, Health_Status_Filter_Ports::health_in>("NODE5")

    }; // input generator coupling 
    dynamic::modeling::EOCs eocs_SIM = {
        dynamic::translate::make_EOC<outp_out,outp_out>("NODE1"),
        dynamic::translate::make_EOC<outp_out,outp_out>("NODE2"),
        dynamic::translate::make_EOC<outp_out,outp_out>("NODE3"),
        dynamic::translate::make_EOC<outp_out,outp_out>("NODE4"),
        dynamic::translate::make_EOC<outp_out,outp_out>("NODE5")
    };
     dynamic::modeling::ICs ics_SIM = {
        dynamic::translate::make_IC<outp_out, Room_Model_Ports::room_model_in>("NODE1","room_model"),
        dynamic::translate::make_IC<Room_Model_Ports::room_model_out, inp_in2>("room_model","NODE1"),

        dynamic::translate::make_IC<outp_out, Room_Model_Ports::room_model_in>("NODE2","room_model"),
        dynamic::translate::make_IC<Room_Model_Ports::room_model_out, inp_in2>("room_model","NODE2"),

        dynamic::translate::make_IC<outp_out, Room_Model_Ports::room_model_in>("NODE3","room_model"),
        dynamic::translate::make_IC<Room_Model_Ports::room_model_out, inp_in2>("room_model","NODE3"),

        dynamic::translate::make_IC<outp_out, Room_Model_Ports::room_model_in>("NODE4","room_model"),
        dynamic::translate::make_IC<Room_Model_Ports::room_model_out, inp_in2>("room_model","NODE4"),

        dynamic::translate::make_IC<outp_out, Room_Model_Ports::room_model_in>("NODE5","room_model"),
        dynamic::translate::make_IC<Room_Model_Ports::room_model_out, inp_in2>("room_model","NODE5")
     };
     shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> SIM;
    SIM = make_shared<dynamic::modeling::coupled<TIME>>(
        "SIM", submodels_SIM, iports_SIM, oports_SIM, eics_SIM, eocs_SIM, ics_SIM
    );

    /*** TOP MODEL***/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_out)};
    dynamic::modeling::Models submodels_TOP = {input_reader_health,SIM};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<outp_out,outp_out>("SIM")
    }; 
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<health_status>::out, inp_SIM>("input_reader_health","SIM") // input from the sim
  
    };
    shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    //Loggers
    static ofstream out_messages("../simulation_results/main_node_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
        return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/main_node_test_output_state.txt");
    struct oss_sink_state{
            static ostream& sink(){
             return out_state;
        }
    };
    using state = logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>,oss_sink_state>;
    using log_messages = logger::logger<logger::logger_messages,dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_state>;
    using logger_top = logger::multilogger<state, log_messages, global_time_mes,global_time_sta>;


    //Runner call
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("20:00:00:000"));
    return 0;

}