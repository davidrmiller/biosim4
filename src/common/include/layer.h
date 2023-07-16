#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include "column.h"

namespace BS {
    
    class Layer {
    
        public:
            Layer(uint16_t numCols, uint16_t numRows);
            Column& operator[](uint16_t colNum);
            const Column& operator[](uint16_t colNum) const;
            void zeroFill();
        
        private:
            std::vector<Column> data;
    
    };
}