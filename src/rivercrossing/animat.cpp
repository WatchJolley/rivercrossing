#include "rivercrossing/animat.h"

animat::animat(int y)
{
  this->age = 0;
  this->x = 0;
  this->carrying = false;
  this->y =  random( 20 );
  this->y =  y;
  this->debug = false;
  this->death = 0;
}
