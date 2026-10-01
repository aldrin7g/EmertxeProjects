#include "help.h"
#include "common.h"

void display_help(){
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, Y, "                     MP3 TAG READER V2.3                   ", O, RST);
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, M, "  USAGE", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, B, "  -v <file.mp3>                             View MP3 tags ", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, B, "  -e <file.mp3> <tag> <new_value>           Edit MP3 tag  ", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, B, "  -h                                        Display help  ", O, RST);
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, M, "  AVAILABLE TAGS", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, B, "  -y  Year            -t  Title            -A  Artist", O, RST);
    printf("%s        |%s%-60s%s|%s\n", O, B, "  -a  Album           -g  Genre            -c  Comment", O, RST);
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
}