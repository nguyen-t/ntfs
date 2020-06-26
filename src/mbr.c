#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.h"

ssize_t load_mbr(int fd, MBR* mbr) {
  off_t current;
  ssize_t length;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  if(lseek(fd, 0, SEEK_SET) < 0) {
    return -1;
  }
  length = read(fd, mbr->raw, sizeof(MBR));

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return length;
}

int mbr_check(MBR* mbr) {
  return mbr->boot_signature == VALID_SIGNATURE;
}

off_t get_partition_offset(MBR* mbr, uint8_t partition_id) {
  for(int i = 0; i < MAX_PARTITIONS; i++) {
    uint8_t type = mbr->partitions[i].type;
    off_t offset = mbr->partitions[i].lba;

    if(type == partition_id) {
      return offset * SECTOR_SIZE;
    }
  }

  return -1;
}
