#ifndef DECODE_H
#define DECODE_H
#include<stdio.h>
#include "types.h" 

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 6

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;        // pointer is fine, just reading argv
    FILE *fptr_src_image;

    /* Secret File Info */
    char secret_fname[50];        // changed to array for strcat
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    uint size_secret_file;

}DecodeInfo;

/* Prototypes */
Status read_and_validate_decode_args(int argc, OperationType* operation_info, DecodeInfo* decode_info, char *argv[]);
Status open_files_decode(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status open_output_file(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status decode_data_from_image(char *data, int size, FILE *fptr_stego);
char   decode_byte_from_lsb(char *image_buffer);
Status do_decoding(DecodeInfo *decInfo);

#endif