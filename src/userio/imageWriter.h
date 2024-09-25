#ifndef IMAGEWRITER_H_INCLUDED
#define IMAGEWRITER_H_INCLUDED

// Creates a graphic frame for each simStep, then
// assembles them into a video at the end of a generation.

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include "../ai/indiv.h"
#include "../params.h"
#include "../peeps.h"

namespace BS {

// This holds all data needed to construct one image frame. The data is
// cached in this structure so that the image writer can work on it in
// a separate thread while the main thread starts a new simstep.
struct ImageFrameData {
    unsigned simStep;
    unsigned generation;
    std::vector<Coord> indivLocs;
    std::vector<uint8_t> indivColors;
    std::vector<Coord> barrierLocs;
    typedef std::vector<std::vector<uint8_t>> SignalLayer;  // [x][y]
    std::vector<SignalLayer> signalLayers; // [layer][x][y]
};


class ImageWriter {
public:
    ImageWriter();
    void startNewGeneration();
    bool saveVideoFrameSync(unsigned simStep, unsigned generation);
    void saveGenerationVideo(unsigned generation);
    void abort();
    void endOfStep(unsigned simStep, unsigned generation);
    void endOfGeneration(unsigned generation);
    std::atomic<unsigned> droppedFrameCount;
private:
    std::atomic<bool> busy;
    std::mutex mutex_;
    std::condition_variable condVar;
    bool dataReady;

    ImageFrameData data;
    bool abortRequested;
    unsigned skippedFrames;
};

} // end namespace BS

#endif // IMAGEWRITER_H_INCLUDED
