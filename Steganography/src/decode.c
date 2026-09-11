#include <stdio.h>
#include "decode.h"

// Function to read and validate decode arguments
Status read_and_validate_decode_args(char* argv[], DecodeInfo* decInfo){
    printf(O "## Decoding Procedure Started ##\n" Rst);
    // Get the source image file name and extn
    if(!get_extn(decInfo->extn_stego_file, argv[2])){
        fprintf(stderr,E "Error: Invalid Source file Name \"%s\"!\n" Rst, argv[2]);
        return failure;
    }
    strcpy(decInfo->stego_image_fname, argv[2]);

    // Get the secret file name and extn
    if(argv[3]!=NULL){
        strcpy(decInfo->extn_secret_file, strrchr(argv[3], '.'));
        strcpy(decInfo->secret_fname, argv[3]);
    }
    else{
        printf(I "INFO: Output filename not mentioned, Creating \"decoded.txt\" as default\n" Rst);
        strcpy(decInfo->secret_fname, "decoded.txt");
    }
    printf(S "INFO: Decode Arguments Validation Successful\n" Rst);
    return success;
}

// Function to decode data from image
Status open_stego_file(DecodeInfo* decInfo){
    // Stego Image file
    char* output_path = malloc(sizeof(char) * MAX_FILE_PATH);
    snprintf(output_path, MAX_FILE_PATH, "%s%s", base_path_output, decInfo->stego_image_fname);
    decInfo->fptr_stego_image = fopen(output_path, "rb");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL){
    	perror("fopen");
    	fprintf(stderr,E "ERROR: Unable To Open File %s\n" Rst, decInfo->stego_image_fname);
    	return failure;
    }
    free(output_path);
    printf(S "INFO: Opened Stego Image File: %s\n" Rst, decInfo->stego_image_fname);
    return success;
}

// Function to open the secret file for writing
Status open_secret_file(DecodeInfo* decInfo){
    // Secret file
    char* output_path = malloc(sizeof(char) * MAX_FILE_PATH);
    snprintf(output_path, MAX_FILE_PATH, "%s%s", base_path_output, decInfo->secret_fname);
    decInfo->fptr_secret = fopen(output_path, "w");
    // Do Error handling
    if (decInfo->fptr_secret == NULL){
    	perror("fopen");
    	fprintf(stderr,E "ERROR: Unable To Open File %s\n" Rst, output_path);
    	return failure;
    }
    free(output_path);
    printf(S "INFO: Opened Secret File: %s\n" Rst, decInfo->secret_fname);
    return success;
}


// Function to decode data from image
Status decode_data_from_image(char* bytes, int size, FILE* fptr_stego){
    char image_buffer[8];
    int iter = 0;
    while(iter<size){
        if(fread(image_buffer, sizeof(char), 8, fptr_stego) < 8){
            fprintf(stderr, E "ERROR: Reading Image Data failed\n" Rst);
            return failure;
        }
        bytes[iter++] = decode_byte_from_lsb(image_buffer);
    }
    return success;
}

// Function to decode a byte from the lsb of 8 bytes
char decode_byte_from_lsb(char* image_buffer){
    unsigned char byte = 0;
    for(int i=0; i<8; i++){
        byte |= ((image_buffer[i] & 1)<<i);
    }
    return byte;
}

// Function to check the magic string signature in the stego image
Status check_magic_string(DecodeInfo* decInfo){
    printf(I "INFO: Checking Magic String Signature\n" Rst);
    // Seek to 54th byte
    char magic[MAGIC_STRING_LEN + 1];
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    if(!decode_data_from_image(magic, MAGIC_STRING_LEN, decInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Decoding Magic String failed\n" Rst);
        return failure;
    }
    magic[MAGIC_STRING_LEN] = '\0';

    if(!strcmp(magic, MAGIC_STRING)){
        printf(S "INFO: Magic String Matched!\n" Rst);
        return success;
    }
    else{
        fprintf(stderr,E "ERROR: Magic String Doesn't Match!\n" Rst);
        return failure;
    }
}

// Function to decode the secret file extension from the stego image
Status decode_secret_file_extn(DecodeInfo* decInfo){
    // Decode the size of the secret file extension
    char extn_size;
    if(!decode_data_from_image(&extn_size, 1, decInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Decoding Secret File Extension Size failed\n" Rst);
        return failure;
    }
    printf(S "INFO: Decoded Secret File Extension Size: %d bytes\n" Rst, extn_size);
    char extn[extn_size+1];
    extn[extn_size] = '\0';
    // Decode the secret file extension
    if(!decode_data_from_image(extn, extn_size, decInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Decoding Secret File Extension failed\n" Rst);
        return failure;
    }
    printf(S "INFO: Decoded Secret File Extension: \"%s\"\n" Rst, extn);
    if(strcmp(extn, decInfo->extn_secret_file)){
        strcpy(decInfo->extn_secret_file, extn);
        char* dot = strchr(decInfo->secret_fname, '.');
        *dot = '\0';
        strcat(decInfo->secret_fname, decInfo->extn_secret_file);
    }
    open_secret_file(decInfo);
    return success;
}

// Function to decode the secret file size from the stego image
Status decode_secret_file_size(DecodeInfo* decInfo){
    char size[sizeof(int)];
    if(!decode_data_from_image(size, sizeof(int), decInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Decoding Secret File Size failed\n" Rst);
        return failure;
    }
    memcpy(&decInfo->size_secret_file, size, sizeof(int));
    printf(S "INFO: Decoded Secret Data Size: %d bytes\n" Rst, decInfo->size_secret_file);
    return success;
}

// Function to decode the secret file data from the stego image
Status decode_secret_file_data(DecodeInfo* decInfo){
    char data[decInfo->size_secret_file];
    if(!decode_data_from_image(data, decInfo->size_secret_file, decInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Decoding Secret File Data failed😟\n" Rst);
        return failure;
    }
    fwrite(data, sizeof(char), decInfo->size_secret_file, decInfo->fptr_secret);
    printf(S "INFO: Decoded Secret File Data Successfully🤩\n" Rst);
    return success;
}

// Function to perform the decoding process
Status do_decoding(DecodeInfo* decInfo){
    if(open_stego_file(decInfo) == failure)
        return failure;
    if(check_magic_string(decInfo) == failure)
        return failure;
    if(decode_secret_file_extn(decInfo) == failure)
        return failure;
    if(decode_secret_file_size(decInfo) == failure)
        return failure;
    if(decode_secret_file_data(decInfo) == failure)
        return failure;
    return success;
}