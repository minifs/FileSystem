#ifndef _INODE_H_
#define _INODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "log.h"

#define BYTE_SIZE 8
#define INODE_SIZE 128
#define INODE_NUM 4096


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
    extern uint8_t inode_map[INODE_NUM/8];
} superblock_2;

typedef struct single_indirect_block {
    short num12[512];
} single_indirect_block;

/*
 * Function prototypes
 */
int get_inode_bypath (const char *path, inode *inode);

int get_inode_byid (const int inode_id, inode *inode);

int update_inode_bypath (const char *path, const inode *inode);

int update_inode_byid (const int *inode_id, const inode *inode);

int create_inode (const char *path, const inode *inode);

int del_inode (const char *path);

int del_inode (const int inode_id);

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

