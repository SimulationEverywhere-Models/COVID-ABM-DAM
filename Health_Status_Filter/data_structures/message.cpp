#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const health_status& msg) {
  os << msg.Person_ID << " " << msg.IsSick;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/



istream& operator>> (istream& is, health_status& msg) {
  is >> msg.Person_ID;
  is >> msg.IsSick;

  return is;
}