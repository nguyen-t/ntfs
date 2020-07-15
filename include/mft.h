#ifndef MFT_H
#define MFT_H

#include <stdint.h>
#include <sys/types.h>
#include "vbr.h"

#define MFT_SIZE 1024
#define MFT_MAGIC (char[]) { 'F', 'I', 'L', 'E' }

typedef union {
  struct __attribute__((packed)) {
    uint8_t magic_number[4];
    uint16_t update_sequence_offset;
    uint16_t fixup_length;
    uint64_t logfile_sequence_number;
    uint16_t sequence_number;
    uint16_t hard_link_count;
    uint16_t attribute_offset;
    uint16_t flags;
    uint32_t real_size;
    uint32_t allocated_size;
    uint64_t base_reference;
    uint16_t next_attribute_id;
    uint16_t boundary_align;
    uint32_t mft_no;
    uint8_t body[976];
  };
  uint8_t raw[MFT_SIZE];
} MFT;

MFT* mft_next(int, off_t*);
int mft_check(MFT*);
int mft_directory(MFT*);
int mft_deleted(MFT*);
void mft_print(MFT*);

#endif
