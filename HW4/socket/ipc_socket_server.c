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
#define MAX_ARR_SIZE 100 

int server;

int exit_flag = 0;

int client_conn;

typedef struct 
{
	char buffer[MAX_ARR_SIZE];
	uint32_t buffer_len;
	uint32_t user_led_state;
}ipc_t;



static void signal_handler(int signum)
{
	FILE *file_ptr;
	file_ptr = fopen("log_sock.txt", "a+");
	fprintf(file_ptr,"\n[%ld]\t Server exit!", (long int)time(NULL));
	printf("\nExiting server");
	fclose(file_ptr);
	exit_flag = 1;
	close(client_conn);
	exit(0);
}


int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);
	FILE *log_sock;
	ipc_t server_struct;
	int rval;

	log_sock = fopen("log_sock.txt", "w+");
	if(log_sock == NULL)
	{
		printf("\nFile opening failed");
		return -1;
	}

	fprintf(log_sock,"\nSocket server! PID : %d", getpid());
	fclose(log_sock);


	server = socket(AF_INET,SOCK_STREAM,0);

	if(server < 0)
	{
		perror("\nServer socket open failed");
		exit(1);
	}

	struct sockaddr_in server_addr;

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NO); 

	if(bind(server,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
	{
		perror("\nBinding failure");
		exit(1);
	}


	if(listen(server, 5) < 0)
	{
		perror("\nListen failure");
		exit(1);
	}
	else
	{
		printf("\nWaiting for client");
	}

	struct sockaddr_in client_conn_addr;
	int client_conn_len = sizeof(client_conn_addr);
	char client_rec_buff[MAX_ARR_SIZE];
	char temp_buffer[MAX_ARR_SIZE];

	client_conn = accept(server, (struct sockaddr *)&client_conn_addr, (socklen_t *)&client_conn_len);

		if(client_conn == -1)
			perror("\nServer accept failed");
	server_struct.user_led_state = 0;
	while(exit_flag == 0)
	{
		log_sock = fopen("log_sock.txt", "a+");

		if((rval = recv(client_conn, server_struct.buffer, sizeof(server_struct.buffer),0))<0)
			perror("Reading error");
		else if(rval == 0)
		{
			printf("Connection ended\n");
			fprintf(log_sock,"\n[%ld]\t Server exit!", (long int)time(NULL));
			break;
		}
		else
		{
			sscanf(server_struct.buffer, "%s", temp_buffer);
			
			if(strcmp(temp_buffer,"LEDON") == 0)
            {
                server_struct.user_led_state = 1;
            }
            else if(strcmp(temp_buffer,"LEDOFF") == 0)
            {
                server_struct.user_led_state = 0;
            }
            else
            {
            	server_struct.user_led_state = 0;
            }
			printf("Message: %s\n",server_struct.buffer);
			fprintf(log_sock, "\n[%ld]\t", (long int)time(NULL));
			fprintf(log_sock,"\tServer received - Message: %s LED state: %d", server_struct.buffer, server_struct.user_led_state);
			fclose(log_sock);
		}
		
		
		
		printf("\nEnter message to send to client:\n");
		scanf("%[^\n]%*c", client_rec_buff);
		log_sock = fopen("log_sock.txt", "a+");
		fprintf(log_sock, "\n[%ld]\t", (long int)time(NULL));
		fprintf(log_sock,"\tServer sending: %s", client_rec_buff);
		send(client_conn, client_rec_buff, MAX_ARR_SIZE, 0);

		fclose(log_sock);
		
	}
	close(server);
	return 0;
}
