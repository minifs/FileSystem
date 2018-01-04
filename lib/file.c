#include "block.h"
#include "inode_entry.h"
#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Read the entire file
 */
int read_file_by_path(const char *path, void **buf)
{
    inode *file_inode = get_inode_from_path( path );
    if( file_inode==NULL ) {
        printf("No such file.\n");
        return -1;
    }

    int file_size = file_inode->filesize;
    void *read_buf = (void*)malloc( file_size );
    int ret = read_file ( file_inode, read_buf );

    if(ret<0) {
        free(read_buf);
        return -2;
    }

    *buf = read_buf;

    return file_size;
}

/*
 * Read the file up to 'size' bytes
 */
int _read_file_by_path(const char *path, void *buf, int size)
{

    if(size <= 0)
        return 0;

    void* tmp = NULL;
    int ret = read_file_by_path(path, &tmp);
    if(ret<0) {
        return ret;
    }

    int file_size = ret;
    int read_size = 0;
    if(size > file_size)
        read_size = file_size;
    else
        read_size = size;

    strncpy( (char*)buf, (const char*)tmp, read_size );

    free(tmp);

    return read_size;
}

/*
 * Write file
 */
int write_file_by_path(const char *path, void *buf, int size)
{
    if( size <= 0 )
        return 0;

    inode *file_inode = get_inode_from_path( path );
    if(file_inode == NULL) {
        printf("No such file.\n");
        return -1;
    }


    void *write_buf = (void*)malloc( size );
    strncpy( (char*)write_buf, (const char*)buf, size );
    //printf("write:%s\n", (char*)write_buf);

    int ret = write_file ( file_inode, write_buf );
    if(ret<0) {
        return ret;
    }

    time_t cur_time;
    time (&cur_time);
    file_inode->filesize = size;
    file_inode->timestamp = cur_time;
    update_inode ( file_inode );

    free( write_buf );
    return 0;
}

/*
 * Delete file
 */
int delete_file_by_path(const char *path)
{
    int i=0;

    if( path[strlen(path)-1] == '/' ) {
        printf("It is a directory.\n");
        return -1;
    }

    inode *file_inode = get_inode_from_path( path );
    if(file_inode==NULL) {
        printf("No such file.\n");
        return -2;
    }

    int ret = delete_inode_from_table(file_inode);
    if(ret<0) {
        printf("fail:delete_inode_from_table(...).\n");
        return -3;
    }
    ret = delete_file ( file_inode );
    if(ret<0) {
        printf("fail:delete_file(...).\n");
        return -4;
    }

    return 0;
}

