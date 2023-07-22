#pragma once

#include "grid.h"
#include "../../random.h"

namespace BS {

    /**
     *
    */
    class GridBuilder
    {
        public:
            
            enum barrierType {
                none,
                vertical_bar,
                random_vertical_bar,
                five_bars_staggered,
                horizontal_bar,
                three_floating_islands,
                spots
            };

            GridBuilder(RandomUintGenerator &r);

            Grid createBarrier(Grid &g, GridBuilder::barrierType type);

        private:

            RandomUintGenerator& randomUint;
    };
    
}