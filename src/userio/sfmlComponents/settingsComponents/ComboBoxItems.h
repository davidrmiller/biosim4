#ifndef COMBOBOXITEMS_H_INCLUDED
#define COMBOBOXITEMS_H_INCLUDED


#include <string>
namespace BS
{
    struct ComboBoxItem
    {
        int index;
        unsigned value;
        std::string text;
        ComboBoxItem(){}
        ComboBoxItem(int index, unsigned value, std::string text) : index(index), value(value), text(text) {}
    };
}
#endif