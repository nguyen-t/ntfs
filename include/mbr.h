#ifndef MBR_H
#define MBR_H

#include <stdint.h>
#include <sys/types.h>

#define MBR_SIZE          512
#define SECTOR_SIZE       512
#define MAX_PARTITIONS    4
#define VALID_SIGNATURE   0xAA55

typedef struct __attribute__((packed)) {
  uint8_t boot_status;
  uint8_t chs_start[3];
  uint8_t type;
  uint8_t chs_end[3];
  uint32_t lba;
  uint32_t size;
} Partition_Table;

typedef union {
  struct __attribute__((packed)) {
    uint8_t bootstrap_code[446];
    Partition_Table partitions[4];
    uint16_t boot_signature;
  };
  uint8_t raw[MBR_SIZE];
} MBR;

ssize_t mbr_read(int, MBR*);
int mbr_check(MBR*);
off_t mbr_partition_offset(MBR*, uint8_t);

#endif
