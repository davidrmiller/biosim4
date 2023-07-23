#include "../include/indivBuilder.h"


namespace BS
{

    IndivBuilder::IndivBuilder(RandomUintGenerator &r, Grid &g, GenomeBuilder &builder, unsigned dist) :
        randomUint{r}, grid{g}, genomeBuilder{builder}, longProbeDist{dist}
    {}

    /**
     * 
    */
    void IndivBuilder::initIndividual(Indiv &i, uint16_t index)
    {
        Coord loc = findEmptyLocation();

        i.initialize(
            index, 
            loc,
            genomeBuilder.makeRandomGenome(),
            DirFactory::random8(),
            longProbeDist
        );

        // place the indiv on the Grid
        grid.set(loc, index);
    }

    void IndivBuilder::reinitIndividual(Indiv &i, uint16_t index, const std::vector<std::shared_ptr<Genome>> &parentGenomes)
    {
        Coord loc = findEmptyLocation();

        i.initialize(
            index, 
            loc,
            genomeBuilder.generateChildGenome(parentGenomes),
            DirFactory::random8(),
            longProbeDist
        );

        // place the indiv on the Grid
        grid.set(loc, index);
    }

    Coord IndivBuilder::findEmptyLocation() const
    {
        Coord loc;
        uint16_t size_x = grid.sizeX();
        uint16_t size_y = grid.sizeY();

        while (true) {
            loc.x = randomUint(0, size_x - 1);
            loc.y = randomUint(0, size_y - 1);
            if (grid.isEmptyAt(loc)) {
                break;
            }
        }
        return loc;
    }

} // namespace BS
