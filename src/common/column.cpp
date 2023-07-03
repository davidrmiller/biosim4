#include "column.h"

namespace BS {

    Column::Column(uint16_t numRows) : data 
    { 
        std::vector<uint16_t>(numRows, 0) 
    } { }

    void Column::zeroFill() 
    { 
        std::fill(data.begin(), data.end(), 0); 
    }

    uint16_t& Column::operator[](uint16_t rowNum) 
    { 
        return data[rowNum]; 
    }

    uint16_t Column::operator[](uint16_t rowNum) const
    { 
        return data[rowNum]; 
    }

    size_t Column::size() const 
    { 
        return data.size();
    }
}