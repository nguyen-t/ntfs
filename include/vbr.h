#ifndef VBR_H
#define VBR_H

#include <stdint.h>
#include <sys/types.h>

#define VBR_SIZE 512

// NTFS Boot Sector offset
// Not used anymore
typedef enum {
  JUMP_INSTRUCTION              = 0x0000u,
  OEM_ID                        = 0x0003u,
  BYTES_PER_SECTOR              = 0x000Bu,
  SECTORS_PER_CLUSTER           = 0x000Du,
  RESERVED_SECTORS              = 0x000Eu,
  UNUSED0                       = 0x0010u,
  UNUSED1                       = 0x0013u,
  MEDIA_DESCRIPTOR              = 0x0015u,
  UNUSED2                       = 0x0016u,
  SECTORS_PER_TRACK             = 0x0018u,
  NUMBER_OF_HEADS               = 0x001Au,
  HIDDEN_SECTORS                = 0x001Cu,
  UNUSED3                       = 0x0020u,
  UNUSED4                       = 0x0024u,
  TOTAL_SECTORS                 = 0x0028u,
  MFT_CLUSTER_NUMBER            = 0x0030u,
  MFT_MIRROR_CLUSTER_NUMBER     = 0x0038u,
  UNITS_PER_FILE_RECORD_SEGMENT = 0x0040u,
  UNUSED5                       = 0x0041u,
  UNITS_PER_INDEX_BUFFER        = 0x0044u,
  UNUSED6                       = 0x0045u,
  VOLUME_SERIAL_NUMBER          = 0x0048u,
  CHECKSUM                      = 0x0050u,
  BOOTSTRAP_CODE                = 0x0054u,
  END_OF_SECTOR_MARKER          = 0x01FEu
} VBR_Offset;

// NTFS Boot Sector structure
typedef union {
  struct __attribute__((packed)) {
    uint8_t jump_instruction[3];
    uint8_t oem_id[8];
    struct __attribute__((packed)) {
      uint16_t bytes_per_sector;
      uint8_t sectors_per_cluster;
      uint16_t reserved_sectors;
      uint8_t unused0[3];
      uint8_t unused1[2];
      uint8_t media_descriptor;
      uint8_t unused2[2];
      uint16_t sectors_per_track;
      uint16_t number_of_heads;
      uint32_t hidden_sectors;
      uint8_t unused3[4];
    } bpb;
    struct __attribute__((packed)) {
      uint8_t unused4[4];
      uint64_t total_sectors;
      uint64_t mft_cluster_number;
      uint64_t mft_mirror_cluster_number;
      union {
        uint8_t bytes_per_file_record_segment;
        uint8_t clusters_per_file_record_segment;
      };
      uint8_t unused5[3];
      union {
        uint8_t bytes_per_index_buffer;
        uint8_t clusters_per_index_buffer;
      };
      uint8_t unused6[3];
      uint64_t volume_serial_number;
      uint32_t checksum;
    } ebpb;
    uint8_t bootstrap_code[426];
    uint8_t end_of_sector_marker[2];
  };
  uint8_t raw[VBR_SIZE];
} VBR;

ssize_t load_vbr(int, VBR*, off_t);

#endif
