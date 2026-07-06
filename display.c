#include <stdio.h>
#include <string.h>
#include "decode_1.h"
#include "types.h"
#include "common.h"

int out(DecodeInfo* decode_info)
{
printf("\t\t:::ENCODED DATA IS:::\n");
        char word[100];

        FILE* fp = fopen(decode_info->secret_fname,"r");
        if(fp==NULL)
        {
            return 0;
        }

        int ch;
        while((ch=fgetc(fp))!=EOF)
        {
            putchar(ch);
        }

        fclose(fp);
}