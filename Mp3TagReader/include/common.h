#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned int uint;

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

#endif