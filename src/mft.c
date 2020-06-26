#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"
#include "mft.h"

int load_mft(int fd, MFT* mft, VBR* vbr, off_t base) {
  off_t current;
  int bytes_read;
  uint16_t sector_size = *vbr->bpb.bytes_per_sector;
  uint8_t cluster_size = *vbr->bpb.sectors_per_cluster;
  uint64_t mft_cluster = *vbr->ebpb.mft_cluster_number;
  uint64_t mft_offset  = sector_size * cluster_size * mft_cluster + base;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Find and read MFT
  if(lseek(fd, mft_offset, SEEK_SET) < 0)  {
    return -1;
  }
  if((bytes_read = read(fd, mft->raw, 1024)) < 0) {
    return -1;
  }

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) return -1;

  return bytes_read;
}

int load_mft_mirror(int fd, MFT* mft, VBR* vbr, off_t base) {
  off_t current;
  int bytes_read;
  uint16_t sector_size        = *vbr->bpb.bytes_per_sector;
  uint8_t cluster_size        = *vbr->bpb.sectors_per_cluster;
  uint64_t mft_mirror_cluster = *vbr->ebpb.mft_mirror_cluster_number;
  uint64_t mft_offset         = sector_size * cluster_size * mft_mirror_cluster + base;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return -1;
  }

  // Find and read MFT mirror
  if(lseek(fd, mft_offset, SEEK_SET) < 0) {
    return -1;
  }
  if((bytes_read = read(fd, mft->raw, 1024)) < 0) {
    return -1;
  }

  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    return -1;
  }

  return bytes_read;
}
