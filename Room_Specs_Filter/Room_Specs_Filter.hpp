#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "message.hpp"

using namespace cadmium;
using namespace std;


//Port definition
//definition of input/output events 
struct Room_Specs_Filter_Ports{
	struct room_in : public in_port<room_specs>{};
	struct room_out : public out_port<room_specs>{};
};

template<typename TIME> class Room_Specs_Filter{
	public:

	long PERSON_ID;

    using input_ports=std::tuple<typename Room_Specs_Filter_Ports::room_in>;
	using output_ports=std::tuple<typename Room_Specs_Filter_Ports::room_out>;

	//state definition 
	struct state_type{
		vector<room_specs> msgs_passing_filter;
	}; state_type state; 

    //default constructor
    Room_Specs_Filter () noexcept{}
	Room_Specs_Filter (long i_person_ID) noexcept{
    	PERSON_ID = i_person_ID;
    	state.msgs_passing_filter.clear();
	}

    //internal transition function
    void internal_transition(){
	//clear message passing through the filter 
	    state.msgs_passing_filter.clear();
    }

    //external transition function
    void external_transition(TIME, typename make_message_bags<input_ports>::type mbs){
        for (int i = 0; get_messages<typename Room_Specs_Filter_Ports::room_in>(mbs).size()>i; i++){
       	    if(PERSON_ID == get_messages<typename Room_Specs_Filter_Ports::room_in>(mbs)[i].Person_ID){
       		    state.msgs_passing_filter.push_back(get_messages<typename Room_Specs_Filter_Ports::room_in>(mbs)[i]);
      		}
    	}
    }

    //confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    //output function
    typename make_message_bags<output_ports>::type output() const{
 	    typename make_message_bags<output_ports>::type bags;
        for (int i = 0; i < state.msgs_passing_filter.size(); i++){
            get_messages<typename Room_Specs_Filter_Ports::room_out>(bags).push_back(state.msgs_passing_filter[i].Person_ID); 
  
        }
    return bags;
    }

    //time advance function
    TIME time_advance() const{
	    TIME next_internal;
	    if (state.msgs_passing_filter.empty() == true){
		    next_internal = numeric_limits<TIME>::infinity();
	    }else{
		    next_internal = TIME();
	    }
	    return next_internal;
    };

    friend std::ostringstream& operator<<(std::ostringstream& os, const typename Room_Specs_Filter<TIME>::state_type& i) {
        for (int j = 0; j < (i.msgs_passing_filter).size(); j++){
            os << "for person " << i.msgs_passing_filter[j].Person_ID << " " << i.msgs_passing_filter[j].people_in_room << " " << i.msgs_passing_filter[j].room_size << " " << i.msgs_passing_filter[j].viral_particles << " " << i.msgs_passing_filter[j].room_ID;
        
        }
        return os;
    }

};


