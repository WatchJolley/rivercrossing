#ifndef CONFIG_H
#define CONFIG_H

#include "tools/random.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>


struct ga_struct
{
    std::vector<double> genome;    // the genome
    double fitness;                // its fitness
    bool   complete;               // for threading
};

typedef std::vector<ga_struct> ga_vector; // for brevity

namespace config
{
    extern std::string model;
    extern std::string DN_network;
    extern std::string SM_network;
    extern int         DN_EVOLED;
    extern int         SM_EVOLED;
    extern bool        USE_MOVEMENT;
    extern int         GA_POPSIZE;
    extern int         GA_MAXGEN;
    extern float       GA_ELITRATE;
    extern float       GA_MUTATIONRATE;
    extern unsigned    GA_SEED;
    extern int         GA_GENERATION;
    extern int         GA_GENOME;
    extern int         DN_HIDDEN_LAYER;
    extern int         DN_HIDDEN_LAYER2;

    extern int         THREADS;

    extern int SM_size;
    extern int DN_size;
    extern int MV_size;

    // loads config.jscon with evoultion parameters
    bool load_config_file(char* file_location);

    // writes generation and population to snapshot
    void write_snapshot_file(ga_vector &gav, int gen);

    // loads snaphot with weights for population and generation
    bool load_snapshot(ga_vector &ga_population);

    // calcualte size of genome
    int cacl_genome_size();

    // get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime();

    // set seeds for random.h class with the system clock
    void set_seed(unsigned seed);

    // log string to std::out with date and time
    void log(std::string string_);

    // prints settings for each hybrid network
    void print_network();
}

#endif
