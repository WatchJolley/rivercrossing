'''
This script formats the output files from the RiverCrossing executable 
to a csv file.  

If continuousRiverCrossing.sge was used, this script follows the folder
structure used in the default implementation of that script; change 
accordingly if needed.

The CSV can be used with the included R scripts

'''

#!/usr/bin/env python
import os

# parameters to change -----------------------------
outputName = "RGBnets" # csv output name
numOfGens = 10000      # the num of gens to write to csv
root_directory = "../runs/" #location of files

# name of input files
names = ["RGB_CNN_Network_Mutation_0.28", "RGB_CNN_Network_Mutation_0.32", "RGB_CNN_Network_Mutation_0.36"]

f = open(root_directory + outputName + ".csv", 'w')

# titles for csv output - taken from input files
f.write("Name,Run,Generation,Fitness,Depth")

# --------------------------------------------------

# return true if completed numOfGens, false if not
def write_to_file(name,run,file):
    # loop through file line by line
    if os.path.isfile(file):
        file = open(file)
        for line in file:
            if ((line.find("Generation:")) > 0):  # filter out only lines with data
                generation = (line[line.find("Generation:") + 12:line.find("|")-1])
                fitness    = (line[line.find("Fittest:") + 9:line.find("| Average:")-1])
                if (fitness == "4.000000"):
                    for gen in range(int(generation), numOfGens):
                        f.write(name + "," + run + "," + str(gen) + "," + fitness + ",1\r\n")
                    return "true"
                else:
                    if (int(generation) >= numOfGens):
                        return "true"
                    f.write(name + "," + run + "," + generation + "," + fitness + ",1\r\n")

# for each name in 'names' list
for name in names:
    directory = root_directory + name
    # sort directory names
    dir = os.listdir(directory)
    dir.sort()
    # for each run directory within the expirment name
    for item in dir:
        if os.path.isdir(os.path.join(directory, item)):

            # setting up parameters
            run         = item[item.find("run-") + 4]
            path        = directory + "/run-" + run + "/train/"
            sofarDir    = path + "so-far"
            file_name   = name + "-training.out"

            # if there are archived data files
            if (os.path.isdir(path)):
                for root, dirs, files in os.walk(sofarDir):
                    # loop through file line by line
                    if((write_to_file(name,run, root + "/" + file_name)) == "true"):
                        break

            if ((write_to_file(name, run, path + "/" + file_name)) == "true"):
                break

# close file
f.close()
