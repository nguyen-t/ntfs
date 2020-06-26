#ifndef MFT_H
#define MFT_H

#include <stdint.h>
#include <sys/types.h>
#include "ntfs.h"

#define MFT_SIZE 1024

typedef union {
  // TODO: Add MFT info
  uint8_t raw[MFT_SIZE];
} MFT;

int load_mft(int, MFT*, VBR*, off_t);
int load_mft_mirror(int, MFT*, VBR*, off_t);

#endif
