#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "ntfs.h"
#include "mft.h"

int load_mft(int fd, uint8_t mft[1024], VBR* bs, off_t base) {
  off_t start = lseek(fd, 0, SEEK_CUR);
  int bytes_read;
  uint16_t sector_size = *bs->bpb.bytes_per_sector;
  uint8_t cluster_size = *bs->bpb.sectors_per_cluster;
  uint64_t mft_cluster = *bs->ebpb.mft_cluster_number;
  uint64_t mft_offset = sector_size * cluster_size * mft_cluster + base;

  // Find and read MFT
  lseek(fd, mft_offset, SEEK_SET);
  bytes_read = read(fd, mft, 1024);

  // Reset file pointer
  lseek(fd, start, SEEK_SET);

  return bytes_read;
}
