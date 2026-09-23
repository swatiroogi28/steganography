#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

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
Status open_files_encoding(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
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
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)
{
    /*
    -> check argv[2] have ".bmp" as last 4 char
        if not,print error  msg,return e_faillure
       
         encInfo->src_image _fname=argv[2]
          encInfo->sercet_fname=argv[3]
    -> check argv[4] == NULL 
        *if null  output as "output.bmp" 
        encInfo->stego_image_fname="output.bmp"
        *else
            validate argv[4]is ".bmp"
            * if not ,print error msg,return e_failure
            * else,encInfo->stego_image_fname="output.bmp"

    -> call open_file(encInfo)== e_failure
        return e_failure
    return e_sucess
    */
    if(argc<4)
    {
        printf("Invalid Input\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For dencoding: ./a.out -d stego.bmp [decode.txt]\n");
        return e_failure;
    }
    int len=strlen(argv[2]);
    if(len<4||argv[2][len-4]!='.'|| argv[2][len-3]!='b'|| argv[2][len-2]!='m'|| argv[2][len-1]!='p')
    {
        printf("Error: source file should be .bmp\n");
        return e_failure;
    }
    encInfo->src_image_fname=argv[2];
    encInfo->secret_fname=argv[3];
    if(argv[4]==NULL)
    {
        encInfo->stego_image_fname="output.bmp";
    }
    else
    {
        len=strlen(argv[4]);
        if(len<4 ||argv[4][len-4]!='.'|| argv[4][len-3]!='b'|| argv[4][len-2]!='m'|| argv[4][len-1]!='p')
        {
            printf("Error: output file should be .bmp\n");
            return e_failure;
        }
        encInfo->stego_image_fname=argv[4];
    }
    if(open_files_encoding(encInfo)==e_failure)
    {
        printf("Error: unable to open file\n");
        return e_failure;
    }
    return e_success;


}
/*
Status open_files(EncodeInfo *encInfo)
{

    -> open source file in read mode
        encInfo-> src_image _fname 
        * if return value is NULL print the error and return e_failure
        *else  ,fptr_src_image=fopen()
    ->  open sercet file in read mode
        encInfo-> sercet_file 
        * if return value is NULL print the error and return e_failure
        *else  ,fptr_sercet=fopen()
    -> open source file in write mode
        encInfo-> stego_image _fname 
        fptr_stego_image=fopen()
    return e_success
    
    encInfo->fptr_src_image=fopen(encInfo->src_image_fname,"r");
    if(encInfo->fptr_src_image==NULL)
    {
        printf("Failed to open Source file\n");
        return e_failure;
    }
    encInfo->fptr_secret=fopen(encInfo->secret_fname,"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("Failed to open secret file\n");
        return e_failure;
    }
    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
    if(encInfo->fptr_stego_image==NULL)
    {
        printf("Failed to open output file\n");
        return e_failure;
    }
    return e_success;
}
*/
Status do_encoding(EncodeInfo *encInfo)
{

    /*
    // call check_capacity(encInfo)==e_failure
        print error msg ,return e_failure
    // copy_bmp_header(fptr_src_file,fptr_dest_file)==e_failure
        print error msg,return e_failure
    // call encode_magic_string(MAGIC_STRING,encInfo)==e_failure
        print error msg ,return e_failure
    // call encode_secret_file_extn_size(encInfo)==e_failure
         print error msg ,return e_failure
    // call encode_secret_file_extn(extern_secret_file,encInfo)==e_failure
         print error msg ,return e_failure
    // call encode_secret_file_size(size_secret_file,encInfo)==e_failure
        print error msg ,return e_failure
    // call encode_secret_file_data(encInfo)== e_failure
        print error msg ,return e_failure
    // call copy_remaining_img_data(fptr_scr_file,fptr_dest_file)== e_failure
        print error msg ,return e_failure
        
    return e_success
    */
    if(check_capacity(encInfo)==e_failure)
    {
        printf("Error: check capacity failed\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("Error:  copy bmp header failed\n");
        return e_failure;
    }
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
        printf("Error: Failed to encode magic string\n");
        return e_failure;
    }
    if(encode_secret_file_extn_size(encInfo)==e_failure)
    {
        printf("Error: Failed to encode secret file extension size\n");
        return e_failure;
    }
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_failure)
    {
        printf("Error: Failed to encode secret file extension\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure)
    {
        printf("Error: Failed to encode secret file size\n");
        return e_failure;
    }
    if(encode_secret_file_data(encInfo)==e_failure)
    {
        printf("Error: Failed to encode secret file data\n");
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("Error: Failed to copy the remaining data\n");
        return e_failure;
    }
    
}
Status check_capacity(EncodeInfo *encInfo)
{
    /*
    ->call get_image_size_for_bmp(encode->fptr_src_image)
        image_capacity =get_image_size()
    -> call get_file_size(encode-> fptr_sercet)
        size_secret_file=get_file_size()
    -> check ((14+ size_secret_file)*8) > image_capacity
        return e_failure
    return e_sucess
    */
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    if(((14 + encInfo->size_secret_file)*8) > encInfo->image_capacity)
    {
        printf("Insufficient Image capacity\n");
        return e_failure;
    }
    return e_success;
    
}
uint get_file_size(FILE *fptr)
{
    /*
    -> move the offset to last pos
    -> return ftell() 
    */
    fseek(fptr,0,SEEK_END);
    uint size = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return size;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
    -> move the file ptr to the seek_set , //also secret file
    -> declare the buffer of 54 bytes and then  char buff[54]
    -> read 54 bytes fron scr file
    -> write 54 bytes to dest file 
    -> validate the that copy is correctly done or not 
    -> return e_succes
    */
    char buff[54];
    fseek(fptr_src_image,0,SEEK_SET);
    fseek(fptr_dest_image,0,SEEK_SET);
    if(fread(buff,54,1,fptr_src_image)!=1)
    {
        return e_failure;
    }
    if(fwrite(buff,54,1,fptr_dest_image)!=1)
    {
        return e_failure;
    }
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        declare a buffer of 8 bytes
        char buff[8]
        for loop 2 times or length of magic string
        read 8 bytes from source file
        encode_byte_to_lsv(magic_string[1],buff)
        write the encoded buff to output_file
        ->return e_success
    */
    char buff[8];
    for(int i=0;i<strlen(magic_string);i++)
    {
        fread(buff,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        for(int i=7;i>=0;i--)
        {
         ->get the ith bit is set or not
            ==if set,set the lsb of image_buffer[not i make expression]
            == if clear,clear the lsb of image_buffer[]


        }
    */
    for(int i=7;i>=0;i--)
    {
        if((data>>i)&1)
        {
            image_buffer[7-i]=image_buffer[7-i]|1;
        }
        else
        {
            image_buffer[7-i]=image_buffer[7-i]& ~1;
        }
    }
    return e_success;
}
Status encode_secret_file_extn_size( EncodeInfo *encInfo)
{
    /*
        -> char *dot=strchr(secret_file_name,'.')
        -> strcpy(extern_secret_file,dot);

        -> declare a buff[32]
        -> read 32 byte from scr_file into buff
        -> call encode_size_to_lsb(strlen(extern_secret_file),buff)
        -> write the 32 bytes buffer to output_file
        return e_success;
    */
    char *dot=strchr(encInfo->secret_fname,'.');
    strcpy(encInfo->extn_secret_file,dot);

    char buff[32];
    fread(buff,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(strlen(encInfo->extn_secret_file),buff);
    fwrite(buff,32,1,encInfo->fptr_stego_image);
    return e_success;
}


Status encode_size_to_lsb( int size,char *Image_buff)
{
    /*
        for(int i=31;i>=0;i--)
        {
         ->get the ith bit is set or not
            ==if set,set the lsb of image_buffer[not i make expression]
            == if clear,clear the lsb of image_buffer[]

    // find the error part and return e_failure
        }
        return e_success;
    */
    for(int i=31;i>=0;i--)
    {
        if((size>>i)&1)
        {
            Image_buff[31-i]=Image_buff[31-i]|1;
        }
        else
        {
            Image_buff[31-i]=Image_buff[31-i]& ~1;
        }
    }
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        declare buff[8]
        -> read 8 byte from scr_image
        ->encode_byte_to_lsb(file_extn[],buff)
        -> write the 8 bytes of buff to out;put_file

        return e_success;

    */
    char buff[8];
    for(int i=0;i<strlen(encInfo->extn_secret_file);i++)
    {
        fread(buff,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(encInfo->extn_secret_file[i],buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        -> declare buff[32]

        -> read 32 byte from scr_image
        -> call encode_size_to_lsb(file_fice,buff)
        -> write the 32 bytes of buff to output_file
        return e_success
    */
    char buff[32];
    fread(buff,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buff);
    fwrite(buff,32,1,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        declare the buffer[8],data
    => loop  for  extension length till eof of secret_file
        -> read 8 bytes from scr_file
        -> read one byte from secret_file(char by char)
        -> call encode_byte_to_lsb(data,buff)
    return e_success
    */
    char buff[8];
    char data;
    while(fread(&data,1,1,encInfo->fptr_secret)==1)
    {
        fread(buff,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data,buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        -> declare a char as a data
    => loop till eof of scr_file
        -> read a char from source file 
        -> write the data to dest_file

    return e_success
    */
    char data;
    while(fread(&data,1,1,fptr_src)==1)
    {
       fwrite(&data,1,1,fptr_dest);
    }
    return e_success;
}