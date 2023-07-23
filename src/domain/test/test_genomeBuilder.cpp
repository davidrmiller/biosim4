#include "../include/genomeBuilder.h"
#include "../../random.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestMakeRandomGenome","[GenomeBuilder]") {
        
        RandomUintGenerator randomUint;
        randomUint.initialize(true, 12345678);

        unsigned min = 24;
        unsigned max = 24;
         
        unsigned genomeMaxLength = 300; 
        double pointMutationRate = 0.001;
        double geneInsertionDeletionRate = 0.0;
        double deletionRatio = 0.5;
        bool sexualReproduction = true;
        bool chooseParentsByFitness = true;

        GenomeBuilder b = GenomeBuilder(
            randomUint,
            min,
            max,
            genomeMaxLength,
            pointMutationRate,
            geneInsertionDeletionRate,
            deletionRatio,
            sexualReproduction,
            chooseParentsByFitness
        );
        
        Genome genome = b.makeRandomGenome();

        CHECK(genome.size() >= min);
        CHECK(genome.size() <= max);
        bool sourceTypeResult = false;
        bool sinkTypeResult = false;

        for (Gene gene : genome.genes()) {
            sourceTypeResult = (gene.sourceType == SENSOR || gene.sourceType == NEURON);
            CHECK(sourceTypeResult);
            
            sinkTypeResult = (gene.sinkType == NEURON || gene.sinkType == ACTION);
            CHECK(sinkTypeResult);
        }
    }
}