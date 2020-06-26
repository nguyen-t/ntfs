#ifndef NTFS_H
#define NTFS_H

#include "mbr.h"
#include "vbr.h"
#include "mft.h"

#define NTFS_PARTITION_ID 0x07u

// Debug stuff
#ifdef NDEBUG
  #define error_core() ((void) 0)
#else
  #include <stdio.h>
  #include <errno.h>
  #include <string.h>
  #define error_code() printf("%s", strerror(errno))
#endif

#endif
