#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#define PORT_NO 6000
#define LOOP_BACK_ADDR "127.0.0.1"
#define MAX_ARR_SIZE 100

int exit_flag = 0;
int client;

typedef struct 
{
	char buffer[25];
	uint32_t buffer_len;
	uint32_t user_led_state;
}ipc_t;

static void signal_handler(int signum)
{
	FILE *file_ptr;
	file_ptr = fopen("log_sock.txt", "a+");
	printf("\nExiting client");
	// fputs("\nClient exit", file_ptr);
	fprintf(file_ptr,"\n[%ld]\t Client exit!", (long int)time(NULL));
	fclose(file_ptr);
	exit_flag = 1;
	close(client);
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);
	FILE *log_sock;
	ipc_t client_struct;
	int rval;
	char temp_buffer[MAX_ARR_SIZE];


	log_sock = fopen("log_sock.txt", "a+");
	if(log_sock == NULL)
	{
		printf("\nFile opening failed");
		return -1;
	}

	fprintf(log_sock,"\nSocket client! PID : %d", getpid());
	fclose(log_sock);

	client = socket(AF_INET,SOCK_STREAM,0);

	if(client < 0)
	{
		perror("\nClient socket open failed");
		exit(1);
	}

	struct sockaddr_in client_addr;

	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT_NO);
	client_addr.sin_addr.s_addr = inet_addr(LOOP_BACK_ADDR);

	 if(connect(client,(struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
            printf("Error on connect\n");
            return 1;
    }

    char server_rec_buff[25];
    client_struct.user_led_state = 0;

    while(exit_flag == 0)
    {	
    	printf("Enter message");
    	scanf("%[^\n]%*c", client_struct.buffer);
    	log_sock = fopen("log_sock.txt", "a+");
    	fprintf(log_sock, "\n[%ld]\t", (long int)time(NULL));
    	fprintf(log_sock,"\tClient sending: %s", client_struct.buffer);

    	fclose(log_sock);

    	send(client, client_struct.buffer, strlen(client_struct.buffer)+1 , 0);

    	read(client, client_struct.buffer, MAX_ARR_SIZE);

    	sscanf(client_struct.buffer, "%s", temp_buffer);
			
		if(strcmp(temp_buffer,"LEDON") == 0)
        {
            client_struct.user_led_state = 1;
        }
        else if(strcmp(temp_buffer,"LEDOFF") == 0)
        {
            client_struct.user_led_state = 0;
        }
        else
        {
        	client_struct.user_led_state = 0;
        }
    	log_sock = fopen("log_sock.txt", "a+");
		fprintf(log_sock, "\n[%ld]\t", (long int)time(NULL));
    	fprintf(log_sock,"\tClient received:- Message: %s LED state: %d", client_struct.buffer, client_struct.user_led_state);
    	fclose(log_sock);
    }

	close(client);

    return 0;
}