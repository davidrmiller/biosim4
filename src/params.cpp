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
//    2. Add a member and its default value to privParams in ParamManager::setDefault()
//          in params.cpp.
//    3. Add an else clause to ParamManager::ingestParameter() in params.cpp.
//    4. Add a line to the user's parameter file (default name biosim4.ini)

namespace BS {

void ParamManager::setDefaults()
{
    privParams.sizeX = 128;
    privParams.sizeY = 128;
    privParams.challenge = 0;

    privParams.genomeInitialLengthMin = 16;
    privParams.genomeInitialLengthMax = 16;
    privParams.genomeMaxLength = 20;
    privParams.logDir = "./logs/";
    privParams.imageDir = "./images/";
    privParams.population = 100;
    privParams.stepsPerGeneration = 100;
    privParams.maxGenerations = 100;
    privParams.barrierType = 0;
    privParams.replaceBarrierType = 0;
    privParams.replaceBarrierTypeGenerationNumber = (uint32_t)-1;
    privParams.numThreads = 1;
    privParams.signalLayers = 1;
    privParams.maxNumberNeurons = privParams.genomeMaxLength / 2;
    privParams.pointMutationRate = 0.0001;
    privParams.geneInsertionDeletionRate = 0.0001;
    privParams.deletionRatio = 0.7;
    privParams.killEnable = false;
    privParams.sexualReproduction = true;
    privParams.chooseParentsByFitness = true;
    privParams.populationSensorRadius = 2.0;
    privParams.signalSensorRadius = 1;
    privParams.responsiveness = 0.5;
    privParams.responsivenessCurveKFactor = 2;
    privParams.longProbeDistance = 16;
    privParams.shortProbeBarrierDistance = 3;
    privParams.valenceSaturationMag = 0.5;
    privParams.saveVideo = true;
    privParams.videoStride = 1;
    privParams.videoSaveFirstFrames = 0;
    privParams.displayScale = 1;
    privParams.agentSize = 2;
    privParams.genomeAnalysisStride = 1;
    privParams.displaySampleGenomes = 0;
    privParams.genomeComparisonMethod = 1;
    privParams.updateGraphLog = false;
    privParams.updateGraphLogStride = 16;
    privParams.graphLogUpdateCommand = "/usr/bin/gnuplot --persist ./tools/graphlog.gp";
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
    bool isInt = checkIfInt(val);
    int iVal = isInt ? std::stoi(val.c_str()) : 0;
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
        else if (name == "challenge" && isUint && uVal < (uint16_t)-1) {
            privParams.challenge = uVal; break;
        }
        else if (name == "genomeinitiallengthmin" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.genomeInitialLengthMin = uVal; break;
        }
        else if (name == "genomeinitiallengthmax" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.genomeInitialLengthMax = uVal; break;
        }
        else if (name == "logdir") {
            privParams.logDir = val; break;
        }
        else if (name == "imagedir") {
            privParams.imageDir = val; break;
        }
        else if (name == "population" && isUint && uVal > 0 && uVal < (uint32_t)-1) {
            privParams.population = uVal; break;
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
        else if (name == "replacebarriertype" && isUint && uVal < (uint32_t)-1) {
            privParams.replaceBarrierType = uVal; break;
        }
        else if (name == "replacebarriertypegenerationnumber" && isInt && iVal >= -1) {
            privParams.replaceBarrierTypeGenerationNumber = (iVal == -1 ? (uint32_t)-1 : iVal); break;
        }
        else if (name == "numthreads" && isUint && uVal > 0 && uVal < (uint16_t)-1) {
            privParams.numThreads = uVal; break;
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
        else if (name == "genomecomparisonmethod" && isUint) {
            privParams.genomeComparisonMethod = uVal; break;
        }
        else if (name == "updategraphlog" && isBool) {
            privParams.updateGraphLog = bVal; break;
        }
        else if (name == "updategraphlogstride" && isUint && uVal > 0) {
            privParams.updateGraphLogStride = uVal; break;
        }
        else if (name == "updategraphlogstride" && val == "videoStride") {
            privParams.updateGraphLogStride = privParams.videoStride; break;
        }
        else {
            std::cout << "Invalid param: " << name << " = " << val << std::endl;
        }
    } while (0);
}


void ParamManager::updateFromConfigFile()
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
            std::transform(name.begin(), name.end(), name.begin(),
                        [](unsigned char c){ return std::tolower(c); });
            auto value0 = line.substr(delimiterPos + 1);
            auto delimiterComment = value0.find("#");
            auto value = value0.substr(0, delimiterComment);
            auto rawValue = value;
            value.erase(std::remove_if(value.begin(), value.end(), isspace),
                                 value.end());
            //std::cout << name << " " << value << '\n' << std::endl;
            ingestParameter(name, value);
        }
    }
    else {
        std::cerr << "Couldn't open config file " << configFilename << ".\n" << std::endl;
    }
}

} // end namespace BS
