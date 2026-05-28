#include "mp3tag.h"

static unsigned int decode_synchsafe(unsigned char *bytes)
{
    return ((unsigned int)bytes[0] << 21) |
           ((unsigned int)bytes[1] << 14) |
           ((unsigned int)bytes[2] << 7 ) |
           ((unsigned int)bytes[3]);
}

void do_view(char *argv[], MP3 *mp3)
{
    FILE *fp = mp3->fptr_src_audio;

    printf(BLUE "\n=== AUDIO DETAILS ===\n" RESET);

    /* ---------- ID3v2 ---------- */
    if(mp3->has_id3v2)
    {
        unsigned char size_bytes[4];

        fseek(fp, 6, SEEK_SET);
        fread(size_bytes, 1, 4, fp);

        unsigned int tag_size = decode_synchsafe(size_bytes);

      
        fseek(fp, 10, SEEK_SET);

        while(ftell(fp) < (long)(10 + tag_size))
        {
            char frame_id[5] = {0};
            unsigned char fsize[4] = {0};
            unsigned char fflags[2];

            fread(frame_id, 1, 4, fp);
            fread(fsize, 1, 4, fp);
            fread(fflags, 1, 2, fp);

            
            if(frame_id[0] == 0)
                break;

            unsigned int frames_size;

            /* ---------- ID3v2.3 ---------- */
            if(mp3->id3_version == 3)
            {
                frames_size =
                    ((unsigned int)fsize[0] << 24) |
                    ((unsigned int)fsize[1] << 16) |
                    ((unsigned int)fsize[2] << 8 ) |
                    ((unsigned int)fsize[3]);
            }

            /* ---------- ID3v2.4 ---------- */
            else if(mp3->id3_version == 4)
            {
                frames_size =
                    ((unsigned int)fsize[0] << 21) |
                    ((unsigned int)fsize[1] << 14) |
                    ((unsigned int)fsize[2] << 7 ) |
                    ((unsigned int)fsize[3]);
            }
            else
            {
                printf(RED "Unsupported ID3 version\n" RESET);
                return;
            }

            if(frames_size == 0 || frames_size > 100000)
                break;

            char *data = malloc(frames_size + 1);

            if(data == NULL)
            {
                printf(RED "Memory allocation failed\n" RESET);
                return;
            }

            fread(data, 1, frames_size, fp);
            data[frames_size] = '\0';

            char text[1024] = {0};


            if(frame_id[0] == 'T' || strcmp(frame_id, "COMM") == 0)
            {
                unsigned char encoding = data[0];
                if(encoding == 0x00 || encoding == 0x03)
                {
                    strncpy(text, data + 1, sizeof(text) - 1);
                }
                else if(encoding == 0x01 || encoding == 0x02)
                {
                    int j = 0;
                    for(int i = 3; i < frames_size && j < 1023; i += 2)
                    {
                        text[j++] = data[i];
                    }
                    text[j] = '\0';
                }
                else
                {
                    strcpy(text, "Unsupported Encoding");
                }
            }


            if(strcmp(frame_id, "TIT2") == 0)
            {
                printf(GREEN "Title    : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TPE1") == 0)
            {
                printf(GREEN "Artist   : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TALB") == 0)
            {
                printf(GREEN "Album    : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TYER") == 0)
            {
                printf(YELLOW "Year     : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TDRC") == 0)
            {
                printf(YELLOW "Date     : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TRCK") == 0)
            {
                printf(YELLOW "Track    : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TCON") == 0)
            {
                printf(YELLOW "Genre    : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TCOP") == 0)
            {
                printf(YELLOW "Copyright: %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TCOM") == 0)
            {
                printf(BLUE "Composer : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "COMM") == 0)
            {
                printf(BLUE "Comment  : %s\n" RESET, text);
            }

            else if(strcmp(frame_id, "TLEN") == 0)
            {
                printf(BLUE "Length   : %s ms\n" RESET, text);
            }

            else if(strcmp(frame_id, "TBPM") == 0)
            {
                printf(BLUE "BPM      : %s\n" RESET, text);
            }

            else
            {
                printf(BLUE "%-8s : (size=%u)\n" RESET,
                       frame_id, frames_size);
            }

            free(data);
        }
    }

    /* ---------- ID3v1 ---------- */
    if(mp3->has_id3v1)
    {
        printf(GREEN "\n=== ID3v1 Tag ===\n" RESET);

        printf(GREEN  "Title   : %.30s\n" RESET, mp3->tag.title);
        printf(GREEN  "Artist  : %.30s\n" RESET, mp3->tag.artist);
        printf(GREEN  "Album   : %.30s\n" RESET, mp3->tag.album);
        printf(YELLOW "Year    : %.4s\n" RESET, mp3->tag.year);
        printf(BLUE   "Comment : %.30s\n" RESET, mp3->tag.comment);
        printf(YELLOW "Genre   : %d\n" RESET, mp3->tag.genre);
    }

    else
    {
        printf(YELLOW "\nNo ID3v1 tag in this file.\n" RESET);
    }

    printf(RED "-------------------------------------------\n" RESET);
    printf(GREEN "=== DETAILS DISPLAYED SUCCESSFULLY ===\n" RESET);
}