#ifndef ATYPE_H
#define ATYPE_H

#include "attribute.h"

typedef struct __attribute__((packed)) {
  uint32_t segment_low;
  uint16_t segment_high;
  uint16_t sequence_number;
} File_Reference;

typedef struct __attribute__((packed)) {
  File_Reference parent_dir;
  uint8_t reserved[0x38u];
  uint8_t name_length;
  uint8_t flags;
  uint16_t name[];
} Type_0030h;

void print_0030h(Attribute_Resident*);

#endif
