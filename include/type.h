#ifndef TYPE_H
#define TYPE_H

#include "attribute.h"

typedef struct __attribute__((packed)) {
  ATTR_Nonresident header;
  uint8_t parent_dir[6];
  uint16_t sequence_number[2];
  uint64_t file_creation_time;
  uint64_t file_modified_time;
  uint64_t mft_modified_time;
  uint64_t file_read_time;
  uint64_t allocated_size;
  uint64_t real_size;
  uint32_t flags;
  uint32_t others;
  uint8_t name_length;
  uint8_t padding;
  uint16_t name[];
} Type_0030H;

#endif
