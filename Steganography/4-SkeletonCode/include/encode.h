#ifndef ENCODE_H
#define ENCODE_H

#include "common.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct{
    //File Names
    char src_image_fname[MAX_FILE_NAME];
    char secret_fname[MAX_FILE_NAME];
    char stego_image_fname[MAX_FILE_NAME];

    //File extensions
    char extn_src_file[MAX_FILE_SUFFIX];
    char extn_secret_file[MAX_FILE_SUFFIX];
    char extn_stego_file[MAX_FILE_SUFFIX];

    //Open File pointers
    FILE *fptr_src_image;
    FILE *fptr_secret;
    FILE *fptr_stego_image;

    /* Source Image info */
    uint image_capacity;

    int size_secret_file;

} EncodeInfo;


/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_encode(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(EncodeInfo *encInfo);

/* Get file size */
Status get_file_size(EncodeInfo *encInfo);

/* Copy bmp image header */
Status copy_bmp_header(EncodeInfo *EncodeInfo);

/* Store Magic String */
Status encode_magic_string(EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
void encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(EncodeInfo* EncodeInfo);

#endif