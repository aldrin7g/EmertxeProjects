#include "help.h"

void display_help(){
    printf("--------------------------------\n");
    printf("        MP3 TAG READER\n");
    printf("--------------------------------\n");
    printf("Usage:\n");
    printf("  mp3tag -v <file.mp3>                      View the tags of the specified mp3 file\n");
    printf("  mp3tag -e <file.mp3> <tag> <new_value>    Edit the specified tag of the mp3 file\n");
    printf("  mp3tag -h                                 Display this help message\n");
    printf("\n");
    printf("Tags:\n");
    printf("  -y   Year\n");
    printf("  -t   Title\n");
    printf("  -A   Artist\n");
    printf("  -a   Album\n");
    printf("  -g   Genre\n");
    printf("  -c   Comment\n");
}