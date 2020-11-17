#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>
using namespace cadmium;
using namespace std;


//Port definition
//definition of input/output events 
struct Room_Specs_Filter_Ports{
	struct room_in : public in_port<float/>{};
	struct room_out : public out_port<float> {};
};

struct room_in{
	long PersonID;
	int people_in_room;
	float room_size;
	float viral_particles;
	long RoomID;
};
struct room_out{
	long PersonID;
	int people_in_room;
	float room_size;
	float viral_particles;
	long RoomID;
};


template<typename TIME> class Room_Specs_Filter{
	TIME UpdateTime;

	Room_Specs_Filter () noexcept{
    	UpdateTime = TIME("00:00:05:00");
    	state.next_internal = std:numeric_limits<TIME>::infinity();
    	state.model_active = false;
	}
}
    
//state definition 
struct state_type{
	long PersonID;
	int people_in_room;
	float room_size;
	float viral_particles;
	long RoomID;
	
	bool model_active;
	bool next_internal;
};

//contains all messages passing the filter 
struct msgs_passing_filter{
	vector<> messages;
};

	state_type state;
	using input_ports::tuple<typename Room_Specs_Filter_Ports::room_in>;
	using output_ports::tuple<typename Room_Specs_Filter_Ports::room_out>;

//external transition function
void external_transition(TIME, typename make_message_bags<input_ports>::type mbs){
	if (state.PersonID == ID) {
	msgs_passing_filter.messages.insert(people_in_room);
	msgs_passing_filter.messages.insert(room_size);
	msgs_passing_filter.messages.insert(viral_particles);
	msgs_passing_filter.messages.insert(RoomID);
	}
}

//internal transition function
void internal_transition(){
	//clear message passing through the filter 
	msgs_passing_filter.messages.clear;
}

//time advance function
TIME time_advance() const{
	TIME next_internal;
	if (msgs_passing_filter.active)
		next_internal = numeric_limits<TIME>::infinity();
	else {
		next_internal = 0;
	}
	return next_internal;
}

//output function
typename make_message_bags<output_ports>::type output() const{
 	typename make_message_bags<output_ports>::type bags;
 
  	if (msgs_passing_filter.messages.empty() == False){
  		get_messages<typename Room_Specs_Filter_Ports::room_out(bags).push_back(msgs_passing_filter); 
  	}

 return bags;
}



