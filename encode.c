#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
Status read_and_validate_encode_args(int argc,OperationType* operation_info,EncodeInfo* encode_info,char *argv[])
{
    /*
    -here what we have to check is that its 
    the correct operation mode or not.
    -also we have to validate the number of 
    inputs which are given by the user
    */
    
    if(argc<4||argc>5)
    {
        printf("ERROR:Invalid inputs from the user \n");
        return e_failure;
    }
    
    if(strstr(argv[2],".bmp")!=NULL && strstr(argv[3],".txt")!=NULL)
    {
    //assign the values of the varibale for the 
    //perticular varibale in encodeinfo
    //assigning the src image to var of Encode_info
    //same as src img secrete txt and o/p bmp image
        encode_info->src_image_fname=argv[2];   //strcpy
        encode_info->secret_fname=argv[3];
    //if user is giving the output image file name 
        if(argc==5)
        {
            if(argv[4]!=NULL)
            {
                encode_info->stego_image_fname=argv[4];
            }
        }
    //if they are not giving the name of the ouput file then create
        else
        {
            encode_info->stego_image_fname="stego_img.bmp";
        }
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

//here we calculate the size of the secrete file
//we calculate the size by the offset position
//move end to offset and that position * bytes 

uint get_file_size(FILE* fptr_secrt)
{
    fseek(fptr_secrt,0,SEEK_END);
    uint size=ftell(fptr_secrt);
    rewind(fptr_secrt);
    return size;

}

//checking the capacity of the i/p image
//1bytes of secrete data consumes 8bytes of RGB data

Status check_capacity(EncodeInfo *encInfo)
{

    int required_size =
        (
        strlen(MAGIC_STRING) +
        sizeof(int) +
        strlen(encInfo->extn_secret_file) +
        sizeof(long) +
        encInfo->size_secret_file
        ) * 8;
    if((encInfo->image_capacity)>required_size)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

// copy the header information of the i/p image 
// copy to the newly created .bmp file which is going to be o/p of encoding
//the header information is 54bytes thats y im directly putting 54 bytes

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];

    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    fread(header, 54, 1, fptr_src_image);
    fwrite(header, 54, 1, fptr_dest_image);

    return e_success;
}



Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
         // Clear LSB of image byte, then put secret bit there
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}




Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[MAX_IMAGE_BUF_SIZE];  // 8 bytes

    for(int i = 0; i < size; i++)
    {
        // Step 1: Read 8 bytes from source image
        fread(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_src_image);

        // Step 2: Hide 1 secret byte into those 8 image bytes
        encode_byte_to_lsb(data[i], image_buffer);

        // Step 3: Write modified 8 bytes to stego image
        fwrite(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_stego_image);
    }
    return e_success;
}



Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image((char*)magic_string, strlen(magic_string),
        encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image((char*)file_extn, strlen(file_extn),
        encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

//encode the secrete file size
Status encode_secret_file_size(uint file_size, EncodeInfo *encInfo)
{
    if(encode_data_to_image((char*)&file_size, sizeof(uint),
        encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

//actual encoding of the secrete file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[MAX_SECRET_BUF_SIZE];  // 1 byte

    // Read 1 byte at a time from secret file
    while(fread(buffer, MAX_SECRET_BUF_SIZE, 1, encInfo->fptr_secret) > 0)
    {
        // Encode that 1 byte into 8 image bytes
        if(encode_data_to_image(buffer, MAX_SECRET_BUF_SIZE,
            encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
        {
            return e_failure;
        }
    }
    return e_success;
}


//copy the remaining bytes of the src image to stego image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}