/**
 * @\file	kernel_sort_cron.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This program calls a user written system call ksort every 10 minutes to sort a user array in kernel space 
 *			using CRON
 * @\date	02/10/2019
 * References: https://www.geeksforgeeks.org/merge-sort/, 
 *			   https://stackoverflow.com/questions/2408976/struct-timeval-to-printable-format
 *
 */

#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#define MIN_INDEX 1
#define MAX_INDEX 256

/**
--------------------------------------------------------------------------------------------
k_printArr
--------------------------------------------------------------------------------------------
* This function prints the array
*
*   @\param arr     array input to be printed
*			len 	array length

*   @\return        void
*
**/
void k_printArr(int32_t *arr, int32_t len)
{
	int32_t i;
	for(i=0;i<len;i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main()
{
	srand(time(0));
	
	
	struct timeval tv;
	time_t t_time;
	gettimeofday(&tv, NULL); // Getting data and time from gettimeofday system call
	t_time = tv.tv_sec;
	
	clock_t t_start,t_end;

	int32_t arr_len = (rand()%(MAX_INDEX - MIN_INDEX +1)) + MIN_INDEX;

	int32_t usr_inp[arr_len];
	int32_t usr_op[arr_len];
	int32_t i;

	printf("Current PID = %d\n", getpid()); // Getting processor ID from getpid system call
	printf("Current User ID = %d\n", getuid()); // Getting user ID from getuid system call
	printf("Current date and time = %s\n", ctime(&t_time)); // Format the date and time using ctime function
	
	for(i=0;i<arr_len;i++)
	{
		usr_inp[i] = rand()%MAX_INDEX;
	}
	
	t_start = clock();
	long int ret = syscall(398, usr_inp, usr_op , arr_len);
	
	if(ret != 0)
	{
		printf("Kernel sort failed\n");
	}
	else
	{
		printf("Pre sort = ");
		k_printArr(usr_inp,arr_len);
		printf("\n");

		printf("Post sort = ");
		k_printArr(usr_op,arr_len);
	}

	t_end = clock() - t_start;
	printf("\n Time taken for the program to execute = %f seconds",((double)t_end/CLOCKS_PER_SEC));
	
	return 0;
}