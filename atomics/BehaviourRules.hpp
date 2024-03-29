#ifndef BehaviourRules_hpp
#define BehaviourRules_hpp

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>


#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <limits>
#include <assert.h>
#include <string>
#include <list>
#include <random>
#include <vector>
#include <iostream>

#include <cadmium/modeling/dynamic_model_translator.hpp>

#include "../data/message.hpp" // include message file

#include "../data/void_load.hpp" 

using namespace cadmium;
using namespace std;
using namespace decision_maker_behaviour_structures;

//Port Definition
struct BehaviourRules_Ports {
    struct BehaviourRulesInHealth : public in_port<bool>{};
    struct BehaviourRulesInRoom : public in_port<room_specs>{};
    struct BehaviourRulesOut : public out_port<person_node>{};
};

// Output events
template <typename TIME> class BehaviourRules {
    public:

    TIME TimeAtHome; 
    TIME TimeInRoom;

    using input_ports=std::tuple<typename BehaviourRules_Ports::BehaviourRulesInHealth, typename BehaviourRules_Ports::BehaviourRulesInRoom>;
    using output_ports=std::tuple<typename BehaviourRules_Ports::BehaviourRulesOut>;

    struct MovementInfo {
        string room_id_entering = "";
        string room_id_leaving = "";
    };
    

    //State definition 
    struct state_type{
        DecisionMakerBehaviour person;
        MovementInfo info;
        float ViralLoad;
        TIME SpentInLocation;
        bool MaskWearing;
        float DistanceFromPeople;

    //    person_node person_out; // change to person mask and distance
    };
    state_type state;

    BehaviourRules()noexcept{
        
    }
     BehaviourRules (string file_name_in, TIME i_In_Room, TIME i_At_Home) noexcept{
        const char * in = file_name_in.c_str();
        state.person.load(in);
        TimeInRoom = i_In_Room; 
        TimeAtHome = i_At_Home;
        state.person.location = "home";
        state.info.room_id_entering = ""; //added March 24 update
        state.info.room_id_leaving = ""; //added March 24 update
        state.person.isSick = state.person.isSick;
        state.MaskWearing = state.person.wearingMask;
        state.DistanceFromPeople = state.person.distance;
        state.ViralLoad = 0;
        state.SpentInLocation= TIME();
    }            

    //internal transition
    void internal_transition() {

       if(state.info.room_id_leaving != "" || state.info.room_id_entering != ""){
         if (state.SpentInLocation < TimeInRoom && state.person.location == "4th_Mackenzie") { // && person in room or at home set movement info null 
            state.info.room_id_leaving = "";
            state.info.room_id_entering = "";
         } else if (state.SpentInLocation < TimeAtHome && state.person.location == "home") {
            state.info.room_id_leaving = "";
            state.info.room_id_entering = "";
         }
      }else{
         if (state.person.location == "4th_Mackenzie") { 
            state.info.room_id_leaving = state.person.location; // Set to "room location"
            state.info.room_id_entering = "home";
            state.person.location = "home";
            state.SpentInLocation = TIME();
         } else if (state.person.location == "home") { // same for previous
            state.info.room_id_leaving = state.person.location;
            state.info.room_id_entering = "4th_Mackenzie"; // Set to "room location"
            state.person.location = "4th_Mackenzie";
            state.SpentInLocation = TIME();
         }
      }

       
        if(state.info.room_id_entering == "4th_Mackenzie"){
            
        for(int i=0; i<state.person.behaviourRulesPerson.size(); i++){
            float safeDist = 0;
            bool MaskWearing = false;
            if(state.person.behaviourRulesPerson[i].SafeDistanceProbability == "Low"){
                int a = (rand() % 100);
                if(a<90){
                    safeDist = rand() % 1 + 1 ;
                    }else{
                        safeDist = rand() % 2 + 2;
                }
                }else{
                    int a = (rand() % 100);
                    if(a<90){
                        safeDist = rand() % 2 + 2 ;
                    }else{
                    safeDist = rand() % 1 + 1;
                    }
                }
                if(state.person.behaviourRulesPerson[i].MaskWearingProbability == "Low"){
                int a = (rand() % 100);
                if(a<90){
                    MaskWearing = false;
                }else{
                    MaskWearing = true;
                }
                }else{
                    int a = (rand() % 100);
                    if(a<90){
                        MaskWearing = true;
                    }else{
                        MaskWearing = false;
                    }
                }
                
                // if(MaskWearing != state.person.wearingMask && state.SpentInLocation < TimeInRoom ){
                //     state.info.room_id_entering = "4th_Mackenzie"; 
                // }
                // if(safeDist != state.person.distance && state.SpentInLocation < TimeInRoom){
                //     state.info.room_id_entering = "4th_Mackenzie"; 
                // }
                state.DistanceFromPeople = safeDist;
                state.MaskWearing = MaskWearing;
             } 
             //cout<< state.SpentInLocation<<"\n";
        }
    } 

    // Exeternal transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){

         vector<bool> msg_health_in = get_messages<typename BehaviourRules_Ports::BehaviourRulesInHealth>(mbs);
        if(msg_health_in.size()>1){
            assert(false);
        }else if(msg_health_in.size()==1){
            state.person.isSick = msg_health_in[0];
            state.SpentInLocation = TIME();
        }
        
        vector <room_specs> msg_in = get_messages<typename BehaviourRules_Ports::BehaviourRulesInRoom>(mbs);
            for( int i=0; i<msg_in.size(); i++){
                state.ViralLoad += msg_in[i].viral_particles;

            }
            if(state.ViralLoad > 1000 && state.person.isSick == false){
                state.person.isSick = true;
                state.info.room_id_entering = "4th_Mackenzie";
            }
           
            state.SpentInLocation += e;
           cout<< state.SpentInLocation << "\n";
        
    }    

    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
        internal_transition();
        external_transition(TIME(),std::move(mbs));

    }

    //Output Function
    // use movement info for defining the output for InTrueOutFalse, only generate output for non empty strings
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        typename make_message_bags<input_ports>::type mbs;
       vector <bool> msg_in = get_messages<typename BehaviourRules_Ports::BehaviourRulesInHealth>(mbs);
         person_node person_output;

        if(state.info.room_id_leaving != "" || state.info.room_id_entering != ""){
            person_output.Person_ID = state.person.iD;
            person_output.Room_ID_Leaving = state.info.room_id_leaving;
            person_output.Room_ID_Entering = state.info.room_id_entering;
            person_output.IsSick = state.person.isSick;
            person_output.mask_wearing = state.MaskWearing;
            person_output.room_ID_person = state.person.location;
            person_output.distance_from_people = state.DistanceFromPeople;
            
            get_messages<typename BehaviourRules_Ports::BehaviourRulesOut>(bags).push_back(person_output);
        }

        // if (state.info.room_id_entering != ""){
        //     person_output.Person_ID = state.person.iD;
        //     person_output.Room_ID_Entering = state.info.room_id_entering;
        //     person_output.Room_ID_Leaving = state.info.room_id_leaving;
        //     person_output.IsSick = state.person.isSick;
        //     person_output.mask_wearing = state.MaskWearing;
        //     person_output.room_ID_person = state.person.location;
        //     person_output.distance_from_people = state.DistanceFromPeople;
            

        //      get_messages<typename BehaviourRules_Ports::BehaviourRulesOut>(bags).push_back(person_output);
        // }

        // if (msg_in.size()==1){
        //     person_output.Person_ID = state.person.iD;
        //     person_output.Room_ID_Entering = state.info.room_id_entering;
        //     person_output.Room_ID_Leaving = state.info.room_id_leaving;
        //     person_output.IsSick = state.person.isSick;
        //     person_output.mask_wearing = state.MaskWearing;
        //     person_output.room_ID_person = state.person.location;
        //     person_output.distance_from_people = state.DistanceFromPeople;
            

        //      get_messages<typename BehaviourRules_Ports::BehaviourRulesOut>(bags).push_back(person_output);
        // }
       
         return bags;
    }


    //time advance function
    TIME time_advance() const{
        TIME next_internal;
        if (state.info.room_id_leaving != "" && state.info.room_id_entering != "") { // if NOT null, ta 0
            next_internal = TIME();
       }else if (state.SpentInLocation <= TimeInRoom && state.person.location == "4th_Mackenzie") { // in room  
            next_internal = TimeInRoom - state.SpentInLocation;
        } else if (state.SpentInLocation <= TimeAtHome && state.person.location == "home") { 
            next_internal = TimeAtHome - state.SpentInLocation;
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }

        return next_internal;
        // cout<<next_internal<< "\n";

    };

    friend ostringstream& operator<<(ostringstream& os, const typename BehaviourRules<TIME>::state_type& i) { // change variables 

            if(i.info.room_id_leaving != ""){
                os<< " Room leaving: " << i.info.room_id_leaving << "\n";
            }
            if(i.info.room_id_entering !=""){
                os<<" Room entering "<< i.info.room_id_entering <<"\n";
            }
        
            os<<" Person ID: " << i.person.iD << " Sick? "<< i.person.isSick << " Mask wearing: " << i.MaskWearing
            <<" Room ID: " << i.person.location << " Distance from people: " << i.DistanceFromPeople << "\n";
        
       

      
        return os;
    } 
    #endif
};
