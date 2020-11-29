#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

struct room_specs{
room_specs(){}
room_specs(long i_person_id, int i_people_in_room, float i_room_size, float i_viral_particles, long i_room_ID)
    :Person_ID(i_person_id), people_in_room(i_people_in_room), room_size(i_room_size), viral_particles(i_viral_particles), room_ID(i_room_ID){}

    long Person_ID;
	int people_in_room;
	float room_size;
	float viral_particles;
	long room_ID;
};

istream& operator>> (istream& is, room_specs& msg);
ostream& operator<< (ostream& os, room_specs& msg);

#endif
