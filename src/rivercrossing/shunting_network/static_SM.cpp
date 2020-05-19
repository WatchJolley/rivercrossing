#include "rivercrossing/shunting_network/static_SM.h"


void static_SM::activate()
{
    largestActivity = 0.0;
    int gX = RIVER::x;
    int gY = RIVER::y;
    int cycleMax = (gX*gY)/8;

    for (int x=0;x<gX;x++) for (int y=0;y<gY;y++)
      activity[x][y] = 0;

    for (int cycle = 0; cycle < cycleMax; cycle++) {
      float aNEW[gX][gY];
      for (int x=0;x<gX;x++) for (int y=0;y<gY;y++) {
        aNEW[x][y] = iota[x][y] + 0.125 * (m(x-1,y) +m(x,y-1) +m(x+1,y) +m(x,y+1) + m(x-1,y-1) +m(x-1,y+1) +m(x+1,y+1) +m(x+1,y-1)  );
        if (iota[x][y] == -1 ) aNEW[x][y] = -1;
        if (aNEW[x][y] > 15) aNEW[x][y]=15.0;
      }

      for (int x=0;x<gX;x++) for (int y=0;y<gY;y++)
      {
        activity[x][y] = aNEW[x][y];

        // for normalisation purposes
        if (activity[x][y] > largestActivity) largestActivity = activity[x][y];
      }

    }
}
