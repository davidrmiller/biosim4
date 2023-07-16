#include "../include/layer.h"

namespace BS {

    Layer::Layer(uint16_t numCols, uint16_t numRows) : data 
    { std::vector<Column>(numCols, Column(numRows)) } 
    { 
        
    }
    
    Column& Layer::operator[](uint16_t colNum)
    {
        return data[colNum]; 
    }

    const Column& Layer::operator[](uint16_t colNum) const
    { 
        return data[colNum]; 
    }
    
    void Layer::zeroFill()
    { 
        for (Column &col : data) {
             col.zeroFill(); 
        } 
    }

} // namespace BS
