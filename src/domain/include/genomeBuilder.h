#pragma once

#include <memory>
#include "./genome.h"
#include "../../random.h"

namespace BS
{
    class GenomeBuilder {

        public:
            GenomeBuilder(RandomUintGenerator &r, unsigned min, unsigned max, unsigned gml, double pmr, double gidr, double dr, bool sr, bool cp);

            std::shared_ptr<Genome> makeRandomGenome();
            
            std::shared_ptr<Genome> generateChildGenome(const std::vector<std::shared_ptr<Genome>> &parentGenomes);

        private:
            RandomUintGenerator& randomUint;
            unsigned minLength;
            unsigned maxLength;
            unsigned genomeMaxLength;
            double pointMutationRate;
            double geneInsertionDeletionRate;
            double deletionRatio;
            bool sexualReproduction;
            bool chooseParentsByFitness;
            
            Gene makeRandomGene();
            void applyPointMutations(std::shared_ptr<Genome> genome);
            void cropLength(std::shared_ptr<Genome> genome, unsigned length);
            void randomBitFlip(std::shared_ptr<Genome> genome);
            void randomInsertDeletion(std::shared_ptr<Genome> genome);

    };


} // namespace BS
