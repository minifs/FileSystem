#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "inode_entry.h"
#include "log.h"

/*
 * Read the entire file
 */
int read_file_by_path(const char *path, void **buf);

/*
 * Read the file up to 'size' bytes
 */
int _read_file_by_path(const char *path, void *buf, int size);

/*
 * Write file
 */
int write_file_by_path(const char *path, void *buf, int size);

/*
 * Delete file
 */
int delete_file_by_path(const char *path);


/*
 * Create file
 */
 int create_file(const char *dpath, int fname_len);
 
 /*
 * Rename file
 */
 int rename_file(const char *fpath1, const char *fname1, const char *fname2, int fname2_len)
 
 /*
 * Search file
 */
 int search_file(const char *dpath, const char *fname);