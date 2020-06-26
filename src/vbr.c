#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"

ssize_t load_vbr(int fd, VBR* vbr, off_t base) {
  off_t current;
  ssize_t length;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Read VBR starting at base address
  if(lseek(fd, base, SEEK_SET) < 0) {
    return -1;
  }

  length = read(fd, vbr->raw, sizeof(VBR));

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return length;
}
