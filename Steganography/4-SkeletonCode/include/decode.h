#ifndef DECODE_H
#define DECODE_H

#include "common.h"

typedef struct{
    //File Names
    char stego_image_fname[MAX_FILE_NAME];
    char secret_fname[MAX_FILE_NAME];

    //File extensions
    char extn_stego_file[MAX_FILE_SUFFIX];
    char extn_secret_file[MAX_FILE_SUFFIX];

    //Open File pointers
    FILE *fptr_stego_image;
    FILE *fptr_secret;

    /* Source Image info */
    int size_secret_file;

} DecodeInfo;

/* Function prototypes */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char* argv[], DecodeInfo* decInfo);

/* Open required files */
Status open_files_decode(DecodeInfo* decInfo);

/* Decode data from stego image */
Status decode_data_from_image(char* bytes, int size, FILE* fptr_stego);

/* Decode a single byte from LSB */
char decode_byte_from_lsb(char* image_buffer);

/* Check if the magic string is present in the stego image */
Status check_magic_string(DecodeInfo* decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo* decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo* decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo* decInfo);

/* Perform the decoding process */
Status do_decoding(DecodeInfo* decInfo);

#endif