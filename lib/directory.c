#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include "inode_entry.h"

/*
 * read-in every inode_entry into memery
 */
char* dir_init()
{
    char *init_pwd;
    struct inode_entry inode_entry;
    int i;
    // preparing to cache inode_entry table in inode_entries
    inode_entries = (struct inode_entry*)malloc(INODE_NUM*sizeof(struct inode_entry));
    assert(inode_entries != NULL);
    // read-in every inode into cache
    for(i=0; i<4096; i++) {
        struct inode_entry inode_entries[i] = query_inode(inode_entry *inode_entry);
    }
    init_pwd = inode_entries[0].filename;
    return (char *)init_pwd;
}

