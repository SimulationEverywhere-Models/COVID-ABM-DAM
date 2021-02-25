#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>


using namespace std;


struct room_specs{
room_specs(){}
room_specs(long i_person_id, int i_people_in_room, float i_room_size, float i_viral_particles, string i_room_ID)
    :Person_ID(i_person_id), people_in_room(i_people_in_room), room_size(i_room_size), viral_particles(i_viral_particles), room_ID(i_room_ID){}

    long Person_ID;
	int people_in_room;
	float room_size;
	float viral_particles;
	string room_ID;
};
struct person_node{
person_node(){}
person_node(long i_person_id, bool i_InTrueOutFalse, bool i_IsSick, bool i_mask_wearing, string i_room_ID, float i_distance_from_people)
    :Person_ID(i_person_id), InTrueOutFalse(i_InTrueOutFalse), IsSick(i_IsSick), mask_wearing(i_mask_wearing), room_ID(i_room_ID), distance_from_people(i_distance_from_people){}

    long Person_ID; //change back to long when xml is fixed
    bool InTrueOutFalse;
	bool IsSick;
	bool mask_wearing;
	string room_ID;
    float distance_from_people;
};

istream& operator>> (istream& is, room_specs& msg);
istream& operator>> (istream& is, person_node& msg);
ostream& operator<< (ostream& os, room_specs& msg);
ostream& operator<< (ostream& os, person_node& msg);

#endif