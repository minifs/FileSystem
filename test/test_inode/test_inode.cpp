//testgcd.cpp
#include <gtest/gtest.h>
#include "block.h"
#include "log.h"

TEST(write_block, errorcode)
{
    ASSERT_EQ(4096, create_filesystem("./filesystem.txt"));
    int block_ID;
    ASSERT_EQ(6, write_block(&block_ID, (void *)"hihihi", 6));
    ASSERT_EQ(19, write_block(&block_ID, (void *)"Yes it can write it", 19));
    ASSERT_EQ(8, write_block(&block_ID, (void *)"Third on", 8));
    ASSERT_EQ(3, write_block(&block_ID, (void *)"abc", 3));
    ASSERT_EQ(3, write_block(&block_ID, (void *)"abc", 3));
}

TEST(write_largelarge_file, errorcode)
{
    
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
