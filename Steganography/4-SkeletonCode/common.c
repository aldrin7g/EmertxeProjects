#include "common.h"

OperationType check_operation_type(char* argv[]){
    if(argv[1]==NULL)
        return unsupported;
    if(strcmp(argv[1], "-e") == 0)
        return encode;
    else if(strcmp(argv[1], "-d") == 0)
        return decode;
    else
        return unsupported;
}

Status get_extn(char* result, char* filename){
    if(filename == NULL || result == NULL) 
        return failure;

    char* temp = strrchr(filename, '.');
    // No extension
    if(temp==NULL) return failure;
    // '.' at index 0
    if(temp==filename) return failure;
    // '.' at last index
    if(*(temp+1)==0) return failure;
    
    strcpy(result, temp);
    return success;
}