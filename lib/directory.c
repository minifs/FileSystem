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
const struct inode_entry* inode_entries;
/*typedef struct inode_entry {
    short inode_id;
    int filesize;
    short file_type;
    short name_len;
    char filename[32];
} inode_entry;*/

//inode_entry Inode_Entry(int i)
inode Inode_Entry(int i)
{
    // Waiting for read_inode
    inode_entries = (struct inode_entry*)malloc(INODE_NUM*sizeof(struct inode_entry));
    // For test
    /*struct inode_entry inode_entries[10] = {
        {1, 850, 2, 4, "root"},
        {2, 10, 1, 13, "root/test1.md"},
        {3, 20, 1, 13, "root/test2.md"},
        {4, 68, 2, 12, "root/folder1"},
        {5, 68, 1, 21, "root/folder1/test3.md"}
    };*/
    return inode_entries[i];
}

/*
 * read-in every inode_entry into memery
 */
char* dir_init()
{
    char *init_pwd;
    int i;
    // preparing to cache inode_entry table in inode_entries
    //struct inode_entry* inode_entries = (struct inode_entry*)malloc(INODE_NUM*sizeof(struct inode_entry));
    inode_entries = (struct inode_entry*)malloc(INODE_NUM*sizeof(struct inode_entry));
    assert(inode_entries != NULL);
    // read-in every inode into cache
    // Waiting for read_inode function
    /*struct inode_entry inode_entries[10] = {
        {1, 850, 2, 4, "root"},
        {2, 10, 1, 13, "root/test1.md"},
        {3, 20, 1, 13, "root/test2.md"},
        {4, 68, 2, 12, "root/folder1"},
        {5, 68, 1, 21, "root/folder1/test3.md"}
    };*/

    /*for(i=0; i<4096; i++) {
        inode_entries[i];// = query_inode(inode_entry *inode_entry);
    }*/
    init_pwd = inode_entries[0].filename;
    printf("%s\n", inode_entries[0].filename);
    return (char *)init_pwd;
}

void split_token(char **arr, char *str, const char *token, int num_entry)
{
    char *s = strtok(str, token);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, token);
        num_entry++;
    }
}

int split_num(char **arr, char *str, const char *token, int num_entry)
{
    char *s = strtok(str, token);
    while(s != NULL) {
        num_entry++;
        *arr++ = s;
        s = strtok(NULL, token);
    }
    return num_entry;
}

/*
 * Get filename from CLI
 * (We don't have this iterm) Check this file type is DIR, if not, return "This is not Dirctory" string
 * Get the files under the filename, then return the string.
 */
char* dir_ls(const char *filename)
{
    char* file_name;
    char *ls_list;
    char *no_dir;
    char rootstring[] = "root";
    char *c = rootstring;
    int i;
    ls_list=(char *)malloc(sizeof(char));
    inode_entries = (struct inode_entry*)malloc(INODE_NUM*sizeof(struct inode_entry));
    struct inode_entry inode_entries[10];
    for(i=0; i<INODE_NUM; i++) {
        inode_entries[i] = Inode_Entry(i);
    }
    printf("%s\n", filename);
    char *arr_filename[4];
    char *token_filename=(char *)filename;
    int num_filename=0;
    num_filename = split_num(arr_filename, (char *)filename, "/", num_filename);
    split_token(arr_filename,token_filename, "/", num_filename);
    for(i=0; i<INODE_NUM; i++) {
        // Using strtok cut "/" , then save the token to array (filename+inode)
        // Using strcmp to compare every array value
        // Saving the value into ls_list
        char *arr_entry[4];
        int num_entry=0;
        char *inode_entries_filename = inode_entries[i].filename;
        num_entry = split_num(arr_entry, inode_entries[i].filename, "/", num_entry);
        split_token(arr_entry, inode_entries_filename, "/", num_entry);
        if (num_entry>0) {
            char *arr_name = arr_entry[num_entry-1];
            if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-2])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-1]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-3])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-2]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-4])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-3]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-5])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-4]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-6])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-5]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-7])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-6]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-8])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-7]);
                    strcat(ls_list, "\n");
                }
            } else if(strcmp(arr_filename[num_filename-1], arr_entry[num_entry-9])==0) {
                if (num_filename+1==num_entry) {
                    strcat(ls_list, arr_entry[num_entry-8]);
                    strcat(ls_list, "\n");
                }
            } else {
                continue;
            }
        }
    }
    return (char *)ls_list;
}

