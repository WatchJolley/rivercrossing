#include "rivercrossing/shunting_network/CNN_SM.h"

void CNN_SM::buildNetwork()
{
    for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++)
        CNN_output[x][y] = 0;

    int count = 0;
    CNN_filter pos( 3, evolved_genome, count);
    filters.push_back(pos);
}

void CNN_SM::buildNetwork(vector<double> genome)
{
    for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++)
        CNN_output[x][y] = 0;

    int count = 0;
    CNN_filter pos( 3, genome, count);
    filters.push_back(pos);
}

void CNN_SM::activate()
{
    largestActivity = 0.0;

    for(int i = 0; i < 50; i++) {
        for(int x=0; x < RIVER::x; x++) for(int y=0; y < RIVER::y; y++)   {

          double sum = 0.0;
          int strtPos =       -(filters.at(0).getSize()/2);
          int endPos  =	filters.at(0).getSize() - abs(strtPos);

          for(int fx=strtPos; fx < endPos; fx++) for(int fy=strtPos; fy < endPos; fy++)   {
              double cell =  getTemp(fx + x, fy + y);
              double weight = filters.at(0).getWeight( fx + abs(strtPos) , fy + abs(strtPos) );
              sum += cell * weight;
          }
          sum += iota[x][y] * 1.0;

          // limit outputs to a spefific value
          activity[x][y] = min(sum, RIVER::max_iota);

          // for normalisation purposes
          if (activity[x][y] > largestActivity) largestActivity = activity[x][y];
      }

      // ReLU
      for(int x=0; x < RIVER::x; x++) for(int y=0; y < RIVER::y; y++)
          CNN_output[x][y] = max(activity[x][y], 0.0);
  }
}

double CNN_SM::getTemp(int x, int y)
{
    if (( x < RIVER::x) && ( x >= 0) && ( y < RIVER::y) && ( y >= 0)) return CNN_output[x][y];
    return 0;
}

double CNN_SM::getCell(int x, int y)
{
  if (( x < RIVER::x) && ( x >= 0) && ( y < RIVER::y) && ( y >= 0)) return CNN_output[x][y];
      return -999;
}

double CNN_SM::getCell2(int x, int y, double activity[20][20])
{
  if (( x < RIVER::x) && ( x >= 0) && ( y < RIVER::y) && ( y >= 0))
    return activity[x][y];
      return -1000;
}

bool CNN_SM::inRange(int x, int y)
{
  if (( x < RIVER::x) && ( x >= 0) && ( y < RIVER::y) && ( y >= 0)) return true;
      return false;
}

void CNN_SM::printPath() {

  double newActivity[RIVER::x][RIVER::y];
  double newActivity1[RIVER::x][RIVER::y];
  double normActivity[RIVER::x][RIVER::y];

  int lowestVal   =  -999;
  int strtPos     =  -1;
  int endPos      =   1;
  int networkSize =  (RIVER::x*RIVER::y);

  for (int y = 0; y < RIVER::x; y++) for (int x = 0; x < RIVER::y; x++) newActivity[x][y]  = 0.0;

  for (int y = 0; y < RIVER::x; y++) {
    for (int x = 0; x < RIVER::y; x++) {
      cout << CNN_output[y][x];
      if (x != RIVER::x-1) cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;


  // remove non-essential activity values
  for(int x=0; x < RIVER::x; x++) for(int y=0; y < RIVER::y; y++)   {

    double cellVal    = CNN_output[x][y];
    bool   keep       =  true;
    int    higherVals =   0;

    for(int fx=strtPos; fx <= endPos; fx++) for(int fy=strtPos; fy <= endPos; fy++)   {
        double c =  getCell(fx + x, fy + y);
        if (c > cellVal) higherVals += 1;
        if (higherVals > 1) keep = false;
    }

    double newA;

    if (keep) {
        newA = cellVal;
    } else {
        newA = 0;
    }

    newActivity[x][y] = newA;
  }

  for (int y = 0; y < RIVER::x; y++) {
    for (int x = 0; x < RIVER::y; x++) {
      cout << newActivity[y][x];
      if (x != RIVER::x-1) cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;

  //------------------------------------------------------------------------------------------------

  // update norm Activity Landscape
  for (int y = 0; y < RIVER::x; y++) for (int x = 0; x < RIVER::y; x++) normActivity[x][y] = 0.0;

  for(int x=0; x < RIVER::x; x++) for(int y=0; y < RIVER::y; y++) {

      // update new Activity Landscape
      for (int yy = 0; yy < RIVER::x; yy++) for (int xx = 0; xx < RIVER::y; xx++) newActivity1[xx][yy] = newActivity[xx][yy];

      for (int r = 9; r > 0 ; r--) {
        double  positions[9][2];
        double  largest = lowestVal;
        int     duplicates = 0;
        int     filled = 0;

        for (int yy = 0; yy < 9; yy++) for (int xx = 0; xx < 2; xx++) positions[xx][yy] = 0.0;

        // find Largest Value
        for(int fx=strtPos; fx <= endPos; fx++)     {
          for(int fy=strtPos; fy <= endPos; fy++)   {
            int xx = fx + x;
            int yy = fy + y;
            if (inRange(xx,yy))
            {
              double cellVal = newActivity1[xx][yy];
              if (cellVal > largest) {
                largest = cellVal;
                positions[0][0] = xx;
                positions[0][1] = yy;
              }
            }
          }
        }

        // check for duplicates
        for(int fx=strtPos; fx <= endPos; fx++) for(int fy=strtPos; fy <= endPos; fy++)   {
            int xx = fx + x;
            int yy = fy + y;
            if (inRange(xx,yy)) {
              double cellVal = newActivity1[xx][yy];
              if (cellVal == largest) {
                if (cellVal == lowestVal)
                { // filled indicates when the landscape has been fully scanned
                filled++;
                } else {
                if ( (positions[0][0] != xx) && (positions[0][1] != yy) )
                  { // if there are more than one $Largest
                    newActivity1[xx][yy] = lowestVal;
                    duplicates += 1;
                    positions[duplicates][0] = xx;
                    positions[duplicates][1] = yy;
                   }
                 }
               }
             } else {
                filled++;
             }
          }

        // when the network is fully scanned
        if (filled == 9) break;

        if (duplicates > 0)
        { // if duplicates print same value
          for (int d = 0; d <= duplicates; d++) {
            int newX = positions[d][0];
            int newY = positions[d][1];
            normActivity[newX][newY] = networkSize-r;
            newActivity1[newX][newY] = lowestVal;
          }
          r = r - duplicates;
        } else
        { // if single print single value
            int newX = positions[0][0];
            int newY = positions[0][1];
            normActivity[newX][newY] = networkSize-r;
            newActivity1[newX][newY] = lowestVal;
        }

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
