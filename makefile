CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include 
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

# TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)

message.o : data/message.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) data/message.cpp -o build/message.o

#message.o : Health_Status_Filter/message.cpp
	#$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) data/message.cpp -o build/message.o

#message.o : Room_Specs_Filter/message.cpp
	#$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) Room_Specs_Filter/message.cpp -o build/message.o

#message.o : Behaviour_Rules/message.cpp
	#$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) Behaviour_Rules/message.cpp -o build/message.o

main_node.o: main_node.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) main_node.cpp -o build/main_node.o

main_Health_Status_Filter_test.o: test/main_Health_Status_Filter_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) test/main_Health_Status_Filter_test.cpp -o build/main_Health_Status_Filter_test.o

main_unit_test.o: test/main_unit_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) test/main_unit_test.cpp -o build/main_unit_test.o

main_BehaviourRules_unit_test.o: test/main_BehaviourRules_unit_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) test/main_BehaviourRules_unit_test.cpp -o build/main_BehaviourRules_unit_test.o

room_model_test.o: test/room_model_test.cpp		
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) test/room_model_test.cpp -o build/room_model_test.o

tests: main_node.o main_Health_Status_Filter_test.o main_unit_test.o main_BehaviourRules_unit_test.o room_model_test.o message.o
	$(CC) -g -o bin/MAIN_HEALTH_STATUS_FILTER_TEST build/main_Health_Status_Filter_test.o build/message.o
	$(CC) -g -o bin/MAIN_UNIT_TEST build/main_unit_test.o build/message.o
	$(CC) -g -o bin/MAIN_BEHAVIOUR_RULES_UNIT_TEST build/main_BehaviourRules_unit_test.o build/message.o
	$(CC) -g -o bin/ROOM_MODEL_TEST build/room_model_test.o build/message.o

#TARGET TO COMPILE ONLY NODE SIMULATOR
simulator: main_node.o message.o
	$(CC) -g -o bin/MAIN_NODE build/main_node.o build/message.o 

all: simulator tests

clean:
	rm -f bin/* build/*