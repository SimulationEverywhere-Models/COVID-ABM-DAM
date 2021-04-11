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
    struct room_model_in_entering : public in_port<person_node>{};
    struct room_model_in_leaving : public in_port<person_node>{};
    struct room_model_out : public out_port<room_specs>{};
};

template<typename TIME> class Room_Model{
  public:

    long RoomSize;
    string RoomID;

    using input_ports=std::tuple<typename Room_Model_Ports::room_model_in_entering, Room_Model_Ports::room_model_in_leaving>;
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
      //  cout << "sick: " << state.NumberOfSickPeople << endl;
        
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
                person_virus.Person_ID_room = state.People_In_Room[i].Person_ID;
                person_virus.room_ID_room = state.People_In_Room[i].room_ID_person;
                person_virus.room_size = RoomSize; 
                if(state.People_In_Room[i].mask_wearing){
                
                    person_virus.viral_particles = state.ViralLoadPerPersonMask;
                }else{
                    person_virus.viral_particles = state.ViralLoadPerPersonNoMask;

                }
                state.Exposed_To_Virus.push_back(person_virus);
            }
        }
       
        vector<person_node> msg_bag = get_messages<typename Room_Model_Ports::room_model_in_entering>(mbs);
        vector<person_node> msg_bag2 = get_messages<typename Room_Model_Ports::room_model_in_leaving>(mbs);
        // cout << msg_bag2.size() << "\n";
        // cout << msg_bag.size() << "\n";
        room_specs person_virus;
        bool PersonInRoom = false;
       // cout << "msg bas size: " << msg_bag.size() << endl;
    //   if (msg_bag.empty()!= true) {
       for(int i = 0 ; i < msg_bag.size(); i++ ){
            person_node msg_in_entering = msg_bag[i];
            //Person coming in
            if (msg_in_entering.Room_ID_Entering == RoomID){ // rename InOut
                for (int k =0; k < state.People_In_Room.size(); k++){
                    if(msg_in_entering.Person_ID == state.People_In_Room[k].Person_ID){
                        PersonInRoom = true;
                        if(state.People_In_Room[k].IsSick != msg_in_entering.IsSick){
                            state.People_In_Room[k].IsSick = msg_in_entering.IsSick;
                        if(msg_in_entering.IsSick == true){
                            state.NumberOfSickPeople++;
                        }else{
                            state.NumberOfSickPeople--;
                        }
                    }
                        if(state.People_In_Room[k].mask_wearing != msg_in_entering.mask_wearing){
                            state.People_In_Room[k].mask_wearing = msg_in_entering.mask_wearing;
                            if (msg_in_entering.mask_wearing == true) {
                            state.NumberOfPeopleWearingMasks++;
                            }else{
                                state.NumberOfPeopleWearingMasks--;
                            }
                        }
                        if(state.People_In_Room[k].distance_from_people != msg_in_entering.distance_from_people){
                            if (msg_in_entering.distance_from_people >= SOCIAL_DISTANCE) {
                            state.NumberOfPeopleSocialDistancing++;
                            }else{
                                state.NumberOfPeopleSocialDistancing--;
                            }
                        }
                        break;
                    }
                }
                if(PersonInRoom == false){
                    state.NumberOfPeople++;
                    person_virus.people_in_room = state.NumberOfPeople;
                    person_virus.Person_ID_room = msg_in_entering.Person_ID;
                    person_virus.room_ID_room = msg_in_entering.room_ID_person;
                    person_virus.room_size = RoomSize;
                
                    if (msg_in_entering.mask_wearing == true) {
                        state.NumberOfPeopleWearingMasks++;
                    }
                    if (msg_in_entering.distance_from_people >= SOCIAL_DISTANCE) {
                        state.NumberOfPeopleSocialDistancing++;
                    }
                    if (msg_in_entering.IsSick == true){
                         state.NumberOfSickPeople++;
                    }  
                    person_virus.viral_particles = 0;
                    state.Exposed_To_Virus.push_back(person_virus);
                    state.People_In_Room.push_back(msg_in_entering);
                }
            }
       }
   //    }
       // processing people leaving the room
       for(int s = 0 ; s < msg_bag2.size(); s++){ 
          // cout << "msg bag size: " << msg_bag2.size() << endl;
           person_node msg_in_leaving = msg_bag2[s];
          if(msg_in_leaving.Room_ID_Leaving == RoomID ){ 
                for(int j =0; j< state.People_In_Room.size(); j++){
                    cout << msg_in_leaving.Person_ID << " vs " << state.People_In_Room[j].Person_ID << "\n";
                    if(msg_in_leaving.Person_ID == state.People_In_Room[j].Person_ID) {
                       state.People_In_Room.erase(state.People_In_Room.begin()+j);
                        state.NumberOfPeople--;
                        if (msg_in_leaving.mask_wearing == true) {
                             state.NumberOfPeopleWearingMasks--;
                        }
                        if (msg_in_leaving.distance_from_people >= SOCIAL_DISTANCE) {
                            state.NumberOfPeopleSocialDistancing--;
                         }
                         if (msg_in_leaving.IsSick == true){
                            state.NumberOfSickPeople--;
                         }
                         if(state.NumberOfSickPeople > 0){
                             state.Exposed_To_Virus.erase(state.Exposed_To_Virus.begin()+j);
                         }
                         break;
                    }
                }          
            
            }

           }
    
            if(state.NumberOfPeople == 0){
                state.NumberOfPeopleSocialDistancing = 0;
                state.NumberOfPeopleWearingMasks = 0;
                state.NumberOfSickPeople = 0;
            }
        
        for (int l = 0; l < state.People_In_Room.size(); l++){
            //state.Exposed_To_Virus[l].number_of_social_distancing = state.NumberOfPeopleSocialDistancing;
            state.Exposed_To_Virus[l].people_in_room = state.NumberOfPeople;
            //state.Exposed_To_Virus[l].number_of_mask_wearers = state.NumberOfPeopleWearingMasks;
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
        os << "Number of People in room: "<< i.NumberOfPeople <<" number of people wearing masks: " << i.NumberOfPeopleWearingMasks << " Number of people social distancing: " << i.NumberOfPeopleSocialDistancing << " Number of sick people: " << i.NumberOfSickPeople <<"\n ";
       // }
        
        for (int j=0; j<(i.Exposed_To_Virus).size(); j++){
            os <<" Persons Identification: "<< i.Exposed_To_Virus[j].Person_ID_room  <<  " Room Size: " <<i.Exposed_To_Virus[j].room_size
            << " Viral Particles: " << i.Exposed_To_Virus[j].viral_particles << " Room ID: " << i.Exposed_To_Virus[j].room_ID_room << "\n ";

        }
        
        return os;
    }
           
    #endif // room.hpp
};
