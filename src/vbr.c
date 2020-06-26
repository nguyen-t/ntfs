#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"

ssize_t vbr_read(int fd, VBR* vbr, off_t offset) {
  off_t current;
  ssize_t length;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Read VBR starting at offset address
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return -1;
  }

  length = read(fd, vbr->raw, sizeof(VBR));

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return length;
}

off_t vbr_mft_offset(VBR* vbr, off_t offset) {
  uint16_t sector_size = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size = vbr->bpb.sectors_per_cluster;
  uint64_t mft_cluster = vbr->ebpb.mft_cluster_number;

  return sector_size * cluster_size * mft_cluster + offset;
}

off_t vbr_mirror_offset(VBR* vbr, off_t offset) {
  uint16_t sector_size        = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size        = vbr->bpb.sectors_per_cluster;
  uint64_t mft_mirror_cluster = vbr->ebpb.mft_mirror_cluster_number;

  return sector_size * cluster_size * mft_mirror_cluster + offset;
}
