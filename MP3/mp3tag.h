
#ifndef MP3TAG_H
#define MP3TAG_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"    



#pragma pack(1)
typedef struct
{
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    unsigned char genre;

}MP3Tag;


/*to edit details*/
typedef enum {
    e_title,    
    e_artist,   
    e_album,    
    e_year,     
    e_comment, 
    e_unsupported_edit
} EditOption;



typedef struct _MP3
{
    /*MP3 INFO*/
    char *src_mp3_fname;
    FILE *fptr_src_audio;
    long int file_size;

    MP3Tag tag;
    int has_id3v2;
    int has_id3v1;
    int id3_version;

    
}MP3;

//check operation type
OperationType check_operation_type(char *argv[]);

//checking file is mp3 or not
Status validate_file(char *argv[2],MP3 *mp3);

//to view the details of audio file
void do_view(char *argv[],MP3 *mp3);


//to change the details
EditOption check_edit_option(char *argv[]);

//to know the what user selected for change
Status do_edit(char *argv[], MP3 *mp3);


extern MP3 mp3;

#endif
