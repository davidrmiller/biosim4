// analysis.cpp -- various reports

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>
#include <cstdlib>
#include "simulator.h"

namespace BS {

// This converts sensor numbers to descriptive strings.
std::string sensorName(Sensor sensor)
{
    switch(sensor) {
    case AGE: return "age"; break;
    case BOUNDARY_DIST: return "boundary dist"; break;
    case BOUNDARY_DIST_X: return "boundary dist X"; break;
    case BOUNDARY_DIST_Y: return "boundary dist Y"; break;
    case LAST_MOVE_DIR_X: return "last move dir X"; break;
    case LAST_MOVE_DIR_Y: return "last move dir Y"; break;
    case LOC_X: return "loc X"; break;
    case LOC_Y: return "loc Y"; break;
    case LONGPROBE_POP_FWD: return "long probe population fwd"; break;
    case LONGPROBE_POP_FWD_INV: return "long probe population fwd inverse"; break;
    case LONGPROBE_BAR_FWD: return "long probe barrier fwd"; break;
    case LONGPROBE_BAR_FWD_INV: return "long probe barrier fwd inverse"; break;
    case LONGPROBE_SAF_FWD: return "long probe safe area fwd"; break;
    case LONGPROBE_SAF_FWD_INV: return "long probe safe area fwd inverse"; break;
    case LONGPROBE_FOOD_FWD: return "long probe food area fwd"; break;
    case LONGPROBE_FOOD_FWD_INV: return "long probe food area fwd inverse"; break;
    case LONGPROBE_MICE_FWD: return "long probe mice fwd"; break;
    case LONGPROBE_MICE_FWD_INV: return "long probe mice fwd inverse"; break;
    case LONGPROBE_CATS_FWD: return "long probe cats fwd"; break;
    case LONGPROBE_CATS_FWD_INV: return "long probe cats fwd inverse"; break;
    case BARRIER_FWD: return "short probe barrier fwd-rev"; break;
    case BARRIER_FWD_INV: return "short probe barrier fwd-rev inverse"; break;
    case BARRIER_LR: return "short probe barrier left-right"; break;
    case BARRIER_LR_INV: return "short probe barrier left-right inverse"; break;
    case SAFEAREA_FWD: return "short probe safe area fwd-rev"; break;
    case SAFEAREA_FWD_INV: return "short probe safe area fwd-rev inverse"; break;
    case SAFEAREA_LR: return "short probe safe area left-right"; break;
    case SAFEAREA_LR_INV: return "short probe safe area left-right inverse"; break;
    case FOODAREA_FWD: return "short probe food area fwd"; break;
    case FOODAREA_FWD_INV: return "short probe food area fwd inverse"; break;
    case FOODAREA_LR: return "short probe food area left-right"; break;
    case FOODAREA_LR_INV: return "short probe food area left-right inverse"; break;
    case OSC1: return "osc1"; break;
    case POPULATION: return "population"; break;
    case POPULATION_MICE: return "population mice"; break;
    case POPULATION_CATS: return "population cats"; break;
    case POPULATION_FWD: return "population fwd"; break;
    case POPULATION_FWD_INV: return "population fwd inverse"; break;
    case POPULATION_FWD_MICE: return "population fwd mice"; break;
    case POPULATION_FWD_MICE_INV: return "population fwd mice inverse"; break;
    case POPULATION_FWD_CATS: return "population fwd cats"; break;
    case POPULATION_FWD_CATS_INV: return "population fwd cats inverse"; break;
    case POPULATION_LR: return "population LR"; break;
    case POPULATION_LR_INV: return "population LR inverse"; break;
    case POPULATION_LR_MICE: return "population LR mice"; break;
    case POPULATION_LR_MICE_INV: return "population LR mice inverse"; break;
    case POPULATION_LR_CATS: return "population LR cats"; break;
    case POPULATION_LR_CATS_INV: return "population LR cats inverse"; break;
    case RANDOM: return "random"; break;
    case SIGNAL0: return "signal 0"; break;
    case SIGNAL0_FWD: return "signal 0 fwd"; break;
    case SIGNAL0_LR: return "signal 0 LR"; break;
    case GENETIC_SIM_FWD: return "genetic similarity fwd"; break;
    default: assert(false); break;
    }
}


// Converts action numbers to descriptive strings.
std::string actionName(Action action)
{
    switch(action) {
    case MOVE_EAST: return "move east"; break;
    case MOVE_WEST: return "move west"; break;
    case MOVE_NORTH: return "move north"; break;
    case MOVE_SOUTH: return "move south"; break;
    case MOVE_FORWARD: return "move fwd"; break;
    case MOVE_X: return "move X"; break;
    case MOVE_Y: return "move Y"; break;
    case SET_RESPONSIVENESS: return "set inv-responsiveness"; break;
    case SET_OSCILLATOR_PERIOD: return "set osc1"; break;
    case EMIT_SIGNAL0: return "emit signal 0"; break;
    case KILL_FORWARD: return "kill fwd"; break;
    case MOVE_REVERSE: return "move reverse"; break;
    case MOVE_LEFT: return "move left"; break;
    case MOVE_RIGHT: return "move right"; break;
    case MOVE_RL: return "move R-L"; break;
    case MOVE_RANDOM: return "move random"; break;
    case SET_LONGPROBE_DIST: return "set longprobe dist"; break;
    default: assert(false); break;
    }
}


// This converts sensor numbers to mnemonic strings.
// Useful for later processing by graph-nnet.py.
std::string sensorShortName(Sensor sensor)
{
    switch(sensor) {
    case AGE: return "Age"; break;
    case BOUNDARY_DIST: return "ED"; break;
    case BOUNDARY_DIST_X: return "EDx"; break;
    case BOUNDARY_DIST_Y: return "EDy"; break;
    case LAST_MOVE_DIR_X: return "LMx"; break;
    case LAST_MOVE_DIR_Y: return "LMy"; break;
    case LOC_X: return "Lx"; break;
    case LOC_Y: return "Ly"; break;
    case LONGPROBE_POP_FWD: return "LPf"; break;
    case LONGPROBE_POP_FWD_INV: return "LPfi"; break;
    case LONGPROBE_BAR_FWD: return "LPb"; break;
    case LONGPROBE_BAR_FWD_INV: return "LPbi"; break;
    case LONGPROBE_SAF_FWD: return "LPs"; break;
    case LONGPROBE_SAF_FWD_INV: return "LPsi"; break;
    case LONGPROBE_FOOD_FWD: return "LPfo"; break;
    case LONGPROBE_FOOD_FWD_INV: return "LPfoi"; break;
    case LONGPROBE_MICE_FWD: return "LPm"; break;
    case LONGPROBE_MICE_FWD_INV: return "LPmi"; break;
    case LONGPROBE_CATS_FWD: return "LPc"; break;
    case LONGPROBE_CATS_FWD_INV: return "LPci"; break;
    case BARRIER_FWD: return "Bfd"; break;
    case BARRIER_FWD_INV: return "Bfdi"; break;
    case BARRIER_LR: return "Blr"; break;
    case BARRIER_LR_INV: return "Blri"; break;
    case SAFEAREA_FWD: return "SAfd"; break;
    case SAFEAREA_FWD_INV: return "SAfdi"; break;
    case SAFEAREA_LR: return "SAlr"; break;
    case SAFEAREA_LR_INV: return "SAlri"; break;
    case FOODAREA_FWD: return "FOfd"; break;
    case FOODAREA_FWD_INV: return "FOfdi"; break;
    case FOODAREA_LR: return "FOlr"; break;
    case FOODAREA_LR_INV: return "FOlri"; break;
    case OSC1: return "Osc"; break;
    case POPULATION: return "Pop"; break;
    case POPULATION_MICE: return "Popm"; break;
    case POPULATION_CATS: return "Popc"; break;
    case POPULATION_FWD: return "Pfd"; break;
    case POPULATION_FWD_INV: return "Pfdi"; break;
    case POPULATION_FWD_MICE: return "Pfdm"; break;
    case POPULATION_FWD_MICE_INV: return "Pfdmi"; break;
    case POPULATION_FWD_CATS: return "Pfdc"; break;
    case POPULATION_FWD_CATS_INV: return "Pfdci"; break;
    case POPULATION_LR: return "Plr"; break;
    case POPULATION_LR_INV: return "Plri"; break;
    case POPULATION_LR_MICE: return "Plrm"; break;
    case POPULATION_LR_MICE_INV: return "Plrmi"; break;
    case POPULATION_LR_CATS: return "Plrc"; break;
    case POPULATION_LR_CATS_INV: return "Plrci"; break;
    case RANDOM: return "Rnd"; break;
    case SIGNAL0: return "Sg"; break;
    case SIGNAL0_FWD: return "Sfd"; break;
    case SIGNAL0_LR: return "Slr"; break;
    case GENETIC_SIM_FWD: return "Gen"; break;
    default: assert(false); break;
    }
}


// Converts action numbers to mnemonic strings.
// Useful for later processing by graph-nnet.py.
std::string actionShortName(Action action)
{
    switch(action) {
    case MOVE_EAST: return "MvE"; break;
    case MOVE_WEST: return "MvW"; break;
    case MOVE_NORTH: return "MvN"; break;
    case MOVE_SOUTH: return "MvS"; break;
    case MOVE_X: return "MvX"; break;
    case MOVE_Y: return "MvY"; break;
    case MOVE_FORWARD: return "Mfd"; break;
    case SET_RESPONSIVENESS: return "Res"; break;
    case SET_OSCILLATOR_PERIOD: return "OSC"; break;
    case EMIT_SIGNAL0: return "SG"; break;
    case KILL_FORWARD: return "Klf"; break;
    case MOVE_REVERSE: return "Mrv"; break;
    case MOVE_LEFT: return "MvL"; break;
    case MOVE_RIGHT: return "MvR"; break;
    case MOVE_RL: return "MRL"; break;
    case MOVE_RANDOM: return "Mrn"; break;
    case SET_LONGPROBE_DIST: return "LPD"; break;
    default: assert(false); break;
    }
}


// List the names of the active sensors and actions to stdout.
// "Active" means those sensors and actions that are compiled into
// the code. See sensors-actions.h for how to define the enums.
void printSensorsActions()
{
    unsigned i;
    std::cout << "Sensors:" << std::endl;
    for (i = 0; i < Sensor::NUM_SENSES; ++i) {
        std::cout << "  " << sensorName((Sensor)i) << std::endl;
    }
    std::cout << "Actions:" << std::endl;
    for (i = 0; i < Action::NUM_ACTIONS; ++i) {
        std::cout << "  " << actionName((Action)i) << std::endl;
    }
    std::cout << std::endl;
}


// Format: 32-bit hex strings, one per gene
void Indiv::printGenome(std::ofstream& genomeTextfile) const
{
    if (genomeTextfile.is_open()) {
        for (Gene gene : genome) {
            uint32_t n;
            std::memcpy(&n, &gene, sizeof(n));
            genomeTextfile << " " << std::hex << std::setfill('0') << std::setw(8) << n;
        }
    } else {
        assert(false);
    }
}


///*
//Example format:
//
//    ACTION_NAMEa from:
//    ACTION_NAMEb from:
//        SENSOR i
//        SENSOR j
//        NEURON n
//        NEURON m
//    Neuron x from:
//        SENSOR i
//        SENSOR j
//        NEURON n
//        NEURON m
//    Neuron y ...
//*/
//void Indiv::printNeuralNet() const
//{
//    for (unsigned action = 0; action < Action::NUM_ACTIONS; ++action) {
//        bool actionDisplayed = false;
//        for (auto & conn : nnet.connections) {
//
//            assert((conn.sourceType == NEURON && conn.sourceNum < p.maxNumberNeurons)
//                || (conn.sourceType == SENSOR && conn.sourceNum < Sensor::NUM_SENSES));
//            assert((conn.sinkType == ACTION && conn.sinkNum < Action::NUM_ACTIONS)
//                || (conn.sinkType == NEURON && conn.sinkNum < p.maxNumberNeurons));
//
//            if (conn.sinkType == ACTION && (conn.sinkNum) == action) {
//                if (!actionDisplayed) {
//                    std::cout << "Action " << actionName((Action)action) << " from:" << std::endl;
//                    actionDisplayed = true;
//                }
//                if (conn.sourceType == SENSOR) {
//                    std::cout << "   " << sensorName((Sensor)(conn.sourceNum));
//                } else {
//                    std::cout << "   Neuron " << (conn.sourceNum % nnet.neurons.size());
//                }
//                std::cout << " " << conn.weightAsFloat() << std::endl;
//            }
//        }
//    }
//
//    for (size_t neuronNum = 0; neuronNum < nnet.neurons.size(); ++neuronNum) {
//        bool neuronDisplayed = false;
//        for (auto & conn : nnet.connections) {
//            if (conn.sinkType == NEURON && (conn.sinkNum) == neuronNum) {
//                if (!neuronDisplayed) {
//                    std::cout << "Neuron " << neuronNum << " from:" << std::endl;
//                    neuronDisplayed = true;
//                }
//                if (conn.sourceType == SENSOR) {
//                    std::cout << "   " << sensorName((Sensor)(conn.sourceNum));
//                } else {
//                    std::cout << "   Neuron " << (conn.sourceNum);
//                }
//                std::cout << " " << conn.weightAsFloat() << std::endl;
//            }
//        }
//    }
//}
//


// This prints a neural net in a form that can be processed with
// graph-nnet.py to produce a graphic illustration of the net.
void Indiv::printIGraphEdgeList(unsigned index, unsigned generation, const std::string& species) const
{
    std::string iGraphEdgeListFilename;
    iGraphEdgeListFilename = p.outputPath + p.netDir + "/net-gen-" +
        std::to_string(generation) + "-species-" + species + "-index-"
        + std::to_string(index) + ".txt";


    std::ofstream foutput;
    foutput.open(iGraphEdgeListFilename, std::ios::app);

    if (foutput.is_open()) {
        for (auto & conn : nnet.connections) {
            if (conn.sourceType == SENSOR) {
                foutput << sensorShortName((Sensor)(conn.sourceNum));
            } else {
                foutput << "N" << std::to_string(conn.sourceNum);
            }

            foutput << " ";

            if (conn.sinkType == ACTION) {
                foutput << actionShortName((Action)(conn.sinkNum));
            } else {
                foutput << "N" << std::to_string(conn.sinkNum);
            }
            foutput << " " << std::to_string(conn.weight) << std::endl;
        }
    } else {
        assert(false);
    }

    foutput.close();

    std::string graphFilename = p.outputPath + p.graphDir + "/net-gen-" + std::to_string(generation) +
        "-species-" + species + "-index-" + std::to_string(index) + ".svg";

    std::string command = "python3 tools/graph-nnet.py " + iGraphEdgeListFilename + " " + graphFilename;
    std::system(command.c_str());

}


float averageGenomeLength()
{
    unsigned count = 100;
    unsigned numberSamples = 0;
    unsigned long sum = 0;

    while (count-- > 0) {
        sum += peeps[randomUint(1, p.population)].genome.size();
        ++numberSamples;
    }
    return sum / numberSamples;
}


// The epoch log contains one line per generation in a format that can be
// fed to graphlog.gp to produce a chart of the simulation progress.
// ToDo: remove hardcoded filename.
void appendEpochLog(unsigned generation, unsigned numberSurvivors, unsigned survivedMice, unsigned survivedCats, unsigned murderCount)
{
    std::ofstream foutput;

    if (generation == 0) {
        foutput.open(p.outputPath + p.logDir + "/epoch-log.txt");
        foutput.close();
    }

    foutput.open(p.outputPath + p.logDir + "/epoch-log.txt", std::ios::app);

    if (foutput.is_open()) {
        if (generation == 0) {
            if (p.logGeneration == true) {
                foutput << "generation";
            }
            if (p.logSurvivors == true) {
                foutput << " Survivors";
            }
            if (p.logDiversity == true) {
                foutput << " Diversity";
            }
            if (p.logDiversityMice ==  true) {
                foutput << " DiversityMice";
            }
            if (p.logDiversityCats == true) {
                foutput << " DiversityCats";
            }
            if (p.logAverageGenomeLength == true) {
                foutput << " averageGenomeLength";
            }
            if (p.logSurvivorsMice ==  true) {
                foutput << " SurvivedMice";
            }
            if (p.logSurvivorsCats == true) {
                foutput << " SurvivedCats";
            }
            if (p.logEatenMice ==  true) {
                foutput << " EatenMice";
            }
            foutput << std::endl;
        }

        if (p.logGeneration == true) {
            foutput << generation;
        }
        if (p.logSurvivors == true) {
            foutput << " " << numberSurvivors;
        }
        if (p.logDiversity == true) {
            foutput << " " << geneticDiversity();
        }
        if (p.logDiversityMice == true) {
            foutput << " " << geneticDiversitySpecifiedPopulation("mouse");
        }
        if (p.logDiversityCats == true) {
            foutput << " " << geneticDiversitySpecifiedPopulation("cat");
        }
        if (p.logAverageGenomeLength == true) {
            foutput << " " << averageGenomeLength();
        }
        if (p.logSurvivorsMice == true) {
            foutput << " " << survivedMice;
        }
        if (p.logSurvivorsCats == true) {
            foutput << " " << survivedCats;
        }
        if (p.logEatenMice == true) {
            foutput << " " << murderCount;
        }
        foutput << std::endl;
    } else {
        assert(false);
    }
}


// Create textfile which contains maximum population range
void createPopulationRange()
{
    std::ofstream foutput(p.outputPath + p.logDir + "/population-range.txt", std::ios::trunc);

    if (foutput.is_open()) {
        foutput << p.population << std::endl;
    } else {
        assert(false);
    }
}

// Create textfile which contains the survivors of the generation with the most survivors
void createPopulationFinalRange(unsigned numberSurvivors, unsigned generation) {
    int previousNumberSurvivors = 0;

    // Get the previous number of survivors from the file.
    std::ifstream finput(p.outputPath + p.logDir + "/population-range-final.txt");

    if (finput.is_open()) {
        finput >> previousNumberSurvivors;
        finput.close();
    } else {
        assert(false);
    }

    if (numberSurvivors > previousNumberSurvivors) {
        // Empty the file and write the new value into it.
        std::ofstream fempty(p.outputPath + p.logDir + "/population-range-final.txt", std::ios::trunc);
        if (fempty.is_open()) {
            fempty << numberSurvivors << std::endl;
            fempty.close();
        } else {
            assert(false);
        }
    }

    if (generation == p.maxGenerations - 1) {
        std::string command = p.graphLogFinalUpdateCommand + " " + p.outputPath + " " + p.imageDir + " " + p.logDir;
        system(command.c_str());

        std::ofstream fempty(p.outputPath + p.logDir + "/population-range-final.txt", std::ios::trunc);
        if (fempty.is_open()) {
            fempty << 0 << std::endl;
            fempty.close();
        } else {
            assert(false);
        }
    }
}

// Create textfile which contains the survivors of the run with the most survivors
void createPopulationFinalRangeMultipleRuns(unsigned numberSurvivors, unsigned run) {
    int previousNumberSurvivors = 0;

    // Get the previous number of survivors from the file.
    std::ifstream finput(p.outputPath + p.logDir + "/population-range-final.txt");

    if (finput.is_open()) {
        finput >> previousNumberSurvivors;
        finput.close();
    } else {
        assert(false);
    }

    if (numberSurvivors > previousNumberSurvivors) {
        // Empty the file and write the new value into it.
        std::cout << "New number of survivors: " << numberSurvivors << std::endl;
        std::ofstream fempty(p.outputPath + p.logDir + "/population-range-final.txt", std::ios::trunc);
        if (fempty.is_open()) {
            fempty << numberSurvivors << std::endl;
            fempty.close();
        } else {
            assert(false);
        }
    }

    if (run == p.numRuns - 1) {
        std::string command = p.graphLogFinalUpdateCommand + " " + p.outputPath + " " + p.imageDir + " " + p.logDir;
        std::system(command.c_str());

        std::ofstream fempty(p.outputPath + p.logDir + "/population-range-final.txt", std::ios::trunc);
        if (fempty.is_open()) {
            fempty << 0 << std::endl;
            fempty.close();
        } else {
            assert(false);
        }
    }
}

// Print stats about pheromone usage.
void displaySignalUse()
{
    if (Sensor::SIGNAL0 > Sensor::NUM_SENSES && Sensor::SIGNAL0_FWD > Sensor::NUM_SENSES && Sensor::SIGNAL0_LR > Sensor::NUM_SENSES) {
        return;
    }

    unsigned long long sum = 0;
    unsigned count = 0;

    for (int16_t x = 0; x < p.sizeX; ++x) {
        for (int16_t y = 0; y < p.sizeY; ++y) {
            unsigned magnitude = signals.getMagnitude(0, { x, y });
            if (magnitude != 0) {
                ++count;
                sum += magnitude;
            }
        }
    }
    std::cout << "Signal spread " << ((double)count / (p.sizeX * p.sizeY)) << "%, average " << ((double)sum / (p.sizeX * p.sizeY)) << std::endl;
}


// Print how many connections occur from each kind of sensor neuron and to
// each kind of action neuron over the entire population. This helps us to
// see which sensors and actions are most useful for survival.
void displaySensorActionReferenceCounts()
{
    std::vector<unsigned> sensorCounts(Sensor::NUM_SENSES, 0);
    std::vector<unsigned> actionCounts(Action::NUM_ACTIONS, 0);

    for (unsigned index = 1; index <= p.population; ++index) {
        if (peeps[index].alive) {
            const Indiv &indiv = peeps[index];
            for (const Gene &gene : indiv.nnet.connections) {
                if (gene.sourceType == SENSOR) {
                    assert(gene.sourceNum < Sensor::NUM_SENSES);
                    ++sensorCounts[(Sensor)gene.sourceNum];
                }
                if (gene.sinkType == ACTION) {
                    assert(gene.sinkNum < Action::NUM_ACTIONS);
                    ++actionCounts[(Action)gene.sinkNum];
                }
            }
        }
    }

    std::cout << "Sensors in use:" << std::endl;
    for (unsigned i = 0; i < sensorCounts.size(); ++i) {
        if (sensorCounts[i] > 0) {
            std::cout << "  " << sensorCounts[i] << " - " << sensorName((Sensor)i) << std::endl;
        }
    }
    std::cout << "Actions in use:" << std::endl;
    for (unsigned i = 0; i < actionCounts.size(); ++i) {
        if (actionCounts[i] > 0) {
            std::cout << "  " << actionCounts[i] << " - " << actionName((Action)i) << std::endl;
        }
    }
}


void displaySampleGenomes(unsigned count, unsigned generation, std::vector<uint16_t> miceIndexes = {}, std::vector<uint16_t> catsIndexes = {})
{
    unsigned index = 1; // indexes start at 1
    unsigned catIndex = 1; // used to count cats from which neuronal nets are drawn
    std::string genomeMiceFilename = p.outputPath + p.genomeDir + "/genome-mice-" + std::to_string(generation) + ".txt";
    std::ofstream fmouse;
    fmouse.open(genomeMiceFilename);
    for (index = 1; index <= p.population * p.miceRatio; ++index) {
        assert(peeps[index].species == "mouse");
        if (fmouse.is_open()) {
            // Print the genomes of a mouse into the txt.
            fmouse << std::dec << index;
            peeps[index].printGenome(fmouse);
            fmouse << std::endl;
            // Draw the graph of a neuronal net from a mouse.
            if (index <= count && p.displayBestGenomes == false) {
                peeps[index].printIGraphEdgeList(index, generation, peeps[index].species);
            }
        } else {
            assert(false);
        }
    }
    fmouse.close();

    std::string genomeCatsFilename = p.outputPath + p.genomeDir + "/genome-cats-" + std::to_string(generation) + ".txt";
    std::ofstream fcat;
    fcat.open(genomeCatsFilename);
    for (index = p.population * p.miceRatio + 1; index <= p.population; ++index) { // <= : the last cat was skipped
        assert(peeps[index].species == "cat");
        if (fcat.is_open()) {
            // Print the genomes of a cat into the txt.
            fcat << std::dec << index;
            peeps[index].printGenome(fcat);
            fcat << std::endl;
            // Draw the graph of a neuronal net from a cat.
            if (catIndex <= count && p.displayBestGenomes == false) {
                peeps[index].printIGraphEdgeList(index, generation, peeps[index].species);
                catIndex = catIndex + 1;
            }
        } else {
            assert(false);
        }
    }
    fcat.close();

    if (p.displayBestGenomes == true) {
        // Display the best mice
        for (index = 0; index < miceIndexes.size(); ++index) {
            assert(peeps[miceIndexes.at(index)].species == "mouse");
            peeps[miceIndexes.at(index)].printIGraphEdgeList(miceIndexes.at(index), generation, peeps[miceIndexes.at(index)].species);
        }
        // Display the best cats
        for (index = 0; index < catsIndexes.size(); ++index) {
            assert(peeps[catsIndexes.at(index)].species == "cat");
            peeps[miceIndexes.at(index)].printIGraphEdgeList(catsIndexes.at(index), generation, peeps[catsIndexes.at(index)].species);
        }
    }
    displaySensorActionReferenceCounts();
}


void displayMeanValues() {
    std::string filename = p.outputPath + p.logDir + "/mean-values.txt";
    std::ofstream fmean;
    fmean.open(filename);
    if (fmean.is_open()) {
        // Print the mean values into the txt.
        fmean << "meanSurvivors: " << p.meanSurvivors << std::endl;
        fmean << "meanSurvivorsMice: " << p.meanSurvivorsMice << std::endl;
        fmean << "meanSurvivorsCats: " << p.meanSurvivorsCats << std::endl;
        fmean << "meanEatenMice: " << p.meanEatenMice << std::endl;
        fmean << "meanFoodEaten: " << p.meanFoodEaten << std::endl;
        fmean << "meanSuccessfullCats: " << p.meanSuccessfullCats << std::endl;
    } else {
        assert(false);
    }
}

void saveBestGenomes(std::string species) {
    if (species == "mouse") {
        std::ofstream foutput(p.outputPath + "bestMice", std::ios::trunc);
        if (foutput.is_open()) {
            for (unsigned index = 1; index <= p.population * p.miceRatio; ++index) {
                assert(peeps[index].species == "mouse");
                if (foutput.is_open()) {
                    // Print the genomes of a mouse into the txt.
                    foutput << std::dec << index;
                    peeps[index].printGenome(foutput);
                    foutput << std::endl;
                } else {
                    assert(false);
                }
            }
        } else {
            assert(false);
        }
    } else {
        std::ofstream foutput(p.outputPath + "bestCats", std::ios::trunc);
        if (foutput.is_open()) {
            for (unsigned index = p.population * p.miceRatio + 1; index < p.population; ++index) {
                assert(peeps[index].species == "cat");
                if (foutput.is_open()) {
                    foutput << std::dec << index;
                    peeps[index].printGenome(foutput);
                    foutput << std::endl;
                } else {
                    assert(false);
                }
            }
        } else {
            assert(false);
        }
    }
}

} // end namespace BS
