#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"
#include "mft.h"

MFT* mft_read(int fd, off_t offset) {
  MFT* mft = malloc(sizeof(MFT));
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Find and read MFT
  if(lseek(fd, offset, SEEK_SET) < 0)  {
    return NULL;
  }
  if(read(fd, mft->raw, sizeof(MFT)) < 0) {
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return NULL;
  }

  return mft;
}

MFT* mft_mirror_read(int fd, off_t offset) {
  MFT* mft = malloc(sizeof(MFT));
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Find and read MFT mirror
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return NULL;
  }
  if(read(fd, mft->raw, sizeof(MFT)) < 0) {
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return NULL;
  }

  return mft;
}

int mft_check(MFT* mft) {
  // Pointer magic
  return *(uint32_t*) mft->magic_number == *(uint32_t*) MAGIC_NUMBER;
}

int mft_directory(MFT* mft) {
  return (mft->flags & 0x0020u) >> 1;
}

int mft_deleted(MFT* mft) {
  return (mft->flags & 0x0001u) == 0;
}
