#include "inode_entry.h"
#include "block.h"

int main ()
{
    init_superblock();
    dump_inode_bitmap();
    

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
    create_inode(&test_inode_entry_1);

    inode_entry test_inode_entry_2;
    create_inode(&test_inode_entry_2);

    printf("------------------------\n\n");

    // ------ delete inode test ------
    printf("------ delete inode test ------\n");

    

    printf("------------------------\n\n");

}