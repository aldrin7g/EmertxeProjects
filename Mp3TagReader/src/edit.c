#include "edit.h"
#include "view.h"

const char* edit_op_arg_check(const char* op_type){
    if(strcmp(op_type, "-y")==0)
        return "TYER";
    else if(strcmp(op_type, "-t")==0)
        return "TIT2";
    else if(strcmp(op_type, "-A")==0)
        return "TPE1";
    else if(strcmp(op_type, "-a")==0)
        return "TALB";
    else if(strcmp(op_type, "-g")==0)
        return "TCON";
    else if(strcmp(op_type, "-c")==0)
        return "COMM";
    return NULL;
}

Status check_mp3_tag(const char* edit_tag, FileInfo* file){
    // Read Mp3 Tag
    char tag[5];
    fread(tag, sizeof(char), 4, file->fp);
    tag[4] = '\0';

    fseek(file->fp, -4, SEEK_CUR);
    if(strcmp(tag, edit_tag) == 0)
        return success;

    return failure;
}

Status write_new_tag_size(FILE* temp_fp, int frame_size){
    byte size_buffer[4];
    size_buffer[0] = (frame_size >> 24) & 0xFF;
    size_buffer[1] = (frame_size >> 16) & 0xFF;
    size_buffer[2] = (frame_size >> 8) & 0xFF;
    size_buffer[3] = frame_size & 0xFF;

    if(fwrite(size_buffer, sizeof(byte), 4, temp_fp) != 4)
        return failure;
    return success;
}

Status write_new_tag_data(FILE* temp_fp, const char* new_data){
    size_t len = strlen(new_data);
    if(fwrite(new_data, sizeof(char), len, temp_fp) != len)
        return failure;
    return success;
}

Status copy_remaining_data(FILE* temp_fp, FileInfo* file){
    byte buffer[1024];
    size_t bytes_read;
    while((bytes_read = fread(buffer, sizeof(byte), sizeof(buffer), file->fp)) > 0){
        if(fwrite(buffer, sizeof(byte), bytes_read, temp_fp) != bytes_read)
            return failure;
    }
    return success;
}

Status update_tag_end(FILE* temp_fp, int tag_size){
    int pos = ftell(temp_fp);
    byte size[4];
    size[0] = (tag_size >> 21) & 0x7F;
    size[1] = (tag_size >> 14) & 0x7F;
    size[2] = (tag_size >> 7)  & 0x7F;
    size[3] = tag_size & 0x7F;

    fseek(temp_fp, 6, SEEK_SET);
    if (fwrite(size, sizeof(byte), 4, temp_fp) != 4)
        return failure;

    fseek(temp_fp, pos, SEEK_SET);
    return success;
}

Status edit_mp3_tag(const char* new_data, const char* edit_tag, FileInfo* file){
    FILE* temp_fp = fopen("temp.mp3", "wb");
    if(temp_fp==NULL){
        fprintf(stderr, "ERROR: Unable to open temporary file for writing\n");
        return failure;
    }

    // Read src file header size
    read_mp3_info_size(file);

    byte header[10];
    fseek(file->fp, 0, SEEK_SET);
    fread(header, sizeof(byte), 10, file->fp);
    fwrite(header, sizeof(byte), 10, temp_fp);

    while(ftell(file->fp) < file->tag_end){
        if(check_mp3_tag(edit_tag, file))
            break;
        fputc(fgetc(file->fp), temp_fp);
    }

    char tag_frame[4];
    fread(tag_frame, sizeof(char), 4, file->fp);
    fwrite(tag_frame, sizeof(char), 4, temp_fp);

    // Adjust offset position in source file to skip old frame data
    byte size_buffer[4];
    fread(size_buffer, sizeof(byte), 4, file->fp);
    int old_frame_size = convert_frame_size(size_buffer);

    int new_frame_size = strlen(new_data)+1;
    write_new_tag_size(temp_fp, new_frame_size); // +1 for the encoding byte

    char flags[3];
    fread(flags, sizeof(char), 3, file->fp);
    fwrite(flags, sizeof(char), 3, temp_fp);
    write_new_tag_data(temp_fp, new_data);
    
    // Skip old text
    fseek(file->fp, old_frame_size-1, SEEK_CUR);
    copy_remaining_data(temp_fp, file);

    file->tag_end += (new_frame_size - old_frame_size);
    update_tag_end(temp_fp, file->tag_end - 10);

    fclose(file->fp);
    fclose(temp_fp);

    remove(file->fname);
    if(rename("temp.mp3", file->fname) != 0){
        fprintf(stderr, "ERROR: Unable to rename temporary file\n");
        return failure;
    }
    return success;
}