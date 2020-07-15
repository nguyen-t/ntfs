#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.h"

MBR* mbr_read(int fd) {
  MBR* mbr;
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Go to sector 0 and read MFT
  if(lseek(fd, 0, SEEK_SET) < 0) {
    return NULL;
  }
  if((mbr = malloc(sizeof(MBR))) == NULL) {
    return NULL;
  }
  if(read(fd, mbr->raw, sizeof(MBR)) < 0) {
    free(mbr);
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    free(mbr);
    return NULL;
  }
  if(!mbr_check(mbr)) {
    free(mbr);
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
