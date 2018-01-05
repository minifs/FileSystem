#include "inode_entry.h"
#include "block.h"

int main ()
{
    create_filesystem("./filesystem.txt");

    init_superblock();
    //dump_inode_bitmap();
    
    //char test_file[100] = "The quick brown fox jumps over the lazy dog\0";
    // char test_bigfile[2000];
    // char read_file_buf[2000];

    // memset(test_bigfile, 88, 2000);
    // test_bigfile[1999] = '\0';

    // memset(read_file_buf, 0, 2000);

    /*
    // ------ bitmap test ------

    printf("------ bitmap test ------\n");

    // get bitmap
    int inode_id;
    inode_id = 87;
    printf("Before set inode 87: %X\n", query_inode_bitmap(inode_id));

    // bitmap set
    printf("Setting inode 87\n");
    set_inode_bitmap (87);
    printf("After set inode 87: %X\n", query_inode_bitmap(inode_id));

    // Clear bit 
    clear_inode_bitmap (87);
    printf("After clear inode 87: %X\n", query_inode_bitmap(inode_id));

    printf("------------------------\n\n");
    */

    /*
    // ------ clear inode test ------
    printf("------ clear inode test ------\n");
    clear_inode_bitmap (0);
    dump_inode_bitmap();

    printf("------------------------\n\n");
    */

    // ------ create inode test ------
    printf("------ create inode test ------\n");
    inode_entry test_inode_entry_1;
    init_inode(&test_inode_entry_1, sizeof(test_inode_entry_1));
    create_inode(&test_inode_entry_1);
    dump_inode(&test_inode_entry_1);

    // inode_entry test_inode_entry_2;
    // create_inode(&test_inode_entry_2);

    // printf("------------------------\n\n");

    // ------ delete inode test ------
    // printf("------ delete inode test ------\n");
    // printf("------------------------\n\n");

    // ------ Write Test ------
    // test_inode_entry_1.filesize = int(sizeof(test_bigfile));
    // write_file(&test_inode_entry_1, test_bigfile);
    // dump_inode(&test_inode_entry_1);

    // read_file(&test_inode_entry_1, (void *)read_file_buf);
    // printf("read result: %s\n", read_file_buf);

    // delete_file(&test_inode_entry_1);
    // dump_inode_bitmap();

    return 0;
}