#include "common.h"
#include "view.h"
#include "edit.h"
#include "help.h"

int main(int argc, char** argv){
    FileInfo file;
    Mp3Tag tags = {0};
    
    if(validate_ip_arg_count(argc)==failure){
        display_help();
        return 1;
    }
    
    Type op_type = chk_operation_type(argv[1]);
    if(op_type==invalid){
        display_help();
        return 1;
    }
    else if(op_type==help){
        display_help();
        return 0;
    }

    if(validate_mp3_file(argv[2], &file)==failure){
        display_help();
        return 1;
    }
    if(op_type==view){
        read_mp3_tags(&tags, &file);
        display_mp3_tags(&tags);
    }
    else if(op_type==edit){
        const char* edit_tag = edit_op_arg_check(argv[3]);
        if(edit_tag==NULL){
            fprintf(stderr, "ERROR: Invalid edit operation\n");
            return 1;
        }
        edit_mp3_tag(argv[4], edit_tag, &file);
    }
    return 0;
}