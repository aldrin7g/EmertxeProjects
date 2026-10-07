#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned int uint;

#define E   "\033[31m"          // Print Errors
#define B   "\033[38;5;117m"    // Prompts
#define W   "\033[37m"          // General
#define O   "\033[38;5;208m"    // Main Menu
#define C   "\033[96m"          // Table Borders
#define Y   "\033[33m"          // Main Header in Table
#define M   "\033[95m"          // Sub Header in Table
#define G   "\033[32m"          // Print Info
#define RST "\033[0m"           // Reset Colour

typedef enum{
    failure,
    success
}Status;

typedef enum{
    invalid,
    view,
    edit,
    help,
}Type;

typedef struct{
    const char* fname;
    FILE* fp;
    int tag_end;
}FileInfo;

typedef struct{
    char* title;
    char* artist;
    char* album;
    char* year;
    char* genre;
    char* comment;
}Mp3Tag;

// Function Prototypes
Status validate_ip_arg_count(int argc);
Type chk_operation_type(const char* op_type);
Status validate_mp3_file(const char* fname, FileInfo* file);
int endian_convert(byte* size_buf);

#endif