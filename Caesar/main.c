#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"
#include "HardCodedData.h"
#include "ThreadCreation.h"

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("Please provide a correct arguments!\n");
		return STATUS_CODE_FAILURE;
	}

    return Thread_Manage(argv[1], argv[2], argv[3], argv[4]);
}