#ifndef NTFS_H
#define NTFS_H

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#else
  #error Only little endian is supported
#endif

#include "mbr.h"
#include "vbr.h"
#include "mft.h"
#include "attribute.h"
#include "atype.h"

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
