#include <gtest/gtest.h>
#include "../src/column.h"

namespace BS {

TEST(ColumnTests, TestColumn) {
    
    uint16_t rows = 128;
    Column col = Column(rows);

    EXPECT_EQ(rows, col.size());

}

TEST(ColumnTests, TestColumnZeroFill) {
    
    uint16_t rows = 128;
    Column col = Column(rows);

    col.zeroFill();

    EXPECT_EQ(0, col[1]);
    EXPECT_EQ(0, col[13]);
    EXPECT_EQ(0, col[127]);

}

}