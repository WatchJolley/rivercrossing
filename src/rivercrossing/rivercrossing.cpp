#include "rivercrossing/rivercrossing.h"
#include "rivercrossing/movement_network.h"

bool RiverCrossing::optimisationCheck(int x, int y)
{
  double startA = SM.getActivity(x, y);
  double bestA = startA;
  int newX = x;
  int newY = y;

  for (int x2 = f(x, -2, RIVER::x); x2 <= f(x, 2, RIVER::x); x2++) {
    for (int y2 = f(y, -2, RIVER::y); y2 <= f(y, 2, RIVER::y); y2++) {
      if ( !((x2 == x) && (y2 == y)) ) {
        if ( SM.getActivity(x2, y2) > bestA) {
          bestA = SM.getActivity(x2, y2);
          newX = x2;
          newY = y2;
        }
      }
    }
  }

  x = newX;
  y = newY;

  // if no activity value is larger then exit
  if (startA == bestA) {
      return true;
  }
  return false;
}

void RiverCrossing::action(bool pickUpAction)
{
    agent.carrying = pickUpAction;
    worlds.cell[ agent.x ][ agent.y ] = EMPTYCELL;
    updateNetworks();
}

bool RiverCrossing::move() {
    if (USE_MOVEMENT)
    {
        vector<double> input;
        vector<double> outputs;
        int active  = 0;
        double normActivity = SM.returnLargestActivity();

        movement.loadInputs(agent.x, agent.y, SM, input, normActivity);
        movement.update(input);
        movement.getOutputs(outputs);

        for (int i = 0; i < 8; i++)
        {
            if (  outputs.at(active) < outputs.at(i) ) active = i;
        }

        return updatePostions( postions[active][0], postions[active][1] );

    } else {

        int x = agent.x;
        int y = agent.y;
        int newX = x;
        int newY = y;

        double startA = SM.getActivity(x, y);
        double bestA  = startA;

        for (int x2 = f(x, -1, RIVER::x); x2 <= f(x, 1, RIVER::x); x2++) {
          for (int y2 = f(y, -1, RIVER::y); y2 <= f(y, 1, RIVER::y); y2++) {
            if ( !((x2 == x) && (y2 == y)) ) {
              if ( SM.getActivity(x2, y2) > bestA) {
                bestA = SM.getActivity(x2, y2);
                newX = x2;
                newY = y2;
              }
            }
          }
        }

        agent.x = newX;
        agent.y = newY;

        // if no activity value is larger then exit
        if (startA == bestA)
            if(optimisationCheck(agent.x, agent.y))
                return true;

        return false;
    }
}

int RiverCrossing::step()
{
    bool failed = move();
    worlds.updateHeatmap(agent.x,agent.y);

    if (failed) return TRAP;

    updateNetworks(true);

    // the action of an animat is based on the postion
    if (!DN_network.compare("RGB")) {
        vector<double> inputs;
        int o = worlds.getCell( agent.x, agent.y );
        DN.loadInputs(worlds, inputs, o, agent.carrying);
        DN.update(inputs);
    }

    double pickUp = DN.getAction();

    // pick up Stone
    if ((!agent.carrying) && ( pickUp >= 0.1f) && (worlds.cell[agent.x][agent.y] == STONE))
      action(true);

    // put down Stone
    if ((agent.carrying) && ( pickUp <= -0.1f) && (worlds.cell[agent.x][agent.y] == WATER))
      action(false);

    agent.age += 1;

    if (agent.debug == true) {
        worlds.printAnimat(agent.x, agent.y);
        SM.print(agent.x, agent.y);
    }

    return worlds.getCell( agent.x, agent.y );
}

int RiverCrossing::run(int river, bool analyse, int seed, bool debug)
{
    if (seed)
        randomise(seed);
    complete = false;
    worlds = dsWorld(river);
    agent = animat(getStarting());
    worlds.updateHeatmap(agent.x,agent.y);

    if (debug) agent.debug = true;
    updateNetworks();

    while (true) {
        int c = 0;
        c = step();
        if (c == RESOURCE) {
            if (agent.debug == true) worlds.printHeatmap();
            if (analyse) ages[river].push_back(agent.age);
            complete = true;
            return 1;
        }
        if (c == TRAP || c == WATER) {
            agent.death = c;
            if (agent.debug == true) worlds.printHeatmap();
            return 0;
        }
        if (agent.age >= 200) {
            agent.death = AGE;
            if (agent.debug == true) worlds.printHeatmap();
            return 0;
        }
    }
}

double RiverCrossing::evaluate()
{
  int fitness = 0;
  for (int world = 0; world < 4; world++)
  {
      if (world != fitness ) break;
      fitness += run(world);
  }
 return fitness;
}

RiverCrossing::RiverCrossing(vector<double> genome)
{
    // create seperate genomes for each network
    vector<int> genomeSplit = {config::DN_size, config::SM_size, config::MV_size};
    vector<double> genomes[genomeSplit.size()];

    for (int i = 0; i < genomeSplit.size(); i++)
    {
        if (genomeSplit.at(i) != 0)
        {
            auto it = std::next( genome.begin(), genomeSplit.at(i));
            std::move(genome.begin(), it, std::back_inserter(genomes[i]));  // ##
            genome.erase(genome.begin(), it);
        }
    }

   // configure network types
   this->DN_network = config::DN_network;
   this->SM_network = config::SM_network;
   DN.configureType(DN_network);
   SM.configureType(SM_network);

   // configure movement network
   this->USE_MOVEMENT = config::USE_MOVEMENT;

    // build DN
    if (genomes[0].size() > 0)
    { DN.buildNetwork(genomes[0]);
    } else { DN.buildNetwork(); }

    // build SM
    if (genomes[1].size() > 0)
    { SM.buildNetwork(genomes[1]);
    } else { SM.buildNetwork(); }

    // build Movement
    if (USE_MOVEMENT)
      movement.buildNetwork(genomes[2]);

  for(int i = 0; i < 5; i++) iotas.push_back( 0.0 );
  needToUpdateShunting = false;
  complete = false;
}
