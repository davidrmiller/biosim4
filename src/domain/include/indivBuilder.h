#pragma once
#include <memory>
#include "./indiv.h"
#include "./genomeBuilder.h"
#include "./neuralNetBuilder.h"

#include "../../common/include/grid.h"
#include "../../dirFactory.h"

namespace BS
{
    class IndivBuilder {

        public:
            
            IndivBuilder(RandomUintGenerator &r, Grid &g, GenomeBuilder &builder, NeuralNetBuilder nBuilder, unsigned dist);

            void initIndividual(Indiv &i, uint16_t index);
            void reinitIndividual(Indiv &i, uint16_t index, const std::vector<std::shared_ptr<Genome>> &parentGenomes);
        
        private:

            Coord findEmptyLocation() const;

            RandomUintGenerator& randomUint;
            Grid &grid;
            GenomeBuilder &genomeBuilder;
            NeuralNetBuilder & nnetBuilder;

            unsigned longProbeDist;
    };

} // namespace BS

