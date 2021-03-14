#ifndef Room_Model_HPP
#define Room_Model_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <list>
#include <random>
#include <vector>
#include <iostream>

#include "../data/message.hpp"

#define SOCIAL_DISTANCE 2


using namespace cadmium;
using namespace std;

//Port definition
//definition of input/output events
struct Room_Model_Ports{
    struct room_model_in : public in_port<person_node>{};
    struct room_model_out : public out_port<room_specs>{};
};

template<typename TIME> class Room_Model{
  public:

    long RoomSize;
    string RoomID;

    using input_ports=std::tuple<typename Room_Model_Ports::room_model_in>;
    using output_ports=std::tuple<typename Room_Model_Ports::room_model_out>;


    //State definition 
    struct state_type{
        int NumberOfPeople;
        int NumberOfPeopleWearingMasks;
        int NumberOfPeopleSocialDistancing;
        float ViralLoad;
        float ViralLoadPerPersonMask;
        float ViralLoadPerPersonNoMask;
        int NumberOfSickPeople;

       
       vector<person_node> People_In_Room;
       vector<room_specs> Exposed_To_Virus;

    }; state_type state;

    //default constructor
    Room_Model () noexcept{
        state.Exposed_To_Virus.clear();
        state.People_In_Room.clear();
        state.NumberOfPeople = 0;
        state.NumberOfPeopleWearingMasks = 0;
        state.NumberOfPeopleSocialDistancing = 0;
        state.ViralLoad = 0;
        state.ViralLoadPerPersonMask = 0;
        state.ViralLoadPerPersonNoMask = 0;
        state.NumberOfSickPeople = 0;
    }
    Room_Model (string i_room_ID, long i_room_size) noexcept{
        RoomID = i_room_ID;
        RoomSize = i_room_size;
        state.Exposed_To_Virus.clear();
        state.People_In_Room.clear();
        state.NumberOfPeople = 0;
        state.NumberOfPeopleWearingMasks = 0;
        state.NumberOfPeopleSocialDistancing = 0;
        state.ViralLoad = 0;
        state.ViralLoadPerPersonMask = 0;
        state.ViralLoadPerPersonNoMask = 0;
        state.NumberOfSickPeople = 0;
    }

    //internal transition
    void internal_transition() {
        state.Exposed_To_Virus.clear();
        state.ViralLoad=0;

    }


    //update viral load
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        cout << "sick: " << state.NumberOfSickPeople << endl;
        
        if (state.NumberOfSickPeople > 0) {
           float h_min = e.getHours()*60;
           float m_min = e.getMinutes();
           float s_min = e.getSeconds()/60;
           float elap_min = (h_min + m_min + s_min); // convert elapsed time to minutes 
           //cout<<elap_min<<" ";
           state.ViralLoad = state.NumberOfSickPeople * (20) * elap_min;
           //cout<<state.ViralLoad;
    
            if (state.NumberOfPeopleWearingMasks > 0) {
                state.ViralLoadPerPersonNoMask = (0.75 * state.ViralLoad)/(state.NumberOfPeople-state.NumberOfPeopleWearingMasks);
                //cout<<state.ViralLoadPerPerson<<" ";
                state.ViralLoadPerPersonMask = (0.25 * state.ViralLoad)/state.NumberOfPeopleWearingMasks;              
                
            }else {
                state.ViralLoadPerPersonNoMask = state.ViralLoad/state.NumberOfPeople;
                //cout<<state.ViralLoadPerPerson<<" ";            
            }
            cout << "people in room:" << state.People_In_Room.size() << endl;
            for(int i =0; i< state.People_In_Room.size(); i++){
                room_specs person_virus;
                person_virus.Person_ID = state.People_In_Room[i].Person_ID;
                person_virus.room_ID = state.People_In_Room[i].room_ID;
                person_virus.room_size = RoomSize; 
                //person_virus.number_of_mask_wearers = state.NumberOfPeopleWearingMasks;
                if(state.People_In_Room[i].mask_wearing){
                
                    person_virus.viral_particles = state.ViralLoadPerPersonMask;
                }else{
                    person_virus.viral_particles = state.ViralLoadPerPersonNoMask;

                }
                state.Exposed_To_Virus.push_back(person_virus);
            }
        }
        //cout<<"size of exposed to virus is: &&&& "<<state.Exposed_To_Virus.size() << endl;
        //int m =0;
        /*while(m<2){
           cout << "inside while: " << m << endl;
           m++;
        }
     */
        vector<person_node> msg_bag = get_messages<typename Room_Model_Ports::room_model_in>(mbs);
       // cout << "msg bas size: " << msg_bag.size() << endl;
       for(int i = 0 ; i < msg_bag.size(); i++ ){
            cout << "inside the msg loop" << endl;
            room_specs person_virus;
            person_node msg_in = msg_bag[i];
            //Person coming in
            if (msg_in.InTrueOutFalse == true){ // rename InOut
                //cout << "person in" << endl;
            
                bool PersonInRoom = false;

                for (int k =0; k < state.People_In_Room.size(); k++){
                    if(msg_in.Person_ID == state.People_In_Room[k].Person_ID){
                        PersonInRoom = true;
                        break;
                    }
                }
                if(PersonInRoom == false){
                    state.NumberOfPeople++;
                    //cout<< state.NumberOfPeople <<" ";
                    person_virus.people_in_room = state.NumberOfPeople;
                    person_virus.Person_ID = msg_in.Person_ID;
                    person_virus.room_ID = msg_in.room_ID;
                    person_virus.room_size = RoomSize;
                    state.People_In_Room.push_back(msg_in);
                
                    if (msg_in.mask_wearing == true) {
                        state.NumberOfPeopleWearingMasks++;
                        person_virus.number_of_mask_wearers = state.NumberOfPeopleWearingMasks;
                    }
                    if (msg_in.distance_from_people >= SOCIAL_DISTANCE) {
                        state.NumberOfPeopleSocialDistancing++;
                        person_virus.number_of_social_distancing = state.NumberOfPeopleSocialDistancing;
                    }
                    if (msg_in.IsSick == true){
                         state.NumberOfSickPeople++;
                    }  
                    person_virus.viral_particles = 0;
                    state.Exposed_To_Virus.push_back(person_virus);
                }
            }else{ //(get_messages<typename Room_Model_Ports::room_model_in>(mbs)[i].InOut == false){
                for(int j =0; j< state.People_In_Room.size(); j++){
                    if(msg_in.Person_ID == state.People_In_Room[j].Person_ID) {
                       state.People_In_Room.erase(state.People_In_Room.begin()+j);
                       state.Exposed_To_Virus.erase(state.Exposed_To_Virus.begin()+j);
                        state.NumberOfPeople--;
                        if (msg_in.mask_wearing == true) {
                             state.NumberOfPeopleWearingMasks--;
                        }
                        if (get_messages<typename Room_Model_Ports::room_model_in>(mbs)[i].distance_from_people >= SOCIAL_DISTANCE) {
                            state.NumberOfPeopleSocialDistancing--;
                         }
                         if (get_messages<typename Room_Model_Ports::room_model_in>(mbs)[i].IsSick == true){
                            state.NumberOfSickPeople--;
                         }
                        break;
                    }
                }          
            
            }
      // cout<< state.Exposed_To_Virus <<" ";
        }
        for (int l = 0; l < state.People_In_Room.size(); l++){
            state.Exposed_To_Virus[l].number_of_social_distancing = state.NumberOfPeopleSocialDistancing;
            state.Exposed_To_Virus[l].people_in_room = state.NumberOfPeople;
            state.Exposed_To_Virus[l].number_of_mask_wearers = state.NumberOfPeopleWearingMasks;
        }
    
     
    }

    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        internal_transition();
        external_transition(TIME(),std::move(mbs));
    }

    //Output Function
    typename make_message_bags<output_ports>::type output() const{
        //output # of people in the room, size, room id, viral load
        typename make_message_bags<output_ports>::type bags; 
        vector<room_specs> Exposed_To_Virus;

        for (int i = 0; i < state.Exposed_To_Virus.size(); i++){
            get_messages<typename Room_Model_Ports::room_model_out>(bags).push_back(state.Exposed_To_Virus[i]);
            
        }
        return bags;
    }

    //time advance function
    TIME time_advance() const{
        TIME next_internal;
        if(state.Exposed_To_Virus.empty() == true){
            next_internal = numeric_limits<TIME>::infinity();
        }else{
            next_internal = TIME();
        }
    return next_internal;
    };
    
    friend std::ostringstream& operator<<(std::ostringstream& os, const typename Room_Model<TIME>::state_type& i){
      //  if(int k=(i.Exposed_To_Virus).size()){
        os << "Number of People in room "<< i.NumberOfPeople <<" number of people wearing masks: " << i.NumberOfPeopleWearingMasks << " Number of people social distancing: " << i.NumberOfPeopleSocialDistancing << " Number of sick people: " << i.NumberOfSickPeople <<"\n ";
       // }
        
        for (int j=0; j<(i.Exposed_To_Virus).size(); j++){
            os <<" Person ID: "<< i.Exposed_To_Virus[j].Person_ID  << " Number of people in room: " << i.Exposed_To_Virus[j].people_in_room << " Room Size: " <<i.Exposed_To_Virus[j].room_size
            << " Viral Particles: " << i.Exposed_To_Virus[j].viral_particles << " Room ID: " << i.Exposed_To_Virus[j].room_ID << "\n ";

        }
        
        return os;
    }
           
    #endif // room.hpp
};