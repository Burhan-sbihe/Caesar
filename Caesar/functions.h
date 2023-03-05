/*
# FILE Description:
# In this header file we have declared our functions
*/


#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


/***FUNCTION DECLARTIONS*****/

/////////////////////////////////////////////////////////////////////////////////////////

int count_the_number_of_lines(char* file_path);

/*
This function counts how many lines there are in the file
Argument:
the path of file
Return value:
the number of lines in the file
*/

////////////////////////////////////////////////////////////////////////////////////////

int count_the_number_of_bytes(char* file_path, int* number_of_bytes_in_each_line);

/*
This function counts how many bytes  there are in the file and how mani bytes in each line
Argument:
1)the path of file
2)pointer (integer array) that stores in each index the number of bytes in line index
  (bytes in line 0 stored at index 0)
Return value:
the number of bytes in the file
*/

///////////////////////////////////////////////////////////////////////////////////////

char* modify_adress_for_output(char* the_path);

/*
This function modify the path of input to the output path
Argument:
the path of file input
Return value:
pointer of the output path
*/

///////////////////////////////////////////////////////////////////////////////////////

HANDLE create_file_for_reading_and_chick_if_succeded(char* the_path_of_file);

/*
This function creates file for reading with function CreateFile()
Argument:
the path of file to be created for reading
Return value:
HANDLE for the File , if not secceded we send an appropiate error messege
*/

//////////////////////////////////////////////////////////////////////////////////////

void reading_file_by_number_of_bytes(HANDLE handle_for_file, char* buffer, DWORD dwBytesToRead, DWORD* ptr);

/*
	This function  gets the specified bytes from the file and put it into buffer
	Arguments:
	1)handle  of the file to be readed
	2)the buffer that we want to put the data that we readed
	3)variable that stores how many bytes that have been readed
	4)Address that pointer to the line to be readed from the file
	Return value:
	none.
*/

/////////////////////////////////////////////////////////////////////////////////////

HANDLE create_file_for_writing_and_chick_if_succeded_for_first_Thread(char* the_path_of_file);

/*
This function creates file for writing with function CreateFile() before we call the all other
threads, in order to specify the number of bytes at the output file
Argument:
1)the path of file to be created
Return value:
HANDLE for the File , if not secceded we send an appropiate error messege
*/

////////////////////////////////////////////////////////////////////////////////////

void writing_for_file_by_number_of_bytes(HANDLE handle_for_file, char* buffer, DWORD dwBytesTowrite, DWORD* str);

/*
	This function  writes  the specified bytes from the buffer to  the file
	Argument:
	1)handle for the file to be written
	2)the buffer that we want to write to the file
	3)variable that stores how many bytes that have been written
	4)Address that pointer to the line to to write in the file
	Return value:
	none.
*/

//////////////////////////////////////////////////////////////////////////////////


int string_to_int(char* string);

/*
 This function turns the string into number (if given number as string)
 Argument:
 string of number
 Retun value:
 the number in integer type
 */

 ////////////////////////////////////////////////////////////////////////////////

void decrypted_one_line(char* line, int key, int size, char* DecOrEnc);

/* This function decrypt/encrypt the a string with the key in the input
 Argument:
 1)string that we want to decrypt/encrypt
 2)Key that given by the user to decrypt/encrypt
 3)size is the length of the string to be decrypt/encrypt
 4)global pointer that stores if we want to decrypt/encrypt
 Retun value:
 None
 */

 ///////////////////////////////////////////////////////////////////////////////

char* alloc_str(int size);

/*
This function allocates a memory in type of char* by the size we entered
Argument:
The size of the memory wa we want to allocate
Return value :
Pointer to the memory allocated
*/

///////////////////////////////////////////////////////////////////////////////

HANDLE create_file_for_writing_and_chick_if_succeded(char* the_path_of_file);

/*
This function creates file for writing with function CreateFile() in the thread function
to give the thread access to the file in order to write
Argument:
1)the path of file to be created
Return value:
HANDLE for the File , if not secceded we send an appropiate error messege

  */
  ////////////////////////////////////////////////////////////////////////////

#endif // _FUNCTIONS_H_