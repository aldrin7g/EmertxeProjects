#include "common.h"

Status validate_ip_arg_count(int argc){
    char view_arg_count_chk = (argc==3);
    char edit_arg_count_chk = (argc==5);
    char help_arg_count_chk = (argc==2);
    if(view_arg_count_chk || edit_arg_count_chk || help_arg_count_chk)
        return success;
    fprintf(stderr, E"ERROR: Invalid number of arguments\n"RST);
    return failure;
}

Type chk_operation_type(const char* op_type){
    if(strcmp(op_type, "-v")==0)
        return view;
    else if(strcmp(op_type, "-e")==0)
        return edit;
    else if(strcmp(op_type, "-h")==0)
        return help;
    
    fprintf(stderr, E"ERROR: Invalid operation type\n"RST);
    return invalid;
}

Status validate_mp3_file(const char* fname, FileInfo* file_info){
    // Check file Extension
    const char* extn = strrchr(fname, '.');
    if(extn==NULL || strcmp(extn, ".mp3")!=0){
        fprintf(stderr, E"ERROR: Invalid file extension\n"RST);
        return failure;
    }

    // Check file existence
    file_info->fp = fopen(fname, "rb");
    if(file_info->fp==NULL){
        fprintf(stderr, E"ERROR: File %s not found\n"RST, fname);
        return failure;
    }

    // Check Mp3 file Signature
    char sig[4]; sig[3] = '\0';
    fseek(file_info->fp, 0, SEEK_SET);
    fread(sig, sizeof(char), 3, file_info->fp);
    if(strcmp(sig, "ID3")!=0){
        fprintf(stderr, E"ERROR: Invalid MP3 file signature\n"RST);
        return failure;
    }

    // Check Mp3 Version
    byte ver, rev;
    fseek(file_info->fp, 3, SEEK_SET);
    fread(&ver, sizeof(byte), 1, file_info->fp);
    fread(&rev, sizeof(byte), 1, file_info->fp);
    if(!(ver==3 && rev==0)){
        fprintf(stderr, E"ERROR: Invalid MP3 file version\n"RST);
        return failure;
    }

    file_info->fname = fname;
    return success;
}