#pragma once
#include <memory>
#include <vector>
#include <cstddef>
#include "./gene.h"

#include "../../sensors-actions.h"

namespace BS
{
    
    class Genome {

        public:
            
            Genome();

            Genome(const std::shared_ptr<Genome> other);

            void add(Gene g);

            void renumber();

            // overwrite this Genomes genes with the others
            void clone(const std::shared_ptr<Genome> other);

            const std::vector<Gene>& genes() const;
            
            size_t size() const;
            bool empty() const;
            
            float jaroWinklerDistance(const std::shared_ptr<Genome> other) const;
            float hammingDistanceBytes(const std::shared_ptr<Genome> other) const;
            float hammingDistanceBits(const std::shared_ptr<Genome> other) const;

            void erase(unsigned length);
            void eraseBack(unsigned length);
            void eraseAt(unsigned index);

            const Gene& geneAt(unsigned index) const;
            Gene& geneAt(unsigned index);

            void overlayWithSliceOf(const std::shared_ptr<Genome> other, uint16_t index0, uint16_t index1);

            uint8_t color() const;

        private:
            std::vector<Gene> genome;
    };

} // namespace BS