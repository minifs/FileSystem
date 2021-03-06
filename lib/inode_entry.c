#include"inode_entry.h"
#include"block.h"

// BYTE_SIZE
// INODE_SIZE
// INODE_NUM

superblock_2 superblock_inode;

/*
 * struct initialize
 */

int init_superblock()
{
    memset (&superblock_inode, 0,  sizeof(superblock_inode));
    // load superblock_2(block 1) from disk
    read_block(SUPERBLOCK_2_ID, &superblock_inode);
    dump_inode_bitmap();
    return 0;
}

int init_inode(inode *inode_entry, size_t length)
{
    memset (inode_entry, 0, length);
    return 0;
}


/*
 * inode bitmap and internal operation
 */

int set_inode_bitmap (int id)
{
    superblock_inode.inode_map[id>>3] |= 1ULL << (id%BYTE_SIZE);

    // Save Superblock_2
    modify_block(SUPERBLOCK_2_ID, &superblock_inode, (int)(sizeof (superblock_inode)));

    return 0;
}

int clear_inode_bitmap (int id)
{
    superblock_inode.inode_map[id>>3] &= ~(1ULL << (id%BYTE_SIZE));

    // Save Superblock_2
    modify_block(SUPERBLOCK_2_ID, &superblock_inode, (int)(sizeof (superblock_inode)));

    return 0;
}

int query_inode_bitmap (int id)
{
    int target_bit;

    // load block, and only select the wanted bit
    target_bit = superblock_inode.inode_map[id>>3] >> ( id % 8 );
    target_bit = target_bit % 2;
    return target_bit;
}

int dump_inode_bitmap ()
{
    // Show whole bitmap, show in HEX
    int i;
    for(i = 0; i < INODE_NUM / 8; i++) {

        if(i%32 == 0) {
            printf("\n");
        } else {
            if(i%2 == 0) {
                printf(" ");
            }
        }

        printf("%X", superblock_inode.inode_map[i]%16);
        printf("%X", superblock_inode.inode_map[i]/16);
    }
    printf("\n");

    return 0;
}

int dump_inode (inode *inode_entry)
{
    int i;

    LOG_DEBUG("------ Dump inode_entry ------\n");
    LOG_DEBUG("inode_id: %d\n", inode_entry->inode_id);
    LOG_DEBUG("filesize: %d\n", inode_entry->filesize);
    LOG_DEBUG("uid: %d\n", inode_entry->uid);
    LOG_DEBUG("gid: %d\n", inode_entry->gid);
    LOG_DEBUG("filemode: %d\n", inode_entry->filemode);
    LOG_DEBUG("timestamp: %d\n", inode_entry->timestamp);
    LOG_DEBUG("file_type: %d\n", inode_entry->file_type);
    LOG_DEBUG("name_len: %d\n", inode_entry->name_len);
    for (i = 0; i < 15; i++) {
        LOG_DEBUG("num[%d]: %d\n", i, inode_entry->num[i]);
    }
    LOG_DEBUG("filename: %s\n", inode_entry->filename);
    LOG_DEBUG("-----------------------------\n");

    return 0;
}

int dump_block (int block_id, void *address, size_t len)
{
    int i;
    printf("NO.%d block, Length %d :\n", block_id, len);
    for(i = 0; i < len; i++) {
        char *c_buf = (char*)address+i;
        printf("%c", *c_buf);
    }
    printf("\n");
}


/*
 * inode extrnal operation
 */

int query_inode (inode *inode_entry)
{

    // load block contain target inode
    // copy to the inod_entry struct
    // [DEBUG] Show all varible of inode_entry
    // return

    // get block id
    int inode_block_id = inode_entry->inode_id / (BLOCK_SIZE / INODE_SIZE) + 2;

    // get the seq in inode_group (0~7)
    int inode_block_seq = inode_entry->inode_id % (BLOCK_SIZE / INODE_SIZE);

    inode_group inode_group;
    //memset((void*)inode_group, 0, sizeof(inode_group));

    // check if inode exist in bitmap
    if(!query_inode_bitmap(inode_entry->inode_id)) {
        // LOG_ERROR("inode_id:%d doesn't exist in inode bitmap\n", inode_entry->inode_id);
        return -1;
    } else {
        LOG_DEBUG("inode_id:%d HIT !!!\n", inode_entry->inode_id);

    }

    read_block(inode_block_id, &inode_group);

    LOG_DEBUG ("Load inode id:%d from disk\n", inode_entry->inode_id);

    if(inode_entry->inode_id == inode_group.inode_list[inode_block_seq].inode_id) {
        LOG_DEBUG ("Copy inode to memory\n");
        memcpy(inode_entry, &inode_group.inode_list[inode_block_seq], sizeof(inode));
        //inode_entry = &inode_group.inode_list[inode_block_seq];
    } else {
        LOG_ERROR("inode_id in disk DOESN'T MATCH inode_id you requested !!!\n");
        return -1;
    }

    // dump_inode(inode_entry);

    return 0;
}

int update_inode (inode *inode_entry)
{

    int ret;

    // get block id
    int inode_block_id = inode_entry->inode_id / (BLOCK_SIZE / INODE_SIZE) + 2;

    // get the seq in inode_group (0~7)
    int inode_block_seq = inode_entry->inode_id % (BLOCK_SIZE / INODE_SIZE);

    inode_group inode_group;
    // load whole inode_block
    read_block (inode_block_id, (void *)&inode_group);
    // memory copy to right place
    inode_group.inode_list[inode_block_seq] = *inode_entry;

    LOG_DEBUG("Size of inode_group: %d.\n", (int)(sizeof(inode_group)));


    // write whole inode_block back
    ret = modify_block( inode_block_id, &inode_group, (int)(sizeof(inode_group)) );


    LOG_DEBUG("Return: %d\n", ret);

    //dump_inode(inode_entry);

    return 0;
}

int create_inode (inode *inode_entry)
{

    int i = 0;
    int target_id;
    int block_ret;

    inode_entry->uid = 0;
    inode_entry->gid = 0;
    inode_entry->filemode = 664;
    inode_entry->timestamp = time(NULL);
    for(i = 0; i < 15; i++) {
        inode_entry->num[i] = 0;
    }

    // assign inode_id and set bitmap up
    i = 0;
    while (1) {
        if (query_inode_bitmap(i) == 0) {
            LOG_DEBUG("Assign inode id: %d. Filename: %s\n", i, inode_entry->filename);
            target_id = i;
            set_inode_bitmap(target_id);
            inode_entry->inode_id = target_id;
            dump_inode_bitmap();
            break;
        } else {
            i++;
        }

        if (i >= INODE_NUM) {
            LOG_ERROR("NO inode available anymore !!\n");
            return -1;
            break;
        }
    }

    // save superblock to disk
    modify_block(SUPERBLOCK_2_ID, &superblock_inode, (int)(sizeof (superblock_inode)));

    // save inode to block, and write block
    update_inode(inode_entry);

    // return
    return 0;
}

int delete_inode (inode *inode_entry)
{
    // clear inode id bitmap
    clear_inode_bitmap(inode_entry->inode_id);
    LOG_DEBUG("inode %d bitmap clear. Remember to release the struct memory.\n", inode_entry->inode_id);
    LOG_DEBUG("query_inode_bitmap(%d) = %d\n", inode_entry->inode_id, query_inode_bitmap(inode_entry->inode_id));
    // return
    return 0;
}

/*
 * File operation
 * Note: Believe inode_entry will contain all block ids
 */

int delete_file (inode *inode_entry)
{
    int i;
    single_indirect_block single_indirect_block;
    int block_inuse;
    int read_block_return;

    // load indirect block_num
    if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ - 1] != 0) {
        LOG_DEBUG("indirect blocks exist. load them !\n");
        read_block_return = read_block(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ - 1], (void *)&single_indirect_block);
        LOG_DEBUG("read block result: %d\n", read_block_return);
    }

    //// find block_inuse (comment because it's actually no use)


    // release block

    // direct block. Clear until meet block_id == 0
    for (i = 0; i < SINGLE_INDIRECT_BLOCK_SEQ; i++) {
        if (inode_entry->num[i] != 0) {
            LOG_DEBUG("Delete direct block id: %d\n", inode_entry->num[i]);
            delete_block(inode_entry->num[i]);
        } else {
            LOG_DEBUG("All direct Block Clear. Total %d blocks\n", i+1);
            break;
        }
    }

    // indirect block exist. Clear until meet block_id == 0
    if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ - 1] != 0) {
        for (i = 0; i < BLOCK_SIZE / 8; i++) {
            if (single_indirect_block.num12[i] != 0) {
                LOG_DEBUG("Delete indirect block id: %d\n", single_indirect_block.num12[i]);
                delete_block(single_indirect_block.num12[i]);
            } else {
                LOG_DEBUG("All indirect Block Clear. Total %d blocks\n", i+1);
                break;
            }
        }
    }

    // clear the bit in inode_bitmap
    clear_inode_bitmap(inode_entry->inode_id);

    // return
    return 0;
}

int read_file (inode *inode_entry, void* file)
{
    int i;
    single_indirect_block single_indirect_block;
    int block_inuse;
    int read_block_return;
    int next_block_id;
    char file_buffer[BLOCK_SIZE];

    if(inode_entry->filesize == 0) {
        file_buffer[0] = '\0';
        memmove(file + inode_entry->filesize, &file_buffer[0], 1 );
        return 0;
    }

    // load indirect block_num
    if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ - 1] != 0) {
        LOG_DEBUG("indirect blocks exist. load them !\n");
        read_block_return = read_block(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ - 1], (void *)&single_indirect_block);
        //LOG_DEBUG("read block result: %d", read_block_return);
    }

    // find block_inuse (comment because it's actually no use)

    // load and connect block to file
    i = 0; // count direct block. and then count indirect block without back to zero
    next_block_id = inode_entry->num[i];

    while(next_block_id != 0) {
        // load block
        read_block(next_block_id, file_buffer);

        // load next block
        if(i+1 < SINGLE_INDIRECT_BLOCK_SEQ) {
            next_block_id = inode_entry->num[i+1];
        } else { // load indirect block
            next_block_id = single_indirect_block.num12[i-12 + 1];
        }

        // connect blocks
        if(next_block_id != 0) { //not the end of file block
            memmove(file + i * BLOCK_SIZE, file_buffer, BLOCK_SIZE);

            // clear buffer with zero
            memset(file_buffer, 0, sizeof(file_buffer));
        } else { // end of file block
            memmove(file + i * BLOCK_SIZE, file_buffer, inode_entry->filesize - i*BLOCK_SIZE );

            // clear buffer with zero
            memset(file_buffer, 0, sizeof(file_buffer));

            // upper layer will treat as string, so we need to append '\0' Orz
            file_buffer[0] = '\0';
            memmove(file + inode_entry->filesize, &file_buffer[0], 1 );

            // clear buffer with zero
            memset(file_buffer, 0, sizeof(file_buffer));
        }

        if((i+1) * BLOCK_SIZE > inode_entry->filesize / BLOCK_SIZE + 1) {
            LOG_WARN("loaded file is LARGER then filesize\n");
        }

        i++;



    }
    LOG_DEBUG("Load block num: %d, File Size: %d\n", i, inode_entry->filesize);




    // return
    return 0;
}

int write_file (inode *inode_entry, void* file)
{
    int i;
    single_indirect_block single_indirect_block;
    int block_inuse;
    int block_return;
    int read_block_return;
    char file_buffer[BLOCK_SIZE];
    int block_id_buf;
    int block_data_len;

    LOG_DEBUG("Let's Write file with filesize: %d\n", inode_entry->filesize);

    // load indirect block_num
    if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] != 0) {
        LOG_DEBUG("indirect blocks exist. load them !\n");
        read_block_return = read_block(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ], (void *)&single_indirect_block);
        //LOG_DEBUG("read block result: %d", read_block_return);
    }

    // find block_inuse
    if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] == 0) { //no indirect block
        for (i = 0; i < SINGLE_INDIRECT_BLOCK_SEQ; i++) {
            if (inode_entry->num[i] == 0) {
                block_inuse = i;
                LOG_DEBUG("inode_id: %d has %d blocks\n", inode_entry->inode_id, block_inuse);
                break;
            }
        }
    } else {
        for (i = 0; i < BLOCK_SIZE / 8; i++) {
            if (single_indirect_block.num12[i] == 0) {
                block_inuse = i + (SINGLE_INDIRECT_BLOCK_SEQ);
                LOG_DEBUG("inode_id: %d has %d blocks\n", inode_entry->inode_id, block_inuse);
                break;
            }
        }
    }

    // Whether block_inuse ? filesize / block_size + 1
    // i = now_block_id

    if(block_inuse < inode_entry->filesize / BLOCK_SIZE + 1) { // block num will increase
        // modify blocks until there is no block available
        for(i = 0; i < block_inuse; i++) {
            memcpy(file_buffer, file + i*BLOCK_SIZE, BLOCK_SIZE);
            if(i < SINGLE_INDIRECT_BLOCK_SEQ) { //direct block
                // dump_block(inode_entry->num[i], (void *)&file_buffer, BLOCK_SIZE);
                modify_block(inode_entry->num[i], &file_buffer, BLOCK_SIZE);
            } else { //indirect block
                // dump_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], (void *)&file_buffer, BLOCK_SIZE);
                modify_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], &file_buffer, BLOCK_SIZE);
            }
        }

        // write blocks and **record block_id** in inode block list (or indirect block list)
        // notice (in)direct block_id should write into inode

        for(; i < inode_entry->filesize / BLOCK_SIZE + 1; i++) {
            // determine the length of block and copy block into buffer
            if(i+1 == inode_entry->filesize / BLOCK_SIZE + 1) { // last block of file
                block_data_len = inode_entry->filesize % BLOCK_SIZE;
                //LOG_DEBUG("Block_num increase - Write Blocks. filesize is %d. block_data_len is %d", inode_entry->filesize, block_data_len);
            } else {
                block_data_len = BLOCK_SIZE;
            }

            memcpy(file_buffer,file + i*BLOCK_SIZE, block_data_len);

            write_block(&block_id_buf, &file_buffer, block_data_len);
            // dump_block(block_id_buf, (void *)&file_buffer, block_data_len);

            // direct or indirect block
            if(i < SINGLE_INDIRECT_BLOCK_SEQ) {
                inode_entry->num[i] = block_id_buf;
            } else {
                single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ] = block_id_buf;
            }
        }

        // save indirect block map
        if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] != 0) {
            modify_block(inode_entry->num[12], &single_indirect_block, (int)(sizeof(single_indirect_block)));
        } else {
            if(inode_entry->filesize / BLOCK_SIZE >= SINGLE_INDIRECT_BLOCK_SEQ) {
                write_block(&block_id_buf, &single_indirect_block, (int)(sizeof(single_indirect_block)));
                inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] = block_id_buf;
            }
        }


    } else if(block_inuse > inode_entry->filesize / BLOCK_SIZE + 1) {	// block num will decrease
        // delete blocks will not use and update block list in inode
        for(i = block_inuse - (inode_entry->filesize / BLOCK_SIZE + 1); i < block_inuse; i++ ) {
            // direct or indirect block
            if(i < SINGLE_INDIRECT_BLOCK_SEQ) {
                delete_block(inode_entry->num[i]);
                inode_entry->num[i] = 0;
            } else {
                delete_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ]);
                single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ] = 0;
            }
        }

        // save file by using modify block
        for(i = 0; i < inode_entry->filesize / BLOCK_SIZE + 1; i++) {
            // determine the length of block and copy block into buffer
            if(i == inode_entry->filesize / BLOCK_SIZE + 1) { // last block of file
                block_data_len = inode_entry->filesize % BLOCK_SIZE;
            } else {
                block_data_len = BLOCK_SIZE;
            }

            memcpy(file_buffer, file + i*BLOCK_SIZE, block_data_len);
            if(i < SINGLE_INDIRECT_BLOCK_SEQ) { //direct block
                // dump_block(inode_entry->num[i], (void *)&file_buffer, block_data_len);
                modify_block(inode_entry->num[i], &file_buffer, block_data_len);
            } else { //indirect block
                // dump_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], (void *)&file_buffer, block_data_len);
                modify_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], &file_buffer, block_data_len);
            }
        }

        // save indirect block map
        if(inode_entry->filesize / BLOCK_SIZE + 1 < SINGLE_INDIRECT_BLOCK_SEQ) { //file dont need indirect block(a.k.a. num[12]) anymore. Release it
            delete_block(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ]);
            inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] = 0;
        } else { // save indirect block into disk
            if(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] != 0) { // there exist a indirect block
                modify_block(inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ], &single_indirect_block, (int)(sizeof(single_indirect_block)));
            } else { // there doesnt exist a indirect block
                write_block(&block_id_buf, &single_indirect_block, (int)(sizeof(single_indirect_block)));
                inode_entry->num[SINGLE_INDIRECT_BLOCK_SEQ] = block_id_buf;
            }
        }


    } else if(block_inuse == inode_entry->filesize / BLOCK_SIZE + 1) {	// block num will remind
        // modify blocks
        for(i = 0; i < inode_entry->filesize / BLOCK_SIZE + 1; i++) {
            // determine the length of block and copy block into buffer
            if(i+1 == inode_entry->filesize / BLOCK_SIZE + 1) { // last block of file
                block_data_len = inode_entry->filesize % BLOCK_SIZE;
            } else {
                block_data_len = BLOCK_SIZE;
            }

            memcpy(file_buffer, file + i*BLOCK_SIZE, block_data_len);

            if(i < SINGLE_INDIRECT_BLOCK_SEQ) { //direct block
                // dump_block(inode_entry->num[i], (void *)&file_buffer, block_data_len);
                modify_block(inode_entry->num[i], &file_buffer, block_data_len);
            } else { //indirect block
                // dump_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], (void *)&file_buffer, block_data_len);
                modify_block(single_indirect_block.num12[i - SINGLE_INDIRECT_BLOCK_SEQ], &file_buffer, block_data_len);
            }
        }
    }


    // update inode
    update_inode(inode_entry);

    // return
    return 0;
}



