#ifndef MFT_H
#define MFT_H

#include <stdint.h>
#include <sys/types.h>
#include "ntfs.h"

int load_mft(int, uint8_t[1024], VBR*, off_t);

#endif
