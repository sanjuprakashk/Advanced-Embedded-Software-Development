/**
 * @\file	kernel_sort.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This program calls a user written system call ksort to sort a user array in kernel space
 * @\date	02/10/2019
 *
 */
#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
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
	clock_t t_start,t_end; // Variable to store start and end times
	int32_t arr_len = (rand()%(MAX_INDEX - MIN_INDEX +1)) + MIN_INDEX; // Variable to store array length

	int32_t usr_inp[arr_len]; // Variable to store user input
	int32_t usr_op[arr_len];  // Variable to store user output
	int32_t i;

	printf("Testing for NULL input\n");
	long int ret = syscall(398, NULL, usr_op , arr_len);
	if(ret != 0)
	{
		printf("Array input null test passed\n");
	}

	printf("Testing for NULL output\n");
	ret = syscall(398, usr_inp, NULL , arr_len);
	
	if(ret != 0)
	{
		printf("Array output null test passed\n");
	}

	printf("Testing for invalid array length\n");

	ret = syscall(398, usr_inp, usr_op,-1);
	
	if(ret != 0)
	{
		printf("Invalid array length test passed\n");
	}

	for(i=0;i<arr_len;i++)
	{
		usr_inp[i] = rand()%MAX_INDEX;
	}
	
	t_start = clock();
	ret = syscall(398, usr_inp, usr_op , arr_len);
	
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