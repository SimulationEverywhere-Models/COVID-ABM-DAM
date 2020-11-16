
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>
using namespace cadmium;
using namespace std;

//Port definition

struct Room_Ports {
    struct Room_Specs : public out_port <float>{};
    struct Person_Node : public in_port <float>{};
}

template <typename TIME> class Room{
    SPECS RoomSize;
    SPECS RoomID;
    TIME UpdateTime;

    
    Room () noexcept{
        UpdateTime = TIME("00:00:05:00");
        state.next_internal = std:numeric_limits<TIME>::infinity();
        state.model_active = false;

    }

    //State definition 
    struct state_type{
        int NumberOfPeople;
        int NumberOfPeopleWearingMasks;
        int NumberOfPeopleSocialDistancing
        float PercentOfPeopleWearingMasks;
        float PercentOfPeopleSocialDistancing;
        float ViralLoad;
        int SickPeople;
        bool model_active;
        bool next_internal;
        int TimeUpdateViralLoad;
    };
    state_type state;
    using input_ports::tuple<typename Room_Ports::Person_Node>;
    using output_ports::tuple<typename Room_Ports::Room_Specs>;

    //internal transition
    void internal_transition() {
        state.ViralLoad = 
        // want 5 air changes per hour
        // update viral load every 12 minutes
        // or when sick person enters room 

        /*
        int tempSickPeople = SickPeople;
        if (SickPeople >= tempSickPeople) {
            UpdateViralLoad();
        } else {
            TIME("00:12:00:00") = TimeUpdateViralLoad;
        }

        while (TIME("01:00:00:00")) {
            for (int i = 0; i < 5; i++) {
                ChangeAir();
            }
        }
        */
        
    }
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        //update number of people in room
        //update percent of people wearing a mask
        //update percent of people social distancing 
        //update time for updating viral load

            if (bool PersonIn=true) {
                state.NumberOfPeople = (state.NumberOfPeople +1);
                state.PercentOfPeopleWearingMasks = 100 * state.NumberOfPeopleWearingMasks/state.NumberOfPeople;
                state.PercentOfPeopleSocialDistancing = 100 * state.NumberOfPeopleWearingMasks/state.NumberOfPeople;
            }
            else if (bool PersonOut=true) {
                state.NumberOfPeople=(state.NumberOfPeople -1);
                state.PercentOfPeopleWearingMasks = 100 * state.NumberOfPeopleWearingMasks/state.NumberOfPeople;
                state.PercentOfPeopleSocialDistancing = 100 * state.NumberOfPeopleWearingMasks/state.NumberOfPeople;
            }
            else { 
                state.NumberOfPeople = NumberOfPeople;
            }

            UpdateTime = TIME("00:12:00:00");
            

    }

    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        internal_transition();
        external_transition(TIME(),std::move(mbs));

    }
    //Output Function
    typename make_message_bags<output_ports>::type output() const{
        //output # of people in the room
        //output size 
        //output room id
        //output viral load
        typename make_message_bags<output_ports>::type bags;
        string people_in_room;
        string room_size;
        string room_ID;
        string viral_load
        if (state.elements != 0){
                people_in_room = state.NumberOfPeople;
                room_size = RoomSize;
                room_ID = RoomID;
                viral_load = ViralLoad;
                get_messages<typename Reception_defs::cust>(bags).push_back(people_in_room);
                get_messages<typename Reception_defs::cust>(bags).push_back(room_size);
                get_messages<typename Reception_defs::cust>(bags).push_back(room_Id);
                get_messages<typename Reception_defs::cust>(bags).push_back(viral_load);
            }   
        return bags;



    }
    //time advance function
    TIME time_advance() const{
        return state.next_internal;

    }
    
   
}
