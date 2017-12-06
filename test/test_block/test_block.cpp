//testgcd.cpp
#include <gtest/gtest.h>
#include "block.h"

TEST(create_block, errorcode)
{
	ASSERT_EQ(4096, create_block("./filesystem.txt"));
}

TEST(load_block, load_exist_filesystem)
{
	ASSERT_EQ(0, load_block("./filesystem.txt"));
}

TEST(load_block, load_non_exist_filesystem)
{
	ASSERT_EQ(-1, load_block("./aaaa.txt"));
}

TEST(load_block, load_no_good_filesystem)
{
	file_state = 1;
	ASSERT_EQ(-4, load_block("./bug_filesystem.txt"));
}

TEST(write_block, errorcode)
{
	ASSERT_EQ(4096, create_block("./filesystem.txt"));
	int block_ID;
	ASSERT_EQ(6, write_block(&block_ID, (void *)"hihihi", 6));
	ASSERT_EQ(19, write_block(&block_ID, (void *)"Yes it can write it", 19));
	ASSERT_EQ(8, write_block(&block_ID, (void *)"Third on", 8));
}

TEST(modify_block, errorcode)
{
	ASSERT_EQ(0, load_block("./filesystem.txt"));
	int block_ID = 1;
	ASSERT_EQ(10, modify_block(block_ID, (void *)"new hihihi", 10));
	ASSERT_EQ(-2, modify_block(7890, (void *)"NONONO", 6));
	ASSERT_EQ(0, modify_block(2, NULL, 0));
	ASSERT_EQ(-3, modify_block(-8, (void *)"haha no modify_block", 19));
	ASSERT_EQ(-2, modify_block(0, (void *)"I am bad", 8));
}

TEST(read_block, errorcode)
{
	ASSERT_EQ(0, load_block("./filesystem.txt"));
	int block_ID = 2;
	char output[1024] = {0};
	ASSERT_EQ(1024, read_block(block_ID, (void *)output));
        printf("\n\nread block id %d = %s\n\n", block_ID, output);

	ASSERT_EQ(-4, read_block(10, (void *)output));
	//printf("\n\nread block id %d = %s\n\n", block_ID, output);

	ASSERT_EQ(-2, read_block(8998, (void *)output));
	ASSERT_EQ(-3, read_block(-2, (void *)output));
}

TEST(delete_block, errorcode)
{
	ASSERT_EQ(0, load_block("./filesystem.txt"));
	
	ASSERT_EQ(1, block_map[2]);
	ASSERT_EQ(0, delete_block(2));
	ASSERT_EQ(0, block_map[2]);

	ASSERT_EQ(-2, delete_block(1099));
	ASSERT_EQ(-2, delete_block(-9));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
