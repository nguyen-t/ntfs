#ifndef NTFS_H
#define NTFS_H

#include "mbr.h"
#include "vbr.h"
#include "mft.h"

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
