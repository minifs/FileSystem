#include"block.h"
#include"inode_entry.h"
#include"file.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

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

int search_empty(void)
{
    //return an empty inode index to put new inode
    int i;
    for(i=0; i<INODE_SIZE; i++) {
        inode_entry* node = Inode_Entry(i);

        if(node->inode_id == -1) {
            return i;
        }
    }
    return -1;//no empty space
}

/***search file***/
/***return 0:exist , -1:not exist***/
/***return 0:exist , -1:not exist , -2:directory***/
int search_file(const char *fpath)
{
    inode *my_inode = get_inode_from_path(fpath);
    if(my_inode == NULL) {
        return -1;
    } else if(my_inode->file_type != 1) {
        return -2;
    } else return 0;
}

/***create_file tested***/
/***(only update inode in memory)***/
/***return 0:success , -1:illegel length , -2:file exist***/
int create_file(const char *pwd, const char *fname)
{

    inode* inode_ptr = (inode*)malloc(sizeof(inode));

    int pwdlen = strlen(pwd);
    int fnamelen = strlen(fname);
    int totallen = pwdlen+fnamelen;
    if(totallen>32) return -1; //illegal length

    char cfilename[32];
    snprintf(cfilename, 32, "%s/%s", pwd, fname);

    if(search_file(cfilename)==0) {
        return -2;//file exist
    }

    inode_ptr->filesize = 0;
    inode_ptr->file_type = 1; //1=File, 2=Directory
    inode_ptr->name_len = fnamelen;
    snprintf(inode_ptr->filename, 32, "%s", cfilename);
    inode_ptr->inode_id = search_empty(); //find next empty inode
    //inode_ptr->inode_id = create_inode(inode_entries[1]);

    insert_inode_into_table(inode_ptr);
    create_inode(inode_ptr);
    printf("inode id : %d\n",inode_ptr->inode_id);
    printf("filesize : %d\n",inode_ptr->filesize);

    return 0;

}

/***rename_file tested***/
/***(only update inode in memory)***/
/***return 0:success , -1:fname2 illegel length , -2:fpath2 exist***/
int rename_file(const char *pwd, const char *fname1, const char *fname2)
{

    inode* inode_ptr = (inode*)malloc(sizeof(inode));

    int pwdlen = strlen(pwd);
    int fnamelen = strlen(fname2);
    int totallen = pwdlen+fnamelen;
    if(totallen>32) return -1; //illegal length

    char cfilename1[32];
    snprintf(cfilename1, 32, "%s/%s", pwd, fname1);
    char cfilename2[32];
    snprintf(cfilename2, 32, "%s/%s", pwd, fname2);

    if(search_file(cfilename2)==0) {
        return -2;//file exist
    }

    inode_ptr = get_inode_from_path(cfilename1);
    snprintf(inode_ptr->filename, 32, "%s", cfilename2);
    update_inode(inode_ptr);

    return 0;
}
