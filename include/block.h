#ifndef _BLOCK_H_
#define _BLOCK_H_

/*
 * Header file include
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "log.h"

/*
 * Parameter definition
 */
#define SIZEOF_BYTE 8
#define BLOCK_SIZE 1024 //1KB for one block
#define NUMBER_OF_BLOCKS 4096 //number of blocks in file system
#define TOTAL_NUMBER_OF_BYTES BLOCK_SIZE * NUMBER_OF_BLOCKS
#define SUPER_BLOCK_SIZE 1024 - 10 - NUMBER_OF_BLOCKS/SIZEOF_BYTE
#define Assign_block_from 514

#define FILE_SYSTEM_HEADER "FileSystem\0"

/*
 * enum definition
 */
typedef enum {
    TRUE = 1,
    FALSE = 0
} BOOL;

/*
 * Global variable
 */
extern int file_state;
extern uint8_t block_map[NUMBER_OF_BLOCKS/8];
/*
 * Function definition
 */

/*
 * Create block
 */
int create_filesystem(const char *path);

/*
 * Load block
 */
int load_filesystem(const char *path);

/*
 * Super block modify
 */
int modify_super_block(void *block, int block_input_length);

/*
 * Read super block
 */
int read_super_block(void *block, int block_output_length);

/*
 *  Modify block
 */
int modify_block(const int block_ID, void *block, int block_input_length);

/*
 * Write block
 */
int write_block(int *block_ID, void *block, int block_input_length);

/*
 * Read block
 */
int read_block(const int block_ID, void *block);

/*
 * Delete block
 */
int delete_block(const int block_ID);


/*
 * Tool functions
 */
BOOL map_check(int ID);


#endif
