//testgcd.cpp
#include <gtest/gtest.h>
#include "block.h"

TEST(create_block, errorcode)
{
	ASSERT_EQ(4096, create_block("./filesystem.txt", 5));
}

TEST(load_block, errorcode)
{
	ASSERT_EQ(0, load_block("./filesystem.txt"));
	ASSERT_EQ(-1, load_block("./aaaa.txt"));
	ASSERT_EQ(-4, load_block("./bug_filesystem.txt"));
}


int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
