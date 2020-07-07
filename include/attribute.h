#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <sys/types.h>
#include "mft.h"

typedef enum {
  STANDARD_INFORMATION  = 0x0010u,
  ATTRIBUTE_LIST        = 0x0020u,
  FILE_NAME             = 0x0030u,
  VOLUME_VERSION        = 0x0040u,
  OBJECT_ID             = 0x0040u,
  SECURITY_DESCRIPTOR   = 0x0050u,
  VOLUME_NAME           = 0x0060u,
  VOLUME_INFORMATION    = 0x0070u,
  DATA                  = 0x0080u,
  INDEX_ROOT            = 0x0090u,
  INDEX_ALLOCATION      = 0x00A0u,
  BITMAP                = 0x00B0u,
  SYMBOLIC_LINK         = 0x00C0u,
  REPARSE_POINT         = 0x00C0u,
  EA_INFORMATION        = 0x00D0u,
  EA                    = 0x00E0u,
  PROPERTY_SET          = 0x00F0u,
  LOGGED_UTILITY_STREAM = 0x0100u
} ATTR_ID;

// Common header information
typedef struct __attribute__((packed)) {
  uint32_t type;
  uint32_t total_length;
  uint8_t nonresident_flag;
  uint8_t name_length;
  uint16_t name_offset;
  uint16_t flags;
  uint16_t attribute_id;
} ATTR_Header;

typedef struct __attribute__((packed)) {
  uint32_t type;
  uint32_t total_length;
  uint8_t nonresident_flag;
  uint8_t name_length;
  uint16_t name_offset;
  uint16_t flags;
  uint16_t attribute_id;
  uint32_t attribute_length;
  uint16_t attribute_offset;
  uint8_t indexed_flag;
  uint8_t padding;
} ATTR_Resident;

typedef struct __attribute__((packed)) {
  uint32_t type;
  uint32_t total_length;
  uint8_t nonresident_flag;
  uint8_t name_length;
  uint16_t name_offset;
  uint16_t flags;
  uint16_t attribute_id;
  uint64_t start_vcn;
  uint64_t end_vcn;
  uint16_t data_runs_offset;
  uint16_t compression_unit_size;
  uint32_t padding;
  uint64_t allocated_attribute_size;
  uint64_t real_attribute_size;
  uint64_t initialized_stream_size;
} ATTR_Nonresident;

off_t attr_next(MFT*);

#endif
