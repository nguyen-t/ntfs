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

  if(mft) {
    current = mft;
    offset = current->attribute_offset;
  } else {
    ATTR_Header* header = (ATTR_Header*) &current->raw[offset];
    offset += header->total_length;
  }

  #ifdef DEBUG
    pad_print("Attribute offset:");
    printf("%08lx\n", offset);
  #endif

  if(offset >= MFT_SIZE) {
    return -1;
  }

  return offset;
}
