#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"
#include "mft.h"

#ifdef DEBUG
  #include <stdio.h>

  // Keep print formatting consistent
  #define pad_print(s) printf("%-25s ", (s))
#endif

ssize_t mft_read(int fd, MFT* mft, off_t offset) {
  int bytes_read;
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Find and read MFT
  if(lseek(fd, offset, SEEK_SET) < 0)  {
    return -1;
  }
  bytes_read = read(fd, mft->raw, sizeof(MFT));

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return bytes_read;
}

ssize_t mft_mirror_read(int fd, MFT* mft, off_t offset) {
  int bytes_read;
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Find and read MFT mirror
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return -1;
  }
  bytes_read = read(fd, mft->raw, sizeof(MFT));

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return bytes_read;
}

int mft_check(MFT* mft) {
  // Pointer magic
  return *(uint32_t*) mft->magic_number == *(uint32_t*) MAGIC_NUMBER;
}
