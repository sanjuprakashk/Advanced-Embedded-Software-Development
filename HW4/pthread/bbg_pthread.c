#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<math.h>


#define TIMER_MS 100
typedef struct file_name{
	char *file_name;
}file_name;

file_name name;

timer_t timer_cnt;

pthread_t thread1;
pthread_t thread2;

int test_flag = 0;

clock_t start_thread1;
clock_t end_thread1;

clock_t start_thread2;
clock_t end_thread2;

long double cpu_utilization;

void update_timer(int ms_interval)
{
	struct itimerspec update;
	update.it_value.tv_sec = 0;
	update.it_value.tv_nsec = 100000000;
	update.it_interval.tv_sec = 0;
	update.it_interval.tv_nsec = ms_interval;

	int status = timer_settime(timer_cnt, 0, &update, NULL);
	printf("\nupdated timer\n");
}

long double get_cpu_util()
{
	long double cpu_param[4] = {0};
	long double cpu_util = 0;

	FILE *cpu_log;
	cpu_log = fopen("/proc/stat","r");
	fscanf(cpu_log,"%*s %LF %LF %LF %LF", &cpu_param[0],&cpu_param[1],&cpu_param[2],&cpu_param[3]);
	fclose(cpu_log);

	cpu_util = (cpu_param[0] + cpu_param[1] + cpu_param[2]) / (cpu_param[0] + cpu_param[1] + cpu_param[2] + cpu_param[3]);
	cpu_utilization = cpu_util;
	return cpu_util;

}

static void usr1_sig_handler_thread1(int signum)//, siginfo_t *siginfo, void *context)
{
	FILE *file_ptr;
	file_ptr = fopen(name.file_name, "a+");
	fputs("\nThread 1 Exit due to USR1 signal", file_ptr);
	test_flag = 1;
	end_thread1 = clock();
	fprintf(file_ptr,"\nTime taken for thread 1 execution = %f seconds\n",(double)(end_thread1-start_thread1)/CLOCKS_PER_SEC);
	fclose(file_ptr);
	pthread_cancel(thread1);
	exit(0);
}

static void usr2_sig_handler_thread1(int signum)//, siginfo_t *siginfo, void *context)
{
	FILE *file_ptr;
	file_ptr = fopen(name.file_name, "a+");
	fputs("\nThread 1 Exit due to USR2 signal", file_ptr);
	test_flag = 1;
	end_thread1 = clock();
	fprintf(file_ptr,"\nTime taken for thread 1 execution = %f seconds\n",(double)(end_thread1-start_thread1)/CLOCKS_PER_SEC );
	fclose(file_ptr);
	pthread_cancel(thread1);
	exit(0);
}

static void usr1_sig_handler_thread2(int sig, siginfo_t *siginfo, void *context)
{
	FILE *file_ptr;
	file_ptr = fopen(name.file_name, "a");
	fputs("\nThread 2 Exit due to USR1 signal",file_ptr);
	test_flag = 1;
	end_thread2 = clock();
	fprintf(file_ptr,"\nTime taken for thread 2 execution = %f seconds\n",(double)(end_thread2-start_thread2)/CLOCKS_PER_SEC);
	fclose(file_ptr);
	// pthread_exit(0);
	pthread_cancel(thread2);
	exit(0);
}

static void usr2_sig_handler_thread2(int sig, siginfo_t *siginfo, void *context)
{
	FILE *file_ptr;
	file_ptr = fopen(name.file_name, "a");
	fputs("\nThread 2 Exit due to USR2 signal",file_ptr);
	test_flag = 1;
	end_thread2 = clock();
	fprintf(file_ptr,"\nTime taken for thread 2 execution = %f seconds\n",(double)(end_thread2-start_thread2)/CLOCKS_PER_SEC);
	// pthread_exit(0);
	fclose(file_ptr);
	pthread_cancel(thread2);
	exit(0);
}

void timer_handler(union sigval val)
{
	FILE *log_cpu;
	
	long double cpu_util = cpu_utilization;

	printf("\nUtilization = %Lf", cpu_util);

	log_cpu = fopen(name.file_name, "a+");

	if(log_cpu == NULL)
	{
		printf("\nFile opening failed");
	}
	else
	{
		fprintf(log_cpu,"\nCPU Utilization = %Lf %%", cpu_utilization);
	}

	fclose(log_cpu);
	update_timer(TIMER_MS*1000000);

}

void timer_init()
{
	struct sigevent init;

	init.sigev_notify = SIGEV_THREAD;
	init.sigev_notify_function = &timer_handler;
	init.sigev_notify_attributes = NULL;
	init.sigev_value.sival_ptr = &timer_cnt;

	int ret_stat = timer_create(CLOCK_REALTIME, &init, &timer_cnt);

	if(ret_stat != 0)
	{
		printf("Timer creation error");
	}
}


void *thread1_callback(void *arg)
{
	start_thread1= clock();

	int count[26];
	printf("Entered thread1! Thread ID: %lu\n",pthread_self());
	
	int i =0;
	char c;

	signal(SIGUSR1, usr1_sig_handler_thread1);
	signal(SIGUSR2, usr2_sig_handler_thread1);

	file_name *ptr = (file_name *)arg;
	FILE *new_file, *file_ptr_thread1;
	file_ptr_thread1 = fopen(ptr->file_name,"a+");
	fprintf(file_ptr_thread1,"\nEntered Child 1! Thread ID: %lu\n",pthread_self());

	if(file_ptr_thread1 == NULL)
	{
		printf("\nError opening file");
	}

	new_file = fopen("gdb.txt", "r");
	if(new_file == NULL)
	{
		printf("\nError opening input file");
	}

	do
	{
		c = fgetc(new_file);
		if((int)c >=65 && (int)c <=90)
		{
			count[(int)c - 65]++;
		}
		else if((int)c >=97 && (int)c <=122)
		{
			count[(int)c - 97]++;
		}

	}while(!feof(new_file));

	for(i=0;i<26;i++)
	{
		if(count[i] < 100)
		{
			fprintf(file_ptr_thread1,"\nCount of %c is %d", (char)(i+97), count[i]);
		}
	}

	fclose(new_file);
	

	end_thread1 = clock();

	printf("\nChild 1 exit time = %ld\n", end_thread1 - start_thread1);
	fprintf(file_ptr_thread1,"\nTime taken for thread 1 execution = %f seconds\n",(double)(end_thread1-start_thread1)/CLOCKS_PER_SEC);
	fclose(file_ptr_thread1);
	pthread_exit(0);
	//	pthread_cancel(thread1);
}


void *thread2_callback(void *arg)
{
	start_thread2 = clock();
	printf("Entered thread2! Thread ID: %lu\n",pthread_self());

	file_name *ptr = (file_name *)arg;

	FILE *file_ptr_thread2, *log_proc;
	file_ptr_thread2 = fopen(ptr->file_name,"a+");

	fprintf(file_ptr_thread2, "Entered thread2! Thread ID: %lu\n",pthread_self());
	fclose(file_ptr_thread2);

	struct sigaction action_thread_usr1;
	struct sigaction action_thread_usr2;

	action_thread_usr1.sa_sigaction = &usr1_sig_handler_thread2;

	action_thread_usr1.sa_flags = SA_SIGINFO;

	action_thread_usr2.sa_sigaction = &usr2_sig_handler_thread2;

	action_thread_usr2.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &action_thread_usr1, NULL) < 0) {
		perror ("sigaction");
		// return 1;
	}

	if (sigaction(SIGUSR2, &action_thread_usr2, NULL) < 0) {
		perror ("sigaction");
		// return 1;
	}

	update_timer(TIMER_MS*1000000);


 	// signal(SIGUSR1, usr1_sig_handler_thread2);
	// signal(SIGUSR2, usr2_sig_handler_thread2);
    
    while(1)
    {
		cpu_utilization = get_cpu_util() * 100;
    }

	

}


int main(int argc, char *argv[])
{ 	
	int i = 0;
	
	timer_init();
	if(argc < 2)
	{
		printf("Enter file name on command line \n");
		return -1;
	}
	name.file_name = argv[1];

	printf("File name = %s\n", name.file_name);
	
	FILE *file_ptr;
	file_ptr = fopen(name.file_name, "w+");
	fputs("Main thread\n", file_ptr);
	fclose(file_ptr);
	
	pthread_attr_t attribute;
	pthread_attr_init(&attribute);

	pthread_create(&thread1, &attribute, thread1_callback, (void*)&name);
	pthread_create(&thread2, &attribute, thread2_callback, (void*)&name);
	


	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	

	return 0;
}