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
inode* inode_entries[INODE_SIZE];
/*typedef struct inode_entry {
    short inode_id;
    int filesize;
    short file_type;
    short name_len;
    char filename[32];
} inode_entry;*/

inode* Inode_Entry(int i)
{
    if ( i < 0 || i >= INODE_SIZE )
        return NULL;

    return inode_entries[i];
}

/*
 * Just get the init pwd value to CLI
 */
char* dir_init()
{
    char *init_pwd;
    int result;
    short i;
    // preparing to cache inode_entry table in inode_entries
    // read-in every inode into cache
    // Waiting for read_inode function
    for (i = 0; i < INODE_SIZE; i++) {
        inode* tmp = (inode*)malloc(sizeof(inode));
        tmp->inode_id = i;

        result = query_inode(tmp);
        if ( result < 0 )
            tmp->inode_id = -1; // empty inode
        inode_entries[i] = tmp;
    }
    init_pwd = Inode_Entry(0)->filename;
    return init_pwd;
}

int insert_inode_into_table(inode* node)
{
    int i;
    for (i = 0; i < INODE_SIZE; i++) {
        if ( Inode_Entry(i)->inode_id == -1 ) {
            free(Inode_Entry(i));
            inode_entries[i] = node;

            return 0;
        }
    }

    return -1;
}

int delete_inode_from_table(inode* node)
{
    int i;
    for (i = 0; i < INODE_SIZE; i++) {
        inode *n = Inode_Entry(i);
        if ( n->inode_id == node->inode_id ) {
            n->inode_id = -1;
            n->filesize = 0;
            n->uid = 0;
            n->gid = 0;
            n->filemode = 0;
            n->timestamp = 0;
            n->file_type = 0;
            n->name_len = 0;
            sprintf(n->filename, " ");

            return 0;
        }
    }

    return -1;
}

inode* get_inode_from_path(char *path)
{
    int i;
    for (i = 0; i < INODE_SIZE; i++) {
        inode* node = Inode_Entry(i);

        if (node->inode_id == -1) {
            continue;
        }

        if (strcmp(node->filename, path) == 0) {
            return node;
        }
    }

    return NULL;
}

void split_token(char **arr, char *str, const char *token, int num_entry)
{
    char *s = strtok(str, token);
    while (s != NULL) {
        *arr++ = s;
        s = strtok(NULL, token);
        num_entry++;
    }
}

int split_num(char **arr, char *str, const char *token, int num_entry)
{
    char *s = strtok(str, token);
    while (s != NULL) {
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
    ls_list = (char *)malloc(sizeof(char));
    inode_entries = (struct inode_entry*)malloc(INODE_NUM * sizeof(struct inode_entry));
    struct inode_entry inode_entries[INODE_NUM];
    for (i = 0; i < INODE_NUM; i++) {
        inode_entries[i] = Inode_Entry(i);
    }
    printf("%s\n", filename);
    char *arr_filename[32];
    char *token_filename = (char *)filename;
    int num_filename = 0;
    num_filename = split_num(arr_filename, (char *)filename, "/", num_filename);
    split_token(arr_filename, token_filename, "/", num_filename);
    for (i = 0; i < INODE_NUM; i++) {
        // Using strtok cut "/" , then save the token to array (filename+inode)
        // Using strcmp to compare every array value
        // Saving the value into ls_list
        char *arr_entry[MAX_LAYER];
        int num_entry = 0;
        char *inode_entries_filename = inode_entries[i].filename;
        num_entry = split_num(arr_entry, inode_entries[i].filename, "/", num_entry);
        split_token(arr_entry, inode_entries_filename, "/", num_entry);
        if (num_entry > 0) {
            char *arr_name = arr_entry[num_entry - 1];
            if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 2]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 1]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 3]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 2]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 4]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 3]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 5]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 4]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 6]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 5]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 7]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 6]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 8]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 7]);
                    strcat(ls_list, "\n");
                }
            } else if (strcmp(arr_filename[num_filename - 1], arr_entry[num_entry - 9]) == 0) {
                if (num_filename + 1 == num_entry) {
                    strcat(ls_list, arr_entry[num_entry - 8]);
                    strcat(ls_list, "\n");
                }
            } else {
                continue;
            }
        }
    }
    return (char *)ls_list;
}


/*
 * like find feature
 * 1. Get the pwd + folder name together from CLI
 * 2. get all the folder under current pwd from memory
 * 3. compare the same filename
 * 4. if there is the same folder name return "T" else return "F"
 */
bool dir_search(const char *pwd, const char *foldername)
{
    int i;
    bool is_folder = false;
    char *get_files;
    char *pch;
    inode_entries = (struct inode_entry*)malloc(INODE_NUM * sizeof(struct inode_entry));
    struct inode_entry inode_entries[INODE_NUM];
    for (i = 0; i < INODE_NUM; i++) {
        inode_entries[i] = Inode_Entry(i);
    }
    // get all the files from dir_ls
    get_files = dir_ls(pwd);
    // Compare the foldername and the get_files
    // if the name is the same, than check the file_type is it 2
    // if yes return true else return false
    pch = strstr (get_files, foldername);
    if (pch != NULL) {
        printf("%s", pch);
        for (i = 0; i < INODE_NUM; i++) {
            if (strcmp(pwd, inode_entries[i].filename) == 0 && inode_entries[i].file_type == 2) {
                is_folder = true;
                LOG_DEBUG("Finding foldername = %s\t, folder type is %d\t ", foldername, inode_entries[i].file_type);
            }
        }
    }
    return is_folder;
}


/*
 * Like mkdir to create the folder
 * 1. Receive the filename(pwd+folder_name) from CLI
 * 2. If the length > 32 return false
 * 3. Save the filename & len into memory in array inode_entries[x]
 * 4. Send inode_entries[x] to create_inode & Get the inode_id
 *    (create_inode will save the inode_entries[x] into block)
 * 5. Save the inode_id into inode_entries[x].inode_id
 * 6. Return ture
 */
bool dir_create(const char *pwd, const char *foldername)
{
    bool is_created = false;
    bool is_same = false;
    int i;
    char cfilename[32];
    snprintf(cfilename, 32, "%s/%s", pwd, foldername);
    size_t cfilenamelen = strlen(cfilename);

    // Check the length of totallen (pwd + foldername)
    size_t pwdlen = strlen(pwd);
    size_t foldernamelen = strlen(foldername);
    int totallen = pwdlen + foldernamelen;
    if (totallen > 32) {
        return is_created;
    }
    // Check is there the same folder name
    is_same = dir_search(pwd, foldername);
    if (is_same == 1) {
        // If there is the same folder name, return can't create the folder
        return is_created;
    } else {
        // Save the total filename & length into NULL array in structure: inode_entries[x] (save this in memory)
        // After save the structure, then call the create_inode to get the inode_id
        // After get the inode_id the program save the inode_id value in to inode_entries[x].inode_id
        inode_entries = (struct inode_entry*)malloc(INODE_NUM * sizeof(struct inode_entry));
        struct inode_entry inode_entries[INODE_NUM];
        for (i = 0; i < INODE_NUM; i++) {
            size_t filename_len = strlen(inode_entries[i].filename);
            inode_entries[i] = Inode_Entry(i);
            if (filename_len < 1) {
                struct stat st;
                stat(cfilename, &st);
                snprintf(inode_entries[i].filename, 32, "%s", cfilename);
                inode_entries[i].name_len = cfilenamelen;
                inode_entries[i].file_type = 2;
                inode_entries[i].filesize = st.st_size;
                inode_entries[i].inode_id = create_inode(inode_entries[i]);
                if (inode_entries[i].inode_id != 0) {
                    LOG_DEBUG("Create a foldername = %s\t, folder type is %d\t ", inode_entries[i].filename, inode_entries[i].file_type);
                    is_created = true;
                }
                return is_created;
            }
        }
    }
}


/*
 *dir_rename: mv foldername newname,
 *success return 1, failed return 0
 */
bool dir_rename(const char *pwd, const char *foldername, const char *newname)
{
    // printf("dir_rename in\n");
    int i;
    char str[1000];
    sprintf(str, "%s/%s", pwd, newname);
    //check filename length too long or not
    if (strlen(str) > 32) {
        return false;
    }
    //check name the same or not
    if (dir_search(pwd, newname)) {
        return false;
    }
    char filename[32];
    sprintf(filename, "%s/%s", pwd, foldername);
    //look for origin directory & file inside
    inode* node[INODE_NUM];
    for (i = 0; i < INODE_NUM; i++) {
        node[i] = Inode_Entry(i);
        if (strncmp(node[i]->filename, filename, strlen(filename)) == 0) {
            char *value;
            value = strdup(node[i]->filename + (strlen(filename) + 1));
            //check filename length too long or not
            if ((strlen(str) + strlen(value)) > 32)
                return false;
        }
    }

    for (i = 0; i < INODE_NUM; i++) {
        if (strncmp(node[i]->filename, filename, strlen(filename)) == 0) {
            char *val;
            val = strdup(node[i]->filename + (strlen(filename) + 1));
            //change filename and length
            //write back to inode layer
            memset(node[i]->filename, '\0', sizeof(node[i]->filename));
            if (strlen(val) > 0) {
                sprintf(node[i]->filename, "%s/%s", str, val);
                node[i]->name_len = strlen(str) + strlen(val);
                /*call update_inode*/
                int result;
                result = update_inode(node[i]);
                LOG_DEBUG("update_inode return value in dir_rename: %d", result);
            } else {
                strcpy(node[i]->filename, str);
                node[i]->name_len = strlen(str);
                /*call update_inode*/
                int result;
                result = update_inode(node[i]);
                LOG_DEBUG("update_inode return value in dir_rename: %d", result);
            }


        }
    }
    return 1;
}

/*
 *dir_change: cd foldername
 *destination = pwd/foldername
 *success return 1, failed return 0
 */
bool dir_change(const char* destination)
{
    int i;
    // inode_entry inode_entries[INODE_NUM];
    //look for destination directory
    for (i = 0; i < INODE_NUM; i++) {
        inode* node = Inode_Entry(i);
        if (strcmp(node->filename, destination) == 0) {
            //check directory type
            if (node->file_type == 2) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 *dir_delete: rmdir foldername
 *can only delete empty directory
 *success return 1, failed return 0
 */
bool dir_delete(const char *pwd, const char *foldername)
{
    int i;
    char str[1000];
    sprintf(str, "%s/%s", pwd, foldername);
    // inode_entry inode_entries[INODE_NUM];
    //look for directory user want to delete
    for (i = 0; i < INODE_NUM; i++) {
        inode* node = Inode_Entry(i);
        if (strcmp(node->filename, str) == 0) {
            //check directory type
            if (node->file_type == 2) {
                char list[500];
                //check directory is empty or not
                list = dir_ls(str);   /*wait to change*/
                if (strlen(list) > 0) {
                    return 0;
                } else {
                    sprintf(node->filename, "");
                    node->name_len = 0;
                    /*call delete_inode*/
                    int result;
                    result = delete_inode(node);
                    LOG_DEBUG("delete_inode return value in dir_delete: %d", result);
                    return 1;
                }
            }
        }
    }
    return 0;
}
