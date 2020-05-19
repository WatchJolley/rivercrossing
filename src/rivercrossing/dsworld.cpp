#include "rivercrossing/dsworld.h"

dsWorld::dsWorld() { build(0); }

dsWorld::dsWorld(int riverWidth) { build(riverWidth); }

dsWorld::dsWorld(int riverWidth, int worldseed) {
  int seed = worldseed + (riverWidth * worldseed);
  randomise(seed);
  build(riverWidth);
}

void dsWorld::build(int riverWidth) {
  river = riverWidth;
  int c;

  // initialise Heatmaps
  for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++) heatmap[x][y] = 0.0;

  // initialise Cells
  for (int x = 0; x < RIVER::x; x++) for (int y = 0; y < RIVER::y; y++) cell[x][y] = EMPTYCELL;

  // create River
  int riverPosition = ((RIVER::x / 2) + (((RIVER::x / 2) - 2) / 2));

  for (int x = riverPosition; x < riverPosition + (riverWidth); x++) {
    for (int y = 0; y < RIVER::y; y++) {
      cell[x][y] = WATER;
    }
  }

  // place Traps
  float trapProb = int((RIVER::x * RIVER::y) * 0.0175);
  int numTraps = (trapProb < 1) ? 1 : int(trapProb);

    for (int n = 0; n < numTraps; n++) {
      int x;
      int y;
      do {
        x = random(RIVER::x);
        y = random(RIVER::y);
        c = cell[x][y];
      } while (c & RESOURCE);

      cell[x][y] = TRAP;
  }

  // place stone
  for (int n = 0; n < int((RIVER::x * RIVER::x) * 0.0425) + riverWidth; n++) {
    int x;
    int y;
    do {
      x = random(RIVER::x);
      y = random(RIVER::y);
      c = cell[x][y];
    } while (x > riverPosition || c & WATER || c & TRAP);
    cell[x][y] = STONE;
  }

  // placing resource
  int resourcePos = ( (RIVER::x - 1) - (riverPosition + (riverWidth)) )/2;
  cell[ (RIVER::x-1) - resourcePos ][ int(RIVER::y / 2) ] = RESOURCE;

}


void dsWorld::printAnimat(int location_x, int location_y) {
  for (int y = 0; y < RIVER::x; y++) {
    for (int x = 0; x < RIVER::y; x++) {
      if (x == location_x && (y == location_y)) {
          cout << "A";
        } else {
          cout << cell[x][y];
        }
      cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;
}

double dsWorld::getIota(vector<double> iota, int x, int y, bool isObject)
{
    int cell = getCell(x, y);
    double cellIota = iota.at(cell);

    // if using object DN , first input is not used
    if (isObject)
        if (cell == 0) return 0.0;

    return cellIota;
}

void dsWorld::getRGB(int object, vector<double> &RGB)
{
    double R,G,B;
    switch(object) {
        case EMPTYCELL :
            R = -1.0;
            G = -1.0;
            B = -1.0;
            break;
        case RESOURCE :
            R = 255.0;
            G = 255.0;
            B = 0.0;
            break;
        case STONE :
            R = 128.0;
            G = 128.0;
            B = 128.0;
            break;
        case WATER :
            R = 0.0;
            G = 128.0;
            B = 255.0;
            break;
        case TRAP :
            R = 255.0;
            G = 0.0;
            B = 0.0;
            break;
    }
    RGB.push_back(R/255.0);
    RGB.push_back(G/255.0);
    RGB.push_back(B/255.0);
}

void dsWorld::printHeatmap() {
    for (int y = 0; y < RIVER::x; y++) {
      for (int x = 0; x < RIVER::y; x++) {
        cout << heatmap[x][y] * 10;
      if (x != RIVER::x-1) cout << ",";
    }
    cout << " " << endl;
  }
  cout << " " << endl;

}
