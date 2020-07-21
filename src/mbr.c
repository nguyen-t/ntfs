#include <stdio.h>
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

void mbr_partition_list(MBR* mbr) {
  for(int i = 0; i < MAX_PARTITIONS; i++) {
    if(mbr->partitions[i].size > 0) {
      printf("Partition no.:  %d\n", i + 1);
      printf("Partition type: 0x%02x\n", mbr->partitions[i].type);
    }
  }
}

off_t mbr_partition_offset(MBR* mbr, uint8_t num) {
  if(num < 1 || num > 4) {
    return 0;
  }
  return mbr->partitions[num - 1].lba * SECTOR_SIZE;
}
