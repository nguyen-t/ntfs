#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "pad.h"
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
  printf("\n");
  pad_print("Attribute type:");
  printf("0x%02x\n", attr->header.type);
  pad_print("Attribute length:");
  printf("%d\n", attr->header.total_length);
  pad_print("Attribute name length:");
  printf("%d\n", attr->header.name_length);
  pad_print("Attribute name offset:");
  printf("%d\n", attr->header.name_offset);
  pad_print("Attribute instance:");
  printf("0x%02x\n", attr->header.instance);
}
