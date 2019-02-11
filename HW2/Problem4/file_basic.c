/*
 * File : file_basic.c
 *
 *  Created on: Feb 07, 2019
 *  Author: Sanju Prakash Kannioth
 *  Reference: http://iiti.ac.in/people/~tanimad/FileHandlinginCLanguage.pdf
 *
 */

#include <stdio.h>
#include<stdlib.h>

#define ARR_SIZE 20 // Max size of array

enum status{fileOpenFail = -1}; // Error status

FILE *file_ptr; // File pointer

int main()
{
    printf("Life is too short. Keep coding."); // Interesting message to be printed
    
    file_ptr = fopen("AESD.txt", "w+"); // Opening file with read and write permisson
    
    if(file_ptr == NULL)
    {
	    return fileOpenFail;
    }

    putc('A',file_ptr); // Writing single character to file
    fclose(file_ptr);
    
    file_ptr = fopen("AESD.txt", "a+"); // Opening file in append mode
    
    if(file_ptr == NULL)
    {
	    return fileOpenFail;
    }

    char *arr  = (char*)malloc(sizeof(char)*ARR_SIZE); // Allocating character array of size 20 dynamically
    
    printf("\nEnter a string:");
    scanf(" %[^\n]s", arr);
    fputs(arr,file_ptr); // Writinf string to file
    fflush(file_ptr); // Flush standard out
    fclose(file_ptr);
    
    file_ptr = fopen("AESD.txt","r"); // Opening file in read mode
    
    if(file_ptr == NULL)
    {
	    return fileOpenFail;
    }
    
    char temp[20];
    
    printf("\nReading first character:%c",getc(file_ptr)); // Reading single character from file
    fgets(temp,20,file_ptr); // Reading string of characters from file
    printf("\nReading all characters %s\n",temp);
    fclose(file_ptr);
    
    free(arr); 

    return 0;
}
