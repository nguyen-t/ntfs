#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "mft.h"
#include "attribute.h"

Attribute* attribute_next(MFT* mft) {
  static MFT* current;
  static off_t offset;
  Attribute* attr = NULL;

  // Works similarly to strtok()
  if(mft) {
    current = mft;
    offset = current->attribute_offset - offsetof(MFT, body);
  }
  if(current) {
    // Pointer magic to calculate body length
    size_t size = *(uint32_t*) (current->body + offset + offsetof(Attribute_Header, total_length));

    // Bounds checking
    if((offset + offsetof(MFT, body)) >= current->real_size) {
      return NULL;
    }
    if(size == 0) {
      return NULL;
    }
    if((attr = malloc(size)) == NULL) {
      return NULL;
    }

    memcpy(attr, current->body + offset, size);
    offset += size;
  }

  return attr;
}

void attribute_print(Attribute* attr) {
  printf("Unclassified MFT Record\n");
  printf("Record type:        0x%04x\n", attr->header.type);
  printf("Record length:      %d\n", attr->header.total_length);
  printf("Record form code:   %s\n", (attr->header.form_code) ? "Nonresident" : "resident");
  printf("Record name length: %d\n", attr->header.name_length);
  printf("Record flags:       0x%04x\n", attr->header.flags);
  printf("Record instance:    0x%04x\n", attr->header.instance);
  printf("\n");
}
