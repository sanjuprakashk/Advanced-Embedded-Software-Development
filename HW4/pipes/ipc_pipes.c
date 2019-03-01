/*
 * File : ipc_pipes.c
 *
 *  Created on: Feb 28, 2019
 *  Author: Sanju Prakash Kannioth
 *  Reference: https://www.youtube.com/watch?v=7bNMkvcOKlc,  
               https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include<signal.h>
#include<time.h>
#include <sys/wait.h>


int ipc_pipe_parent[2];
int ipc_pipe_child[2];

char buf[100];
char buffer[100];

typedef struct 
{
	char buffer[25];
	uint32_t buffer_len;
	uint32_t user_led_state;
}ipc_t;


static void signal_handler_child(int signum)
{
	FILE *file_ptr;
	file_ptr = fopen("log.txt", "a+");
	// fputs("\nChild exit", file_ptr);
	fprintf(file_ptr,"\n[%ld]\t Child exit!", (long int)time(NULL));
    fclose(file_ptr);
    close(ipc_pipe_child[0]);
    close(ipc_pipe_child[1]);
    exit(0);
}

static void signal_handler_parent(int signum)
{
    FILE *file_ptr;
    file_ptr = fopen("log.txt", "a+");
    // fputs("\nParent exit", file_ptr);
    fprintf(file_ptr,"\n[%ld]\t Parent exit!", (long int)time(NULL));
    fclose(file_ptr);
    close(ipc_pipe_parent[0]);
    close(ipc_pipe_parent[1]);
    exit(0);
}


int main(int argc, char *argv[])
{
	pid_t pid;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	// printf("PID = %d", getpid());
	FILE *file_ptr;
	file_ptr = fopen("log.txt", "w+");
	

	int ret;

	char *message = NULL;
	char *parent_msg = NULL;
	char buf1[20];

	pipe(ipc_pipe_child);
	pipe(ipc_pipe_parent);
	
    fprintf(file_ptr,"\nIPC Pipe! PID:%d\n",getpid());
    fclose(file_ptr);
    pid = fork();

	if(pid == 0)
	{
        signal(SIGINT, signal_handler_child);
        while(1)
        {
         	int i = 0;
            char concat_str[100]; 
            FILE *file_ptr;
        	file_ptr = fopen("log.txt", "a+");
        	char child_message[100];
         	ipc_t send_from_child = {0};

            printf("Enter message from parent:\n");
            scanf("%[^\n]%*c", child_message);
         	char *child_ptr_msg = (char *)&send_from_child;

         	strcpy(send_from_child.buffer, child_message);
         	send_from_child.buffer_len = strlen(send_from_child.buffer);
         	// send_from_child.user_led_state = 0;

            fprintf(file_ptr, "[%ld]\t", (long int)time(NULL));
            printf("PID: \n%d\tParent Sending: Message: %s\n", getpid(), send_from_child.buffer);
            fprintf(file_ptr, "PID: %d\tParent Sending: Message: %s\n", getpid(), send_from_child.buffer);
            fclose(file_ptr);
            
            write(ipc_pipe_parent[1], child_ptr_msg, sizeof(ipc_t)); 
            

          	char child_read_buffer[sizeof(ipc_t)];
          	ipc_t *child_ptr;
            read(ipc_pipe_child[0], child_read_buffer, sizeof(ipc_t));
            child_ptr = (ipc_t *)child_read_buffer; 

            sscanf(child_ptr->buffer,"%s",buf);

            // strcpy(buffer,child_ptr->buffer);
            if(strcmp(buf,"LEDON") == 0)
            {
                child_ptr->user_led_state = 1;
            }
            else if(strcmp(buf,"LEDOFF") == 0)
            {
                child_ptr->user_led_state = 0;
            }

            file_ptr = fopen("log.txt", "a+");
            printf("now: %ld\n", (long int)time(NULL));
            printf("PID: \n%d\tChild Received: Message: %s\tLED State: %d\n", getpid(), child_ptr->buffer, child_ptr->user_led_state);
            fprintf(file_ptr, "[%ld]\t", (long int)time(NULL));
            fprintf(file_ptr, "PID: %d\tChild Received: Message: %s\tLED State: %d\n", getpid(), child_ptr->buffer, child_ptr->user_led_state);
            
            fclose(file_ptr);
        }
    } 
  
    else
    { 
        signal(SIGINT, signal_handler_parent);
        while(1)
        {
            int i = 0;
            
            FILE *file_ptr;
            file_ptr = fopen("log.txt", "a+");
        	char parent_read_buffer[sizeof(ipc_t)];
        	char parent_message[100];
        	ipc_t *parent_ptr;  
            

            read(ipc_pipe_parent[0], parent_read_buffer, sizeof(ipc_t)); 
            parent_ptr = (ipc_t *)parent_read_buffer;
            sscanf(parent_ptr->buffer,"%s",buf);
            // strcpy(buffer,parent_ptr->buffer);

            if(strcmp(buf,"LEDON") == 0)
            {
                parent_ptr->user_led_state = 1;
            }
            else if(strcmp(buf,"LEDOFF") == 0)
            {
                parent_ptr->user_led_state = 0;
            }

            fprintf(file_ptr, "[%ld]\t", (long int)time(NULL));
            printf("PID: \n%d\tParent Received: Message: %s\tLED State: %d\n", getpid(), parent_ptr->buffer, parent_ptr->user_led_state);
            fprintf(file_ptr, "PID: %d\tParent Received: Message: %s\tLED State: %d\n", getpid(), parent_ptr->buffer, parent_ptr->user_led_state);
            fclose(file_ptr);

            ipc_t send_from_parent = {0};

            printf("Enter message from child:\n");
            scanf("%[^\n]%*c", parent_message);
            strcpy(send_from_parent.buffer, parent_message);
    	 	send_from_parent.buffer_len = strlen(send_from_parent.buffer);
    	 	// send_from_parent.user_led_state = 1;

    	 	char *parent_ptr_msg = (char *)&send_from_parent;
            file_ptr = fopen("log.txt", "a+");
            fprintf(file_ptr, "[%ld]\t", (long int)time(NULL));
            printf("PID: \n%d\tChild Sending: Message: %s\n", getpid(), send_from_parent.buffer);
            fprintf(file_ptr, "PID: %d\tChild Sending: Message: %s\n", getpid(), send_from_parent.buffer);
            

            write(ipc_pipe_child[1], parent_ptr_msg, sizeof(ipc_t)); 
            fclose(file_ptr);
        }         
        exit(0); 
    } 
	return 0;
}
