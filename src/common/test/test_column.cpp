#include "../include/column.h"
#include <catch2/catch_test_macros.hpp>

namespace BS {

TEST_CASE("TestColumn","[Column]") {
    
    uint16_t rows = 128;
    Column col = Column(rows);

    CHECK(rows == col.size());

}

TEST_CASE("TestColumnZeroFill","[Column]") {
    
    uint16_t rows = 128;
    Column col = Column(rows);

    col.zeroFill();

    CHECK(0 == col[1]);
    CHECK(0 == col[13]);
    CHECK(0 == col[127]);

}

}