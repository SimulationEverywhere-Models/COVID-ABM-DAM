CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include 
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE BEHAVIOURRULES TEST
message.o : message.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) message.cpp -o build/message.o

main_BehaviourRules_unit_test.o: main_BehaviourRules_unit_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) main_BehaviourRules_unit_test.cpp -o build/main_BehaviourRules_unit_test.o

tests: main_BehaviourRules_unit_test.o message.o
	$(CC) -g -o bin/BehaviourRules_test build/main_BehaviourRules_unit_test.o build/message.o

#TARGET TO COMPILE EVERYTHING
 	all:tests

#CLEAN COMMANDS
 	clean: 
	rm -f bin/* build/*
