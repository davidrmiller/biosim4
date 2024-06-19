#ifndef USERIO_H_INCLUDED
#define USERIO_H_INCLUDED

// Operate with view of simulation throught console, images, or window

#include "imageWriter.h"
#include "SFMLUserIO.h"
#include "../peeps.h"

namespace BS
{
    extern const Params &p;
    
    class UserIO
    {
    public:
        UserIO(bool windowInit, bool videoSaveInit, bool saveGenerations_);
        ~UserIO();

        bool isStopped();
        void handleStep(unsigned simStep, unsigned generation);
        void endOfGeneration(unsigned generation);
        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);

        void log(std::string message);

        bool isPaused();

        bool getLoadFileSelected();
        std::string getLoadFilename();
        void cleanLoadSelection();
    private:
        ImageWriter* imageWriter = nullptr;
        SFMLUserIO* sfmlView = nullptr;

        bool saveGenerations;
    };
}

#endif // USERIO_H_INCLUDED