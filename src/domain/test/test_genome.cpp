#include "../include/genome.h"
#include "./test_helper.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestSize","[Genome]") {
        
        Genome genome;
    
        CHECK(genome.size() == 0);
    }

    TEST_CASE("TestEmpty","[Genome]") {
        
        Genome genome;
    
        CHECK(genome.empty());

        Gene g = makeRandomGene();

        genome.add(g);

        CHECK_FALSE(genome.empty());
    }

    TEST_CASE("TestAdd","[Genome]") {
        
        const std::shared_ptr<Genome> genome = std::make_shared<Genome>();
        genome->add(makeRandomGene());

        CHECK(genome->size() == 1);

        genome->add(makeRandomGene());

        CHECK(genome->size() == 2);
    }

    TEST_CASE("TestGeneAt","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeRandomGene());
        }

        Gene &gene2 = genome.geneAt(0);

        gene2.sourceType = SENSOR;

        Gene &gene3 = genome.geneAt(0);
        CHECK(gene3.sourceType == SENSOR);
    }

    TEST_CASE("TestErase","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeGene(SENSOR, i, NEURON, i, i*10));
        }

        CHECK(genome.size() == 10);

        genome.erase(5);

        CHECK(genome.size() == 5);

        // assert that the last 5 Genes remain
        for (int i = 5; i < 10; i++){
            Gene g = genome.geneAt(i-5);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }
    }

    TEST_CASE("TestEraseCheckBounds","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeRandomGene());
        }

        CHECK(genome.size() == 10);

        genome.erase(50);

        CHECK(genome.size() == 0);

        genome.erase(50);

        CHECK(genome.size() == 0);
    }

    TEST_CASE("TestEraseBack","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeGene(SENSOR, i, NEURON, i, i*10));
        }

        CHECK(genome.size() == 10);

        genome.eraseBack(5);

        CHECK(genome.size() == 5);

        // assert that the first 5 Genes remain
        for (int i = 0; i < 5; i++){
            Gene g = genome.geneAt(i);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }

    }

    TEST_CASE("TestEraseBackChecksBounds","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeRandomGene());
        }

        CHECK(genome.size() == 10);

        genome.eraseBack(50);

        CHECK(genome.size() == 0);

    }

    TEST_CASE("TestEraseAt","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeGene(SENSOR, i, NEURON, i, i*10));
        }

        CHECK(genome.size() == 10);

        genome.eraseAt(0);

        CHECK(genome.size() == 9);

        // assert that the first Gene was removed
        for (int i = 0; i < 9; i++){
            Gene g = genome.geneAt(i);
            CHECK(g.sourceNum == i+1);
            CHECK(g.sinkNum == i+1);
        }

        genome.eraseAt(8);

        CHECK(genome.size() == 8);

        // assert that the last Gene was removed
        for (int i = 0; i < 8; i++){
            Gene g = genome.geneAt(i);
            CHECK(g.sourceNum == i+1);
            CHECK(g.sinkNum == i+1);
        }
    }

    TEST_CASE("TestEraseAtChecksBounds","[Genome]") {
        
        Genome genome;

        for (int i = 0; i < 10; i++){
            genome.add(makeGene(SENSOR, i, NEURON, i, i*10));
        }

        CHECK(genome.size() == 10);

        genome.eraseAt(20);

        CHECK(genome.size() == 10);

    }

    TEST_CASE("TestOverlayWithSliceOf","[Genome]") {
        
        const std::shared_ptr<Genome> genome1 = std::make_shared<Genome>();
        // Genome genome1;

        for (int i = 0; i < 10; i++){
            genome1->add(makeGene(SENSOR, i, NEURON, i, i*10));
        }

        const std::shared_ptr<Genome> genome2 = std::make_shared<Genome>();

        for (int i = 0; i < 10; i++){
            genome2->add(makeGene(SENSOR, i+100, NEURON, i+100, i*100));
        }

        genome1->overlayWithSliceOf(genome2, 0, 5);

        CHECK(genome1->size() == 10);

        // first 5 Genes from 1 should be copied into 2
        for (int i = 0; i < 5; i++){
            Gene g = genome1->geneAt(i);
            CHECK(g.sourceNum == i+100);
            CHECK(g.sinkNum == i+100);
        }

        // last 5 Genes from 1 should be unchanged
        for (int i = 0; i < 5; i++){
            Gene g = genome1->geneAt(i+5);
            CHECK(g.sourceNum == i+5);
            CHECK(g.sinkNum == i+5);
        }

        genome2->overlayWithSliceOf(genome1, 5, 10);

        CHECK(genome2->size() == 10);

        // first 5 Genes from 2 should be unchanged
        for (int i = 0; i < 5; i++){
            Gene g = genome2->geneAt(i);
            CHECK(g.sourceNum == i+100);
            CHECK(g.sinkNum == i+100);
        }

        // last 5 Genes from 2 should be copied from 1
        for (int i = 0; i < 5; i++){
            Gene g = genome2->geneAt(i+5);
            CHECK(g.sourceNum == i+5);
            CHECK(g.sinkNum == i+5);
        }
    }

    TEST_CASE("TestClone","[Genome]") {
        
        Genome genome1;
        const std::shared_ptr<Genome> genome2 = std::make_shared<Genome>();
        
        for (int i = 0; i < 10; i++){
            genome2->add(makeGene(SENSOR, i, NEURON, i, i));
        }

        CHECK(genome1.size() == 0);

        genome1.clone(genome2);
    
        CHECK(genome1.size() == 10);
        for (int i = 0; i < 10; i++){
            Gene g = genome1.geneAt(i);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }

        // ensure the clone has copied the Genes
        for (int i = 0; i < 10; i++){
            Gene &g = genome1.geneAt(i);
            g.sourceNum = 20;
            g.sinkNum = 30;
        }

        for (int i = 0; i < 10; i++){
            Gene g = genome1.geneAt(i);
            CHECK(g.sourceNum == 20);
            CHECK(g.sinkNum == 30);
        }

        for (int i = 0; i < 10; i++){
            Gene g = genome2->geneAt(i);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }
    }

    TEST_CASE("TestCreateFromOtherGenome","[Genome]") {
        
        // Genome genome1;
        const std::shared_ptr<Genome> genome2 = std::make_shared<Genome>();
        
        for (int i = 0; i < 10; i++){
            genome2->add(makeGene(SENSOR, i, NEURON, i, i));
        }

        Genome genome1 = Genome(genome2);
    
        CHECK(genome1.size() == 10);
        for (int i = 0; i < 10; i++){
            Gene g = genome1.geneAt(i);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }

        // ensure the clone has copied the Genes
        for (int i = 0; i < 10; i++){
            Gene &g = genome1.geneAt(i);
            g.sourceNum = 20;
            g.sinkNum = 30;
        }

        for (int i = 0; i < 10; i++){
            Gene g = genome1.geneAt(i);
            CHECK(g.sourceNum == 20);
            CHECK(g.sinkNum == 30);
        }

        for (int i = 0; i < 10; i++){
            Gene g = genome2->geneAt(i);
            CHECK(g.sourceNum == i);
            CHECK(g.sinkNum == i);
        }
    }

}