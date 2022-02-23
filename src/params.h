#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

// Global simulator parameters

#include <string>

// To add a new parameter:
//    1. Add a member to struct Params in params.h.
//    2. Add a member and its default value to privParams in ParamManager::setDefault()
//          in params.cpp.
//    3. Add an else clause to ParamManager::ingestParameter() in params.cpp.
//    4. Add a line to the user's parameter file (default name biosim4.ini)

namespace BS {

enum class RunMode { STOP, RUN, PAUSE, ABORT };
extern RunMode runMode;

// A private copy of Params is initialized by ParamManager::init(), then modified by
// UI events by ParamManager::uiMonitor(). The main simulator thread can get an
// updated, read-only, stable snapshot of Params with ParamManager::paramsSnapshot.

struct Params {
    unsigned population; // >= 0
    unsigned stepsPerGeneration; // > 0
    unsigned maxGenerations; // >= 0
    unsigned numThreads; // > 0
    unsigned signalLayers; // >= 0
    unsigned genomeMaxLength; // > 0
    unsigned maxNumberNeurons; // > 0
    double pointMutationRate; // 0.0..1.0
    double geneInsertionDeletionRate; // 0.0..1.0
    double deletionRatio; // 0.0..1.0
    bool killEnable;
    bool sexualReproduction;
    bool chooseParentsByFitness;
    float populationSensorRadius; // > 0.0
    unsigned signalSensorRadius; // > 0
    float responsiveness; // >= 0.0
    unsigned responsivenessCurveKFactor; // 1, 2, 3, or 4
    unsigned longProbeDistance; // > 0
    unsigned shortProbeBarrierDistance; // > 0
    float valenceSaturationMag;
    bool saveVideo;
    unsigned videoStride; // > 0
    unsigned videoSaveFirstFrames; // >= 0, overrides videoStride
    unsigned displayScale;
    unsigned agentSize;
    unsigned genomeAnalysisStride; // > 0
    unsigned displaySampleGenomes; // >= 0
    unsigned genomeComparisonMethod; // 0 = Jaro-Winkler; 1 = Hamming
    bool updateGraphLog;
    unsigned updateGraphLogStride; // > 0
    unsigned challenge;
    unsigned barrierType; // >= 0
    bool deterministic;
    unsigned RNGSeed; // >= 0

    // These must not change after initialization
    uint16_t sizeX; // 2..0x10000
    uint16_t sizeY; // 2..0x10000
    unsigned genomeInitialLengthMin; // > 0 and < genomeInitialLengthMax
    unsigned genomeInitialLengthMax; // > 0 and < genomeInitialLengthMin
    std::string logDir;
    std::string imageDir;
    std::string graphLogUpdateCommand;

    // These are updated automatically and not set via the parameter file
    unsigned parameterChangeGenerationNumber; // the most recent generation number that an automatic parameter change occured at
};

class ParamManager {
public:
    const Params &getParamRef() const { return privParams; } // for public read-only access
    void setDefaults();
    void registerConfigFile(const char *filename);
    void updateFromConfigFile(unsigned generationNumber);
    void checkParameters();
private:
    Params privParams;
    std::string configFilename;
    time_t lastModTime; // when config file was last read
    void ingestParameter(std::string name, std::string val);
};

// Returns a copy of params with default values overridden by the values
// in the specified config file. The filename of the config file is saved
// inside the params for future reference.
Params paramsInit(int argc, char **argv);

} // end namespace BS

#endif // PARAMS_H_INCLUDED
