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
struct Room_Specs_Filter{
	struct room_in : public in_port</*float?*/>{};
	struct room_out : public out_port<> {};
};

struct room_in{
	int PersonID;
	int people_in_room;
	float room_size;
	float viral_particles;
	int RoomID;
};
struct room_out{
	int PersonID;
	int people_in_room;
	float room_size;
	float viral_particles;
	int RoomID;
};


template<typename TIME>

class Room_Specs_Filter{
	public:
		using input_ports=tuple<typename //unfinished
		using output_ports=tuple<typename //"
};

//contains all messages passing the filter 
struct msgs_passing_filter{
	
};


//external transition function
void external_transition(TIME, ...){
	if (x.PersonID == ID) 		//questionable
	msgs_passing_filter = x + msg_passing_filter;
}

//internal transition function
void internal_transition(){
	//clear message passing through the filter 
	msgs_passing_filter.clear;
}

TIME time_advance() const{
	//needs fixing
	TIME next_internal;
	if msgs_passing_filter = numeric_limits<TIME>::infinity();
	else = 0;
	return next_internal;
}

//output function
{msgs_passing_filter}



