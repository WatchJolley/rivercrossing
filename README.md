# The River Crossing Task

This C++ implementation of the River Crossing Task allows a modular network structure to be chosen at runtime via a JSON file. This program is multi-threaded via boost threads and the number of threads can also be defined at runtime via the JSON file.

This repo also includes:

* A Sun Grid Engine (SGE) script to use this program with the Sun Grid Scheduler 
* A python script to format the text output to a CSV file
* R scripts to create boxplots and scatterplots from CSV output

The paper associated with this work can be found below:

[1] [Evolving Robust, Deliberate Motion Planning With a Shallow Convolutional
Neural Network](https://www.mitpressjournals.org/doi/pdf/10.1162/isal_a_00099)


## Overview

The RC Task is a hierarchical task where higher-level decisions require the use of lower-level skills. It was devised to demonstrate high-level deliberative and reactive behaviours produced by a modular neural architecture. The neural architecture consists of a Shunting Model with static weights and a Decision Network with evolvable weights. 

Fitness is based upon animat’s ability to locate a resource in a `20`x`20` discrete world. The goal of an agent is to navigate to the target resource in each two-dimensional RC world, within `100` time-steps, while avoiding harmful objects. Once the resource is obtained, the agent proceeds to the next RC World. Each world increases in complexity via an expanding `river` obstruction between agent and resource. Agents must evolve to move randomly positioned `stones` to build bridges to cross the `river`, in order to complete the more challenging RC World environments.


A sample of works the RC Task has been used can be found below:

* [Neuroevolution of Agents Capable of Reactive and Deliberative Behaviours in Novel and Dynamic Environments](http://eprints.keele.ac.uk/30/1/Neuroevolution%20of%20agents%20capable%20of%20reactive%20and%20deliberative%20behaviours%20in%20novel%20and%20dynamic%20environments%20%28AChannon%29.pdf)
* [Discovering and Maintaining Behaviours Inaccessible to Incremental Genetic
Evolution Through Transcription Errors and Cultural Transmission](http://eprints.keele.ac.uk/42/1/Discovering%20and%20maintaining%20behaviours%20inaccessible%20to%20incremental%20genetic%20evolution%20through%20transciption%20errors%20and%20cultural%20transmission%20(AChannon).pdf)
* [Incremental Neuroevolution of Reactive and Deliberative 3D Agents
](https://www.mitpressjournals.org/doi/pdf/10.1162/978-0-262-33027-5-ch063)
* [Analysis of Social Learning Strategies when Discovering and Maintaining Behaviours Inaccessible to Incremental Genetic Evolution](http://eprints.keele.ac.uk/2126/1/sab2016.pdf)
* [Toward evolving robust, deliberate motion planning with HyperNEAT](http://www.channon.net/alastair/papers/ieeeALife2017alt.pdf)

Table of contents
=================

   * [Prerequisites](#prerequisites)
   * [Installation](#installation)
   * [Usage](#usage)
      * [JSON file](#json-file)
   * [Running in real-time](#running-in-real-time)
   * [Running in Sun Grid Engine](#running-in-sun-grid-engine)
      * [Setting runs](#setting-runs)  
      * [Allocate CPUs](#allocate-cpus)  
      * [Setting training parameters](#setting-training-parameters)  
      * [Running training script](#running-training-script)  
      * [Running validation scripts](#running-validation-scripts)
      * [File structure](#file-structure)  
   * [Format output text to CSV](#format-output-text-to-csv)
   * [Create plots](#create-plots)

## Prerequisites

* Boost 1.68.0 (C++ libraries) 
* CMake 3.8.0

The code targets:
* GCC 4.8.0 - 6.4.0
* Scientific Linux 6.10

It has not been tested on any other system.

## Installation

Within the folder `./requirements` run the file `install_requirements` with the command:


```shell
bash install_requirements.sh
```

This installs JSONCPP and applies patches to the CMake file.

To create the executable run the commands:

```shell
mkdir out && cd out
cmake ../ && make
```


## Usage

The program can be run in real-time or using an SGE scheduler. Parameters of each run can be changed via a JSON file; for greater clarity of those options see the paper for details.

A snapshotting feature is present and saves a `snapshot.json` in the same directory as the executable. If the snapshot is in the same directory as the program, on start, the population and setting will be loaded and continue to run until the `max_generation` or a stopping condition is met.

Seeds are set via the system clock time for random RC world creations. If multiple runs are executed they should be staggered by at least a second of system clock time; this is standard in the SGE script. 

The Robustness Test (see paper) can be run by providing a snapshot with no parameter file in the command line parameters. The Robustness Test simulates the fittest agent through `10'000` static RC world configurations with the largest river width size. This assesses the agent’s general performance. 

### JSON file


```json
{
    "name": "Object_Static_Static",
    "decision_network": "object",
    "shunting_network": "static",
    "decision_pre_evolve": 0,
    "shunting_pre_evolve": 0,
    "hidden_layer_size": 10,
    "hidden_layer_size2": 8,
    "population_size": 1000,
    "max_generation": 10001,
    "elitism_rate": 0.1,
    "mutation_rate": 0.05,
    "use_movement_network": false,
    "threads": 20
}
```

* `name` - for naming files and directories using SGE.
* `decision_network` - can be `object` or `RGB`. 
* `shunting_network` - can be `static` or `CNN`. 
* `decision_pre_evolve` - boolean value if the decision network should be pre-evolved to the highest fitness in the `Static` configuration. 
* `shunting_pre_evolve` - boolean value if the shunting model should be pre-evoled to the highest fitness in the `CNN` configuration. 
* `hidden_layer_size` - first hidden layer size when the `RGB` decision network is used.
* `hidden_layer_size2` -second hidden layer size when the `RGB` decision network is used.
* `population_size` - size of population.
* `max_generation` - number of generations, if stopping condition is not met.
* `elitism_rate` - percentage of genomes carried to next generation.
* `mutation_rate` - mutation rate of genome.
* `use_movement_network` - boolean value to use gradient ascent or an neural network for movement.
* `threads` (optional) - Number of threads to use for simulating each animat.

## Running in real-time 

For training, the `RiverCrossing` executable requires the location of a valid JSON file or snapshot file; the snapshot file takes priority if found. 

```shell
./RiverCrossing ../configuration/object_static_static.json
```

If no command-line parameters are provided, the Robustness Test runs instead. A snapshot has to be in the same directory. 

Below is an example of the programs out:

```shell
2020-05-10 17:18:31: Contents of Config
{
    "name": "object_static_static",
    "decision_network": "object",
    "shunting_network": "static",
    "decision_pre_evolve": 0,
    "shunting_pre_evolve": 0,
    "hidden_layer_size": 10,
    "hidden_layer_size2": 8,
    "population_size": 1000,
    "max_generation": 10001,
    "elitism_rate": 0.1,
    "mutation_rate": 0.05,
    "use_movement_network": false,
    "threads": 20
}

2020-05-10 17:18:31: Seed : 2496904628
2020-05-10 17:18:31: Using 19 Threads
2020-05-10 17:18:31: Snapshot file could not be found
2020-05-10 17:18:34: Generation: 0 | Fittest: 1.500000 | Average: 0.079300
2020-05-10 17:18:37: Generation: 1 | Fittest: 1.500000 | Average: 0.132900
2020-05-10 17:18:40: Generation: 2 | Fittest: 1.400000 | Average: 0.166000
2020-05-10 17:18:42: Generation: 3 | Fittest: 1.300000 | Average: 0.175800
2020-05-10 17:18:45: Generation: 4 | Fittest: 1.700000 | Average: 0.209400
```

If a snapshot is found the following line is shown:

```shell
2020-05-10 17:19:21: Loaded Generation 15 from ./snapshot.json
2020-05-10 17:19:25: Generation: 15 | Fittest: 2.000000 | Average: 0.400800
```

When the Robustness Test is activated you see the following output:

```shell
2020-05-10 17:23:04: Loaded Generation 39 from ./snapshot.json
2020-05-10 17:23:04: Name: object_static_static
2020-05-10 17:23:04: Decision Network: object | 0
2020-05-10 17:23:04: Shunting Network: static | 0
2020-05-10 17:23:04: Movement Network: 0
2020-05-10 17:23:04: Generation: 0 | Seed: 357629 | Complete: 1 | Age: 34 | Death: 0
2020-05-10 17:23:04: Generation: 1 | Seed: 400442 | Complete: 1 | Age: 34 | Death: 0
2020-05-10 17:23:04: Generation: 2 | Seed: 689383 | Complete: 1 | Age: 28 | Death: 0
####
2020-05-10 17:24:27: Generation: 9999 | Seed: 934231 | Complete: 1 | Age: 34 | Death: 0
2020-05-10 17:24:27: Average Age: 45.246900
2020-05-10 17:24:27: Quality Test | Levels Complete: 1.000000
```

## Running in Sun Grid Engine

For long and/or multiple runs, the SGE script is suggested. The location of the files can be found at `.\scripts\` and the files being `continuousRiverCrossing.sge` and `RobustnessTest.sge`.

### Setting runs

This line below details how many runs a job produces.

```bash
#$ -t 1-1:1
```

This states the script will run a single job. If the code were to change to the following:

```bash
#$ -t 1-10:1
```

10 jobs would run, starting at iteration 1. The task numbers dictate the directory names.

### Allocate CPUs

The thread values MUST match the value of the thread provided via the command line parameter; the default is 1 if not. See the following:


```bash
#$ -pe threaded 4
```

This is requesting a threaded CPU implementation (necessary for this code) with 4 CPUs. If you want 12 CPUs your code would look like the following:

```bash
#$ -pe threaded 12
```

### Setting training parameters

Parameters to change within the file `continuousRiverCrossing.sge`:

```shell
# needed relative paths
SCRIPT_PATH='/home/ben/experiments/RiverCrossing/scripts/'

# executable name
PROGRAM='RiverCrossing'
DIRECTORY='runs'
PARAMETER_DIR='../configuration'

# variables for snapshotting
let SNAPSHOT_EVERY_TIMESTEP=100
let RUN_TO_TIMESTEP=2000000
```
* `SCRIPT_PATH` - absolute directory of `./RiverCrossing/scripts/`.
* `PROGRAM` - the name of the executable (if changed in CMake).
* `DIRECTORY` - the name of the directory for output files.
* `PARAMETER_DIR`- relative location of JSON files from `SCRIPT_PATH`.
* `SNAPSHOT_EVERY_TIMESTEP` - creates a snapshot at the generation of this value iteration. This also creates a directory for each block of runs. Keeping the snapshots at each iteration unaffected. 
* `RUN_TO_TIMESTEP`- This replaces the `max_generation` in the JSON file.

### Running training script

```shell
qsub continuousRiverCrossing.sge object_static_static 5
```

* `JSON file` - the name of the chosen JSON file.
* `Threads` (optional) - Number of threads to use per run.

### Running validation scripts

Once the training is complete, `RobustnessTest.sge` can be run. It requires similar changes to `PROGRAM` and `DIRECTORY`; they should match those used in training. It can be run by the following:

```shell
qsub RobustnessTest.sge object_static_static 
```


### File structure

This is a visual of the file structure created by both training and validation scripts.

    ./
    ├── RiverCrossing/
    │   └── runs/
    │       └── object_static_static
    │           ├── run-0/
    │           │    ├── configuration/
    │           │    │   └── object_static_static.json
    │           │    ├── train/
    │           │    │   ├── object_static_static-training.log
    │           │    │   ├── object_static_static-training.out
    │           │    │   ├── continue-from -> so-far/300-400
    │           │    │   ├── snapshot.json
    │           │    │   └── so-far/
    │           │    │       ├── 0-100/
    │           │    │       ├── 100-200/
    │           │    │       ├── 200-300/
    │           │    │       └── 300-400/
    │           │    └── validate/
    │           │        ├── object_static_static-robust.out
    │           │        └── snapshot.json
    │           ├── run-1/
    │           └── run-2/
 
Within `runs` the `name` parameter for each JSON file has its own directory. Within this directory is a list of runs; the run is indicated via the numerical value `run-?`. 

Within each run directory are three sub-directories; `configuration`,`train` and `validate`. 

* `configuration` - this holds the JSON file used throughout the run.
* `train` - holds the training output files and each snapshot output with the `so-far` directory. 
* `validate`- holds the Robustness Test output files, with the snapshot used.

## Format output text to CSV

The python script within `./scripts/format_output.py` is designed to work with the file structure that SGE provides. The following lines require editing for your use case:

```python
# parameters to change -----------------------------
outputName = "object_dn" # csv output name
numOfGens = 10000      # the num of gens to write to csv
root_directory = "../runs/" #location of files

# name of input files
names = ["object_static_static"]
```

* `outputName` - name of output csv.
* `numOfGens` - to what generation is printed to the csv.
* `root_directory` - a relative path to the folder containing the runs.
* `names` - a list of folders names within `runs` that are included in the csv.

## Create plots

R scripts are provided in `./scripts/r/`: `boxplots.r` & `scatterplot.r`. Both have parameters to edit at the top of the file for your use-case.


```r
read_file  <- "***.csv" 
write_file <- "***" 
location   <- "***" 

d_width  <- 8.35
d_height <- 4

```

* `read_file` - name of CSV to read.
* `write_file` - name of output file.
* `location` - location of input and output file.
* `d_width` - the width of the plot in inches.
* `d_height` - the height of the plot in inches.

