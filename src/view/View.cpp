#include "View.h"

namespace BS {

    View::View(bool windowInit, bool videoSaveInit)
    {
        if (windowInit) 
            this->sfmlView = new SFMLView();
        if (videoSaveInit) 
            this->imageWriter = new ImageWriter();        
    }

    View::~View()
    {
        if (this->sfmlView != nullptr)
            delete this->sfmlView;
        if (this->imageWriter != nullptr)
            delete this->imageWriter;
    }

    bool View::isStopped()
    {
        if (this->sfmlView != nullptr)
            return this->sfmlView->isStopped();
        return false;
    }

    void View::checkUserInput()
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->updatePollEvents();
    }

    void View::endOfStep(unsigned simStep, unsigned generation)
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->endOfStep();

        if (this->imageWriter != nullptr)
            this->imageWriter->endOfStep(simStep, generation);
    }

    void View::endOfGeneration(unsigned generation)
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->endOfGeneration(generation);

        if (this->imageWriter != nullptr)
            this->imageWriter->endOfGeneration(generation);
    }
}