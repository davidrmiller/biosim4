#pragma once

#include "./genome.h"
#include "../../random.h"

namespace BS
{
    class GenomeBuilder {

        public:
            GenomeBuilder(RandomUintGenerator &r, unsigned min, unsigned max, unsigned gml, double pmr, double gidr, double dr, bool sr, bool cp);

            Genome makeRandomGenome();
            
            Genome generateChildGenome(const std::vector<Genome> &parentGenomes);

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
            void applyPointMutations(Genome &genome);
            void cropLength(Genome &genome, unsigned length);
            void randomBitFlip(Genome &genome);
            void randomInsertDeletion(Genome &genome);

    };


} // namespace BS
