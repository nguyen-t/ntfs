#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.h"

int mbr_check(int fd) {
  off_t start;
  uint16_t signature;

  if((start = lseek(fd, 0, SEEK_CUR)) < 0) return -1;

  // Get MBR signature
  if(lseek(fd, MBR_BOOT_SIGNATURE, SEEK_SET) < 0) return -1;
  if(read(fd, &signature, sizeof(uint16_t)) < 0) return -1;

  // Reset file pointer
  if(lseek(fd, start, SEEK_SET) < 0) return -1;

  return signature == VALID_SIGNATURE;
}

off_t get_ntfs_offset(int fd) {
  off_t start;
  uint8_t type[MAX_PARTITIONS];
  uint32_t offset;

  if((start = lseek(fd, 0, SEEK_CUR)) < 0) return -1;

  // Get partition types for all partitions
  if(lseek(fd, MBR_PARTITION_1 + PARTITION_TYPE, SEEK_SET) < 0) return -1;
  if(read(fd, &type[0], sizeof(uint8_t)) < 0) return -1;
  if(lseek(fd, MBR_PARTITION_2 + PARTITION_TYPE, SEEK_SET) < 0) return -1;
  if(read(fd, &type[1], sizeof(uint8_t)) < 0) return -1;
  if(lseek(fd, MBR_PARTITION_3 + PARTITION_TYPE, SEEK_SET) < 0) return -1;
  if(read(fd, &type[2], sizeof(uint8_t)) < 0) return -1;
  if(lseek(fd, MBR_PARTITION_4 + PARTITION_TYPE, SEEK_SET) < 0) return -1;
  if(read(fd, &type[3], sizeof(uint8_t)) < 0) return -1;

  // Reset file pointer
  if(lseek(fd, start, SEEK_SET) < 0) return -1;

  // Find NTFS partition and calculate offset
  if(type[0] == NTFS_PARTITION_ID) {
    if(lseek(fd, MBR_PARTITION_1 + PARTITION_LBA, SEEK_SET) < 0) return -1;
    if(read(fd, &offset, sizeof(uint32_t)) < 0) return -1;
    return offset * SECTOR_SIZE;
  }
  if(type[1] == NTFS_PARTITION_ID) {
    if(lseek(fd, MBR_PARTITION_2 + PARTITION_LBA, SEEK_SET) < 0) return -1;
    if(read(fd, &offset, sizeof(uint32_t)) < 0) return -1;
    return offset * SECTOR_SIZE;
  }
  if(type[2] == NTFS_PARTITION_ID) {
    if(lseek(fd, MBR_PARTITION_3 + PARTITION_LBA, SEEK_SET) < 0) return -1;
    if(read(fd, &offset, sizeof(uint32_t)) < 0) return -1;
    return offset * SECTOR_SIZE;
  }
  if(type[3] == NTFS_PARTITION_ID) {
    if(lseek(fd, MBR_PARTITION_4 + PARTITION_LBA, SEEK_SET) < 0) return -1;
    if(read(fd, &offset, sizeof(uint32_t)) < 0) return -1;
    return offset * SECTOR_SIZE;
  }

  return -1;
}
