#define _CRT_SECURE_NO_WARNINGS // we were forced to add it because of the warnings of using strcat, strrchr, strcat...
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<windows.h>
#include "HardCodedData.h"
#include "ThreadCreation.h"

/*Global Variables*/
extern char* DecOrEnc;

/*HELP Functions*/
int count_the_number_of_lines(char* file_path)
{
	int counter = 0;
	char tav;
	FILE* the_input;
	fopen_s(&the_input, file_path, "r");
	if (NULL == the_input)
	{
		printf("The file FAILED to open\nCheck if the file is EXICTING\n");
		exit( STATUS_CODE_FAILURE);
	}
	while (feof(the_input) == 0)
	{
		tav = fgetc(the_input);
		if (tav == back_slash_n)
		{
			counter++;
		}
	}
	fclose(the_input);
	return counter+1;
}

int count_the_number_of_bytes(char* file_path,int* number_of_bytes_in_each_line)
{
	int counter = 0, new_counter = 0;
	char tav= ' ';
	FILE* the_input;
	fopen_s(&the_input, file_path, "r");
	if (NULL == the_input)
	{
		printf("The file FAILED to open\nCheck if the file is EXICTING\n");
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	else
	{
		while (feof(the_input) == 0)
		{	
			new_counter++;
			if (tav == '\n')
			{
				new_counter++;
				*(number_of_bytes_in_each_line) = counter+1;
				counter = 0;
				number_of_bytes_in_each_line++;
			}

			tav = fgetc(the_input);
			counter++;
		}
		*number_of_bytes_in_each_line = counter - 1;
		fclose(the_input);
	}
	return new_counter - 1;
}

char* alloc_str(rsize_t size) { // rsize_t type because strlen returns this type and strrchr gets this type
	char* str = NULL;
	str = (char*)malloc((size) * sizeof(char));
	if (str == NULL) {
		printf("Error while allocating memory!\n");
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	return str;
}

char* modify_adress_for_output(char* the_path)
{
	char back_slash = 92;
	char* ret;
	char* file_path;
	rsize_t the_path_len = strlen(the_path); // the type strlen returns is rsize_t
	
	file_path = alloc_str(the_path_len + 1); // +1 for '\0'
	if (file_path == NULL)// although we checked in the fucntion alloc, we got warning so we checked one more time
	{
		printf("Fatal error: Memory Allocation FAILED!\n");
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	strcpy_s(file_path, the_path_len + 1, the_path);
	ret = strrchr(file_path, back_slash);
	ret++;
	*(ret) = back_slash_zero;
	if (!strcmp(DecOrEnc, "-d"))
	{
		strcat(file_path, "decrypted.txt"); // we used _CRT_SECURE_NO_WARNINGS to cancel the warning of strcat
		//this fuction must be used because we must add the name of the file to the path to create 
		//the file with the right name
	}
	else if (!strcmp(DecOrEnc, "-e"))
	{
		strcat(file_path, "encrypted.txt");
	}
	else
	{
		printf("YOU DID NOT Enroll The RIGHT flag for Decryption / Eecryption !\n");
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	return file_path;
}

HANDLE create_file_for_reading_and_chick_if_succeded(char* the_path_of_file)
{
	HANDLE the_handle_of_file;
	the_handle_of_file = CreateFile(the_path_of_file,  // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_ALWAYS,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);   // no attr. template

	if (the_handle_of_file == INVALID_HANDLE_VALUE)
	{
		printf("Terminal failure: Unable to open file for read %08x \n", GetLastError());
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	return the_handle_of_file;
}

void reading_file_by_number_of_bytes(HANDLE handle_for_file, char* buffer, DWORD dwBytesToRead, DWORD* ptr)
{
	BOOL read_sign;
	read_sign = ReadFile(handle_for_file, buffer, dwBytesToRead, (LPDWORD)ptr ,NULL);
	if (read_sign == 0)
	{
		printf("READING FILE IS NOT SUCCEDED!\n");
		printf("Program failed! Ending program. Last Error = 0x%x\n", GetLastError());
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
}

HANDLE create_file_for_writing_and_chick_if_succeded_for_first_Thread(char* the_path_of_file)
{
	HANDLE the_handle_of_file;
	the_handle_of_file = CreateFile(the_path_of_file,       // name of the write
		FILE_SHARE_WRITE,          // open for writing
		1,                      //  share mode
		NULL,                   // default security
		CREATE_ALWAYS,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (the_handle_of_file == INVALID_HANDLE_VALUE)
	{
		printf("Terminal failure: INVALID Handle Value.%08x \n", GetLastError());
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	return the_handle_of_file;
}

HANDLE create_file_for_writing_and_chick_if_succeded(char* the_path_of_file)
{
	HANDLE the_handle_of_file;
	the_handle_of_file = CreateFile(the_path_of_file,       // name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_WRITE,                      //  share mode
		NULL,                   // default security
		OPEN_ALWAYS,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (the_handle_of_file == INVALID_HANDLE_VALUE)
	{
		printf("Terminal failure: INVALID Handle Value.%08x \n", GetLastError());
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
	return the_handle_of_file;
}

void writing_for_file_by_number_of_bytes(HANDLE handle_for_file, char* buffer, DWORD dwBytesTowrite, DWORD* str)
{
	BOOL bErrorFlag = FALSE;
	bErrorFlag = WriteFile(		
		handle_for_file,           // open file handle
		buffer,      // start of data to write
		dwBytesTowrite,  // number of bytes to write
		(LPDWORD)str, // number of bytes that were written
		NULL);

	if (bErrorFlag == 0)
	{
		printf("WRITING IS NOT SUCCEDED!\n");
		printf("Terminal failure: Unable to write in file.%08x \n", GetLastError());
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
}

int string_to_int(char* string)
{
	int key;
	key = atoi(string);
	return key;
}

void decrypted_one_line(char* line, int key, int size, char* DecOrEnc)
{
	char the_decyfir_outcome;
	if (!strcmp(DecOrEnc , "-d"))
	{
		while ((size > 0))
		{
			if ((*line >= a) && (*line <= z))
			{
				the_decyfir_outcome = (*line - 'a' - key);
				if (the_decyfir_outcome >= 0)
				{
					*line = 'a' + (*line - 'a' - key) % 26;
				}
				else
				{
					*line = ('a' + (*line - 'a' - key) % 26) + 26;
				}
			}
			if ((*line >= A) && (*line <= Z))
			{
				the_decyfir_outcome = (*line - 'A' - key);
				if (the_decyfir_outcome >= 0)
				{
					*line = 'A' + (*line - 'A' - key) % 26;
				}
				else
				{
					*line = ('A' + (*line - 'A' - key) % 26) + 26;
				}
			}
			if ((*line >= zero_asci) && (*line <= nine_asci))
			{
				the_decyfir_outcome = (*line - '0' - key);
				if (the_decyfir_outcome >= 0)
				{
					*line = '0' + (*line - '0' - key) % 10;
				}
				else
				{
					*line = ('0' + (*line - '0' - key) % 10) + 10;
				}
			}
			line++;
			size--;
		}
	}
	else if (!strcmp(DecOrEnc, "-e"))
	{
		while ((size > 0))
		{
			if ((*line >= a) && (*line <= z))
			{
				the_decyfir_outcome = (*line - 'a' + key);

				if (the_decyfir_outcome >= 0)
				{
					*line = 'a' + (*line - 'a' + key) % 26;
				}
				else
				{
					*line = ('a' + (*line - 'a' + key) % 26) + 26;
				}
			}
			if ((*line >= A) && (*line <= Z))
			{
				the_decyfir_outcome = (*line - 'A' + key);
				if (the_decyfir_outcome >= 0)
				{
					*line = 'A' + (*line - 'A' + key) % 26;
				}
				else
				{
					*line = ('A' + (*line - 'A' + key) % 26) + 26;
				}
			}
			if ((*line >= zero_asci) && (*line <= nine_asci))
			{
				the_decyfir_outcome = (*line - '0' + key);
				if (the_decyfir_outcome >= 0)
				{
					*line = '0' + (*line - '0' + key) % 10;
				}
				else
				{
					*line = ('0' + (*line - '0' + key) % 10) + 10;
				}
			}
			line++;
			size--;
		}
	}
	else
	{
		printf("YOU DID NOT Enroll The RIGHT flag for Encryption / Eecryption !");
		free_allocated();
		exit(STATUS_CODE_FAILURE);
	}
}