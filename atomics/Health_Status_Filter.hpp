#ifndef Health_Status_Filter_HPP
#define Health_Status_Filter_HPP


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string> 
#include <stdlib.h>
#include <random>

#include "../data/message.hpp"

using namespace cadmium;
using namespace std;


//Port definition
//definition of input/output events 
struct Health_Status_Filter_Ports{
    struct health_out : public out_port<bool>{};
    struct health_in : public in_port<health_status>{};
};

// >> & <<

template <typename TIME> class Health_Status_Filter{
    public: 

    long PERSON_ID;

    using input_ports=std::tuple<typename Health_Status_Filter_Ports::health_in>;
    using output_ports=std::tuple<typename Health_Status_Filter_Ports::health_out>;

    //State Definition
    struct state_type {

    	vector<health_status> msgs_passing_filter;   
    };
    state_type state;
   
    Health_Status_Filter () noexcept{
    }
    Health_Status_Filter (long i_person_id) noexcept{
    PERSON_ID = i_person_id;        
    state.msgs_passing_filter.clear();
    }

    //internal transition function
    void internal_transition(){
	//clear message passing through the filter 
	state.msgs_passing_filter.clear();
    }


    //external transition function
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
    	//Loop!
	for (int i = 0 ; get_messages<typename Health_Status_Filter_Ports::health_in>(mbs).size()>i ; i++ ){
       	if(PERSON_ID == get_messages<typename Health_Status_Filter_Ports::health_in>(mbs)[i].Person_ID){
       		state.msgs_passing_filter.push_back(get_messages<typename Health_Status_Filter_Ports::health_in>(mbs)[i]);
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
    //output IsSick
    typename make_message_bags<output_ports>::type bags;
 
        for (int i = 0; i < state.msgs_passing_filter.size(); i++){
            get_messages<typename Health_Status_Filter_Ports::health_out>(bags).push_back(state.msgs_passing_filter[i].IsSick); 
  
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

    friend std::ostringstream& operator<<(std::ostringstream& os, const typename Health_Status_Filter<TIME>::state_type& i) {
        for (int j = 0; j < (i.msgs_passing_filter).size(); j++){
            os << "for person" << i.msgs_passing_filter[j].Person_ID << " " << i.msgs_passing_filter[j].IsSick;
        
        }
        return os;
    }

};
#endif