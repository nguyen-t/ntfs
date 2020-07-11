#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"

#ifdef DEBUG
  #include <stdio.h>

  // Keep print formatting consistent
  #define pad_print(s) printf("%-25s ", (s))
#endif

VBR* vbr_read(int fd, off_t offset) {
  VBR* vbr = malloc(sizeof(VBR));
  off_t current;
  
  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Read VBR starting at offset address
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return NULL;
  }
  if(read(fd, vbr->raw, sizeof(VBR)) < 0) {
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return NULL;
  }

  // #ifdef DEBUG
  //   pad_print("Partition type:");
  //   printf("%s\n", vbr->oem_id);
  // #endif

  return vbr;
}

off_t vbr_mft_offset(VBR* vbr, off_t offset) {
  uint16_t sector_size = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size = vbr->bpb.sectors_per_cluster;
  uint64_t mft_cluster = vbr->ebpb.mft_cluster_number;
  off_t mft_start      = sector_size * cluster_size * mft_cluster + offset;

  // #ifdef DEBUG
  //   pad_print("MFT start:");
  //   printf("0x%08lx\n", mft_start);
  // #endif

  return mft_start;
}

off_t vbr_mirror_offset(VBR* vbr, off_t offset) {
  uint16_t sector_size    = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size    = vbr->bpb.sectors_per_cluster;
  uint64_t mirror_cluster = vbr->ebpb.mft_mirror_cluster_number;
  off_t mirror_start = sector_size * cluster_size * mirror_cluster + offset;

  // #ifdef DEBUG
  //   pad_print("MFT mirror start:");
  //   printf("0x%08lx\n", mirror_start);
  // #endif

  return mirror_start;
}
