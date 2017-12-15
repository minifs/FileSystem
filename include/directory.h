#ifndef DIRECTORY_H
#define DIRECTORY_H

#define MAX_FILENAME_LENGTH 32
#define MAX_LEVEL 10     //max level number of directory

struct dir_entry {
    int inode;          // Number of the inode that this directory entry points to
    int type;           // dir entry type: file or sub dir
    int length;         // Length of this directory entry.
    int name_length;    // Length of the file name
    char name[32];      // File name
}


/*
 * Create directory
 */
int create_dir(const char *path);

#endif
