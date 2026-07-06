#include <stdio.h>
#include <string.h>
#include "decode_1.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(int argc, OperationType* operation_info, DecodeInfo* decode_info, char *argv[])
{
    if(argc < 3 || argc > 4)
    {
        printf("ERROR: Invalid inputs from the user\n");
        return e_failure;
    }

    if(strstr(argv[2], ".bmp") != NULL)
    {
        decode_info->src_image_fname = argv[2];

        if(argc == 4)
            strcpy(decode_info->secret_fname, argv[3]);  // strcpy into array
        else
            strcpy(decode_info->secret_fname, "output"); // default base name

        return e_success;
    }
    else
    {
        return e_failure;
    }
    
}

Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");
    if(decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);
        return e_failure;
    }
    else
    {
        return e_success;
    }
    
}

// THE CORE: reads 8 image bytes, extracts LSB from each → 1 secret byte
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for(int i = 0; i < 8; i++)
    {
        data = (data << 1) | (image_buffer[i] & 1); // collect LSB of each byte
    }
    return data;
}

// Calls decode_byte_from_lsb for 'size' number of bytes
Status decode_data_from_image(char *data, int size, FILE *fptr_stego)
{
    char image_buffer[MAX_IMAGE_BUF_SIZE]; // 8 bytes

    for(int i = 0; i < size; i++)
    {
        fread(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_stego);
        data[i] = decode_byte_from_lsb(image_buffer);
    }

    return e_success;
}

// Step 1: decode and verify magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic[3]; // "#*" + null terminator

    //decode the magic string and put into the magic[]
    decode_data_from_image(magic, strlen(MAGIC_STRING), decInfo->fptr_src_image);
    magic[strlen(MAGIC_STRING)] = '\0';

    //check if that extracted magic[] is matches the MAGIC_STRING or not
    if(strcmp(magic, MAGIC_STRING) != 0)
    {
        printf("ERROR: Magic string mismatch. Image has no hidden data\n");
        return e_failure;
    }
    printf("SUCCESS: Magic string matched -> %s\n", magic);
    return e_success;
}

// Step 2: decode extension e.g. ".txt"
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    // encoding stored strlen(extn) bytes, extn was ".txt" = 4 chars
    decode_data_from_image(decInfo->extn_secret_file,4,decInfo->fptr_src_image);
    decInfo->extn_secret_file[4] = '\0';

    printf("SUCCESS: Extension decoded -> %s\n", decInfo->extn_secret_file);

    // now build full filename: "output" + ".txt" = "output.txt"
    if (strstr(decInfo->secret_fname, ".") == NULL)
    {
        strcat(decInfo->secret_fname, decInfo->extn_secret_file);
    }
    printf("SUCCESS: Output file will be -> %s\n", decInfo->secret_fname);

    return e_success;
}

// Step 3: open the output file AFTER we know the extension
Status open_output_file(DecodeInfo *decInfo)
{
    //open the output file with the extn .txt with write binary mode(wb)
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
    if(decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to create file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    printf("SUCCESS: Output file opened -> %s\n", decInfo->secret_fname);
    return e_success;
}

// Step 4: decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{

    decode_data_from_image((char*)&decInfo->size_secret_file, sizeof(uint), decInfo->fptr_src_image);
    printf("SUCCESS: Secret file size decoded -> %u bytes\n", decInfo->size_secret_file);
    return e_success;
}

// Step 5: decode the actual secret data and write to output file
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[MAX_SECRET_BUF_SIZE]; // 1 byte at a time

    for(uint i = 0; i < decInfo->size_secret_file; i++)
    {
        decode_data_from_image(buffer, MAX_SECRET_BUF_SIZE, decInfo->fptr_src_image);
        fwrite(buffer, MAX_SECRET_BUF_SIZE, 1, decInfo->fptr_secret);
    }
    printf("SUCCESS: Secret data decoded and written\n");
    return e_success;
}

// Master function — calls all steps in order
Status do_decoding(DecodeInfo *decInfo)
{
    // skip 54-byte BMP header
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    //decode the magic string
    if(decode_magic_string(decInfo) == e_failure)
        return e_failure;

    
    //decode the secrete file extn
    if(decode_secret_file_extn(decInfo) == e_failure)
    return e_failure;

    //open the output files
    if(open_output_file(decInfo) == e_failure)       
    return e_failure;
    
    //decode the secrete file size
    if(decode_secret_file_size(decInfo) == e_failure) 
    return e_failure; 

    //decode the secrete file data
    if(decode_secret_file_data(decInfo) == e_failure)  
    return e_failure;

    fclose(decInfo->fptr_secret);

    printf("SUCCESS: Decoding COMPLETE! Output saved to -> %s\n", decInfo->secret_fname);
    return e_success;
}
