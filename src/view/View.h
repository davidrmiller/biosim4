#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

// Operate with view of simulation throught console, images, or window

#include "imageWriter.h"
#include "SFMLView.h"

namespace BS
{
    class View
    {
    public:
        View(bool windowInit, bool videoSaveInit);
        ~View();

        bool isStopped();
        void checkUserInput();
        void endOfStep(unsigned simStep, unsigned generation);
        void endOfGeneration(unsigned generation);

    private:
        ImageWriter* imageWriter = nullptr;
        SFMLView* sfmlView = nullptr;
    };
}

#endif // VIEW_H_INCLUDED