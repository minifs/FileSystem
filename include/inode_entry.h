#ifndef _INODE_H_
#define _INODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "log.h"
#include "block.h"

#define BYTE_SIZE 8
#define INODE_SIZE 128
#define INODE_NUM 4096
#define SUPERBLOCK_2_ID 1
#define SINGLE_INDIRECT_BLOCK_SEQ 12



/*
 * The directory entry & inode structure.
 */
typedef struct inode_entry {
    short inode_id;
    int filesize;
    short uid;
    short gid;
    short filemode;
    time_t timestamp;
    short file_type;
    short name_len;
    int num[15];
    char filename[32];
} inode;


typedef struct superblock_2 {
    short block_used;
    short block_unuse;
    short inode_used;
    short inode_unuse;
    uint8_t inode_map[INODE_NUM/8];
} superblock_2;

typedef struct single_indirect_block {
    short num12[BLOCK_SIZE / 8];
} single_indirect_block;

typedef struct inode_group {
    inode inode_list[BLOCK_SIZE / INODE_SIZE];
} inode_group; // inodes in a block

/*
 * Inode Function prototypes(Internal)
 */

int init_superblock();

int init_inode(inode *inode_entry, size_t length);

int set_inode_bitmap (int id);

int clear_inode_bitmap (int id);

int query_inode_bitmap (int id);

int dump_inode_bitmap ();

/*
 * Inode Function prototypes(External)
 */
int query_inode (inode *inode_entry);

int update_inode (inode *inode_entry);

int create_inode (inode *inode_entry);

int delete_inode (inode *inode_entry);

/*
 * File Function prototypes
 */
int read_file (inode *inode_entry, void* file);

// Delete files including inode
int delete_file (inode *inode_entry);

int write_file (inode *inode_entry, void* file);

/*
 * Superblock Function prototypes
 */
int set_inode_bitmap (int id);

int clear_inode_bitmap (int id);

int query_inode_bitmap (int id);


/*
 * Directory function
 */
char* dir_init();

char* dir_ls(const char *filename);

bool dir_search(const char *filename);

bool dir_create(const char *filename);

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
enum {
    EXT2_FT_UNKNOWN     = 0,  /* Unknown File Type */
    EXT2_FT_REG_FILE    = 1,  /* Regular File */
    EXT2_FT_DIR         = 2,  /* Directory File */
    EXT2_FT_CHRDEV      = 3,  /* Character Device */
    EXT2_FT_BLKDEV      = 4,  /* Block Device */
    EXT2_FT_FIFO        = 5,  /* Buffer File */
    EXT2_FT_SOCK        = 6,  /* Socket File */
    EXT2_FT_SYMLINK     = 7,  /* Symbolic Link */
    EXT2_FT_MAX
};

#endif

