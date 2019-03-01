/*
 * File : ipc_shared_mem_process2.c
 *
 *  Created on: Feb 28, 2019
 *  Author: Sanju Prakash Kannioth
 *  Reference: https://www.youtube.com/watch?v=dQE6BDNc3z0,
 *		       
 *
 */
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

#define SM_FILE_NAME "/shared_mem"
#define MAX_BUFFER_SIZE (100)
#define MEM_SIZE (200)

typedef struct 
{
	char buffer[100];
	uint32_t buffer_len;
	uint32_t user_led_state;
	int mutex_var;
}ipc_t;

ipc_t *shared_addr; 

static void signal_handler(int signum)
{
	FILE *file_ptr;
	file_ptr = fopen("log_shared_mem.txt", "a+");
	fprintf(file_ptr,"\n[%ld]\t Process2 exit!", (long int)time(NULL));
	printf("\nExiting process2");
	fclose(file_ptr);
	// exit_flag = 1;
	// close(client_conn);
	exit(0);
}

int main()
{

	signal(SIGINT, signal_handler);
	char temp_buffer[MAX_BUFFER_SIZE];
	FILE *log_shared_mem;
	log_shared_mem = fopen("log_shared_mem.txt", "a+");
	if(log_shared_mem == NULL)
	{
		printf("\nFile opening failed");
		return -1;
	}

	fprintf(log_shared_mem,"\nShared memory process 2! PID : %d\tShared memory name :%s", getpid(),SM_FILE_NAME);
	fclose(log_shared_mem);

	int shared_mem_fd = shm_open(SM_FILE_NAME, O_CREAT | O_RDWR, 0666);	

	int shared_mem_size = sizeof(ipc_t);

	int ret_val = ftruncate(shared_mem_fd, shared_mem_size);
	if(ret_val < 0)
	{
		perror("ftruncation error");
		exit(1);
	}

	shared_addr = mmap(NULL, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED , shared_mem_fd, 0);	
	if(shared_addr < 0)
	{
		perror("shared memory map error");
		exit(1);
	}

	shared_addr->user_led_state = 0;

	while(1)
	{
		shared_addr->mutex_var = 0;

		while (shared_addr->mutex_var != 1)
	    	sleep(1);
			
		printf("Process 2 receivng %s\n",shared_addr->buffer);
		sscanf(shared_addr->buffer, "%s", temp_buffer);
			
		if(strcmp(temp_buffer,"LEDON") == 0)
        {
           shared_addr->user_led_state = 1;
        }
        else if(strcmp(temp_buffer,"LEDOFF") == 0)
        {
            shared_addr->user_led_state = 0;
        }
		log_shared_mem = fopen("log_shared_mem.txt", "a+");
    	fprintf(log_shared_mem, "\n[%ld]\t", (long int)time(NULL));
    	fprintf(log_shared_mem,"\tProcess 2 received: %s LED state: %d", shared_addr->buffer,shared_addr->user_led_state);

    	fclose(log_shared_mem);

		printf("Process 2 sending:\n");

		scanf("%[^\n]%*c",shared_addr->buffer);
		log_shared_mem = fopen("log_shared_mem.txt", "a+");
    	fprintf(log_shared_mem, "\n[%ld]\t", (long int)time(NULL));
    	fprintf(log_shared_mem,"\tProcess 2 sending: %s", shared_addr->buffer);

    	fclose(log_shared_mem);
		shared_addr->mutex_var = 0;

	}
}



