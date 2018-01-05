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
inode* inode_entries[INODE_NUM];
/*typedef struct inode_entry {
    short inode_id;
    int filesize;
    short file_type;
    short name_len;
    char filename[32];
} inode_entry;*/

inode* Inode_Entry(int i)
{
    if ( i < 0 || i >= INODE_NUM )
        return NULL;

    return inode_entries[i];
}

/*
 * Just get the init pwd value to CLI
 */
char* dir_init()
{
    init_superblock();
    memset(inode_entries, '\0', sizeof(inode_entries));
    LOG_DEBUG("init_pwd\n");
    char *init_pwd;
    int result;
    short i;
    int flag=0;
    // preparing to cache inode_entry table in inode_entries
    // read-in every inode into cache
    // Waiting for read_inode function
    for (i = 0; i < INODE_NUM; i++) {
        inode* tmp = (inode*)malloc(sizeof(inode));

        memset(tmp, '\0', sizeof(tmp));
        tmp->inode_id = i;
        result = query_inode(tmp);
        if ( result < 0 )
            tmp->inode_id = -1; // empty inode
        inode_entries[i] = tmp;
        if (tmp->filename != NULL && strcmp(tmp->filename, "/")==0){
            flag=1;
        }
    }
    if (flag==0){
        snprintf(inode_entries[0]->filename, 32, "/");
        inode_entries[0]->name_len = 1;
        inode_entries[0]->file_type = 2;
        inode_entries[0]->filesize = 0;
        inode_entries[0]->inode_id = create_inode(inode_entries[0]);
    }
    init_pwd = Inode_Entry(0)->filename;
    LOG_DEBUG("init_pwd, inode_entries_0_filename: %s\n", inode_entries[0]->filename);
    return init_pwd;
}

int insert_inode_into_table(inode* node)
{
    int i;
    for (i = 0; i < INODE_NUM; i++) {
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
    for (i = 0; i < INODE_NUM; i++) {
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

inode* get_inode_from_path(const char *path)
{
    int i;
    for (i = 0; i < INODE_NUM; i++) {
        inode* node = Inode_Entry(i);

        if (node->inode_id == -1) {
            continue;
        }

        if (strcmp(node->filename, path) == 0) {
            if(node->file_type == 1){
                return node;    
            }
        }
    }

    return NULL;
}

/*
 *dir_rename: ls filename,
 *please declare a char ls_list[500] as input, after the function call, ls_list will have ls result.
 *success return 0, failed return -1
 */
int dir_ls(char* ls_list, const char *filename)
{
    int i, j;
    char *list[1000];
    int count = 0;
    strcpy(ls_list, "");
    int return_flag = -1;
    for (i = 0; i < INODE_NUM; i++) {
        inode* node = Inode_Entry(i);
        if (strncmp(node->filename, filename, strlen(filename)) == 0) {
            return_flag = 0;
            char buf[100], *value, *name;
            if (strcmp(filename, "/") == 0)
                value = strdup(node->filename + (strlen(filename)));
            else
                value = strdup(node->filename + (strlen(filename) + 1));
            name = strsep(&value, "/");

            if (strlen(name) > 0) {
                int flag = 0;
                for (j = 0; j < count; j++) {
                    if (strcmp(list[j], name) == 0) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1) continue;
                list[count] = strdup(name);
                count++;
                if(node->file_type == 1)
                    sprintf(buf, "%s\n", name);
                if(node->file_type == 2)
                    sprintf(buf, "%s/\n", name);
                strcat(ls_list, buf);
            }
        }
    }
    return return_flag;
}

/*
 * like find feature
 * 1. Get the pwd + folder name together from CLI
 * 2. get all the folder under current pwd from memory
 * 3. compare the same filename
 * 4. if there is the same folder name return "T" else return "F"
 */
int dir_search(const char *pwd, const char *foldername)
{
    int i;
    bool is_folder = false;
    char get_files[500];
    char *pch;
    inode* node[INODE_NUM];
    // get all the files from dir_ls
    dir_ls(get_files, pwd);
    // Compare the foldername and the get_files
    // if the name is the same, than check the file_type is it 2
    // if yes return true else return false
    pch = strstr (get_files, foldername);
    if (pch != NULL) {
        for (i = 0; i < INODE_NUM; i++) {
            node[i] = Inode_Entry(i);
            if (strcmp(pwd, node[i]->filename) == 0 && node[i]->file_type == 2) {
                is_folder = true;
                LOG_DEBUG("Finding foldername = %s\t, folder type is %d\n", foldername, node[i]->file_type);
            }
        }
    }
    if (is_folder == true) {
        return 0;
    } else {
        return -1;
    }
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
int dir_create(const char *pwd, const char *foldername)
{
    bool is_created = false;
    bool is_same = false;
    int i;
    char cfilename[32];
    if (strcmp(pwd, "/") == 0) {
        snprintf(cfilename, 32, "/%s", foldername);
    } else {
        snprintf(cfilename, 32, "%s/%s", pwd, foldername);
    }
    size_t cfilenamelen = strlen(cfilename);

    // Check the length of totallen (pwd + foldername)
    size_t pwdlen = strlen(pwd);
    size_t foldernamelen = strlen(foldername);
    int totallen = pwdlen + foldernamelen;
    if (totallen > 32) {
        is_created = false;
    }
    // Check is there the same folder name
    is_same = dir_search(pwd, foldername);
    if (is_same == 0) {
        // If there is the same folder name, return can't create the folder
        is_created = false;
    } else {
        // Save the total filename & length into NULL array in structure: inode_entries[x] (save this in memory)
        // After save the structure, then call the create_inode to get the inode_id
        // After get the inode_id the program save the inode_id value in to inode_entries[x].inode_id
        inode* node[INODE_NUM];
        for (i = 0; i < INODE_NUM; i++) {
            node[i] = Inode_Entry(i);
            size_t filename_len = strlen(node[i]->filename);
            if (filename_len < 1) {
                struct stat st;
                stat(cfilename, &st);
                snprintf(node[i]->filename, 32, "%s", cfilename);
                node[i]->name_len = cfilenamelen;
                node[i]->file_type = 2;
                node[i]->filesize = st.st_size;
                int is_create_inode;
                is_create_inode = create_inode(node[i]);
                if (is_create_inode == 0) {
                    LOG_DEBUG("Create a foldername = %s\t, folder type is %d\n", node[i]->filename, node[i]->file_type);
                    is_created = true;
                    if (is_created == true) {
                        return 0;
                    } else {
                        return -1;
                    }
                }
            }
        }
    }
    if (is_created == true) {
        return 0;
    } else {
        return -1;
    }
}


/*
 *dir_rename: mv foldername newname,
 *success return 0, failed return -1
 */
int dir_rename(const char *pwd, const char *foldername, const char *newname)
{
    int i;
    char str[1000];
    if (strcmp(pwd, "/") == 0)
        sprintf(str, "/%s", newname);
    else
        sprintf(str, "%s/%s", pwd, newname);
    //check length too long or not
    if (strlen(str) > 32) {
        return -1;
    }
    //check name the same or not
    if (dir_search(pwd, newname) == 0) {
        return -1;
    }
    char filename[32];
    if (strcmp(pwd, "/") == 0)
        sprintf(filename, "/%s", foldername);
    else
        sprintf(filename, "%s/%s", pwd, foldername);

    inode* node[INODE_NUM];
    for (i = 0; i < INODE_NUM; i++) {
        node[i] = Inode_Entry(i);
        if (strncmp(node[i]->filename, filename, strlen(filename)) == 0) {
            char *value;
            value = strdup(node[i]->filename + (strlen(filename) + 1));
            if ((strlen(str) + strlen(value)) > 32)
                return -1;
        }
    }
    //look for origin directory
    int result;
    for (i = 0; i < INODE_NUM; i++) {
        // printf("loop in\n");
        // printf("get inode\n");
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
                result = update_inode(node[i]);
                LOG_DEBUG("update_inode return value in dir_rename: %d\n", result);
            } else {
                strcpy(node[i]->filename, str);
                node[i]->name_len = strlen(str);
                /*call update_inode*/
                result = update_inode(node[i]);
                LOG_DEBUG("update_inode return value in dir_rename: %d\n", result);
            }


        }
    }
    return 0;
}

/*
 *dir_change: cd foldername
 *destination = pwd/foldername
 *success return 0, failed return -1
 */
int dir_change(const char* destination)
{
    int i;
    // inode_entry inode_entries[INODE_NUM];
    //look for destination directory
    for (i = 0; i < INODE_NUM; i++) {
        inode* node = Inode_Entry(i);
        if (strcmp(node->filename, destination) == 0) {
            //check directory type
            if (node->file_type == 2) {
                return 0;
            }
        }
    }
    return -1;
}

/*
 *dir_delete: rmdir foldername
 *can only delete empty directory
 *success return 0, failed return -1
 */
int dir_delete(const char *pwd, const char *foldername)
{
    int i;
    char str[1000];
    if (strcmp(pwd, "/") == 0)
        sprintf(str, "/%s", foldername);
    else
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
                dir_ls(list, str);   /*wait to change*/
                if (strlen(list) > 0) {
                    return -1;
                } else {
                    sprintf(node->filename, "");
                    node->name_len = 0;
                    node->file_type = 0;
                    node->inode_id = -1;
                    /*call delete_inode*/
                    int result;
                    result = delete_inode(node);
                    LOG_DEBUG("delete_inode return value in dir_delete: %d\n", result);
                    return 0;
                }
            }
        }
    }
    return -1;
}
