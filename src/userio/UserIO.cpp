#include "UserIO.h"

namespace BS {

    UserIO::UserIO(bool windowInit, bool videoSaveInit)
    {
        if (windowInit) 
            this->sfmlView = new SFMLUserIO();
        if (videoSaveInit) 
            this->imageWriter = new ImageWriter();        
    }

    UserIO::~UserIO()
    {
        if (this->sfmlView != nullptr)
            delete this->sfmlView;
        if (this->imageWriter != nullptr)
            delete this->imageWriter;
    }

    bool UserIO::isStopped()
    {
        if (this->sfmlView != nullptr)
            return this->sfmlView->isStopped();
        return false;
    }

    void UserIO::checkUserInput()
    {
        if (this->sfmlView != nullptr) {
            this->sfmlView->updatePollEvents();
            this->sfmlView->updateInput();
        }
    }

    void UserIO::endOfStep(unsigned simStep, unsigned generation)
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->endOfStep();

        if (this->imageWriter != nullptr)
            this->imageWriter->endOfStep(simStep, generation);
    }

    void UserIO::endOfGeneration(unsigned generation)
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->endOfGeneration(generation);

        if (this->imageWriter != nullptr)
            this->imageWriter->endOfGeneration(generation);
    }

    void UserIO::log(std::string message)
    {
        if (this->sfmlView != nullptr)
            this->sfmlView->log(message);
        
        std::cout << message << std::endl;
    }

    bool UserIO::isPaused()
    {
        if (this->sfmlView != nullptr)
            return this->sfmlView->isPaused();

        return false;
    }
}