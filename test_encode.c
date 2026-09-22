#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    /*
    if(check_operation_type(argv[1][1])==e_encode)
    -> call read_and_validate_encode_args(argv,&encInfo)==e_sucess
        ->call do_encoding(&encInfo)==e_success
            print "Encoding is success"

    */
    if(check_operation_type(argv[1][1])==e_encode)
    {
        if(read_and_validate_encode_args(argc,argv,&encInfo)==e_success)
        {
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding is success\n");
            }
        }
    }
    else if(check_operation_type(argv[1][1])==e_decode)
    {
        if(read_and_validate_decode_args(argc,argv,&decInfo)==e_success)
        {
            if(do_decoding(&decInfo)==e_success)
            {
                printf("Decoding is success\n");
            }
        }
    }
    return 0;
}

OperationType check_operation_type(char opt)
{
    if(opt=='e')
    {
        return e_encode;
    }
    else if(opt=='d')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

}
