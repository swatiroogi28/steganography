#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"


Status open_files_decoding(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
    if(argc<3)
    {
        printf("Invalid Input\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For dencoding: ./a.out -d stego.bmp [decode.txt]\n");
        return e_failure;
    }
    int len=strlen(argv[2]);
    if(len<4||argv[2][len-4]!='.'|| argv[2][len-3]!='b'|| argv[2][len-2]!='m'|| argv[2][len-1]!='p')
    {
        printf("Error: output file should be .bmp\n");
        return e_failure;
    }
    decInfo->src_image_fname=argv[2];
    if(argc==3)
    {
        decInfo->secret_fname="decode";
    }
    else
    {
        decInfo->secret_fname=argv[3];
    }
    if(open_files_decoding(decInfo)==e_failure)
    {
        printf("Error: unable to open file\n");
        return e_failure;
    }
    return e_success;

}
Status do_decoding(DecodeInfo *decInfo)
{
    if(decode_magic_string(MAGIC_STRING,decInfo)==e_failure)
    {
        printf("Error: Failed to decode magic string\n");
        return e_failure;
    }
    if(decode_secret_file_extn_size(decInfo)==e_failure)
    {
        printf("Error: Failed to decode secret file extension size\n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo->extn_size,decInfo)==e_failure)
    {
        printf("Error: Failed to decode secret file extension\n");
        return e_failure;
    }
    if(decode_secret_file_size(decInfo)==e_failure)
    {
        printf("Error: Failed to decode secret file size\n");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo)==e_failure)
    {
        printf("Error: Failed to decode secret data\n");
        return e_failure;
    }
    return e_success;

}
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    //move to the 54 byte
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    //declare a buff and cahr data=0;
    char buff[8];
    char data;
    for(int i=0;i<strlen(magic_string);i++)
    {
        data=0;
        fread(buff,8,1,decInfo->fptr_src_image);
        decode_byte_from_lsb(&data,buff);
        if(data!=magic_string[i])
        {
            printf("Magic String is not same\n");
            return e_failure;
        }
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    for(int i=7 ;i>=0;i--)
    {
        if(image_buffer[7-i]&1)
        {
            *data=*data|(1<<i);
        }
    }
    return e_success;
}
Status decode_secret_file_extn_size( DecodeInfo *decInfo)
{
    char buff[32];
    fread(buff,32,1,decInfo->fptr_src_image);
    decode_size_from_lsb(&decInfo->extn_size,buff);
    return e_success;
}
Status decode_size_from_lsb(long *size,char *Image_buff)
{
    *size=0;
    for(int i=31;i>=0;i--)
    {
        if(Image_buff[31-i]&1)
        {
            *size=*size|(1<<i);
        }
    }
    return e_success;
}
Status decode_secret_file_extn(long file_extn, DecodeInfo *decInfo)
{
    char buff[8];
    char data;
    for(int i=0;i<file_extn;i++)
    {
        fread(buff,8,1,decInfo->fptr_src_image);
        data=0;
        decode_byte_from_lsb(&data,buff);
        decInfo->extn_secret_file[i]=data;
    }
    decInfo->extn_secret_file[file_extn]='\0';
    return e_success;
}
Status decode_secret_file_size( DecodeInfo *decInfo)
{
    char buff[32];
    fread(buff,32,1,decInfo->fptr_src_image);
    decode_size_from_lsb(&decInfo->size_secret_file,buff);
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char filename[50];
    strcpy(filename,decInfo->secret_fname);
    strcat(filename,decInfo->extn_secret_file);
    decInfo->fptr_secret = fopen(filename, "w");
    if(decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        return e_failure;
    }
    char buff[8];
    char data;
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        fread(buff,8,1,decInfo->fptr_src_image);
        data=0;
        decode_byte_from_lsb(&data,buff);
        fwrite(&data,1,1,decInfo->fptr_secret);
    }
    return e_success;
}