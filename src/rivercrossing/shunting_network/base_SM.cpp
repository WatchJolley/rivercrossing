#include "rivercrossing/shunting_network/base_SM.h"

void base_SM::init() {
  for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++) {
        iota[x][y] = 0.0;
        activity[x][y] = 0.0;
    }
}

void base_SM::print(int Cx, int Cy) {
    for (int y = 0; y < RIVER::x; y++) {
      for (int x = 0; x < RIVER::y; x++) {
      if ((Cx == x) && (Cy == y)) {
        cout << "A";
      } else {
        cout << activity[x][y];
      }
      if (x != RIVER::y-1) cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;
}

void base_SM::printNorm() {

   double newActivity[RIVER::x][RIVER::y];
   double normActivity[RIVER::x][RIVER::y];
   int lowestVal = -999;

   // update new Activity Landscape
   for (int y = 0; y < RIVER::x; y++) for (int x = 0; x < RIVER::y; x++) newActivity[x][y] = activity[x][y];
   for (int y = 0; y < RIVER::x; y++) for (int x = 0; x < RIVER::y; x++) normActivity[x][y] = 0.0;

  int networkSize =  (RIVER::x*RIVER::y);

  for (int r = 0; r < networkSize ; r++) {
    double positions[9][2];
    double largest = lowestVal;
    int duplicates = 0;
    int filled = 0;

    for (int y = 0; y < 9; y++) for (int x = 0; x < 2; x++) positions[x][y] = 0.0;

    // find Largest Value
    for (int y = 0; y < RIVER::x; y++) {
      for (int x = 0; x < RIVER::y; x++) {
          if (newActivity[x][y] > largest) {
              largest = newActivity[x][y];
              positions[0][0] = x;
              positions[0][1] = y;
          }
      }
    }

    // check for duplicates
    for (int y = 0; y < RIVER::x; y++) {
      for (int x = 0; x < RIVER::y; x++) {
      if (newActivity[x][y] == largest) {
          if (newActivity[x][y] == lowestVal)
          { // filled indicates when the landscape has been fully scanned
              filled++;
          } else {
            if ( (positions[0][0] != x) && (positions[0][1] != y) )
            { // if there are more than one $Largest
              newActivity[x][y] = lowestVal;
              duplicates += 1;
              positions[duplicates][0] = x;
              positions[duplicates][1] = y;
            }
          }
        }
      }
    }

    // when the network is fully scanned
    if (filled == networkSize)
      break;

    if (duplicates > 0)
    { // if duplicates print same value
      for (int d = 0; d <= duplicates; d++) {
        int nX = positions[d][0];
        int nY = positions[d][1];
        normActivity[nX][nY]  = networkSize-r;
        newActivity[nX][nY]   = lowestVal;
      }
    } else
    { // if single print single value
        int nX = positions[0][0];
        int nY = positions[0][1];
        normActivity[nX][nY]  = networkSize-r;
        newActivity[nX][nY]   = lowestVal;
    }
  }

  // print landscape
  for (int y = 0; y < RIVER::x; y++) {
    for (int x = 0; x < RIVER::y; x++) {
      cout << normActivity[y][x];
      if (x != RIVER::x-1) cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;

}
