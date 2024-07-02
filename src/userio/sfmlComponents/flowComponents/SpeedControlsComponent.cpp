#include "SpeedControlsComponent.h"

namespace BS
{
    SpeedControlsComponent::SpeedControlsComponent(tgui::Widget::Ptr positionReferenceComponent_, tgui::Container::Ptr container_, const float controlOffset_)
    {
        this->positionReferenceWidget = positionReferenceComponent_;
        this->container = container_;
        this->controlOffset = controlOffset_;
    }

    /**
     * Initializes the speed controls for the right panel component.
     *
     * @param min The minimum value for the speed controls.
     * @param max The maximum value for the speed controls.
     * @param initValue The initial value for the speed controls.
     * @param changeSpeedCallback A callback function that is called when the value of the speed controls changes.
     *
     * @throws None.
     */
    void SpeedControlsComponent::init(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback)
    {        
        // create SpinControl
        tgui::SpinControl::Ptr speedControls = tgui::SpinControl::create();

        // create MIN button
        tgui::Button::Ptr minButton = tgui::Button::create("MIN");
        minButton->setPosition({"5%", bindBottom(this->positionReferenceWidget) + this->controlOffset});
        minButton->onPress([changeSpeedCallback, min, speedControls]()
        {
            changeSpeedCallback(min);
            speedControls->setValue(min);
        });
        minButton->setHeight(this->positionReferenceWidget->getSize().y);
        minButton->setWidth("15%");
        this->container->add(minButton, "SpeedMinControls");

        // config SpinControl
        speedControls->setMinimum(min);
        speedControls->setMaximum(max);
        speedControls->setStep(1);
        speedControls->setValue(initValue);

        speedControls->setPosition({bindRight(minButton) + 5.f, bindBottom(this->positionReferenceWidget) + this->controlOffset});
        speedControls->setHeight(this->positionReferenceWidget->getSize().y);
        speedControls->setWidth("25%");

        speedControls->onValueChange([changeSpeedCallback](float value)
        {
            changeSpeedCallback(value);
        });
        this->container->add(speedControls, "SpeedControls");

        // create MAX button
        tgui::Button::Ptr maxButton = tgui::Button::create("MAX");
        maxButton->setPosition({bindRight(speedControls) + 5.f, bindBottom(this->positionReferenceWidget) + this->controlOffset});
        maxButton->onPress([changeSpeedCallback, max, speedControls]()
        {
            changeSpeedCallback(max);
            speedControls->setValue(max);
        });
        maxButton->setHeight(this->positionReferenceWidget->getSize().y);
        maxButton->setWidth("15%");
        this->container->add(maxButton, "SpeedMaxControls");

        this->labelReferenceWidget = minButton;
    }

    tgui::Widget::Ptr SpeedControlsComponent::getLabelReferenceWidget()
    {
        return this->labelReferenceWidget;
    }
}