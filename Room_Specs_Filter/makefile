CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include 
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $ (shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE HEALTH FILTER TEST
message.o : message.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES)
	message.cpp -o build/message.o

main_unit_test.o: main_unit_test.cpp
	$(CC)   -g  -c  $(CFLAGS)   $(INCLUDECADMIUM)   $(INCLUDEDESTIMES) main_unit_test.cpp -o build/main_unit_test

tests: main_unit_test.o message.o
	$(CC) -g -o bin/RoomSpecsFilter_test build/main_unit_test.o build/message.o

 #TARGET TO COMPILE EVERYTHING
 	all:tests

 #CLEAN COMMANDS
 	clean: 
	rm -f bin/* build/*