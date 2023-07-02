#ifndef COLUMN_H_INCLUDED
#define COLUMN_H_INCLUDED

#include <cstdint>
#include <vector>
#include <functional>

namespace BS {

    /**
     * A Column of cells within a Grid
    */
    struct Column {

        Column(uint16_t numRows);
    
        void zeroFill();

        uint16_t& operator[](uint16_t rowNum);
        
        uint16_t operator[](uint16_t rowNum) const;
        
        size_t size() const;

    private:
        std::vector<uint16_t> data;
    };

} // end namespace BS

#endif // COLUMN_H_INCLUDED
