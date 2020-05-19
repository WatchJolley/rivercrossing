#ifndef RIVERCROSSING_H
#define RIVERCROSSING_H

#include <iostream>
#include <numeric>

#include "tools/defines.h"
#include "tools/random.h"
#include "tools/config.h"

#include "rivercrossing/animat.h"
#include "rivercrossing/shuntingmodel.h"
#include "rivercrossing/decisionnetwork.h"
#include "rivercrossing/movement_network.h"
#include "rivercrossing/dsworld.h"


using namespace std;

class RiverCrossing
{
private:
  // variables
  dsWorld         worlds;
  animat          agent;
  DecisionNetwork DN;
  ShuntingModel   SM;
  movement_network movement;
  vector<double>  iotas;
  int             seed;
  bool            needToUpdateShunting;
  bool            complete;
  vector<int>     ages[4];
  const int postions[8][2] = { // up, down, right, left
                              {1, 0}    , {-1,0},
                              {0,1}     , {0,-1},
                              {-1, 1}   , {1,1},
                              {-1,-1}   , {1,-1}
                             };

  std::string DN_network;
  std::string SM_network;
  bool USE_MOVEMENT;


  // methods
  int step();

  void action(bool pickUpAction);
  bool optimisationCheck(int x, int y);
  bool move();

  // inline methods
  double  p(double f) {
    f = (f > 0) ? f : 0;
    return f;
  }

  int f(int value, int addition, int axisSize)
  {
    int move = value + addition;
    int result = ( inRange(move, axisSize) ) ? move : value;
    return result;
  }

  bool inRange(int location, int xisSize)
  {
    if ( (0 <= location) && (location < xisSize ) ) {
       return true;
    }
    return false;
  }

  int getStarting()
  {
      int y, c;
      do {
        y = random(RIVER::y);
        c = worlds.getCell(0,y);
      } while (c != 0);

     return y;
  }

  bool updatePostions(int x, int y)
  {
      // cout << "Actions X : " << x << " | Y : " << y << endl;
      // cout << "Before X : " << agent.x << " | Y : " << agent.y << endl;
      if ( inRange( agent.x + x, RIVER::x ) ) {
          agent.x += x;
      } else {
          return true;
      }
      if ( inRange( agent.y + y, RIVER::y ) ) {
          agent.y += y;
      } else {
          return true;
      }
      // cout << "After X : " << agent.x << " | Y : " << agent.y << endl;
      return false;
  }

  void updateDN() {

   vector<double> inputs;
   if (!DN_network.compare("object"))
   {
       int o = worlds.getCell( agent.x, agent.y );
       DN.loadInputs(worlds, inputs, o, agent.carrying);
       DN.update(inputs);

       // check if SM needs updating
       for (int o = 0; o < RIVER::numOfObjects; o++)
           if (DN.getOutput(o) != iotas.at(o))
               needToUpdateShunting = true;

       for (int o = 0; o < RIVER::numOfObjects; o++) iotas.at(o) = DN.getOutput(o);
   } else if (!DN_network.compare("RGB")) {

       for (int o = 0; o < RIVER::numOfObjects; o++)
       {
           DN.loadInputs(worlds, inputs, o, agent.carrying);
           DN.update(inputs);

           // checks if SM needs updating
           if (DN.getOutput(1) != iotas.at(o))
                needToUpdateShunting = true;

           iotas.at(o) = DN.getOutput(1);

           // remove empty space from Shutning
           if (EMPTYCELL == o) iotas.at(o) = 0;
       }

   }
  };

  void updateSM() {
    bool isObject = ((!DN_network.compare("RGB"))) ? false : true;
    for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++) {
       double iota = worlds.getIota(iotas, x, y, isObject);
       SM.updateIotas( x, y, iota );
    }
    SM.activate();
  };

  void updateNetworks(bool checkSM=false)
  {
      updateDN();

      if(checkSM){
          if (needToUpdateShunting) {
            needToUpdateShunting = false;
            updateSM();
          }
      } else {
          updateSM();
      }
  }

public:
  RiverCrossing( vector<double> genome );
  int run(int river=0, bool analyse=false, int seed=0, bool debug=false);
  double    evaluate();

  double    returnAvgAge(int river) { return accumulate( ages[river].begin(), ages[river].end(), 0.0)/ages[river].size(); };
  dsWorld*  returnWorld() { return &worlds; };
  int       returnAge() { return agent.age; };
  int       returnDeath() { return agent.death; };
};

#endif // RIVERCROSSING_H
