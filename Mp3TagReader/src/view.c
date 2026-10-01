#include "view.h"

Status read_mp3_info_size(FileInfo* file){
    // Read Tag Size
    byte size[4];
    fseek(file->fp, 6, SEEK_SET);
    if(fread(size, sizeof(byte), 4, file->fp) != 4){
        fprintf(stderr, E"ERROR: Failed to read tag size\n"RST);
        return failure;
    }
    int tag_size =
          ((size[0] & 0x7F) << 21)
        | ((size[1] & 0x7F) << 14)
        | ((size[2] & 0x7F) << 7)
        | (size[3] & 0x7F);
    file->tag_end = 10 + tag_size;
    return success;
}

int read_frame_size(FileInfo* file){
    // Read Frame Size
    byte size_buffer[4];
    if (fread(size_buffer, sizeof(byte), 4, file->fp) != 4)
        return -1;
    int frame_size = convert_frame_size(size_buffer);
    // Skip 2 bytes of flags & 1 Encoding byte
    fseek(file->fp, 3, SEEK_CUR);
    return frame_size-1; // Subtract 1 for the encoding byte
}

uint convert_frame_size(byte* buffer){
    // Convert 4-byte big-endian frame size to integer
    uint size;
    size = (buffer[0] << 24) |
           (buffer[1] << 16) |
           (buffer[2] << 8)  |
           buffer[3];
    return size;
}

Status skip_unknown_frame(char* tag, uint frame_size, FileInfo* file){
    if (strcmp(tag, "TIT2") != 0 &&
        strcmp(tag, "TPE1") != 0 &&
        strcmp(tag, "TALB") != 0 &&
        strcmp(tag, "TYER") != 0 &&
        strcmp(tag, "TCON") != 0 &&
        strcmp(tag, "COMM") != 0)
    {
        fseek(file->fp, frame_size, SEEK_CUR);
        return success;
    }
    return failure;
}

Status read_mp3_frame(Mp3Tag* tags, FileInfo* file)
{
    char tag[5];
    if (fread(tag, 1, 4, file->fp) != 4){
        fprintf(stderr, E"ERROR: Failed to read frame tag\n"RST);
        return failure;
    }
    tag[4] = '\0';

    uint frame_size = read_frame_size(file);

    if(skip_unknown_frame(tag, frame_size, file))
        return success;

    char *data = malloc(frame_size + 1);
    if (fread(data, sizeof(byte), frame_size, file->fp) != frame_size){
        free(data);
        fprintf(stderr, E"ERROR: Failed to read frame data for tag %s\n"RST, tag);
        return failure;
    }
    data[frame_size] = '\0';
    if (strcmp(tag, "TIT2") == 0)
        tags->title = data;
    else if (strcmp(tag, "TPE1") == 0)
        tags->artist = data;
    else if (strcmp(tag, "TALB") == 0)
        tags->album = data;
    else if (strcmp(tag, "TYER") == 0)
        tags->year = data;
    else if (strcmp(tag, "TCON") == 0)
        tags->genre = data;
    else if (strcmp(tag, "COMM") == 0)
        tags->comment = data;
    return success;
}

Status read_mp3_tags(Mp3Tag* tags, FileInfo* file){
    // Read Tag Size
    read_mp3_info_size(file);

    // Skip the 10 bytes of Header
    fseek(file->fp, 10, SEEK_SET);

    while(ftell(file->fp) < file->tag_end) {
        if(!read_mp3_frame(tags, file)){
            fprintf(stderr, E"ERROR: Failed to read MP3 frame\n"RST);
            return failure;
        }
    }
    fclose(file->fp);
    return success;
}

Status display_mp3_tags(Mp3Tag *tags)
{
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    printf("%s        |                     %sMP3 TAG READER V2.3%s                    |%s\n", O, Y, O, RST);
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    printf("%s        |%s  Title   :%s %-47s %s|%s\n", O, G, W, tags->title, O, RST);
    printf("%s        |%s  Artist  :%s %-47s %s|%s\n", O, G, W, tags->artist, O, RST);
    printf("%s        |%s  Album   :%s %-47s %s|%s\n", O, G, W, tags->album, O, RST);
    printf("%s        |%s  Year    :%s %-47s %s|%s\n", O, G, W, tags->year, O, RST);
    printf("%s        |%s  Genre   :%s %-47s %s|%s\n", O, G, W, tags->genre, O, RST);
    printf("%s        |%s  Comment :%s %-47s %s|%s\n", O, G, W, tags->comment, O, RST);
    printf("%s        +------------------------------------------------------------+%s\n", O, RST);
    return success;
}