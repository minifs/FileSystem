#include "block.h"

int file_state = -1;//only open or load file system once*/
static int num_blocks = 0;
int block_map[NUMBER_OF_BLOCKS] = {0};
char file_system_path[100] = {0};

int create_block(const char *path)
{
	struct stat st;

	if(file_state < 0){
		file_state = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if(file_state < 0){
			LOG_WARN("Fail to create file system, the details : %s\n", strerror(errno));
			return -1;
		}

		if(fstat(file_state, &st) < 0){
			LOG_WARN("File state returns something bad, the details: %s\n", strerror(errno));
			return -2;
		}
		write(file_state, FILE_SYSTEM_HEADER, strlen(FILE_SYSTEM_HEADER));
		snprintf(file_system_path, 100, "%s", path); 
		close(file_state);	
	}
	num_blocks = NUMBER_OF_BLOCKS;
	DEBUG("file_state = %d\n", file_state);
	return num_blocks;
}

int load_block(const char *path)
{
	struct stat st;
	DEBUG("file_state = %d\n", file_state);
	if(file_state > 0){
		if((file_state = open(path, O_RDWR)) < 0){
			LOG_WARN("File system %s does not exist, please call create_block first\n", path);
			return -1;
		}

		if(fstat(file_state, &st) < 0){
			LOG_WARN("File stat returns something bad, the details: %s\n", strerror(errno));
			return -2;
		}
		char checker[11] = {0};
		if(read(file_state, checker, strlen(FILE_SYSTEM_HEADER)) != strlen(FILE_SYSTEM_HEADER)){
			LOG_WARN("Bad read from file system, error code : %s\n", strerror(errno));
			return -3;
		}else{
			DEBUG("checker = %s\n", checker);
			DEBUG("strlen(FILE_SYSTEM_HEADER) = %d\n", strlen(FILE_SYSTEM_HEADER));
			if(strcmp(checker, FILE_SYSTEM_HEADER) != 0){
				LOG_WARN("Bad file system header\n");
				return -4;
			}
		}

		snprintf(file_system_path, 100, "%s", path);
		close(file_state);
		return 0;	
	}else{
		return -1;
	}
}

/*
 * assign a block to put data
 *
 * return >= 1 : assgin works properly
 * return = 0 : fail to assign.
 */
int assign_block(void){
	int i;
	for(i = 1; i < NUMBER_OF_BLOCKS; i++){
		if(block_map[i] == 0){
			return i;
		}
	}
	return 0;
}

int modify_block(const int block_ID, void *block, int block_input_length){
	DEBUG("fil_system_path = %s\n", file_system_path);
	int byte_written;

	if(file_state > 0){
		if((file_state = open(file_system_path, O_RDWR)) < 0){
			LOG_WARN("File system %s does not exist, please call create_block first\n", file_system_path);
			return -1;
		}
		
		if(block_ID >= BLOCK_SIZE){
			LOG_WARN("Invalid block_ID\n");
			return -2;
		}
		
		if(lseek(file_state, block_ID * BLOCK_SIZE, SEEK_SET) < 0){
			LOG_WARN("lseek fail, detail:  %s\n", strerror(errno));
		       	return -3;
		}

		if(block_input_length < BLOCK_SIZE){
			DEBUG("block_input_length < BLOCK_SIZE\n");
			if(write(file_state, block, block_input_length) != block_input_length){
				LOG_WARN("Fail to write, detail: %s\n", strerror(errno));
				return -4;
			}
			byte_written = block_input_length;
		}else{
			DEBUG("block_input_length >= BLOCK_SIZE\n");
			if(write(file_state, block, BLOCK_SIZE) != BLOCK_SIZE){
				LOG_WARN("Fail to write, detail: %s\n", strerror(errno));
				return -4;
			}
			byte_written = BLOCK_SIZE;
		}
		close(file_state);
	}else{
		return -1;
	}

	return byte_written;

}

int write_block(int *block_ID, void *block, int block_input_length)
{
	int byte_written = 0;

	DEBUG("fil_system_path = %s\n", file_system_path);
	if(file_state > 0){
		if((file_state = open(file_system_path, O_RDWR)) < 0){
			LOG_WARN("File system %s does not exist, please call create_block first\n", file_system_path);
			return -1;
		}

		if((*block_ID = assign_block()) != 0){
			DEBUG("block ID = %d\n", *block_ID);
			block_map[*block_ID] = 1;

			if(lseek(file_state, (*block_ID) * BLOCK_SIZE, SEEK_SET) < 0){
				LOG_WARN("lseek fail, detail:  %s\n", strerror(errno));
			       	return -3;
			}

			if(block_input_length < BLOCK_SIZE){
				DEBUG("block_input_length < BLOCK_SIZE\n");
				if(write(file_state, block, block_input_length) != block_input_length){
					LOG_WARN("Fail to write, detail: %s\n", strerror(errno));
					return -4;
				}
				int i;
				char zero = 0x00;
				DEBUG("BLOCK_SIZE - block_input_length = %d\n", BLOCK_SIZE - block_input_length);
				for(i = 0; i < BLOCK_SIZE - block_input_length; i++){
					write(file_state, &zero, sizeof(zero));	
				}
				byte_written = block_input_length;
			}else{
				DEBUG("block_input_length >= BLOCK_SIZE\n");
				if(write(file_state, block, BLOCK_SIZE) != BLOCK_SIZE){
					LOG_WARN("Fail to write, detail: %s\n", strerror(errno));
					return -4;
				}
				byte_written = BLOCK_SIZE;
			}
		}else{
			DEBUG("block ID = %d\n", *block_ID);
			LOG_WARN("There is no block can be assigned\n");
			return -2;
		}
		close(file_state);
	}else{
		return -1;
	}

	return byte_written;
}

int read_block(const int block_ID, void *block)
{
	int byte_read = 0;

	DEBUG("fil_system_path = %s\n", file_system_path);
	if(file_state > 0){
		if((file_state = open(file_system_path, O_RDWR)) < 0){
			LOG_WARN("File system %s does not exist, please call create_block first\n", file_system_path);
			return -1;
		}
		
		if(block_ID >= BLOCK_SIZE){
			LOG_WARN("Invalid block_ID\n");
			return -2;
		}

		if(lseek(file_state, block_ID * BLOCK_SIZE, SEEK_SET) < 0){
			LOG_WARN("lseek fail, detail:  %s\n", strerror(errno));
		       	return -3;
		}
		char tmp[BLOCK_SIZE] = {0};
		if((byte_read = read(file_state, tmp, BLOCK_SIZE)) != BLOCK_SIZE){
			LOG_WARN("Fail to read: %s\n", strerror(errno));
			return -4;
		}

		memcpy(block, tmp, BLOCK_SIZE);

		close(file_state);
	}else{
		return -1;
	}

	return byte_read;
}
