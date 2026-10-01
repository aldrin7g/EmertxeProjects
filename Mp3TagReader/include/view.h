#ifndef VIEW_H
#define VIEW_H

#include "common.h"

Status read_mp3_info_size(FileInfo* file);
int read_frame_size(FileInfo* file);
uint convert_frame_size(byte *buffer);
Status read_mp3_frame(Mp3Tag* tags, FileInfo* file);
Status read_mp3_tags(Mp3Tag* tags, FileInfo* file);
Status display_mp3_tags(Mp3Tag* tags);

#endif