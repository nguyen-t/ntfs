#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "pad.h"
#include "vbr.h"

VBR* vbr_read(int fd, off_t offset) {
  VBR* vbr;
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Read VBR starting at offset address
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return NULL;
  }
  if((vbr = malloc(sizeof(VBR))) == NULL) {
    return NULL;
  }
  if(read(fd, vbr, sizeof(VBR)) < 0) {
    free(vbr);
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    free(vbr);
    return NULL;
  }
  if(!vbr_check(vbr)) {
    free(vbr);
    return NULL;
  }

  return vbr;
}

off_t vbr_mft_offset(VBR* vbr) {
  uint16_t sector_size = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size = vbr->bpb.sectors_per_cluster;
  uint64_t mft_cluster = vbr->ebpb.mft_cluster_number;
  off_t offset = vbr->bpb.hidden_sectors;

  return sector_size * (cluster_size * mft_cluster + offset);
}

int vbr_check(VBR* vbr) {
  // Pointer magic
  return *(uint64_t*) vbr->oem_id == *(uint64_t*) VBR_MAGIC;
}
