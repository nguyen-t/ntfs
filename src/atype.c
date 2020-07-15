#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pad.h"
#include "attribute.h"
#include "atype.h"

// Temporary work-around
void print_0030h(Attribute_Resident* attr) {
  Type_0030h* src = attr->reserved;
  pad_print("Name length:");
  printf("%d\n", src->name_length);
  pad_print("Name:");
  for(int i = 0; i < src->name_length; i++) {
    printf("%c", src->name[i] & 0xFFu);
  }
  printf("\n\n");
}
