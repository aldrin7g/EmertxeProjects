#ifndef EDIT_H
#define EDIT_H

#include "common.h"

// Function Prototypes
const char* edit_op_arg_check(const char* op_type);
Status check_mp3_tag(const char* edit_tag, FileInfo* file);
Status write_new_tag_size(FILE* temp_fp, int frame_size);
Status write_new_tag_data(FILE* temp_fp, const char* new_data);
Status copy_remaining_data(FILE* temp_fp, FileInfo* file);
Status update_tag_end(FILE* temp_fp, int new_tag_size);
Status edit_mp3_tag(const char* new_data, const char* edit_tag, FileInfo* file);
    
#endif