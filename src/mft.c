#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"
#include "mft.h"

int load_mft(int fd, uint8_t mft[1024], VBR* bs, off_t base) {
  off_t start;
  int bytes_read;
  uint16_t sector_size = *bs->bpb.bytes_per_sector;
  uint8_t cluster_size = *bs->bpb.sectors_per_cluster;
  uint64_t mft_cluster = *bs->ebpb.mft_cluster_number;
  uint64_t mft_offset = sector_size * cluster_size * mft_cluster + base;

  if((start = lseek(fd, 0, SEEK_CUR)) < 0) return -1;

  // Find and read MFT
  if(lseek(fd, mft_offset, SEEK_SET) < 0) return -1;
  if((bytes_read = read(fd, mft, 1024)) < 0) return -1;

  // Reset file pointer
  if(lseek(fd, start, SEEK_SET) < 0) return -1;

  return bytes_read;
}
