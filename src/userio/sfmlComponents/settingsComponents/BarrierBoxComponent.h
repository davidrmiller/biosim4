#ifndef BARRIERBOXCOMPONENT_H_INCLUDED
#define BARRIERBOXCOMPONENT_H_INCLUDED

#include "AbstractComboBoxComponent.h"
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    class BarrierBoxComponent : public AbstractComboBoxComponent
    {
    public:
        BarrierBoxComponent(std::function<void(std::string, std::string)> settingsChange);
        void setFromParams() override;
    private:
        void initItems() override;
        void onItemSelect(int index) override;
    };
}

#endif