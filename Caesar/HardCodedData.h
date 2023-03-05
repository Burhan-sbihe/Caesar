/*
# Description:
#	In this header file we have defined some parameters 
*/

#ifndef __HARDCODEDDATA_H__
#define __HARDCODEDDATA_H__

// ****** CONSTANTS *******
#define ERROR_CODE ((int)(-1))
#define a 97
#define z 122
#define A 65
#define Z 90
#define zero_asci 48
#define nine_asci 57
#define back_slash_n '\n'
#define back_slash_zero '\0'

// ***** STATUS CONSTANTS *****
static const int STATUS_CODE_SUCCESS = 0;
static const int STATUS_CODE_FAILURE = -1;

// ***** STRUCTS *****
typedef struct
{
	char* input_path; //input
	char* output_path;  // input
	int start_byte_number;// input
	int end_byte_number;// input
	int key;
} THREAD_Arguments;


#endif // __HARDCODEDDATA_H__