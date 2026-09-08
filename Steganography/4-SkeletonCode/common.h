#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

#define E   "\033[31m"          // Errors
#define O   "\033[33m"          // Operations
#define D   "\033[34m"          // Debug
#define I   "\033[95m"          // Info
#define F   "\033[37m"          // Filenames
#define S   "\033[32m"          // Success
#define Rst "\033[0m"           // Reset

#define MAX_FILE_SUFFIX 5
#define MAX_FILE_NAME 50
#define MAX_FILE_PATH 100
#define BUFFER_SIZE 1024

/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*"
#define MAGIC_STRING_LEN strlen(MAGIC_STRING)
#define base_path_output "output/"
#define base_path_input "input/"

OperationType check_operation_type(char* argv[]);
Status get_extn(char* result, char* filename);

#endif