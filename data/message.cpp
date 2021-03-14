#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

/************* Output stream ************************/
//output content of the structure

ostream& operator<<(ostream& os, const health_status& msg) {
  os << msg.Person_ID << " " << msg.IsSick;
  return os;
}

ostream& operator<<(ostream& os, const room_specs& msg) {
    os << msg.Person_ID << " " << msg.people_in_room << " " << msg.room_size << " " << msg.viral_particles << " " << msg.room_ID << " " << msg.number_of_mask_wearers << " " << msg.number_of_social_distancing;
    return os;
}

ostream& operator<<(ostream& os, const person_node& msg) {
    os << msg.Person_ID << " " << msg.InTrueOutFalse << " " << msg.IsSick << " " << msg.mask_wearing << " " << msg.room_ID << " " << msg.distance_from_people;
    return os;
}

/************* Input stream ************************/
//fill structure with data from file

istream& operator>> (istream& is, health_status& msg) {
  is >> msg.Person_ID;
  is >> msg.IsSick;

  return is;
}

istream& operator>> (istream& is, room_specs& msg) {
    is >> msg.Person_ID;
    is >> msg.people_in_room;
    is >> msg.room_size;
    is >> msg.viral_particles;
    is >> msg.room_ID;
    is >> msg.number_of_mask_wearers;
    is >> msg.number_of_social_distancing;

 return is; 
}

istream& operator>> (istream& is, person_node& msg) {
    is >> msg.Person_ID;
    is >> msg.InTrueOutFalse;
    is >> msg.IsSick;
    is >> msg.mask_wearing;
    is >> msg.room_ID;
    is >> msg.distance_from_people;

    return is;
}