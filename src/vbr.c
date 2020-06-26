#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"

int load_vbr(int fd, VBR* vbr, off_t base) {
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Read VBR starting at base address
  if(lseek(fd, base, SEEK_SET) < 0) {
    return -1;
  }

  if(read(fd, vbr->raw, sizeof(VBR)) < 0) {
    return -1;
  }

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return 0;
}
