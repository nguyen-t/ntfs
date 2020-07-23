#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "attribute.h"
#include "atype.h"

// Temporary work-around
void print_0030h(Attribute* attr) {
  Attribute_Resident* res = (Attribute_Resident*) attr;
  Type_0030h* ref = (Type_0030h*) res->reserved;
  printf("FILE_NAME Record\n");
  printf("Record type:        0x%04x\n", res->header.type);
  printf("Record length:      %d\n", res->header.total_length);
  printf("Record form code:   %s\n", (res->header.form_code) ? "Nonresident" : "resident");
  printf("Record name length: %d\n", res->header.name_length);
  printf("Record flags:       0x%04x\n", res->header.flags);
  printf("Record instance:    0x%04x\n", res->header.instance);
  printf("Value length:       %d\n", res->value_length);
  printf("Value offset        0x%08x\n", res->value_offset);
  printf("FILE_NAME length:   %d\n", ref->file_name_length);
  printf("FILE_NAME flags:    %02x\n", ref->file_name_flags);
  printf("FILE_NAME name:     ");
  for(int i = 0; i < ref->file_name_length; i++) {
    printf("%c", ref->name[i] & 0xFFu);
  }
  printf("\n");
  printf("Parent directory\n");
  printf("    Segment low:     0x%08x\n", ref->parent_dir.segment_low);
  printf("    Segment high:    0x%08x\n", ref->parent_dir.segment_high);
  printf("    Sequence number: 0x%04x\n", ref->parent_dir.segment_low);
  printf("\n");
}
