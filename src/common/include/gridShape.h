#pragma once

#include "gridLocationVisitor.h"

namespace BS {

    class GridShape {

        public:
            virtual void accept(GridLocationVisitor &visitor) = 0;
            
            virtual ~GridShape();
    };
};