CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include 
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ROOM MODEL TEST
message.o : message.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) message.cpp -o build/message.o

room_model_test.o: room_model_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) room_model_test.cpp -o build/room_model_test.o

tests: room_model_test.o message.o
	$(CC) -g -o bin/Room_Model_test build/room_model_test.o build/message.o

 #TARGET TO COMPILE EVERYTHING
 	all:tests

 #CLEAN COMMANDS
 	clean: 
	rm -f bin/* build/*