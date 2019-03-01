/*
 * File : ipc_message_queue_process2.c
 *
 *  Created on: Feb 28, 2019
 *  Author: Sanju Prakash Kannioth
 *  Reference: https://www.youtube.com/watch?v=i0XUbhIBbEc,
 *             https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm,
 *             https://www.geeksforgeeks.org/ipc-using-message-queues/
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


#define QUEUE_NAME "/msg_queue"
#define MAX_BUFFER_SIZE	100


mqd_t msg_queue;

typedef struct 
{
	char buffer[25];
	uint32_t buffer_len;
	uint32_t user_led_state;
}ipc_t;

static void signal_handler(int signum)
{
    FILE *file_ptr;
    file_ptr = fopen("log_queue.txt", "a+");
    // fputs("\nProcess 2 exit", file_ptr);
    fprintf(file_ptr,"\n[%ld]\t Queue 2 exit!", (long int)time(NULL));
    fclose(file_ptr);
    
    mq_close(msg_queue);
    mq_unlink(QUEUE_NAME);
    exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);
	FILE *file_ptr;
	file_ptr = fopen("log_queue.txt", "a+");
	fprintf(file_ptr,"\nIPC Message Queue process 2!PID:%d\tQueue name :%s\n",getpid(),QUEUE_NAME);
    fclose(file_ptr);
    char temp_buffer[100];

    struct mq_attr mq_attributes;

    mq_attributes.mq_flags = 0;
    mq_attributes.mq_maxmsg = 10;
    mq_attributes.mq_msgsize = MAX_BUFFER_SIZE;
    mq_attributes.mq_curmsgs = 5;
    msg_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &mq_attributes);

    ipc_t msg_queue_struct;
    msg_queue_struct.user_led_state = 0;
    while(1)
    {
        
    	file_ptr = fopen("log_queue.txt", "a+");

    	mq_receive(msg_queue, msg_queue_struct.buffer, MAX_BUFFER_SIZE, 0);
        sscanf(msg_queue_struct.buffer, "%s", temp_buffer);
        if(strcmp(temp_buffer,"LEDON") == 0)
        {
            msg_queue_struct.user_led_state = 1;
        }
        else if(strcmp(temp_buffer,"LEDOFF") == 0)
        {
            msg_queue_struct.user_led_state = 0;
        }
        else
        {
            msg_queue_struct.user_led_state = 0;
        }

        fprintf(file_ptr, "\n[%ld]\t", (long int)time(NULL));
    	fprintf(file_ptr,"Process 2 received- Message: %s LED state: %d\n", msg_queue_struct.buffer, msg_queue_struct.user_led_state);
        printf("\nProcess 2 received- Message: %s LED state: %d", msg_queue_struct.buffer, msg_queue_struct.user_led_state);
        fclose(file_ptr);
        
        file_ptr = fopen("log_queue.txt", "a+");

        scanf("%[^\n]%*c", msg_queue_struct.buffer);
        fprintf(file_ptr, "\n[%ld]\t", (long int)time(NULL));
        fprintf(file_ptr,"Process 2 sending- Message: %s LED state: %d\n", msg_queue_struct.buffer, msg_queue_struct.user_led_state);
        mq_send(msg_queue, msg_queue_struct.buffer, MAX_BUFFER_SIZE, 0);

    	fclose(file_ptr);    	 

    }

    mq_close(msg_queue);
    mq_unlink(QUEUE_NAME);
    return 0;
}

