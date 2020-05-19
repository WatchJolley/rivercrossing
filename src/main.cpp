#pragma warning(disable:4786) // disable debug warning

#include "tools/config.h"
#include "tools/threadpool.h"
#include "rivercrossing/rivercrossing.h"

#include <algorithm>

class simulation
{
    private:
        thread_pool *pool;

        double range (double value)
        {
          value = (value >  1.0) ?  1.0 : value;
          value = (value < -1.0) ? -1.0 : value;
          return value;
        }

        inline void print_genome(ga_vector &gav)
        {
          cout << " Genome: ";
          for(int i = 0; i < config::GA_GENOME; i++) cout << gav[0].genome[i] << " , ";
        }

        void init_population(ga_vector &population,
                             ga_vector &buffer )
        {
          // check for snapshot, if so load
          // else initalise new population with random weights
          if (!config::load_snapshot(population))
          {
              for (int i=0; i< config::GA_POPSIZE; i++) {
                  ga_struct animat;
                  animat.fitness = 0;
                  animat.complete = false;

                  for (int g=0; g< config::GA_GENOME; g++)
                    animat.genome.push_back( range(randomGaussianDouble(0.0, 0.5)) );

                  population.push_back(animat);
              }
          }

          buffer.resize(config::GA_POPSIZE);
        }

        void calc_fitness(int i, ga_vector &population)
        {
          ga_struct *animat = &population[i];
          double fitness = 0;
          int cycle = 10;

          for (int i = 0; i < cycle; i++)
          {
            RiverCrossing env( animat->genome );
            fitness += env.evaluate();
          }

          animat->complete = true;
          animat->fitness  = fitness/ double(cycle);
        }

        double calc_pop_fitness(ga_vector &population)
        {
            double average = 0.0;

            if (config::THREADS > 1) { // if mutiple threads
                // add all genomes to a thread pool
                for (int i=0; i< config::GA_POPSIZE; i++) {
                    pool->enqueue(bind(&simulation::calc_fitness, this, i, boost::ref(population)));
                }

                // only continue once all the population is complete
                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                for (int i=0; i< config::GA_POPSIZE; i++) {
                    if( population[i].complete == false ) i = 0;
                }
            } else { // if only one thread
                for (int i=0; i< config::GA_POPSIZE; i++)
                    calc_fitness(i, population);
            }

            // load fitness to get average and reset complete status
            for (int i=0; i< config::GA_POPSIZE; i++) {
                average += population[i].fitness;
                population[i].complete = false;
            }

            average = average / config::GA_POPSIZE;
            return average;
        }

        inline void sort_by_fitness(ga_vector &population)
        {
            sort(population.begin(), population.end(), [](ga_struct a, ga_struct b) {
                return (a.fitness > b.fitness);
            });
        }

        void elitism(ga_vector &population,
                        ga_vector &buffer, int esize )
        {
            for (int i=0; i<esize; i++) {
                buffer.at(i).genome = population.at(i).genome;
                buffer.at(i).fitness = population.at(i).fitness;
            }
        }

        void mutate(ga_struct &animat)
        {
            int ipos = random(config::GA_GENOME);
            animat.genome[ipos] += range(randomGaussianDouble(0.0, config::GA_MUTATIONRATE));
            animat.genome[ipos]  = range(animat.genome[ipos]);
        }

        void mate(ga_vector &population, ga_vector &buffer)
        {
            int esize = config::GA_POPSIZE * config::GA_ELITRATE;
            int tsize = population.at(0).genome.size(), spos, i1, i2;

            elitism(population, buffer, esize);

            // mate the rest
            for (int i=esize; i<config::GA_POPSIZE; i++) {
                i1 = random(config::GA_POPSIZE / 2);
                i2 = random(config::GA_POPSIZE / 2);
                spos = random(tsize);

                vector<double> crossover;

                for(int g = tsize-1; g >= 0; g--)
                {
                   if ( g > spos ) {
                    crossover.push_back( population.at(i1).genome.at(g) );
                   } else {
                    crossover.push_back( population.at(i2).genome.at(g) );
                   }
                }

                buffer[i].genome = crossover;
                mutate(buffer[i]);
            }
        }

        inline void print_gen(ga_vector &gav, int gen, double average)
        {
          config::log("Generation: " + std::to_string(gen) +
                      " | Fittest: " + std::to_string(gav[0].fitness) +
                      " | Average: " + std::to_string(average));
        }
        inline void swap(ga_vector *&population,
                         ga_vector *&buffer)
        { ga_vector *temp = population; population = buffer; buffer = temp; }

    public:
        void run_evolution(ga_vector &pop_alpha)
        {
          // set seed
          config::set_seed( return_time() );

          if (config::THREADS > 1) {
              pool = new thread_pool(config::THREADS);
              config::log("Using " + std::to_string(config::THREADS) + " Threads");
          }

          // initalise population
          ga_vector pop_beta;
          ga_vector *population, *buffer;
          init_population(pop_alpha, pop_beta);
          population = &pop_alpha;
          buffer = &pop_beta;


          for (int generation=config::GA_GENERATION; generation < config::GA_MAXGEN; generation++) {
            double average = calc_pop_fitness(*population);       // returns average fitness
            sort_by_fitness(*population);                         // sort them
            print_gen(*population, generation, average);          // print the best one
            config::write_snapshot_file(*population, generation); // create snapshot

            // ending condition - got to highest fitness
            if ((*population)[0].fitness >= 4)
            {
              print_genome(*population);
              break;
            }

            mate(*population, *buffer); // mate the population together
            swap(population, buffer);   // swap buffers
          }

        }
};

void evaluate_individual(ga_vector &population,  int seed=0)
{
    // gets fittest genome from snapshot
    vector<double> genome = population[0].genome;

    // print info for analysis
    config::print_network();

    // set up enviroment varaibles
    double avgAge       = 0.0;
    double fitness      = 0.0;
    double staticFitness= 0.0;
    double equalfitness = 0.0;

    // set up enviroment
    RiverCrossing *env = new RiverCrossing( genome );
    RiverCrossing *staticEnv;

   // set up static movement network
    if (config::USE_MOVEMENT)
    {
        config::USE_MOVEMENT = 0;
        staticEnv = new RiverCrossing( genome );
        config::USE_MOVEMENT = 1;
    }
    int    cycles = (seed == 0) ? 10000 : 1;
    int    seeds[cycles];

    randomise(12345);
    for (int s = 0; s < cycles; s++)
        seeds[s] = (seed == 0) ? random(cycles * 100) : seed;


    for (int c = 0; c < cycles; c++)
    {
        std::string outputStr;
        int f = env->run(3, true, seeds[c]);
        fitness += f;

        outputStr.append("Generation: "+ std::to_string(c) +
                        " | Seed: "    + std::to_string(seeds[c]) +
                        " | Complete: "+ std::to_string(f) +
                        " | Age: "     + std::to_string(env->returnAge()) +
                        " | Death: "     + std::to_string(env->returnDeath()));

        if (config::USE_MOVEMENT) {
            bool equal = true;
            int f1 = staticEnv->run(3, true, seeds[c]);
            staticFitness += f1;

            dsWorld* world1 = env->returnWorld();
            dsWorld* world2 = staticEnv->returnWorld();

            for (int y = 0; y < RIVER::x; y++) for (int x = 0; x < RIVER::y; x++) {
                if(world1->getHeatmapValue(x,y) != world2->getHeatmapValue(x,y)) {
                    equal = false;
                    break;
                }
            }

            if (equal) equalfitness += 1.0;

            outputStr.append(" | Equal: "    + std::to_string(equal) +
                             " | Complete (Static): "+ std::to_string(f1) +
                             " | Age (Static): "+ std::to_string(staticEnv->returnAge()) +
                             " | Death (Static): "+ std::to_string(staticEnv->returnDeath()));
        }

        config::log(outputStr);
    }

   // print average age
   config::log("Average Age: " +
               std::to_string(env->returnAvgAge(3)));

   // print completetion results
   config::log("Quality Test | Levels Complete: " +
               std::to_string(fitness/cycles));

   // print final completetion results
   if (config::USE_MOVEMENT) {
       config::log("Average Age (static): " +
                   std::to_string(staticEnv->returnAvgAge(3))
                   );
       config::log("Quality Test | Levels Complete (static):  " +
                   std::to_string(staticFitness/cycles)
                   );
       config::log("Number of Equal Runs | " +
                   std::to_string(equalfitness/cycles)
                   );

   }
}

int main(int argc, char* argv[])
{
      ga_vector population;
      switch(argc) {
          case 1 : // Robustness Test
            // load snapshot for fittest genome
            if (config::load_snapshot(population))
                evaluate_individual( population );
            break;
          case 2 : // Training
            // load setting to run experiment
            if (config::load_config_file( argv[1] ))
            {
                simulation s;
                s.run_evolution( population );
            }
            break;
      }

      return 0;
}
