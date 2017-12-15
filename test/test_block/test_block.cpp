//testgcd.cpp
#include <gtest/gtest.h>
#include "block.h"
#include "log.h"

TEST(create_filesystem, errorcode)
{
    ASSERT_EQ(4096, create_filesystem("./filesystem.txt"));
}

TEST(load_filesystem, load_exist_filesystem)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));
}

TEST(load_filesystem, load_non_exist_filesystem)
{
    ASSERT_EQ(-1, load_filesystem("./aaaa.txt"));
}

TEST(load_filesystem, load_no_good_filesystem)
{
    file_state = 1;
    ASSERT_EQ(-4, load_filesystem("./bug_filesystem.txt"));
}

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

TEST(modify_block, errorcode)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));
    int block_ID = 513;
    char buffer[11] = {0};
    ASSERT_EQ(10, modify_block(block_ID, (void *)"new hihihi", 10));
    file_state = open("./filesystem.txt", O_RDWR);
    lseek(file_state, 513*BLOCK_SIZE, SEEK_SET);
    read(file_state, buffer, 10);
    LOG_DEBUG("buffer = %s\n", buffer);
    ASSERT_STREQ(buffer, "new hihihi");
    
    
    ASSERT_EQ(-2, modify_block(7890, (void *)"NONONO", 6));
    ASSERT_EQ(0, modify_block(2, NULL, 0));
    ASSERT_EQ(-3, modify_block(-8, (void *)"haha no modify_block", 19));
    ASSERT_EQ(-2, modify_block(0, (void *)"I am bad", 8));
}

TEST(read_block, errorcode)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));
    int block_ID = 513;
    char output[1024] = {0};
    ASSERT_EQ(1024, read_block(block_ID, (void *)output));
    LOG_DEBUG("\n\nread block id %d = %s\n\n", block_ID, output);
    ASSERT_STREQ(output, "new hihihi");


    ASSERT_EQ(-2, read_block(8998, (void *)output));
    ASSERT_EQ(-2, read_block(-2, (void *)output));
}

TEST(delete_block, errorcode)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));
    int file_state;
    file_state = open("./filesystem.txt", O_RDWR);
    lseek(file_state, strlen(FILE_SYSTEM_HEADER), SEEK_SET);

    char check[NUMBER_OF_BLOCKS/8] = {0};
    read(file_state, check, NUMBER_OF_BLOCKS/8);

    LOG_DEBUG("check before delete block = %x\n", check[64]);
    ASSERT_EQ(0x3f, check[64]);

    ASSERT_EQ(0, delete_block(513));
    lseek(file_state, strlen(FILE_SYSTEM_HEADER), SEEK_SET);
    read(file_state, check, NUMBER_OF_BLOCKS/8);
    LOG_DEBUG("check after delete block = %x\n", check[64]);
    ASSERT_EQ(0x3d, check[64]);

    close(file_state);


    ASSERT_EQ(-2, delete_block(1099));
    ASSERT_EQ(-2, delete_block(-9));

}

TEST(modify_super_block, errorcode)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));

    struct test {
        char a;
        char b;
        int x;
    } te;

    te.a = '1';
    te.b = 'b';
    te.x = 99;


    ASSERT_EQ(sizeof(te), modify_super_block(&te, sizeof(te)));
}

TEST(read_super_block, errorcode)
{
    ASSERT_EQ(0, load_filesystem("./filesystem.txt"));

    struct test {
        char a;
        char b;
        int x;
    } te;

    ASSERT_EQ(sizeof(te), read_super_block(&te, sizeof(te)));
    ASSERT_EQ('1', te.a);
    ASSERT_EQ('b', te.b);
    ASSERT_EQ(99, te.x);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
