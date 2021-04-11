#ifndef room_entering_HPP
#define room_entering_HPP


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string> 
#include <stdlib.h>
#include <random>
#include <iostream>

#include "../data/message.hpp"

using namespace cadmium;
using namespace std;


//Port definition
//definition of input/output events 
struct room_entering_ports{
    struct room_entering_out : public out_port<person_node>{};
    struct room_entering_in : public in_port<person_node>{};
};

template <typename TIME> class room_entering{
    public: 

    string ROOM_ID;

    using input_ports=std::tuple<typename room_entering_ports::room_entering_in>;
    using output_ports=std::tuple<typename room_entering_ports::room_entering_out>;

    //State Definition
    struct state_type {

    	vector<person_node> msgs_passing_filter;   
    };
    state_type state;
   
   //default constructor
    room_entering () noexcept{}
    room_entering (string i_room_id_person) noexcept{
    ROOM_ID = i_room_id_person;        
    state.msgs_passing_filter.clear();
    }

    //internal transition function
    void internal_transition(){
	//clear message passing through the filter 
	state.msgs_passing_filter.clear();
    }


    //external transition function
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        for (int i = 0 ; get_messages<typename room_entering_ports::room_entering_in>(mbs).size()>i ; i++ ){
           // cout<< get_messages<typename room_entering_ports::room_entering_in>(mbs)[i].Room_ID_Entering << "\n";
            if(ROOM_ID == get_messages<typename room_entering_ports::room_entering_in>(mbs)[i].Room_ID_Entering){
                state.msgs_passing_filter.push_back(get_messages<typename room_entering_ports::room_entering_in>(mbs)[i]);
            } 
        }
    }

    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    //output function
    typename make_message_bags<output_ports>::type output() const {
    typename make_message_bags<output_ports>::type bags;
 
        for (int i = 0; i < state.msgs_passing_filter.size(); i++){
            get_messages<typename room_entering_ports::room_entering_out>(bags).push_back(state.msgs_passing_filter[i]);
        }
        return bags;
    }


    TIME time_advance() const{
        TIME next_internal;
        if (state.msgs_passing_filter.empty() == true) {
            next_internal = numeric_limits<TIME>::infinity();
        } else {
            next_internal = TIME(); 
        }

        return next_internal;

    };

    friend std::ostringstream& operator<<(std::ostringstream& os, const typename room_entering<TIME>::state_type& i) {
        for (int j = 0; j < (i.msgs_passing_filter).size(); j++){
            os << "For person: " << i.msgs_passing_filter[j].Person_ID << " Room entering: " << i.msgs_passing_filter[j].Room_ID_Entering << " Room leaving: " << i.msgs_passing_filter[j].Room_ID_Leaving << " Is sick: " << i.msgs_passing_filter[j].IsSick << " Mask wearing: " << i.msgs_passing_filter[j].mask_wearing << " Room ID: " << i.msgs_passing_filter[j].room_ID_person << " Distance from people: " << i.msgs_passing_filter[j].distance_from_people << "\n"; 
        
        }
        return os;
    }

};
#endif