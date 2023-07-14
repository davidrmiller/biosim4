s/^TEST(\(.*\)Tests, \(.*\))/TEST_CASE("\2","[\1]")/
s/EXPECT_EQ(\(.*\), *\(.*\));/CHECK(\1 == \2);/
s/EXPECT_NE(\(.*\), *\(.*\));/CHECK_FALSE(\1 == \2);/
s/EXPECT_TRUE\(.*\);/CHECK\1;/