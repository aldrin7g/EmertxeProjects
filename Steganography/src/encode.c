#include <stdio.h>
#include "encode.h"
/* Function Definitions */

// Function to read and validate encode arguments
Status read_and_validate_encode_args(char* argv[], EncodeInfo* encInfo){
    printf(O "## Encoding Procedure Started ##\n" Rst);
    // Get the source image file name and extn
    if(!get_extn(encInfo->extn_src_file, argv[2])){
        fprintf(stderr,E "Error: Invalid Source file Name \"%s\"!\n" Rst, argv[2]);
        return failure;
    }
    strcpy(encInfo->src_image_fname, argv[2]);

    // Get the secret file name and extn
    if(!get_extn(encInfo->extn_secret_file, argv[3])){
        fprintf(stderr,E "Error: Invalid Secret file Name \"%s\"!\n" Rst, argv[3]);
        return failure;
    }
    strcpy(encInfo->secret_fname, argv[3]);

    // Get the output file name
    if(argv[4]!=NULL){
        if(!get_extn(encInfo->extn_stego_file, argv[4])){
            fprintf(stderr,E "Error: Invalid Output file Name \"%s\"!\n" Rst, argv[4]? argv[4] : "<no name>");
            return failure;
        }
        strcpy(encInfo->stego_image_fname, argv[4]);

        if(strcmp(encInfo->extn_src_file, encInfo->extn_stego_file)!=0){
            fprintf(stderr,E "Error: Output extension don't match source file extension\n" Rst);
            return failure;
        }
    }
    else{
        printf(I "INFO: Output filename not mentioned, Creating \"stego_img.bmp\" as default\n" Rst);
        strcpy(encInfo->stego_image_fname, "stego_img.bmp");
        strcpy(encInfo->extn_stego_file,".bmp");
    }
    printf(S "INFO: Encoding Arguments Validation Success\n" Rst);
    return success;
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files_encode(EncodeInfo* encInfo){
    printf(I "INFO: Opening required files\n" Rst);
    // Src Image file
    char* input_path = malloc(sizeof(char) * MAX_FILE_PATH);
    snprintf(input_path, MAX_FILE_PATH, "%s%s", base_path_input, encInfo->src_image_fname);
    encInfo->fptr_src_image = fopen(input_path, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL){
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable To Open File %s\n", encInfo->src_image_fname);
    	return failure;
    }
    free(input_path);
    printf(S "INFO: Opened Source Image File: %s\n" Rst, encInfo->src_image_fname);

    // Secret file
    input_path = malloc(sizeof(char) * MAX_FILE_PATH);
    snprintf(input_path, MAX_FILE_PATH, "%s%s", base_path_input, encInfo->secret_fname);
    encInfo->fptr_secret = fopen(input_path, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL){
    	perror("fopen");
    	fprintf(stderr, E "ERROR: Unable To Open File %s\n" Rst, encInfo->secret_fname);
    	return failure;
    }
    free(input_path);
    printf(S "INFO: Opened Secret File: %s\n" Rst, encInfo->secret_fname);

    // Stego Image file
    char* output_path = malloc(sizeof(char) * MAX_FILE_PATH);
    snprintf(output_path, MAX_FILE_PATH, "%s%s", base_path_output, encInfo->stego_image_fname);
    encInfo->fptr_stego_image = fopen(output_path, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL){
    	perror("fopen");
    	fprintf(stderr, E "ERROR: Unable To Open File %s\n" Rst, output_path);
    	return failure;
    }
    free(output_path);
    printf(S "INFO: Opened Stego Image File: %s\n" Rst, encInfo->stego_image_fname);
    return success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(EncodeInfo* encInfo){
    uint width, height;
    // Seek to 18th byte
    fseek(encInfo->fptr_src_image, 18, SEEK_SET);

    // Read the width (an int), 18-21
    fread(&width, sizeof(int), 1, encInfo->fptr_src_image);
    // printf("width = %u\n", width);

    // Read the height (an int), 22-25
    fread(&height, sizeof(int), 1, encInfo->fptr_src_image);
    // printf("height = %u\n", height);

    rewind(encInfo->fptr_src_image);
    // Return image capacity
    // i.e, (PixelCount) * 3, 1 pixel(RGB) is 3 bytes
    encInfo->image_capacity = width * height * 3;
    //printf(I "INFO: Successfully obtained Src image capacity: %u bytes\n" Rst, encInfo->image_capacity);
    return encInfo->image_capacity;
}

// Function to get the size of the secret file
Status get_file_size(EncodeInfo* encInfo){
    printf(I "INFO: Checking for %s size\n" Rst, encInfo->secret_fname);
    fseek(encInfo->fptr_secret,0,SEEK_END);
    encInfo->size_secret_file = ftell(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);
    if(encInfo->size_secret_file == 0){
        fprintf(stderr, E "ERROR: Secret file is empty\n" Rst);
        return failure;
    }
    printf(S "INFO: Done. Not Empty\n" Rst);
    return success;
}

// Function to check if the image has enough capacity to hold the secret data
Status check_capacity(EncodeInfo* encInfo){
    if(get_file_size(encInfo) == failure){
        return failure;
    }
    get_image_size_for_bmp(encInfo);

    char magic_string_size = strlen(MAGIC_STRING);
    char secret_extn_count = sizeof(char);
    char secret_extn_size = strlen(encInfo->extn_secret_file);
    char secret_data_count = sizeof(int); //Size of the secret file (No of characters in the secret file, stored as int)
    int secret_file_data = sizeof(char) * encInfo->size_secret_file; //(Size to store the secret file data in the image, stored as char array)

    printf(I "INFO: Checking for %s capacity to handle %s\n" Rst, encInfo->stego_image_fname, encInfo->secret_fname);
    if(encInfo->image_capacity < (uint)(magic_string_size + secret_extn_count + secret_extn_size + secret_data_count + secret_file_data)*8){
        fprintf(stderr, E "ERROR: Insufficient image capacity\n" Rst);
        return failure;
    }
    printf(S "INFO: Done. Found OK\n" Rst);
    return success;
}

// Function to copy the BMP header from source image to stego image
Status copy_bmp_header(EncodeInfo* encInfo){
    printf(I "INFO: Copying Image Header\n" Rst);
    char header[54];
    if(!fread(header, sizeof(char), 54, encInfo->fptr_src_image))
        return failure;
    if(!fwrite(header, sizeof(char), 54, encInfo->fptr_stego_image))
        return failure;
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to encode data into the image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    int iter = 0;
    while(iter < size){
        char image_buffer[8];
        if(fread(image_buffer, sizeof(char), 8 , fptr_src_image) < 8)
            return failure;
        encode_byte_to_lsb(data[iter], image_buffer);
        if(fwrite(image_buffer, sizeof(char), 8, fptr_stego_image) < 8)
            return failure;
        iter++;
    }
    return success;
}

// Function to encode a byte into the lsb of 8 bytes
void encode_byte_to_lsb(char data, char *image_buffer){
    for(int i=0; i<8; i++)
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data>>i) & 1);
}

// Function to encode the magic string into the image
Status encode_magic_string(EncodeInfo* encInfo){
    printf(I "INFO: Encoding Magic String Signature\n" Rst);
    // Seek to 0th byte
    fseek(encInfo->fptr_src_image, 54, SEEK_SET);
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
    if(!encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo->fptr_src_image, encInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Encoding Magic String failed\n" Rst);
        return failure;
    }
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to encode the size of the secret file extension into the image
Status encode_secret_extn_size(EncodeInfo* encInfo){
    printf(I "INFO: Encoding Size of \"%s\" File Extension\n" Rst, encInfo->extn_secret_file);
    char extn_size = strlen(encInfo->extn_secret_file);
    if(!encode_data_to_image(&extn_size, sizeof(char), encInfo->fptr_src_image, encInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Encoding Size Of Secret File Extension failed\n" Rst);
        return failure;
    }
    //printf(S "INFO: Done\n" Rst);
    return success;    
}

// Function to encode the size of the secret file into the image
Status encode_secret_file_extn(EncodeInfo* encInfo){
    printf(I "INFO: Encoding \"%s\" File Extension\n" Rst, encInfo->extn_secret_file);
    if(!encode_data_to_image(encInfo->extn_secret_file, strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Encoding Secret File Extension failed\n" Rst);
        return failure;
    }
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to encode the size of the secret file into the image
Status encode_secret_file_size(EncodeInfo* encInfo){
    printf(I "INFO: Encoding %s File Size\n" Rst, encInfo->secret_fname);
    char size[sizeof(int)]; memcpy(size, &encInfo->size_secret_file, sizeof(int));
    if(!encode_data_to_image(size, sizeof(int), encInfo->fptr_src_image, encInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Encoding Secret File Size failed\n" Rst);
        return failure;
    }
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to encode the secret file data into the image
Status encode_secret_file_data(EncodeInfo* encInfo){
    printf(I "INFO: Encoding %s File Data\n" Rst, encInfo->secret_fname);
    char *data = malloc(sizeof(char) * encInfo->size_secret_file);
    if(data==NULL){
        fprintf(stderr, E "ERROR: Memory Allocation failed\n" Rst);
        return failure;
    }
    if(fread(data, sizeof(char), encInfo->size_secret_file, encInfo->fptr_secret) < encInfo->size_secret_file){
        fprintf(stderr, E "ERROR: Reading Secret File Data failed\n" Rst);
        free(data);
        return failure;
    }
    if(!encode_data_to_image(data, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image)){
        fprintf(stderr, E "ERROR: Encoding Secret File Data failed\n" Rst);
        free(data);
        return failure;
    }
    free(data);
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to copy the remaining image data from source image to stego image
Status copy_remaining_img_data(EncodeInfo* encInfo){
    printf(I "INFO: Copying Remaining Image Data\n" Rst);
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, encInfo->fptr_src_image)) > 0){
        if(fwrite(buffer, 1, bytes_read, encInfo->fptr_stego_image) < bytes_read){
            fprintf(stderr, E "ERROR: Copying Remaining Image Data failed\n" Rst);
            return failure;
        }
    }

    if(ferror(encInfo->fptr_src_image)){
        fprintf(stderr, E "ERROR: Error reading remaining image data\n" Rst);
        return failure;
    }
    //printf(S "INFO: Done\n" Rst);
    return success;
}

// Function to perform the encoding process
Status do_encoding(EncodeInfo *encInfo){
    if(open_files_encode(encInfo) == failure)
        return failure;
    if(check_capacity(encInfo) == failure)
        return failure;
    if(copy_bmp_header(encInfo) == failure)
        return failure;
    if(encode_magic_string(encInfo) == failure)
        return failure;
    if(encode_secret_extn_size(encInfo) == failure)
        return failure;
    if(encode_secret_file_extn(encInfo) == failure)
        return failure;
    if(encode_secret_file_size(encInfo) == failure)
        return failure;
    if(encode_secret_file_data(encInfo) == failure)
        return failure;
    if(copy_remaining_img_data(encInfo) == failure)
        return failure;
    printf(S "INFO: Encoding Process Completed Successfully🤩\n" Rst);
    return success;
}