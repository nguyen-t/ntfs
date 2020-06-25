#ifndef MBR_H
#define MBR_H

#include <sys/types.h>

// Important default values
#define MBR_SIZE          512
#define MAX_PARTITIONS    4
#define SECTOR_SIZE       512
#define VALID_SIGNATURE   0xAA55u
#define NTFS_PARTITION_ID 0x07u

// Important offsets for MBR
typedef enum {
  MBR_PARTITION_1    = 0x01BEu,
  MBR_PARTITION_2    = 0x01CEu,
  MBR_PARTITION_3    = 0x01DEu,
  MBR_PARTITION_4    = 0x01EEu,
  MBR_BOOT_SIGNATURE = 0x01FEu
} MBR_Offset;

typedef enum {
  PARTITION_STATUS            = 0x0000u,
  PARTITION_CHS_START         = 0x0001u,
  PARTITION_TYPE              = 0x0004u,
  PARTITION_CHS_END           = 0x0005u,
  PARTITION_LBA               = 0x0008u,
  PARTITION_NUMBER_OF_SECTORS = 0x000Cu
} Partition_Offset;

int mbr_check(int);
off_t get_ntfs_offset(int);

#endif
