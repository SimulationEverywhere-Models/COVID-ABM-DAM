#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

//Output content of the structure
ostream& operator<<(ostream& os, const room_specs_out& msg) {
    os << msg.Person_ID << " " << msg.people_in_room << " " << msg.room_size << " " << msg.viral_particles << " " << msg.room_ID;
    return os;
}

//Fill structure with data from file
istream& operator>> (istream& is, room_specs_in& msg) {
    is >> msg.Person_ID;
    is >> msg.people_in_room;
    is >> msg.room_size;
    is >> msg.viral_particles;
    is >> msg.room_ID;

 return is; 
}

