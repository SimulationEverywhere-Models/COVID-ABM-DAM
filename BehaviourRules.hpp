#ifndef BehaviourRules_hpp
#define BehaviourRules_hpp

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

// https://www.learncpp.com/cpp-tutorial/header-guards/

#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <limits>
#include <assert.h>
#include <string>
#include <list>
#include <random>
#include <vector>
#include <iostream>

#include <cadmium/modeling/dynamic_model_translator.hpp>
#include "../Behaviour_Rules/message.hpp" // include message file
#include "../data_structures/void_load.hpp"

using namespace cadmium;
using namespace std;
using namespace decision_maker_behaviour_structures;

//Port Definition
struct BehaviourRules_Ports {
   struct BehaviourRulesInHealth: public in_port <bool> {};
   struct BehaviourRulesInRoom: public in_port <room_specs> {};
   struct BehaviourRulesOut: public out_port <person_node> {};
};

// Output events
template < typename TIME > class BehaviourRules {
   public:
    TIME TimeAtHome;
    TIME TimeInRoom;

   using input_ports = std::tuple <typename BehaviourRules_Ports::BehaviourRulesInHealth, typename BehaviourRules_Ports::BehaviourRulesInRoom> ;
   using output_ports = std::tuple <typename BehaviourRules_Ports::BehaviourRulesOut> ;

   struct MovementInfo {
      string room_id_entering = "";
      string room_id_leaving = "";
      bool LeavingCampusFalseEnteringTrue = false;
   };

   //State definition 
   struct state_type {
      DecisionMakerBehaviour person;
      MovementInfo info;
      float ViralLoad;
      TIME SpentInLocation;
      bool MaskWearing;
      float DistanceFromPeople;

      //    person_node person_out; // change to person mask and distance
   };
   state_type state;

   BehaviourRules() noexcept {
      assert(false);

   }

   BehaviourRules(string file_name_in, TIME i_In_Room, TIME i_At_Home) noexcept {
      const char * in = file_name_in.c_str();
      state.person.load(in);
      TimeInRoom = i_In_Room;
      TimeAtHome = i_At_Home;
      state.MaskWearing = false;
      state.DistanceFromPeople = 0;
      state.ViralLoad = 0;
      state.SpentInLocation = TIME();
   }

   //internal transition
   void internal_transition() {
      if (state.SpentInLocation < TimeInRoom && state.person.location == "4th_Mackenzie") { // && person in room or at home set movement info null 
         state.info.room_id_leaving = "";
         state.info.room_id_entering = "";
      } else if (state.SpentInLocation < TimeAtHome && state.person.location == "home") {
         state.info.room_id_leaving = "";
         state.info.room_id_entering = "";
      } else if (state.SpentInLocation == TimeInRoom && state.person.location == "4th_Mackenzie") { 
         state.info.room_id_leaving = state.person.location; // Set to "room location"
         state.info.room_id_entering = "home";
         state.info.LeavingCampusFalseEnteringTrue = false;
         state.person.location = "home";
         state.SpentInLocation = TIME();
      } else if (state.SpentInLocation >= TimeAtHome && state.person.location == "home") { // same for previous
         state.info.room_id_leaving = state.person.location;
         state.info.room_id_entering = "4th_Mackenzie"; // Set to "room location"
         state.info.LeavingCampusFalseEnteringTrue = true;
         state.person.location = "4th_Mackenzie";
         state.SpentInLocation = TIME();
      }

      for (int i = 0; i < 4; i++) {
         if (state.person.behaviourRulesPerson[i].SafeDistanceProbability == "Low") {
            int a = (rand() % 100);
            if (a < 90) {
               state.DistanceFromPeople = rand() % 4 + 1;
            } else {
               state.DistanceFromPeople = rand() % 4 + 6;
            }

         } else {
            int a = (rand() % 100);
            if (a < 90) {
               state.DistanceFromPeople = rand() % 4 + 6;
            } else {
               state.DistanceFromPeople = rand() % 4 + 1;
            }
         }
         if (state.person.behaviourRulesPerson[i].MaskWearingProbability == "Low") {
            int a = (rand() % 100);
            if (a < 90) {
               state.MaskWearing = false;
            } else {
               state.MaskWearing = true;
            }

         } else {
            int a = (rand() % 100);
            if (a < 90) {
               state.MaskWearing = true;
            } else {
               state.MaskWearing = false;
            }
         }
        
         // cout<<state.person.behaviourRulesPerson[1].MaskWearingProbability<<"\n";
         // cout<<state.person.behaviourRulesPerson[2].MaskWearingProbability<<"\n";
         // cout<<state.person.behaviourRulesPerson[3].MaskWearingProbability<<"\n";
      }
      //cout<< state.SpentInLocation<<"\n";

   }

   // Exeternal transition
   void external_transition(TIME e, typename make_message_bags < input_ports > ::type mbs) {

      vector <bool> msg_health_in = get_messages < typename BehaviourRules_Ports::BehaviourRulesInHealth > (mbs);
      if (msg_health_in.size() > 1) {
         assert(false);
      } else if (msg_health_in.size() == 1) {
         state.person.isSick = msg_health_in[0];
      }
      vector <room_specs> msg_in = get_messages < typename BehaviourRules_Ports::BehaviourRulesInRoom > (mbs);
      for (int i = 0; i < msg_in.size(); i++) {
         state.ViralLoad += msg_in[i].viral_particles;
      }
      if (state.ViralLoad > 1000) {
         state.person.isSick = true;
      }
      cout << state.SpentInLocation << "\n";
      state.SpentInLocation += e;
      cout << state.SpentInLocation << "\n";

   }

   void confluence_transition(TIME e, typename make_message_bags < input_ports > ::type mbs) {
      internal_transition();
      external_transition(TIME(), std::move(mbs));

   }

   //Output Function
   // use movement info for defining the output for InTrueOutFalse, only generate output for non empty strings
   typename make_message_bags < output_ports > ::type output() const {
      typename make_message_bags < output_ports > ::type bags;
      person_node person_output;

      if (state.info.room_id_leaving != "") {
         person_output.Person_ID = state.person.iD;
         person_output.room_ID = state.person.location;
         person_output.InTrueOutFalse = state.info.LeavingCampusFalseEnteringTrue;
         person_output.distance_from_people = state.DistanceFromPeople;
         person_output.mask_wearing = state.MaskWearing;
         person_output.IsSick = state.person.isSick;

         get_messages < typename BehaviourRules_Ports::BehaviourRulesOut > (bags).push_back(person_output);
      }

      if (state.info.room_id_entering != "") {
         person_output.Person_ID = state.person.iD;
         person_output.room_ID = state.person.location;
         person_output.InTrueOutFalse = state.info.LeavingCampusFalseEnteringTrue;
         person_output.distance_from_people = state.DistanceFromPeople;
         person_output.mask_wearing = state.MaskWearing;
         person_output.IsSick = state.person.isSick;

         get_messages < typename BehaviourRules_Ports::BehaviourRulesOut > (bags).push_back(person_output);
      }

      return bags;
   }

   //time advance function
   TIME time_advance() const {
      TIME next_internal;
      if (state.SpentInLocation < TimeInRoom && state.person.location == "4th_Mackenzie") { // in room  
         next_internal = TimeInRoom - state.SpentInLocation;
      } else if (state.SpentInLocation < TimeAtHome && state.person.location == "home") {
         next_internal = TimeAtHome - state.SpentInLocation;
      } else { // if NOT null, ta 0
         next_internal = TIME();
      // } else {
      //    next_internal = numeric_limits < TIME > ::infinity();
       }

      return next_internal;
   
   };

   friend ostringstream & operator << (ostringstream & os,
      const typename BehaviourRules < TIME > ::state_type & i) { // change variables 

      if (i.info.room_id_leaving != "") {
         os << " Room leaving: " << i.info.room_id_leaving << "\n";
      }
      if (i.info.room_id_entering != "") {
         os << " Room entering " << i.info.room_id_entering << "\n";
      }

      os << " Person ID: " << i.person.iD << " Sick? " << i.person.isSick << " Mask wearing: " << i.MaskWearing <<
         " Room ID: " << i.person.location << " Distance from people: " << i.DistanceFromPeople << "\n";

      return os;
   }
   #endif
};