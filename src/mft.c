#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include "pad.h"
#include "vbr.h"
#include "mft.h"

MFT* mft_next(int fd, VBR* reset) {
  static off_t offset = -1;
  MFT* mft;
  off_t current;

  // Works like strtok()
  if(reset) {
    offset = vbr_mft_offset(reset);
  }
  if(offset == -1) {
    return NULL;
  }
  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Find and read MFT
  if(lseek(fd, offset, SEEK_SET) < 0)  {
    return NULL;
  }
  if((mft = malloc(sizeof(MFT))) == NULL) {
    return NULL;
  }
  if(read(fd, mft->raw, sizeof(MFT)) < 0) {
    free(mft);
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    free(mft);
    return NULL;
  }
  if(!mft_check(mft)) {
    free(mft);
    return NULL;
  }

  offset += 1024;

  return mft;
}

int mft_check(MFT* mft) {
  // Pointer magic
  return *(uint32_t*) mft->magic_number == *(uint32_t*) MFT_MAGIC;
}

int mft_directory(MFT* mft) {
  return (mft->flags & 0x0020u) >> 1;
}

int mft_deleted(MFT* mft) {
  return (mft->flags & 0x0001u) == 0;
}

void mft_print(MFT* mft) {
  printf("\n");
  pad_print("MFT magic number:");
  for(int i = 0; i < 4; i++) {
    printf("%c", mft->magic_number[i]);
  }
  printf("\n");
  pad_print("MFT deleted:");
  printf("%s\n", (mft_deleted(mft)) ? "Yes" : "No");
  pad_print("MFT type:");
  printf("%s\n", (mft_directory(mft)) ? "Directory" : "File");
  pad_print("MFT real size:");
  printf("%d\n", mft->real_size);
  pad_print("MFT allocated size:");
  printf("%d\n", mft->allocated_size);
}
