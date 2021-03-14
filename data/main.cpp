#include <math.h>
#include <assert.h> 
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>

#include "void_load.hpp"


using namespace std;

using namespace decision_maker_behaviour_structures;


int main(){
	string file_name_in = string("Student1.xml");
	DecisionMakerBehaviour decison_maker_behaviour;
  const char * in = file_name_in.c_str();
  decison_maker_behaviour.load(in);



    

    string debugname = string("test_load_Student1.xml");      
    const char * out = debugname.c_str();
    decison_maker_behaviour.save(out);     


	return 1;
};