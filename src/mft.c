#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"
#include "mft.h"

MFT* mft_next(int fd, VBR* reset) {
  static off_t offset = -1;
  MFT* mft;
  off_t current;

  // Works like strtok()
  if(reset) {
    offset = vbr_mft_offset(reset);
  }
  if(offset == -1) {
    return NULL;
  }
  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Find and read MFT
  if(lseek(fd, offset, SEEK_SET) < 0)  {
    return NULL;
  }
  if((mft = malloc(sizeof(MFT))) == NULL) {
    return NULL;
  }
  if(read(fd, mft, sizeof(MFT)) < 0) {
    free(mft);
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    free(mft);
    return NULL;
  }
  if(!mft_check(mft)) {
    free(mft);
    return NULL;
  }

  offset += 1024;

  return mft;
}

int mft_check(MFT* mft) {
  // Pointer magic
  return *(uint32_t*) mft->magic_number == *(uint32_t*) MFT_MAGIC;
}

int mft_directory(MFT* mft) {
  return (mft->flags & 0x0020u) >> 1;
}

int mft_deleted(MFT* mft) {
  return (mft->flags & 0x0001u) == 0;
}

void mft_print(MFT* mft) {
  printf("Master File Table\n");
  printf("Magic number:            %.4s\n", mft->magic_number);
  printf("Update sequence offset:  0x%04x\n", mft->update_sequence_offset);
  printf("Fixup length:            %d\n", mft->fixup_length);
  printf("Logfile sequence number: 0x%016lx\n", mft->logfile_sequence_number);
  printf("Sequence number:         0x%04x\n", mft->sequence_number);
  printf("Hard link count:         %d\n", mft->hard_link_count);
  printf("Record offset:           0x%04x\n", mft->attribute_offset);
  printf("Flags:                   0x%04x\n", mft->flags);
  printf("Real size:               0x%08x\n", mft->real_size);
  printf("Allocated size:          0x%08x\n", mft->allocated_size);
  printf("Base reference:          0x%016lx\n", mft->base_reference);
  printf("Next attribute ID:       0x%04x\n", mft->next_attribute_id);
  printf("MFT no.:                 0x%08x\n", mft->mft_no);
  printf("\n");
}
