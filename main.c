#include<stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode_1.h"
int main(int argc,char* argv[])
{
    EncodeInfo encodeinfo;
    OperationType operation_info;
    DecodeInfo decodeinfo;

    operation_info=check_operation_type(argv);

    //if the check operation function return the e_encode
    //then do encoding

    if(operation_info==e_encode)
    {

        if(read_and_validate_encode_args(argc,&operation_info,&encodeinfo,argv)==e_failure)
        {
            printf("ERROR: Reading and Validation of the Arguments FAILED\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Reading and Validation of the Argumnets SUCCESSED\n");
        }


        if(open_files(&encodeinfo)==e_failure)
        {
            printf("ERROR: %s function failed\n", "open_files" );
            return 0;
        }
        else
        {
            printf("SUCCESS: %s function completed\n", "open_files" );
        }

        encodeinfo.image_capacity=get_image_size_for_bmp(encodeinfo.fptr_src_image);
        encodeinfo.size_secret_file=get_file_size(encodeinfo.fptr_secret);
        //here the capacity is checked (src_image_size>all_secrete_data)

        if(check_capacity(&encodeinfo)==e_success)
        {
            printf("%s have the capacity to encode %s\n",encodeinfo.src_image_fname,encodeinfo.secret_fname);
        }
        else
        {
            printf("%s doesn't have the capacity to encode %s\n",encodeinfo.src_image_fname,encodeinfo.secret_fname);
            return 0;
        }

        //this function copies the image header exactly same to the stego img

        if(copy_bmp_header(encodeinfo.fptr_src_image,encodeinfo.fptr_stego_image) == e_failure)
        {
            printf("ERROR: copy_bmp_header failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Header copied\n");
        }


        // Encode magic string
        if(encode_magic_string(MAGIC_STRING, &encodeinfo) == e_failure)
        {
            printf("ERROR: encode_magic_string failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Magic string encoded\n");
        }


        // Get extension from secret file name and encode it
        strcpy(encodeinfo.extn_secret_file, strchr(encodeinfo.secret_fname, '.'));
        if(encode_secret_file_extn(encodeinfo.extn_secret_file, &encodeinfo) == e_failure)
        {
            printf("ERROR: encode_secret_file_extn failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Extension encoded\n");
        }



        
        // Encode secret file size
        if(encode_secret_file_size(encodeinfo.size_secret_file, &encodeinfo) == e_failure)
        {
            printf("ERROR: encode_secret_file_size failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: File size encoded\n");
        }


        // Encode secret file data
        if(encode_secret_file_data(&encodeinfo) == e_failure)
        {
            printf("ERROR: encode_secret_file_data failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Secret data encoded\n");
        }


        // Copy remaining image data
        if(copy_remaining_img_data(encodeinfo.fptr_src_image, encodeinfo.fptr_stego_image) == e_failure)
        {
            printf("ERROR: copy_remaining_img_data failed\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Encoding COMPLETE! Output saved to -> %s\n", encodeinfo.stego_image_fname);
        }

    }
    //or do decoding

else if(operation_info==e_decode)
{
        //read and validate the inputs from the CLA(-d)  
        if(read_and_validate_decode_args(argc, &operation_info, &decodeinfo, argv) == e_failure)
        {
            printf("ERROR: Reading and Validation FAILED\n");
            return 0;
        }
        else
        {
            printf("SUCCESS: Arguments validated\n");
        } 

        //open the only one file which is src_img encoded img
        if(open_files_decode(&decodeinfo) == e_failure)
        {
            printf("ERROR: open_files_decode failed\n");
            return 0;
        }
        else 
        {
            printf("SUCCESS: File opened\n");
        }

        //all the encoding the process inside this fucntion
        if(do_decoding(&decodeinfo) == e_failure)      
        {
            printf("ERROR: Decoding FAILED\n");
            return 0;
        }
}

//print a error msg if that i/p operation mode is not available
else if(operation_info==e_unsupported)
{
    printf("ERROR:Invalid operation mode\n");
    return 0;
}

       
}