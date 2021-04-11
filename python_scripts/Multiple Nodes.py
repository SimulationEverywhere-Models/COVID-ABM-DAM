# imports

node = input("Enter the number of nodes: ")

with open("/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/main_node.cpp", 'w') as f:
#with open("/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/main_node.cpp",'r') as read_file:
    # size_of_print = 10
    # for line in range(size_of_print):
    #     stuff = read_file.readline()
    #     print(stuff, end="")
    #     f.write(stuff)

    # Cadmium Simulator header
    f.write("//Cadmium Simulator headers\n")
    f.write("#include <cadmium/modeling/ports.hpp>\n")
    f.write("#include <cadmium/modeling/dynamic_model.hpp>\n")
    f.write("#include <cadmium/modeling/dynamic_model_translator.hpp>\n")
    f.write("#include <cadmium/engine/pdevs_dynamic_runner.hpp>\n")
    f.write("#include <cadmium/logger/common_loggers.hpp>\n")

    # Time class header
    f.write("\n//Time class header\n")
    f.write("#include <NDTime.hpp>\n\n")

    # Change directories if needed
    f.write("#include \"data/tinyXML/tinyxml.h\"\n")
    f.write("#include \"data/tinyXML/tinystr.h\"\n\n")

    f.write("#include \"data/tinyXML/tinyxml.cpp\"\n")
    f.write("#include \"data/tinyXML/tinyxmlerror.cpp\"\n")
    f.write("#include \"data/tinyXML/tinyxmlparser.cpp\"\n")
    f.write("#include \"data/tinyXML/tinystr.cpp\"\n\n")

    f.write("//Messages structures\n")
    f.write("#include \"data/message.hpp\"\n\n")

    # //Atomic model headers
    f.write("//Atomic model headers\n")
    f.write("#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs\n")
    f.write("#include \"atomics/Health_Status_Filter.hpp\"\n")
    f.write("#include \"atomics/Room_Specs_Filter.hpp\"\n")
    f.write("#include \"atomics/BehaviourRules.hpp\"\n")
    f.write("#include \"atomics/room_entering.hpp\"\n")
    f.write("#include \"atomics/room_leaving.hpp\"\n")
    f.write("#include \"atomics/room.hpp\"\n")

    # size_of1st_print = 28
    # size_of2nd_print = 59
    # # This next line is important.
    # # You need to select the number of characters in the file from the very first line all the way
    # # to the beginning line of the //C++ headers.
    # # In VisualStudioCode, selected text will show you the number of character/index at the bottom right.
    # # Input that number in the read_file.seek(*insert number*) and subtract by 2
    # read_file.seek(900 - 2)
    # for line in range(size_of1st_print, size_of2nd_print):
    #     stuff = read_file.readline()
    #     print(stuff, end="")
    #     f.write(stuff)

    # C++ headers
    f.write("\n//C++ headers\n")
    f.write("#include <iostream>\n")
    f.write("#include <chrono>\n")
    f.write("#include <algorithm>\n")
    f.write("#include <string>\n")

    f.write("\nusing namespace std;\n")
    f.write("using namespace cadmium;\n")
    f.write("using namespace cadmium::basic_models::pdevs;\n")

    f.write("\nusing TIME = NDTime;\n")

    # /******** Define input port for coupled models ***********/
    f.write("\n/***** Define input port for coupled models *****/\n")
    f.write("struct inp_in1 : public cadmium::in_port<health_status>{}; //change variable names\n")
    f.write("struct inp_in2 : public cadmium::in_port<room_specs>{};\n")
    f.write("struct inp_SIM : public cadmium::in_port<health_status>{};\n")
    f.write("struct inp_mov : public cadmium::in_port<person_node>{};\n")
    f.write("// add another input for the generator\n")

    # /******* Define output ports for coupled model ********/
    f.write("\n/***** Define output ports for coupled model *****/\n")
    f.write("struct outp_out : public cadmium::out_port<person_node>{};\n")
    f.write("struct outp_room : public cadmium::out_port<room_specs>{};\n")

    # /******** Input Reader atomic model declaration **********/
    f.write("\n\n/****** Input Reader atomic model declaration *******************/\n")
    f.write("template<typename T>\n")
    f.write("class InputReader_health : public iestream_input<health_status, T> {\n")
    f.write("\tpublic:\n")
    f.write("\t\tInputReader_health() = default;\n")
    f.write("\t\tInputReader_health(const char* file_path) : iestream_input<health_status,T> (file_path) {}\n")
    f.write("};\n")

    # *************** int main ******************

    f.write("\nint main(){\n\n")

    f.write("\tstring room_ID = \"4th_Mackenzie\";\n\n")

    for i in range(1, int(node) + 1):
        f.write("\tDecisionMakerBehaviour person" + str(i) + ";\n")
    f.write("\n")

    # Change the directories if needed
    for i in range(1, int(node) + 1):
        f.write("\tperson" + str(i) + ".load(\"../data/XML" + str(i) + ".xml\");\n")
    f.write("\n")

    for i in range(1, int(node) + 1):
        f.write("\tlong id" + str(i) + " = person" + str(i) + ".iD;\n")

    f.write("\t\n/****** Input Reader atomic model instantiation *******************/\n")
    # Change directory if needed
    f.write("\tconst char * i_input_data_health = \"../input_data/health_generator.txt\";\n")
    f.write("\tshared_ptr<dynamic::modeling::model> input_reader_health = dynamic::translate::make_dynamic_atomic_model<InputReader_health, TIME, const char* >(\"input_reader_health\" , move(i_input_data_health));\n\n")

    # Located below is whole bunch of For Loops
    # Change the directories of each line to match thy computer's directories

    f.write("\t/****** Health Status atomic model instantiation *****************/\n")
    for i in range(1, int(node) + 1):
        f.write("\tshared_ptr<dynamic::modeling::model> health_status" + str(i) + " = dynamic::translate::make_dynamic_atomic_model<Health_Status_Filter, TIME, long>(\"health_status" + str(i) + "\", move(id" + str(i) + "));\n")

    f.write("\n\t/****** Room Specs atomic model instantiation *****************/\n")
    for i in range(1, int(node) + 1):
        f.write("\tshared_ptr<dynamic::modeling::model> room_specs" + str(i) + " = dynamic::translate::make_dynamic_atomic_model<Room_Specs_Filter, TIME, long>(\"room_specs" + str(i) + "\", move(id" + str(i) + "));\n")

    f.write("\n\t/****** Behaviour Rules atomic model instantiation *****************/\n")
    for i in range(1, int(node) + 1):
        f.write("\tshared_ptr<dynamic::modeling::model> behaviour_rules" + str(i) + " = dynamic::translate::make_dynamic_atomic_model<BehaviourRules, TIME, string, TIME, TIME>(\"behaviour_rules" + str(i) + "\", \"../data/XML" + str(i) + ".xml\", TIME(\"01:30:00:00\"), TIME(\"01:50:00:00\"));\n")

    f.write("\n\t/****** Room Model atomic model instantiation **************/\n")
    # Thou can change the room's location if desired
    f.write("\tshared_ptr<dynamic::modeling::model> room_model = dynamic::translate::make_dynamic_atomic_model<Room_Model, TIME, string, long>(\"room_model\", \"4th_Mackenzie\", 100);\n")

    f.write("\n\t/****** Room Entering Filter ***************/\n")
    f.write("\tshared_ptr<dynamic::modeling::model> room_entering1 = dynamic::translate::make_dynamic_atomic_model<room_entering, TIME, string>(\"room_entering\", \"4th_Mackenzie\");\n")

    f.write("\n\t/****** Room Leaving Filter ***************/\n")
    f.write("\tshared_ptr<dynamic::modeling::model> room_leaving1 = dynamic::translate::make_dynamic_atomic_model<room_leaving, TIME, string> (\"room_leaving\", \"4th_Mackenzie\");\n\n")

    # Big /*************NODE COUPLED MODEL************/ loops below
    for i in range(1, int(node) + 1):
        f.write("\n\t/*********** NODE " + str(i) + " COUPLED MODEL *************/\n")
        f.write("\tdynamic::modeling::Ports iports_node" + str(i) + " = {typeid(inp_in1),typeid(inp_in2)};\n")
        f.write("\tdynamic::modeling::Ports oports_node" + str(i) + " = {typeid(outp_out)};\n")
        f.write("\tdynamic::modeling::Models submodels_node" + str(i) + " = {health_status" + str(i) + ", room_specs" + str(i) + ", behaviour_rules" + str(i) + "};\n")
        f.write("\tdynamic::modeling::EICs eics_node" + str(i) + " = {\n")
        f.write("\t\tdynamic::translate::make_EIC<inp_in1, Health_Status_Filter_Ports::health_in>(\"health_status" + str(i) + "\"),\n")
        f.write("\t\tdynamic::translate::make_EIC<inp_in2, Room_Specs_Filter_Ports::room_in>(\"room_specs" + str(i) + "\")\n")
        f.write("\t};\n")

        f.write("\tdynamic::modeling::EOCs eocs_node" + str(i) + " = {\n")
        f.write("\t\tdynamic::translate::make_EOC<BehaviourRules_Ports::BehaviourRulesOut,outp_out>(\"behaviour_rules" + str(i) + "\")\n")
        f.write("\t};\n")

        f.write("\tdynamic::modeling::ICs ics_node" + str(i) + " = {\n")
        f.write("\t\tdynamic::translate::make_IC<Health_Status_Filter_Ports::health_out, BehaviourRules_Ports::BehaviourRulesInHealth>(\"health_status" + str(i) + "\",\"behaviour_rules" + str(i) + "\"),\n")
        f.write("\t\tdynamic::translate::make_IC<Room_Specs_Filter_Ports::room_out, BehaviourRules_Ports::BehaviourRulesInRoom>(\"room_specs" + str(i) + "\",\"behaviour_rules" + str(i) + "\"),\n")
        f.write("\t};\n")

        f.write("\n\tshared_ptr<cadmium::dynamic::modeling::coupled<TIME>> NODE" + str(i) + ";\n")
        f.write("\tNODE" + str(i) + " = make_shared<dynamic::modeling::coupled<TIME>>(\n")
        f.write("\t\t\"NODE" + str(i) + "\", submodels_node" + str(i) + ", iports_node" + str(i) + ", oports_node" + str(i) + ", eics_node" + str(i) + ", eocs_node" + str(i) + ", ics_node" + str(i) + "\n")
        f.write("\t);\n")

    # /************ ROOM FILTER MODEL *****************/
    f.write("\n/************ ROOM FILTER MODEL *****************/\n")
    f.write("\tdynamic::modeling::Ports iports_ROOM = {typeid(inp_mov)};\n")
    f.write("\tdynamic::modeling::Ports oports_ROOM = {typeid(outp_room)};\n")
    f.write("\tdynamic::modeling::Models submodels_ROOM = {room_model, room_entering1, room_leaving1};\n")
    f.write("\tdynamic::modeling::EICs eics_ROOM = {\n")
    f.write("\t\tdynamic::translate::make_EIC<inp_mov, room_leaving_ports::room_leaving_in>(\"room_leaving\"),\n")
    f.write("\t\tdynamic::translate::make_EIC<inp_mov, room_entering_ports::room_entering_in>(\"room_entering\")\n")
    f.write("\t};\n")
    f.write("\tdynamic::modeling::EOCs eocs_ROOM = {\n")
    f.write("\t\tdynamic::translate::make_EOC<Room_Model_Ports::room_model_out, outp_room>(\"room_model\")\n")
    f.write("\t};\n")
    f.write("\tdynamic::modeling::ICs ics_ROOM = {\n")
    f.write("\t\tdynamic::translate::make_IC<room_leaving_ports::room_leaving_out, Room_Model_Ports::room_model_in_leaving>(\"room_leaving\", \"room_model\"),\n")
    f.write("\t\tdynamic::translate::make_IC<room_entering_ports::room_entering_out, Room_Model_Ports::room_model_in_entering>(\"room_entering\",\"room_model\")\n")
    f.write("\t};\n")
    f.write("\tshared_ptr<dynamic::modeling::coupled<TIME>> ROOM;\n")
    f.write("\tROOM = make_shared<dynamic::modeling::coupled<TIME>>(\n")
    f.write("\t\t\"ROOM\", submodels_ROOM, iports_ROOM, oports_ROOM, eics_ROOM, eocs_ROOM, ics_ROOM\n")
    f.write("\t);\n\n")

    # /******** SIMULATOR MODEL *********/
    f.write("\t\n/********* SIMULATOR MODEL *********/\n")
    f.write("\tdynamic::modeling::Ports iports_SIM = {typeid(inp_SIM)}; // sim input\n")
    f.write("\tdynamic::modeling::Ports oports_SIM = {typeid(outp_out)};\n")
    f.write("\tdynamic::modeling::Models submodels_SIM = {ROOM")
    for i in range(1, int(node) + 1):
        f.write(", NODE" + str(i))
    f.write("};\n")
    f.write("\tdynamic::modeling::EICs eics_SIM = {\n")
    count = 1
    for i in range(1, int(node) + 1):
        if count == int(node):
            f.write("\t\tdynamic::translate::make_EIC<inp_SIM, inp_in1>(\"NODE" + str(i) + "\")\n")
        else:
            f.write("\t\tdynamic::translate::make_EIC<inp_SIM, inp_in1>(\"NODE" + str(i) + "\"),\n")
        count = count + 1

    # // input generator coupling
    f.write("\n\t}; // input generator coupling\n")
    f.write("\tdynamic::modeling::EOCs eocs_SIM = {\n")
    count = 1
    for i in range(1, int(node) + 1):
        if count == int(node):
            f.write("\t\tdynamic::translate::make_EOC<outp_out,outp_out>(\"NODE" + str(i) + "\")\n")
        else:
            f.write("\t\tdynamic::translate::make_EOC<outp_out,outp_out>(\"NODE" + str(i) + "\"),\n")
        count = count + 1
    f.write("\t};\n")

    f.write("\tdynamic::modeling::ICs ics_SIM = {\n")
    count = 1
    for i in range(1, int(node) + 1):
        f.write("\t\tdynamic::translate::make_IC<outp_out, inp_mov>(\"NODE" + str(i) + "\",\"ROOM\"),\n")
        if count == int(node):
            f.write("\t\tdynamic::translate::make_IC<outp_room, inp_in2>(\"ROOM\",\"NODE" + str(i) + "\")\n")
        else:
            f.write("\t\tdynamic::translate::make_IC<outp_room, inp_in2>(\"ROOM\",\"NODE" + str(i) + "\"),\n\n")
        count = count + 1
    f.write("\t};\n")

    f.write("\tshared_ptr<cadmium::dynamic::modeling::coupled<TIME>> SIM;\n")
    f.write("\tSIM = make_shared<dynamic::modeling::coupled<TIME>>(\n")
    f.write("\t\t\"SIM\", submodels_SIM, iports_SIM, oports_SIM, eics_SIM, eocs_SIM, ics_SIM\n")
    f.write("\t);\n")

    # /********* TOP MODEL *********/
    f.write("\n\t/*** TOP MODEL***/\n")
    f.write("\tdynamic::modeling::Ports iports_TOP = {};\n")
    f.write("\tdynamic::modeling::Ports oports_TOP = {typeid(outp_out)};\n")
    f.write("\tdynamic::modeling::Models submodels_TOP = {input_reader_health,SIM};\n")
    f.write("\tdynamic::modeling::EICs eics_TOP = {};\n")
    f.write("\tdynamic::modeling::EOCs eocs_TOP = {\n")
    f.write("\t\tdynamic::translate::make_EOC<outp_out,outp_out>(\"SIM\")\n")
    f.write("\t};\n")

    f.write("\tdynamic::modeling::ICs ics_TOP = {\n")
    f.write("\t\tdynamic::translate::make_IC<iestream_input_defs<health_status>::out, inp_SIM>(\"input_reader_health\",\"SIM\") // input from the sim\n")
    f.write("\n\t};\n")

    f.write("\tshared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;\n")
    f.write("\tTOP = make_shared<dynamic::modeling::coupled<TIME>>(\n")
    f.write("\t\t\"TOP\", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP\n")
    f.write("\t);\n")

    f.write("\t//Loggers\n")
    # Change thy directory if required
    f.write("\tstatic ofstream out_messages(\"../simulation_results/main_node_test_output_messages.txt\");\n")
    f.write("\tstruct oss_sink_messages{\n")
    f.write("\t\tstatic ostream& sink(){\n")
    f.write("\t\treturn out_messages;\n")
    f.write("\t\t}\n")
    f.write("\t};\n\n")

    # Change thy directory if required
    f.write("\tstatic ofstream out_state(\"../simulation_results/main_node_test_output_state.txt\");\n")
    f.write("\tstruct oss_sink_state{\n")
    f.write("\t\tstatic ostream& sink(){\n")
    f.write("\t\treturn out_state;\n")
    f.write("\t\t}\n")
    f.write("\t};\n")

    f.write("\tusing state = logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>,oss_sink_state>;\n")
    f.write("\tusing log_messages = logger::logger<logger::logger_messages,dynamic::logger::formatter<TIME>, oss_sink_messages>;\n")
    f.write("\tusing global_time_mes = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_messages>;\n")
    f.write("\tusing global_time_sta = logger::logger<logger::logger_global_time,dynamic::logger::formatter<TIME>, oss_sink_state>;\n")
    f.write("\tusing logger_top = logger::multilogger<state, log_messages, global_time_mes,global_time_sta>;\n\n")

    # Runner call
    f.write("\n\t//Runner call\n")
    f.write("\tdynamic::engine::runner<NDTime, logger_top> r(TOP, {0});\n")
    # Thou can change the TIME here again
    f.write("\tr.run_until(NDTime(\"20:00:00:000\"));\n")
    f.write("\treturn 0;\n")

    f.write("\t\n}")