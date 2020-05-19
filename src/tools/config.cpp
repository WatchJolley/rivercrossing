#include "tools/config.h"

#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>

#include <json/json.h>
#include <json/writer.h>

#include <boost/thread.hpp>

using namespace std;

namespace config
{
    std::string model;
    std::string DN_network;
    std::string SM_network;
    int      DN_EVOLED;
    int      SM_EVOLED;
    bool     USE_MOVEMENT;

    int      GA_POPSIZE;
    int      GA_MAXGEN;
    float    GA_ELITRATE;
    float    GA_MUTATIONRATE;
    unsigned GA_SEED;
    int      GA_GENERATION;
    int      GA_GENOME;
    int      DN_HIDDEN_LAYER;
    int      DN_HIDDEN_LAYER2;

    int      THREADS = 1;

    int      SM_size = 0.0;
    int      DN_size = 0.0;
    int      MV_size = 0.0;

    bool load_config_file(char* file_location)
    {
      ifstream file_validity(file_location, ios::binary | ios::ate);

      if (file_validity.tellg() == -1 ) {
          log("Error in loading configuration file");
          return false;
      }

      ifstream file( file_location);
      Json::Reader reader;
      Json::Value obj;
      reader.parse(file, obj); // reader can also read strings


      // assign parameters from file
      model           = obj["name"].asString();
      DN_network      = obj["decision_network"].asString();
      SM_network      = obj["shunting_network"].asString();
      DN_EVOLED       = obj["decision_pre_evolve"].asUInt();
      SM_EVOLED       = obj["shunting_pre_evolve"].asUInt();
      USE_MOVEMENT    = (obj["use_movement_network"].asString().compare("true")) ? 0 : 1;

      GA_POPSIZE      = obj["population_size"].asUInt();
      GA_MAXGEN       = obj["max_generation"].asUInt();
      GA_ELITRATE     = obj["elitism_rate"].asFloat();
      GA_MUTATIONRATE = obj["mutation_rate"].asFloat();
      GA_GENERATION   = 0;
      DN_HIDDEN_LAYER  = obj["hidden_layer_size"].asUInt();
      DN_HIDDEN_LAYER2 = obj["hidden_layer_size2"].asUInt();
      GA_GENOME = cacl_genome_size();

      THREADS = (obj["threads"].asUInt()-1);
      if(THREADS < 1) THREADS = 1;
      if(boost::thread::hardware_concurrency() < THREADS)
          THREADS = boost::thread::hardware_concurrency();

      // print contents of the file
      log("Contents of Config");
      ifstream print( file_location );
      if (print.is_open())
          std::cout << print.rdbuf();
      cout << "" << endl;
      print.close();

      // add a fucntion to know if this worked
      return true;

    }

    void write_snapshot_file(ga_vector &gav, int gen)
    {
      Json::Value event;
      Json::Value population(Json::arrayValue);

      int population_size = gav.size();
      int genome_size = gav.at(0).genome.size();

      // write Weights to population array
      for(int p = 0; p < population_size; p++) {
          std::string genome;
          for(int g = 0; g < genome_size; g++) {
              genome = genome + std::to_string(gav[p].genome[g]);
              if ( g != genome_size-1 ) genome = genome + ",";
          }
          population.append(genome);
      }

      // write attributes to JSON file
      event["name"]= model;
      event["decision_network"]= DN_network;
      event["shunting_network"]= SM_network;
      event["decision_pre_evolve"]= DN_EVOLED;
      event["shunting_pre_evolve"]= SM_EVOLED;
      event["use_movement_network"]= USE_MOVEMENT;
      event["hidden_layer_size"]= DN_HIDDEN_LAYER;
      event["hidden_layer_size2"]= DN_HIDDEN_LAYER2;


      event["generation"]= gen;
      event["population"]= population;

      event["fitness"] = gav.at(0).fitness;

      std::ofstream out("snapshot.json");
      out << event;
      out.close();

    }

    bool load_snapshot(ga_vector &ga_population)
    {
      Json::Value root;   // starts as "null"; will contain the root value after parsing

      // if there is not snapshot file return false
      try
      {
        std::ifstream config_doc("./snapshot.json", std::ifstream::binary);
        config_doc >> root;
      }
      catch (exception& e)
      {
        log("Snapshot file could not be found");
        return false;
      }

      // load populations
      const Json::Value population = root["population"];
      for ( int index = 0; index < population.size(); ++index )  // iterates over the sequence elements.
      {
          string line = population[index].asString();
          std::stringstream ss(line);

          ga_struct animat;
          animat.fitness = 0;
          animat.complete = false;

          while( ss.good() )
          {
              std::string::size_type sz;     // alias of size_t
              string substr;
              getline( ss, substr, ',' );
              if (substr != "") {
                  double g = std::stod (substr,&sz);
                  animat.genome.push_back( g );
              }
          }

          ga_population.push_back(animat);
      }

      // load other varibles
      model           = root["name"].asString();
      DN_network      = root["decision_network"].asString();
      SM_network      = root["shunting_network"].asString();
      DN_EVOLED       = root["decision_pre_evolve"].asUInt();
      SM_EVOLED       = root["shunting_pre_evolve"].asUInt();
      USE_MOVEMENT    = (root["use_movement_network"].asString().compare("true")) ? 0 : 1;
      GA_GENERATION   = root["generation"].asInt() + 1;
      GA_GENOME       = ga_population[0].genome.size();
      DN_HIDDEN_LAYER = root["hidden_layer_size"].asInt();
      DN_HIDDEN_LAYER2 = root["hidden_layer_size2"].asInt();
      GA_GENOME        = cacl_genome_size(); // recalculate via snapshots configuration


      log("Loaded Generation " + std::to_string(GA_GENERATION) + " from ./snapshot.json");
      return true;

    }

    int cacl_genome_size()
    {
        // calcualte size of genome
        int G = 0;
        SM_size = 0.0;
        DN_size = 0.0;
        MV_size = 0.0;

        if ( !SM_EVOLED ) {
            if (!SM_network.compare("CNN"))
                SM_size = 9;
        }
        if ( !DN_EVOLED ) {
            if (!DN_network.compare("object"))
                DN_size = 44;
            if (!DN_network.compare("RGB"))
            {
                int input_layer  = 4;
                int output_layer = 2;
                DN_size += (input_layer       * DN_HIDDEN_LAYER);
                DN_size += (DN_HIDDEN_LAYER   * DN_HIDDEN_LAYER2);
                DN_size += (DN_HIDDEN_LAYER2  * output_layer);
            }
        }

        if (USE_MOVEMENT)
        {
            int input_layer     = 8;
            int output_layer    = 8;
            MV_size += (input_layer     * output_layer );
        }

        G += SM_size;
        G += DN_size;
        G += MV_size;

        return G;
    }

    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

        return buf;
    }

    void set_seed(unsigned seed) {
        GA_SEED = seed; randomise(seed);
        log("Seed : " + std::to_string(GA_SEED));
    }

    void log(std::string string_) {
        std::cout << currentDateTime() << ": " << string_.c_str() << std::endl;
    }

    void print_network() {
        log("Name: " + model);
        log("Decision Network: " + DN_network + " | " + std::to_string(DN_EVOLED));
        log("Shunting Network: " + SM_network + " | " + std::to_string(SM_EVOLED));
        log("Movement Network: " + std::to_string(USE_MOVEMENT));
    }

}
