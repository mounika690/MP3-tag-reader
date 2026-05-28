#include"mp3tag.h"


int main(int argc,char *argv[])
{
    if(argc==1)
    {
        printf(RED"ERROR :No arguments given!\n" RESET);
        printf(YELLOW "Usage:\n" RESET);
        printf(YELLOW "  To view please pass like ./a.out -v mp3filename\n" RESET);
        printf(YELLOW "  To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing _text mp3filename\n" RESET);
        printf(YELLOW "  To get help pass like: ./a.out --help\n" RESET);
        return e_failure;
    }

    OperationType ret=check_operation_type(argv);

    if(ret == e_view)
    {
        FILE *fp = fopen(argv[2],"rb");
        if(fp == NULL)
        {
            printf(RED "File not opened\n" RESET);
            return e_failure;
        }

        /*To check the the TAG->ID3*/
        char tag[4];
        fread(tag,3,1,fp);
        tag[3]='\0';

        //getting the version of id3
        unsigned char major_version;
        unsigned char revision;
        

        if(strcmp(tag,"ID3") == 0)
        {
            printf(GREEN "ID3V2 ID3 Found\n" RESET);

            printf(GREEN "-----------------SELECTED VIEW DETAILS----------\n" RESET);
            fread(&major_version,1,1,fp);
            fread(&revision,1,1,fp);
            mp3.id3_version = major_version;

            printf(RED "------------------------------------------------\n" RESET);
            printf(GREEN "Full Version   : ID3v2.%d.%d\n"RESET ,major_version,revision);
            if(major_version == 3)
            {
                printf("Detected ID3v2.3.0\n");
            }
            else if(major_version == 4)
            {
                printf("Detected ID3v2.4.0\n");
            }
            else
            {
                printf("Unknown ID3 version\n");
            }

        }
        else 
        {
            printf(RED "ID3v2 TAG NOT FOUND\n" RESET);
        }

      
        if((validate_file(argv,&mp3))!=e_failure)
        {
            printf(GREEN "Ready for VIEW/EDIT operations\n" RESET);  
        }

        do_view(argv,&mp3);

        fclose(mp3.fptr_src_audio);
                
       }
       else if(ret==e_edit)
       {
        printf(YELLOW "Edit mode \n" RESET);
        if(do_edit(argv,&mp3)==e_failure)
        {
            printf(RED "Edit failed!\n" RESET);
            return e_failure;
        }
       }

       return e_success;
    
        
}