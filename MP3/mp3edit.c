#include "mp3tag.h"

Status do_edit(char *argv[], MP3 *mp3)
{
    
    EditOption opt = check_edit_option(argv);
    if(opt == e_unsupported_edit)
        return e_failure;

    if(argv[3] == NULL)
    {
        printf(RED "ERROR: No value given!\n" RESET);
        printf(YELLOW "Example: ./a.out -e -t \"New Title\" song.mp3\n" RESET);
        return e_failure;
    }

    
    if(argv[4] == NULL)
    {
        printf(RED "ERROR: No MP3 filename given!\n" RESET);
        printf(YELLOW "Example: ./a.out -e -t \"New Title\" song.mp3\n" RESET);
        return e_failure;
    }

    char *temp_argv[5] = {argv[0], argv[1], argv[4], NULL, NULL};
    if(validate_file(temp_argv, mp3) == e_failure)
    {
        return e_failure;
    }

    if(mp3->has_id3v1)
    {
        fseek(mp3->fptr_src_audio, -128, SEEK_END);
        fread(&mp3->tag, sizeof(MP3Tag), 1, mp3->fptr_src_audio);
    }
   
    if(!mp3->has_id3v1)
    {
        
        memset(&mp3->tag, 0, sizeof(MP3Tag));
        strncpy(mp3->tag.tag, "TAG", 3);
        printf(YELLOW "No ID3v1 tag — creating new one!\n" RESET);
    }

    switch(opt)
    {
        case e_title:
            memset(mp3->tag.title, 0, 30);
            strncpy(mp3->tag.title, argv[3], 29);
            printf(GREEN "Title updated   : %s\n" RESET, mp3->tag.title);
            break;

        case e_artist:
            memset(mp3->tag.artist, 0, 30);
            strncpy(mp3->tag.artist, argv[3], 29);
            printf(GREEN "Artist updated  : %s\n" RESET, mp3->tag.artist);
            break;

        case e_album:
            memset(mp3->tag.album, 0, 30);
            strncpy(mp3->tag.album, argv[3], 29);
            printf(GREEN "Album updated   : %s\n" RESET, mp3->tag.album);
            break;

        case e_year:
            memset(mp3->tag.year, 0, 4);
            strncpy(mp3->tag.year, argv[3], 4);
            printf(GREEN "Year updated    : %.4s\n" RESET, mp3->tag.year);
            break;

        case e_comment:
            memset(mp3->tag.comment, 0, 30);
            strncpy(mp3->tag.comment, argv[3], 29);
            printf(GREEN "Comment updated : %s\n" RESET, mp3->tag.comment);
            break;

        default:
            return e_failure;
    }

    strncpy(mp3->tag.tag, "TAG", 3);

    fclose(mp3->fptr_src_audio);
    mp3->fptr_src_audio = fopen(mp3->src_mp3_fname, "r+b");
    if(mp3->fptr_src_audio == NULL)
    {
        printf(RED "ERROR: Cannot open file for writing!\n" RESET);
        return e_failure;
    }

    if(mp3->has_id3v1)
    {
        fseek(mp3->fptr_src_audio, -128, SEEK_END);
    }
    else
    {
        fseek(mp3->fptr_src_audio, 0, SEEK_END);
    }

    fwrite(&mp3->tag, sizeof(MP3Tag), 1, mp3->fptr_src_audio);

    printf(GREEN "Tag saved to: %s\n" RESET, mp3->src_mp3_fname);

    fclose(mp3->fptr_src_audio);

    mp3->has_id3v2 =0;

    return e_success;

   

}