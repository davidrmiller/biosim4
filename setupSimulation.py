#!/usr/bin/python3

# This file prepares a parallel simulation, setting up the folder structure, config files and table for
# the slurm script.
#
# Terms:
#
# experiment: specific setting for a config file consisting of multiple replicates
#
# replicate: a single evolution/testing with a specific config file setting,
#   all replicates from the same experiment share the same config file settings except the replicatePath
#
# run: an evolution consists of a single run, a testing consists of multiple runs
#
# evolution: consists of multiple generations and a single run,
#   children inherit genomes from their parent(s)
#
# testing: consists of multiple runs and a single generation,
#   genomes are not inherited: they are randomly generated or taken from files
#
# parallel simulation: gets triggered by putting the job-array.sh into the job queue
# consists of multiple evolutions or testings
# The setting of this file influence the results

import os
from tests.pylib.config import TestConfig
from itertools import product
import shutil

# global parameters which influence the folder structure and the config files
outputDir = "outputEvolutions1"
numReplicates = 10

# global parameters which influence the config files and data table
# (the functionality of each parameter is described in "biosim4.ini")
staticMicePositions = ["true", "false"]
staticCatPositions = ["true", "false"]
sizeX = ["128"]
sizeY = ["128"]
population = ["1000"]
miceRatio = ["0.9"]
dynamicPopulation = ["true", "false"]
stepsPerGeneration = ["500"]
maxGenerations = ["100000"] # set the strides to 1000
genomeInitialLengthMin = ["24"]
genomeInitialLengthMax = ["24"]
genomeMaxLength = ["300"]
maxNumberNeurons = ["5", "10"]
killEnable = ["true"]
sexualReproduction = ["true"]
chooseParentsByFitness = ["true"]
pointMutationRate = ["0.001", "0.01"]
responsivenessCurveKFactor = ["2"]
populationSensorRadius = ["2.5"]
longProbeDistance = ["16"]
shortProbeBarrierDistance = ["4"]
shortProbeSafeAreaDistance = ["4"]
shortProbeFoodAreaDistance = ["4"]
signalSensorRadius = ["2.0"]
genomeMiceTextFile = ["none"]
genomeCatsTextFile = ["none"]
challengeMice = ["20"]
challengeCats = ["19"]
foodAreaType = ["6"]
barrierType = ["0"]
safeAreaType = ["6"]
spawnMiceInSafeAreas = ["false"]
deterministic = ["false"]
RNGSeed = ["12345678"]


# modifies a single, specified line in the given config file
def modify_ini(exp_ini, key, value):
    print("Modifying", key, value)
    exp_ini.write_config_setting("DEFAULT", key, value)


# create the needed structure for a single experiment
def create_folders(experimentIndex, replicaIndex):
    if not os.path.isdir(outputDir):
        os.mkdir(outputDir)
    os.chdir(outputDir)
    if not os.path.isdir("experiment{0}".format(str(experimentIndex).zfill(3))):
        os.mkdir("experiment{0}".format(str(experimentIndex).zfill(3)))
    os.chdir("experiment{0}".format(str(experimentIndex).zfill(3)))
    os.mkdir("replicate{0}".format(str(replicaIndex).zfill(3)))
    os.chdir("replicate{0}".format(str(replicaIndex).zfill(3)))
    os.mkdir("genomes")
    os.mkdir("graphs")
    os.mkdir("heatmaps")
    os.mkdir("images")
    os.mkdir("logs")
    os.mkdir("nets")

    os.chdir("logs")
    with open("epoch-log.txt", "w"):
        pass
    with open("population-range.txt", "w") as file:
        file.write("0")
    with open("population-range-final.txt", "w") as file:
        file.write("0")

    os.chdir("..")
    os.chdir("..")
    os.chdir("..")
    os.chdir("..")



# Creates a single entry in the table for a replica.
def create_dat_table_entry(path):
     with open("table.dat", "a") as datfile:
        datfile.write("./bin/Release/biosim4 " + path + "\n")



# use itertools to create all possible combinations of given parameter arrays, each combination being an experiment
def create_config_files():
    experimentIndex = 1
    replicateIndex = 1
    for a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, ab, ac, ad, ae, af, ag, ah, ai in product(staticMicePositions, staticCatPositions,
    sizeX, sizeY, population, miceRatio, dynamicPopulation, stepsPerGeneration, maxGenerations, genomeInitialLengthMin, genomeInitialLengthMax,
    genomeMaxLength, maxNumberNeurons, killEnable, sexualReproduction, chooseParentsByFitness, pointMutationRate,
    responsivenessCurveKFactor, populationSensorRadius, longProbeDistance, shortProbeBarrierDistance,
    shortProbeSafeAreaDistance, shortProbeFoodAreaDistance, signalSensorRadius, genomeMiceTextFile, genomeCatsTextFile,
    challengeMice, challengeCats, foodAreaType, barrierType, safeAreaType,
    spawnMiceInSafeAreas, deterministic, RNGSeed):

        # Create multiple replicas for each experiment
        while replicateIndex <= numReplicates:
            # Create the needed folder structure for the replica
            create_folders(experimentIndex, replicateIndex)

            shutil.copyfile("biosim4.ini", "biosim4replicate.ini")

            # Create needed default section in duplicate
            with open("biosim4replicate.ini", "r", encoding="utf-8") as configfile:
                content = configfile.read()

            with open("biosim4replicate.ini", "w", encoding="utf-8") as configfile:
                configfile.write("[DEFAULT]\n" + content)

            # Load the modified configuration
            config = TestConfig("biosim4replicate.ini")

            # Modify parameters for the replica
            config.load()
            modify_ini(config, "staticMicePositions", a)
            modify_ini(config, "staticCatPositions", b)
            modify_ini(config, "sizeX", c)
            modify_ini(config, "sizeY", d)
            modify_ini(config, "population", e)
            modify_ini(config, "miceRatio", f)
            modify_ini(config, "dynamicPopulation", g)
            modify_ini(config, "stepsPerGeneration", h)
            modify_ini(config, "maxGenerations", i)
            modify_ini(config, "genomeInitialLengthMin", j)
            modify_ini(config, "genomeInitialLengthMax", k)
            modify_ini(config, "genomeMaxLength", l)
            modify_ini(config, "maxNumberNeurons", m)
            modify_ini(config, "killEnable", n)
            modify_ini(config, "sexualReproduction", o)
            modify_ini(config, "chooseParentsByFitness", p)
            modify_ini(config, "pointMutationRate", q)
            modify_ini(config, "responsivenessCurveKFactor", r)
            modify_ini(config, "populationSensorRadius", s)
            modify_ini(config, "longProbeDistance", t)
            modify_ini(config, "shortProbeBarrierDistance", u)
            modify_ini(config, "shortProbeSafeAreaDistance", v)
            modify_ini(config, "shortProbeFoodAreaDistance", w)
            modify_ini(config, "signalSensorRadius", x)
            modify_ini(config, "genomeMiceTextFile", y)
            modify_ini(config, "genomeCatsTextFile", z)
            modify_ini(config, "challengeMice", ab)
            modify_ini(config, "challengeCats", ac)
            modify_ini(config, "foodAreaType", ad)
            modify_ini(config, "barrierType", ae)
            modify_ini(config, "safeAreaType", af)
            modify_ini(config, "spawnMiceInSafeAreas", ag)
            modify_ini(config, "deterministic", ah)
            modify_ini(config, "RNGSeed", ai)

            # Modify output dir, experiment dir and replica dir
            modify_ini(config, "outputDir", outputDir)
            modify_ini(config, "experimentDir", "experiment{0}".format(str(experimentIndex).zfill(3)))
            modify_ini(config, "replicateDir", "replicate{0}".format(str(replicateIndex).zfill(3)))

            # Safe the configuration
            config.save()

            # Move the file to the corresponding replicate folder
            shutil.move("biosim4replicate.ini", outputDir +
                         "/experiment{0}".format(str(experimentIndex).zfill(3)) +
             "/replicate{0}".format(str(replicateIndex).zfill(3)))

            # Create table entry for specific replica
            create_dat_table_entry('"' + outputDir + '/experiment{0}'.format(str(experimentIndex).zfill(3)) +
                                   '/replicate{0}'.format(str(replicateIndex).zfill(3)) + '/biosim4replicate.ini"')

            replicateIndex += 1
        experimentIndex += 1
        replicateIndex = 1


def main():
    with open("table.dat", "w"):
        pass
    if os.path.isdir(outputDir):
        shutil.rmtree(outputDir)
    create_config_files()



if __name__ == '__main__':
    main()


