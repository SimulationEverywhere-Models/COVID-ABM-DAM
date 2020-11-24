#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** health_status ****************/
/*******************************************/
struct health_status{
    health_status(){}
    health_status(long i_person_id, bool i_issick)
   :Person_ID(i_person_id), IsSick(i_issick){}
    
    long Person_ID; 
    bool IsSick;
    
};

istream& operator>> (istream& is, health_status& msg);

ostream& operator<< (ostream& os, const health_status& msg);


#endif // BOOST_SIMULATION_MESSAGE_HPP
