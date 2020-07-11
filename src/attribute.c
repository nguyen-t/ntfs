#include <sys/types.h>
#include "mft.h"
#include "attribute.h"

#ifdef DEBUG
  #include <stdio.h>

  // Keep print formatting consistent
  #define pad_print(s) printf("%-25s ", (s))
#endif

off_t attr_next(MFT* mft) {
  static MFT* current;
  static off_t offset;

  if(offset >= MFT_SIZE) {
    return -1;
  }

  // Works similarly to strtok()
  if(mft) {
    current = mft;
    offset = current->attribute_offset;

  } else if(current) {
    ATTR_Header* header = (ATTR_Header*) &current->raw[offset];

    if(header->type == 0xFFFFFFFF) {
      return -1;
    }

    #ifdef DEBUG
      printf("\n");
      pad_print("Attribute type:");
      printf("0x%02x\n", header->type);
      pad_print("Attribute length:");
      printf("%d\n", header->total_length);
      pad_print("Attribute name length:");
      printf("%d\n", header->name_length);
      pad_print("Attribute name:");
      for(int i = 0; i < header->name_length; i++) {
        printf("%02x ", current->raw[header->name_offset + i]);
      }
      printf("\n");
      pad_print("Attribute ID:");
      printf("0x%02x\n", header->attribute_id);
    #endif

    offset += header->total_length;
  } else {
    return -1;
  }

  return offset;
}
