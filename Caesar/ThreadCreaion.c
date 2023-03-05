#include <Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include "functions.h"
#include "HardCodedData.h"
#include "ThreadCreation.h"

/* Global variables*/
HANDLE *p_threads_handles = NULL;
THREAD_Arguments* Thread;
LPDWORD thread_id;
char* Path_of_output;
int* lines_divided;
int* number_of_bytes_in_each_line;
char* DecOrEnc;

/*HELP FUNCTIONS AND FUNDEMENTAL FUNCTION*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, THREAD_Arguments* thread_arg_p, int thread_idx)
{
	HANDLE thread_handle = NULL; 
	if (NULL == p_start_routine)
	{
		// Should not get here
		printf("Error when creating a Thread\n");
		printf("Received null routine pointer\n");
		close_threads(thread_idx); 
		free_allocated();
		exit(ERROR_CODE);
	}
	if (NULL == p_thread_id)
	{
		// Should not get here
		printf("Error when creating a Thread\n");
		printf("Received null id pointer\n");
		close_threads(thread_idx);/* Close thread handles and check if closing succeeded */
		free_allocated(); 	/*Free ALL allocated Memory*/
		exit(ERROR_CODE);
	}
	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		thread_arg_p,    /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create Thread\n");
		close_threads(thread_idx);/* Close thread handles and check if closing succeeded */
		free_allocated(); 	/*Free ALL allocated Memory*/
		exit(ERROR_CODE);
	}
	// if reached here, creation succed
	return thread_handle;
}


void close_threads(int thread_num) {
	int ret_status = STATUS_CODE_SUCCESS;
	for (int i = 0; i < thread_num; i++)
	{
		if (CloseHandle(p_threads_handles[i]) == FALSE)/*closing all handles of threads*/
		{
			ret_status = STATUS_CODE_FAILURE;
		}
	}
	if (ret_status == STATUS_CODE_FAILURE)
	{
		printf("Error while closing Threads%d\n", GetLastError());
		exit(ERROR_CODE);
	}
}


DWORD WINAPI Deciphering(THREAD_Arguments *Thread)
{
	DWORD ptr;
	DWORD str;
	HANDLE input_file = NULL;
	HANDLE output_file = NULL;
	char* buffer;
	char* input_path = Thread->input_path;
	char* output_path = Thread->output_path;
	int start_byte_number = Thread->start_byte_number;
	int end_byte_number = Thread->end_byte_number;
	int key = Thread->key;
	buffer = (char*)calloc((size_t)end_byte_number - (size_t)start_byte_number + 2 , sizeof(char));// +2 for /0
	
	/*Read, Decrypt/Encrypt and Write */
	input_file = create_file_for_reading_and_chick_if_succeded(input_path);
	ptr = SetFilePointer(input_file, start_byte_number, NULL, FILE_BEGIN);
	reading_file_by_number_of_bytes(input_file, buffer,(end_byte_number - start_byte_number +1), &ptr);
	decrypted_one_line(buffer, key, (end_byte_number - start_byte_number + 1) , DecOrEnc);
	output_file = create_file_for_writing_and_chick_if_succeded(output_path);
	str = SetFilePointer(output_file, start_byte_number, NULL, FILE_BEGIN);
	writing_for_file_by_number_of_bytes(output_file, buffer,(end_byte_number - start_byte_number + 1), &str);

	/*Close Other Handles and check if closing succeeded*/
	if (CloseHandle(input_file)== FALSE)
	{
		printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
		free_allocated();
		return STATUS_CODE_FAILURE;
	}
	if (CloseHandle(output_file) == FALSE)
	{
		printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
		free_allocated();
		return STATUS_CODE_FAILURE;
	}
	/*Free Allocate*/
	free(buffer);
	return STATUS_CODE_SUCCESS;
}


int Thread_Manage(char* path, char* key, char* NumOfThreads, char* decOrenc)
{
	int thread_num, num_of_lines, start = 0, count_bytes = 0, counter=0, bytes_in_lines_thread;
	int i, j;

	HANDLE output;
	DWORD wait_code;
	DecOrEnc = decOrenc;
	thread_num = string_to_int(NumOfThreads); /*get number of threads*/
	num_of_lines = count_the_number_of_lines(path);/*get number of lines*/
	if (thread_num <= 0) // Check if number of threads is positive
	{
		printf("Number Of Thread is NOT positive!\n");
		return STATUS_CODE_FAILURE;
	}
	else
	{
		p_threads_handles = (HANDLE*)malloc((thread_num) * sizeof(HANDLE)); // array of thread handles with size of number threads given
		Thread = (THREAD_Arguments*)malloc((thread_num) * sizeof(THREAD_Arguments)); // array of threads with size of number threads 
		thread_id = (LPDWORD)malloc((thread_num) * sizeof(LPDWORD));
		number_of_bytes_in_each_line = (int*)calloc(num_of_lines , sizeof(int));//we allocate array for each line the bytes 
		count_bytes = count_the_number_of_bytes(path, number_of_bytes_in_each_line);//count the whole bytes
		if ((NULL == Thread) || (NULL == p_threads_handles) || (NULL == thread_id) || (NULL == number_of_bytes_in_each_line)) // check if memory allocation is done 
		{
			printf("Fatal error: Memory Allocation FAILED!\n");
			free_allocated();
			return STATUS_CODE_FAILURE;
		}
		else
		{
			Path_of_output = modify_adress_for_output(path); /* get the path output*/
			output = create_file_for_writing_and_chick_if_succeded_for_first_Thread(Path_of_output);
			if (SetFilePointer(output, count_bytes, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)// check if set file pointer works properly
			{
				printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
				free_allocated();
				return STATUS_CODE_FAILURE;
			} 
			if (SetEndOfFile(output) == INVALID_SET_FILE_POINTER)
			{
				printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
				free_allocated();
				return STATUS_CODE_FAILURE;
			}
			if (CloseHandle(output) == FALSE )// check if handle is closed
			{
				printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
				free_allocated();
				return STATUS_CODE_FAILURE;
			}
			lines_divided = Divide_Lines_To_Threads(num_of_lines, thread_num);   /// call divide lines to threads
			for (j = 0; j < thread_num; j++) 
			{	
				bytes_in_lines_thread = 0;
				Thread[j].key = string_to_int(key); /*initalize the Thread arguments*/
				Thread[j].input_path = path;
				Thread[j].output_path = Path_of_output;
				Thread[j].start_byte_number = start;
				for (i = counter ; i < (counter +lines_divided[j]); i++) /*counts the bytes in the lines of thread[i]*/
				for (i = counter ; i < (counter +lines_divided[j]); i++) /*counts the bytes in the lines of thread[i]*/
				{
					bytes_in_lines_thread = bytes_in_lines_thread + number_of_bytes_in_each_line[i];
				}
				Thread[j].end_byte_number = start + bytes_in_lines_thread - 1;
				start = start + bytes_in_lines_thread; 
				counter = counter + lines_divided[j];
				p_threads_handles[j] = CreateThreadSimple(Deciphering, &thread_id[j], &Thread[j], j);
			}
			wait_code = WaitForMultipleObjects(thread_num, p_threads_handles, TRUE, 20000);		/* Wait */
			if (WAIT_OBJECT_0 != wait_code)
			{
				printf("Error when waiting. Last Error = 0x%x\n", GetLastError());
				free_allocated();
				return STATUS_CODE_FAILURE;
			}
			close_threads(thread_num);	/* Close thread handles and check if closing succeeded */
			free_allocated(); 	/*Free ALL allocated Memory*/
		}
		return STATUS_CODE_SUCCESS;
	}
}

int* Divide_Lines_To_Threads(int num_of_lines, int num_of_threads)
{
	int i;
	int* lines_num_in_each_thread;
	if (num_of_threads != 0)// We already checked if number of threads is positivie before we inter here
	{						//but here we faced a warning that says it could be zero because 
							//we divide by the number of threads so we forced to ask one more time
		int remainder = num_of_lines % num_of_threads;
		lines_num_in_each_thread = (int*)malloc(num_of_threads * sizeof(int));

		if (NULL == lines_num_in_each_thread) // check if memory allocation is done 
		{
			printf("Fatal error: memory allocation failed!\n");
			free_allocated();
			exit(STATUS_CODE_FAILURE);
		}
		else
		{
			for (i = 0; i < num_of_threads; i++)
			{

				lines_num_in_each_thread[i] = (num_of_lines / num_of_threads);

				if (remainder > 0)
				{
					lines_num_in_each_thread[i]++;
					remainder--;
				}
			}
			return lines_num_in_each_thread;
		}
	}
	else 
	{
		printf("Number Of Thread is NOT positive!\n");
		exit(STATUS_CODE_FAILURE);
	}
}

void free_allocated() /* free allocation*/
{
	free(p_threads_handles);
	free(lines_divided);
	free(Thread);
	free(thread_id);
	free(Path_of_output);
	free(number_of_bytes_in_each_line);
}