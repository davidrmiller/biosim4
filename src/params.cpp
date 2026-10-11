// params.cpp
// See params.h for notes.

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <cstdint>
#include <map>
#include "params.h"

// To add a new parameter:
//    1. Add a member to struct Params in params.h.
//    2. Add a member and its default value to privParams in ParamManager::setDefaults()
//          in params.cpp.
//    3. Add an else clause to ParamManager::ingestParameter() in params.cpp.
//    4. Add a line to the user's parameter file (default name biosim4.ini)

namespace BS {

void ParamManager::setDefaults()
{
    privParams.sizeX = 128;
    privParams.sizeY = 128;
    privParams.challengeMice = 20;
    privParams.challengeCats = 19;
    privParams.micePositions = {};
    privParams.catPositions = {};
    privParams.staticMicePositions = false;
    privParams.staticCatPositions = false;

    privParams.foodAreaType = 0;
    privParams.displayFoodLocations = true;
    privParams.genomeInitialLengthMin = 24;
    privParams.genomeInitialLengthMax = 24;
    privParams.genomeMaxLength = 300;
    privParams.outputDir = "output";
    privParams.experimentDir = "experiment1";
    privParams.replicateDir = "replicate1";
    privParams.outputPath = "outputMultiple/experiment1";
    privParams.logDir = "logs";
    privParams.imageDir = "images";
    privParams.heatmapStride = 1;
    privParams.heatmapDir = "heatmaps";
    privParams.netDir = "nets";
    privParams.graphDir = "graphs";
    privParams.genomeDir = "genomes";
    privParams.genomeMiceTextFile = "none";
    privParams.genomeCatsTextFile = "none";
    privParams.population = 3000;
    privParams.miceRatio = 0.5;
    privParams.stepsPerGeneration = 300;
    privParams.maxGenerations = 200000;
    privParams.barrierType = 0;
    privParams.safeAreaType = 0;
    privParams.displaySafeAreas = true;
    privParams.spawnMiceInSafeAreas = true;
    privParams.numThreads = 4;
    privParams.numRuns = 1;
    privParams.signalLayers = 1;
    privParams.maxNumberNeurons = 5;
    privParams.pointMutationRate = 0.001;
    privParams.geneInsertionDeletionRate = 0.0;
    privParams.deletionRatio = 0.5;
    privParams.killEnable = false;
    privParams.sexualReproduction = true;
    privParams.chooseParentsByFitness = true;
    privParams.populationSensorRadius = 2.5;
    privParams.signalSensorRadius = 2.0;
    privParams.responsiveness = 0.5;
    privParams.responsivenessCurveKFactor = 2;
    privParams.longProbeDistance = 16;
    privParams.shortProbeBarrierDistance = 4;
    privParams.shortProbeSafeAreaDistance = 4;
    privParams.shortProbeFoodAreaDistance = 4;
    privParams.valenceSaturationMag = 0.5;
    privParams.saveVideo = true;
    privParams.videoStride = 25;
    privParams.videoSaveFirstFrames = 2;
    privParams.displayScale = 8;
    privParams.agentSize = 4;
    privParams.genomeAnalysisStride = privParams.videoStride;
    privParams.displaySampleGenomes = 5;
    privParams.displayBestGenomes = true;
    privParams.genomeComparisonMethod = 1;
    privParams.updateGraphLog = true;
    privParams.logGeneration = true;
    privParams.logSurvivors = true;
    privParams.logDiversity = true;
    privParams.logDiversityMice = true;
    privParams.logDiversityCats = true;
    privParams.logAverageGenomeLength = true;
    privParams.logSurvivorsMice = true;
    privParams.logSurvivorsCats = true;
    privParams.updateGraphLogStride = privParams.videoStride;
    privParams.deterministic = false;
    privParams.RNGSeed = 12345678;
    privParams.graphLogUpdateCommand = "gnuplot --persist -c ./tools/graphlog.gp";
    privParams.graphLogFinalUpdateCommand = "gnuplot --persist -c ./tools/graphlog-final.gp";
    privParams.parameterChangeGenerationNumber = 0;
}


void ParamManager::registerConfigFile(const char *filename)
{
    configFilename = std::string(filename);
}


bool checkIfUint(const std::string &s)
{
    return s.find_first_not_of("0123456789") == std::string::npos;
}


bool checkIfInt(const std::string &s)
{
    //return s.find_first_not_of("-0123456789") == std::string::npos;
    std::istringstream iss(s);
    int i;
    iss >> std::noskipws >> i; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}


bool checkIfFloat(const std::string &s)
{
    std::istringstream iss(s);
    double d;
    iss >> std::noskipws >> d; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}


bool checkIfBool(const std::string &s)
{
    return s == "0" || s == "1" || s == "true" || s == "false";
}


bool getBoolVal(const std::string &s)
{
    if (s == "true" || s == "1")
        return true;
    else if (s == "false" || s == "0")
        return false;
    else
        return false;
}


void ParamManager::ingestParameter(std::string name, std::string val)
{
    std::transform(name.begin(), name.end(), name.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    //std::cout << name << " " << val << '\n' << std::endl;

    bool isUint = checkIfUint(val);
    unsigned uVal = isUint ? (unsigned)std::stol(val.c_str()) : 0;
    bool isFloat = checkIfFloat(val);
    double dVal = isFloat ? std::stod(val.c_str()) : 0.0;
    bool isBool = checkIfBool(val);
    bool bVal = getBoolVal(val);

    do {
        if (name == "sizex" && isUint && uVal >= 2 && uVal <= (uint16_t)-1) {
            privParams.sizeX = uVal; break;
        }
        else if (name == "sizey" && isUint && uVal >= 2 && uVal <= (uint16_t)-1) {
            privParams.sizeY = uVal; break;
        }
        else if (name == "challengemice" && isUint && uVal < (uint16_t)-1) {
            privParams.challengeMice = uVal; break;
        }
        else if (name == "challengecats" && isUint && uVal < (uint16_t)-1) {
            privParams.challengeCats = uVal; break;
        }
        else if (name == "staticmicepositions" && isBool) {
            privParams.staticMicePositions = bVal; break;
        }
        else if (name == "staticcatpositions" && isBool) {
            privParams.staticCatPositions = bVal; break;
        }
        else if (name == "foodareatype" && isUint && uVal < (uint32_t)-1) {
            privParams.foodAreaType = uVal; break;
        }
        else if (name == "displayfoodlocations" && isBool) {
            privParams.displayFoodLocations = bVal; break;
        }
        else if (name == "genomeinitiallengthmin" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.genomeInitialLengthMin = uVal; break;
        }
        else if (name == "genomeinitiallengthmax" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.genomeInitialLengthMax = uVal; break;
        }
        else if (name == "outputdir") {
            privParams.outputDir = val; break;
        }
        else if (name == "experimentdir") {
            privParams.experimentDir = val; break;
        }
        else if (name == "replicatedir") {
            privParams.replicateDir = val; break;
        }
        else if (name == "logdir") {
            privParams.logDir = val; break;
        }
        else if (name == "imagedir") {
            privParams.imageDir = val; break;
        }
        else if (name == "heatmapstride" && isUint && uVal > 0) {
            privParams.heatmapStride = uVal; break;
        }
        else if (name == "heatmapdir") {
            privParams.heatmapDir = val; break;
        }
        else if (name == "netdir") {
            privParams.netDir = val; break;
        }
        else if (name == "graphdir") {
            privParams.graphDir = val; break;
        }
        else if (name == "genomedir") {
            privParams.genomeDir = val; break;
        }
        else if (name == "genomemicetextfile") {
            privParams.genomeMiceTextFile = val; break;
        }
        else if (name == "genomecatstextfile") {
            privParams.genomeCatsTextFile = val; break;
        }
        else if (name == "population" && isUint && uVal > 0 && uVal < (uint32_t)-1) {
            privParams.population = uVal; break;
        }
        else if (name == "miceratio" && isFloat && dVal >= 0.0 && dVal <= 1.0) {
            privParams.miceRatio = dVal; break;
        }
        else if (name == "dynamicpopulation" && isBool) {
            privParams.dynamicPopulation = bVal; break;
        }
        else if (name == "stepspergeneration" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.stepsPerGeneration = uVal; break;
        }
        else if (name == "maxgenerations" && isUint && uVal > 0 && uVal < 0x7fffffff) {
            privParams.maxGenerations = uVal; break;
        }
        else if (name == "barriertype" && isUint && uVal < (uint32_t)-1) {
            privParams.barrierType = uVal; break;
        }
        else if (name == "safeareatype" && isUint && uVal < (uint32_t)-1) {
            privParams.safeAreaType = uVal; break;
        }
        else if (name == "displaysafeareas" && isBool) {
            privParams.displaySafeAreas = bVal; break;
        }
        else if (name == "spawnmiceinsafeareas" && isBool) {
            privParams.spawnMiceInSafeAreas = bVal; break;
        }
        else if (name == "numthreads" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.numThreads = uVal; break;
        }
        else if (name == "numruns" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.numRuns = uVal; break;
        }
        else if (name == "signallayers" && isUint && uVal < (uint16_t)-1) {
            privParams.signalLayers = uVal; break;
        }
        else if (name == "genomemaxlength" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.genomeMaxLength = uVal; break;
        }
        else if (name == "maxnumberneurons" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.maxNumberNeurons = uVal; break;
        }
        else if (name == "pointmutationrate" && isFloat && dVal >= 0.0 && dVal <= 1.0) {
            privParams.pointMutationRate = dVal; break;
        }
        else if (name == "geneinsertiondeletionrate" && isFloat && dVal >= 0.0 && dVal <= 1.0) {
            privParams.geneInsertionDeletionRate = dVal; break;
        }
        else if (name == "deletionratio" && isFloat && dVal >= 0.0 && dVal <= 1.0) {
            privParams.deletionRatio = dVal; break;
        }
        else if (name == "killenable" && isBool) {
            privParams.killEnable = bVal; break;
        }
        else if (name == "sexualreproduction" && isBool) {
            privParams.sexualReproduction = bVal; break;
        }
        else if (name == "chooseparentsbyfitness" && isBool) {
            privParams.chooseParentsByFitness = bVal; break;
        }
        else if (name == "populationsensorradius" && isFloat && dVal > 0.0) {
            privParams.populationSensorRadius = dVal; break;
        }
        else if (name == "signalsensorradius" && isFloat && dVal > 0.0) {
            privParams.signalSensorRadius = dVal; break;
        }
        else if (name == "responsiveness" && isFloat && dVal >= 0.0) {
            privParams.responsiveness = dVal; break;
        }
        else if (name == "responsivenesscurvekfactor" && isUint && uVal >= 1 && uVal <= 20) {
            privParams.responsivenessCurveKFactor = uVal; break;
        }
        else if (name == "longprobedistance" && isUint && uVal > 0) {
            privParams.longProbeDistance = uVal; break;
        }
        else if (name == "shortprobebarrierdistance" && isUint && uVal > 0) {
            privParams.shortProbeBarrierDistance = uVal; break;
        }
        else if (name == "shortprobesafeareadistance" && isUint && uVal > 0) {
            privParams.shortProbeSafeAreaDistance = uVal; break;
        }
        else if (name == "shortprobefoodareadistance" && isUint && uVal > 0) {
            privParams.shortProbeFoodAreaDistance = uVal; break;
        }
        else if (name == "valencesaturationmag" && isFloat && dVal >= 0.0) {
            privParams.valenceSaturationMag = dVal; break;
        }
        else if (name == "savevideo" && isBool) {
            privParams.saveVideo = bVal; break;
        }
        else if (name == "videostride" && isUint && uVal > 0) {
            privParams.videoStride = uVal; break;
        }
        else if (name == "videosavefirstframes" && isUint) {
            privParams.videoSaveFirstFrames = uVal; break;
        }
        else if (name == "displayscale" && isUint && uVal > 0) {
            privParams.displayScale = uVal; break;
        }
        else if (name == "agentsize" && isFloat && dVal > 0.0) {
            privParams.agentSize = dVal; break;
        }
        else if (name == "genomeanalysisstride" && isUint && uVal > 0) {
            privParams.genomeAnalysisStride = uVal; break;
        }
        else if (name == "genomeanalysisstride" && val == "videoStride") {
            privParams.genomeAnalysisStride = privParams.videoStride; break;
        }
        else if (name == "displaysamplegenomes" && isUint) {
            privParams.displaySampleGenomes = uVal; break;
        }
        else if (name == "displaybestgenomes" && isBool) {
            privParams.displayBestGenomes = bVal; break;
        }
        else if (name == "genomecomparisonmethod" && isUint) {
            privParams.genomeComparisonMethod = uVal; break;
        }
        else if (name == "updategraphlog" && isBool) {
            privParams.updateGraphLog = bVal; break;
        }
        else if (name == "loggeneration" && isBool) {
            privParams.logGeneration = bVal; break;
        }
        else if (name == "logsurvivors" && isBool) {
            privParams.logSurvivors = bVal; break;
        }
        else if (name == "logdiversity" && isBool) {
            privParams.logDiversity = bVal; break;
        }
        else if (name == "logdiversitymice" && isBool) {
            privParams.logDiversityMice = bVal; break;
        }
        else if (name == "logdiversitycats" && isBool) {
            privParams.logDiversityCats = bVal; break;
        }
        else if (name == "logaveragegenomelength" && isBool) {
            privParams.logAverageGenomeLength = bVal; break;
        }
        else if (name == "logsurvivorsmice" && isBool) {
            privParams.logSurvivorsMice= bVal; break;
        }
        else if (name == "logsurvivorscats" && isBool) {
            privParams.logSurvivorsCats = bVal; break;
        }
        else if (name == "logeatenmice" && isBool) {
            privParams.logEatenMice = bVal; break;
        }
        else if (name == "updategraphlogstride" && isUint && uVal > 0) {
            privParams.updateGraphLogStride = uVal; break;
        }
        else if (name == "updategraphlogstride" && val == "videoStride") {
            privParams.updateGraphLogStride = privParams.videoStride; break;
        }
        else if (name == "deterministic" && isBool) {
            privParams.deterministic = bVal; break;
        }
        else if (name == "rngseed" && isUint) {
            privParams.RNGSeed = uVal; break;
        }
        else if (name.front == "[" && name.back == "]") {
            std::cout << "Section header: " << val << std::endl;
        }
        else {
            std::cout << "Invalid param: " << name << " = " << val << std::endl;
        }
    } while (0);
}


void ParamManager::updateFromConfigFile(unsigned generationNumber)
{
    // std::ifstream is RAII, i.e. no need to call close
    std::ifstream cFile(configFilename.c_str());
    if (cFile.is_open()) {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                 line.end());
            if(line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);

            // Process the generation specifier if present:
            auto generationDelimiterPos = name.find("@");
            if (generationDelimiterPos < name.size()) {
                auto generationSpecifier = name.substr(generationDelimiterPos + 1);
                bool isUint = checkIfUint(generationSpecifier);
                if (!isUint) {
                    std::cerr << "Invalid generation specifier: " << name << ".\n";
                    continue;
                }
                unsigned activeFromGeneration = (unsigned)std::stol(generationSpecifier);
                if (activeFromGeneration > generationNumber) {
                    continue; // This parameter value is not active yet
                }
                else if (activeFromGeneration == generationNumber) {
                    // Parameter value became active at exactly this generation number
                    privParams.parameterChangeGenerationNumber = generationNumber; 
                }
                name = name.substr(0, generationDelimiterPos);
            }

            std::transform(name.begin(), name.end(), name.begin(),
                        [](unsigned char c){ return std::tolower(c); });
            auto value0 = line.substr(delimiterPos + 1);
            auto delimiterComment = value0.find("#");
            auto value = value0.substr(0, delimiterComment);
            auto rawValue = value;
            value.erase(std::remove_if(value.begin(), value.end(), isspace),
                                 value.end());
            //std::cout << name << " " << value << '\n' << std::endl;
            if (generationNumber < 1 || (name != "miceratio" && name != "population")) {
            ingestParameter(name, value);
            }
        }
    }
    else {
        std::cerr << "Couldn't open config file " << configFilename << ".\n" << std::endl;
    }
}


// Check parameter ranges, reasonableness, coherency, whatever. This is
// typically called only once after the parameters are first read.
void ParamManager::checkParameters()
{
    if (privParams.deterministic && privParams.numThreads != 1) {
        std::cerr << "Warning: When deterministic is true, you probably want to set numThreads = 1." << std::endl;
    }
}


// Set output path (the path to the folder, in which the output data of the simulation is saved)
void ParamManager::setOutputPath()
{
    privParams.outputPath = privParams.outputDir + "/" + privParams.experimentDir + "/" + privParams.replicateDir + "/";
}


} // end namespace BS
