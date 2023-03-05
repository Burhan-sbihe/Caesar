/*
# FILE Description:
# In this header file we have declared our functions that dealing with cration or closing Threads 
and all of the help function for threads working
*/

#ifndef __THREADCREAION_H__
#define __THREADCREATION_H__

/***FUNCTION DECLARTIONS*****/

/////////////////////////////////////////////////////////////////////////////////////////

void close_threads(int threads_num);
/* this function closes all the handles of the threads, if it fails to close handle it will continue to close 
* the other handles and then checks if failed and exits, as required.
Argument:
the number of handles of threads to be closed
Return:
None
*/
/////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Deciphering(THREAD_Arguments* Thread);
/* This fuction is the Thread Function, which reads the specified data to be decrypt/encrypt,
then decrypt/encrypt the data, crates handle to the file to be written, and write the decrypted/encrypted data 
to the right position in the file (actually it calls the functions that do the needed purpose; functions in functions.c)
Arguments:
struct of the thread that stores the needed thread arguments
Return 
* If the function succeeds, the return value STATUS_CODE_SUCCESS.
* If the function fails, the return value is STATUS_CODE_FAILURE.*/
/////////////////////////////////////////////////////////////////////////////////////////

int Thread_Manage(char* path, char* key, char* NumOfThreads, char* DecOrEnc);
/* THIS FUNCIONS IS THE FUNDEMENTAL FUNCTION 
This function manages the threads,
checks is the number of threads is valid, calls the function to modify the output address and creates the file with 
same amount of bytes in the input file that we counted, 
initializes the thread arguments, Activates the threads, waits for all threads to finish 
then frees the allocated memory and closing the handles.
Arguments
1. the path of the input file
2. the key
3. number of threads
4. the decyrpt\encrypt mode
Return 
* If the function succeeds, the return value STATUS_CODE_SUCCESS.
* If the function fails, the return value is STATUS_CODE_FAILURE.*/
/////////////////////////////////////////////////////////////////////////////////////////

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, THREAD_Arguments* thread_arg_p, int thread_idx);

/* this function creates the threads, it checks if the recevied pointers are valid (NOT NULL) 
and checks if succeded to create the thread if not closes all the thread handles, frees all the allocated memory
and exits.
Arguments:
1.thread function  
2.thread identifier
3. pointer to sturct of the thread -arguments to thread function
4. number of thread 
Return:
Handle of the created thread
*/
/////////////////////////////////////////////////////////////////////////////////////////

int* Divide_Lines_To_Threads(int num_of_lines, int num_of_threads);
/* this functions divides the lines to each thread
* Arguments:
* 1. number of lines in the input file
* 2. number of threads
* Return:
* pointer to array that stores in each index the number of lines for the thread of number of the index
* ( index 2 stores lines of Thread 2)
*/
/////////////////////////////////////////////////////////////////////////////////////////

void free_allocated();
/*
this function frees all the allocated memory 
Arguments:
None
Retrun 
None
*/
/////////////////////////////////////////////////////////////////////////////////////////

#endif // __THREADCREAION_H__