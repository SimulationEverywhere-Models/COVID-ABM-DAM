#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>


using namespace std;

/************** Messages *******************/

struct health_status{
    health_status(){}
    health_status(long i_person_id_health, bool i_issick)
   :Person_ID_health(i_person_id_health), IsSick(i_issick){}
    
    long Person_ID_health; 
    bool IsSick;
};

struct room_specs{
room_specs(){}
room_specs(long i_person_id_room, int i_people_in_room, float i_room_size, float i_viral_particles, string i_room_ID_room)
    :Person_ID_room(i_person_id_room), people_in_room(i_people_in_room), room_size(i_room_size), viral_particles(i_viral_particles), room_ID_room(i_room_ID_room){}

    long Person_ID_room;
	int people_in_room;
	float room_size;
	float viral_particles;
	string room_ID_room;
};

struct person_node{
person_node(){}
person_node(long i_person_id, bool i_InTrueOutFalse, bool i_IsSick, bool i_mask_wearing, string i_room_ID_person, float i_distance_from_people)
    :Person_ID(i_person_id), InTrueOutFalse(i_InTrueOutFalse), IsSick(i_IsSick), mask_wearing(i_mask_wearing), room_ID_person(i_room_ID_person), distance_from_people(i_distance_from_people){}

    long Person_ID; 
    bool InTrueOutFalse;
	bool IsSick;
	bool mask_wearing;
	string room_ID_person;
    float distance_from_people;
};

/***************** Input streams ******************/

istream& operator>> (istream& is, health_status& msg);
istream& operator>> (istream& is, room_specs& msg);
istream& operator>> (istream& is, person_node& msg);


/***************** Output streams ******************/

ostream& operator<< (ostream& os, const health_status& msg);
ostream& operator<< (ostream& os, room_specs& msg);
ostream& operator<< (ostream& os, person_node& msg);

#endif
