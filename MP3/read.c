#include "mp3tag.h"

MP3 mp3;

OperationType check_operation_type(char *argv[])
{
    if(argv[1] == NULL)
    {
        printf(RED "ERROR: No operation given. Use -v (view) or -e (edit)\n" RESET);
        return e_unsupported;
    }
    else if(strcmp(argv[1], "--help") == 0)
    {
        printf(BLUE "------------------HELP MENU--------------\n" RESET);
        printf(BLUE "1. -v  → view mp3 tag contents\n" RESET);
        printf(BLUE "2. -e  → edit mp3 tag contents\n" RESET);
        printf(BLUE "   -t  → edit title\n" RESET);
        printf(BLUE "   -a  → edit artist\n" RESET);
        printf(BLUE "   -A  → edit album\n" RESET);
        printf(BLUE "   -y  → edit year\n" RESET);
        printf(BLUE "   -c  → edit comment\n" RESET);
        printf(YELLOW "-----------------------------------------\n" RESET);
        return e_unsupported;
    }
    else if(strcmp(argv[1], "-v") == 0) return e_view;
    else if(strcmp(argv[1], "-e") == 0) return e_edit;

    printf(RED "ERROR: Unknown operation '%s'\n" RESET, argv[1]);
    return e_unsupported;
}

Status validate_file(char *argv[], MP3 *mp3)
{
    if(argv[2] == NULL)
    {
        printf(RED "ERROR: No MP3 file given\n" RESET);
        printf(YELLOW "Usage: ./a.out -v <song.mp3>\n" RESET);
        return e_failure;
    }

    if(strstr(argv[2], ".mp3") == NULL)
    {
        printf(RED "ERROR: File must be .mp3\n" RESET);
        return e_failure;
    }

    mp3->src_mp3_fname = argv[2];

    mp3->fptr_src_audio = fopen(mp3->src_mp3_fname, "rb");
    if(mp3->fptr_src_audio == NULL)
    {
        printf(RED "ERROR: Cannot open '%s'\n" RESET, mp3->src_mp3_fname);
        return e_failure;
    }

    fseek(mp3->fptr_src_audio, 0, SEEK_END);
    mp3->file_size = ftell(mp3->fptr_src_audio);
    rewind(mp3->fptr_src_audio);

    printf(GREEN "Audio file : %s\n" RESET, mp3->src_mp3_fname);
    printf(GREEN "File size  : %ld bytes\n" RESET, mp3->file_size);

    /* Check ID3v2 at start */
    char identifier[4] = {0};
    fread(identifier, 1, 3, mp3->fptr_src_audio);
    mp3->has_id3v2 = (strcmp(identifier, "ID3") == 0) ? 1 : 0;

    if(mp3->has_id3v2)
        printf(GREEN "ID3v2 tag found\n" RESET);
    else
        printf(YELLOW "No ID3v2 tag\n" RESET);

    /* Check ID3v1 at end — store in mp3->tag */
    fseek(mp3->fptr_src_audio, -128, SEEK_END);
    fread(&mp3->tag, sizeof(MP3Tag), 1, mp3->fptr_src_audio);

    /* BUG FIX 5: has_id3v1 correct name */
    mp3->has_id3v1 = (strncmp(mp3->tag.tag, "TAG", 3) == 0) ? 1 : 0;

    if(mp3->has_id3v1)
        printf(GREEN "ID3v1 tag found\n" RESET);
    else
        printf(YELLOW "No ID3v1 tag\n" RESET);

    return e_success;
}

EditOption check_edit_option(char *argv[])
{
    if(argv[2] == NULL)
    {
        printf(RED "ERROR: No edit option given\n" RESET);
        printf(YELLOW "Options: -t(title) -a(artist) -A(album) -y(year) -c(comment)\n" RESET);
        return e_unsupported_edit;
    }
    else if(strcmp(argv[2], "-t") == 0)
    {
        printf(YELLOW "You chose: Title\n" RESET);
        return e_title;
    }
    else if(strcmp(argv[2], "-a") == 0)
    {
        printf(YELLOW "You chose: Artist\n" RESET);
        return e_artist;
    }
    else if(strcmp(argv[2], "-A") == 0)
    {
        printf(YELLOW "You chose: Album\n" RESET);
        return e_album;
    }
    else if(strcmp(argv[2], "-y") == 0)
    {
        printf(YELLOW "You chose: Year\n" RESET);
        return e_year;
    }
    else if(strcmp(argv[2], "-c") == 0)
    {
        printf(YELLOW "You chose: Comment\n" RESET);
        return e_comment;
    }

    printf(RED "ERROR: Unknown edit option '%s'\n" RESET, argv[2]);
    return e_unsupported_edit;
}