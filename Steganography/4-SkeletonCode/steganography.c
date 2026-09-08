#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    OperationType input = check_operation_type(argv);

    printf(I "INFO: Operation Type: %s\n" Rst, input == encode ? "Encode" : "Decode");

    if(input == encode){
        // do encode
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv, &encInfo) == success)
        {
            if(do_encoding(&encInfo) == success)
                printf(O "## Encoding Done Successfully ##\n" Rst);
            else
                fprintf(stderr,E "ERROR: Encoding of Secret File Failed!\n" Rst);
        }
    }
    else if(input == decode){
        // do decode
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv, &decInfo) == success)
        {
            if(do_decoding(&decInfo) == success)
                printf(O "## Decoding Done Successfully ##\n" Rst);
            else
                fprintf(stderr,E "ERROR: Decoding of Secret File Failed!\n" Rst);
        }   
    }
    else if(input == unsupported){
        fprintf(stderr,E "ERROR: Invalid operation type\n" Rst);
        return failure;
    }
}
