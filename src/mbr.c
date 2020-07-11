#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.h"

#ifdef DEBUG
  #include <stdio.h>

  // Keep print formatting consistent
  #define pad_print(s) printf("%-25s ", (s))
#endif

MBR* mbr_read(int fd) {
  MBR* mbr = malloc(sizeof(MBR));
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Go to sector 0 and read MFT
  if(lseek(fd, 0, SEEK_SET) < 0) {
    return NULL;
  }
  if(read(fd, mbr->raw, sizeof(MBR)) < 0) {
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return NULL;
  }

  return mbr;
}

int mbr_check(MBR* mbr) {
  return mbr->boot_signature == VALID_SIGNATURE;
}

off_t mbr_partition_offset(MBR* mbr, uint8_t partition_id) {
  for(int i = 0; i < MAX_PARTITIONS; i++) {
    uint8_t type = mbr->partitions[i].type;
    off_t offset = mbr->partitions[i].lba;

    if(type == partition_id) {

      // #ifdef DEBUG
      //   pad_print("Partition location:");
      //   printf("0x%08x\n", offset * SECTOR_SIZE);
      // #endif

      return offset * SECTOR_SIZE;
    }
  }

  return -1;
}
