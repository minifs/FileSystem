/*
 * MODIFIED by @sufuf3 for OS class at NCTU
 * to remove some of the unnecessary components.
 */

/*
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#define MAX_FILENAME_LENGTH 32
#define MAX_LEVEL 10     //max level number of directory
#include "types.h"

/*
 * The new version of the directory entry.  Since EXT2 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 */
struct dir_entry {
    __le32  inode;          /* Inode number: Number of the inode that this directory entry points to */
    __le16  rec_len;        /* Directory entry length */
    __u8    name_len;       /* Name length: Length of the file name */
    __u8    file_type;      /* Ext2 directory file types. */
    char    name[32];       /* File name, up to EXT2_NAME_LEN */
};

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
