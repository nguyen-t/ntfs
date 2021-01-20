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

void print_header(Attribute* attr) {
  printf("Attribute Header\n");
  printf("Type:         0x%04x\n", attr->header.type);
  printf("Total length: %d\n", attr->header.total_length);
  printf("Form code:    %s\n", (attr->header.form_code) ? "Nonresident" : "resident");
  printf("Name length:  %d\n", attr->header.name_length);
  printf("Flags:        0x%04x\n", attr->header.flags);
  printf("Instance no.: 0x%04x\n", attr->header.instance);
}

void print_resident(Attribute* attr) {
  Attribute_Resident* res = (Attribute_Resident*) attr;
  printf("Resident Header\n");
  printf("    Value length: %d\n", res->value_length);
  printf("    Value offset: 0x%08x\n", res->value_offset);
}

void print_nonresident(Attribute* attr) {
  Attribute_Nonresident* res = (Attribute_Nonresident*) attr;
  printf("Nonresident Header\n");
  printf("    VCN low:          0x%016lx\n", res->vcn_low);
  printf("    VCN high:         0x%016lx\n", res->vcn_high);
  printf("    Data offset:      0x%04x\n", res->data_offset);
  printf("    Allocated length: %ld\n", res->allocated_length);
  printf("    Real length:      %ld\n", res->real_length);
}

void print_0010h(Attribute* attr) {
  Attribute_Resident* res = (Attribute_Resident*) attr;
  Type_0010h* ref = (Type_0010h*) res->reserved;
  printf("STANDARD_INFORMATION Type\n");
  printf("    Owner ID:    0x%08x\n", ref->owner_id);
  printf("    Security ID: 0x%08x\n", ref->security_id);
}

void print_0030h(Attribute* attr) {
  Attribute_Resident* res = (Attribute_Resident*) attr;
  Type_0030h* ref = (Type_0030h*) res->reserved;
  printf("FILE_NAME Type\n");
  printf("    Parent directory\n");
  printf("        Segment low:     0x%08x\n", ref->parent_dir.segment_low);
  printf("        Segment high:    0x%08x\n", ref->parent_dir.segment_high);
  printf("        Sequence number: 0x%04x\n", ref->parent_dir.segment_low);
  printf("    Flags:       %02x\n", ref->file_name_flags);
  printf("    Name length: %d\n", ref->file_name_length);
  printf("    Name:        ");
  for(int i = 0; i < ref->file_name_length; i++) {
    printf("%c", ref->name[i] & 0xFFu);
  }
  printf("\n");
}

void print_0080h(Attribute* attr) {}

void attribute_print(Attribute* attr) {
  print_header(attr);

  switch(attr->header.form_code) {
    case RESIDENT:
      print_resident(attr);
      break;

    case NONRESIDENT:
      print_nonresident(attr);
      break;
  }

  switch(attr->header.type) {
    case STANDARD_INFORMATION:
      print_0010h(attr);
      break;

    case ATTRIBUTE_LIST:
      break;

    case FILE_NAME:
      print_0030h(attr);
      break;

    case OBJECT_ID:
      break;

    case VOLUME_NAME:
      break;

    case VOLUME_INFORMATION:
      break;

    case DATA:
      break;

    case INDEX_ROOT:
      break;

    case INDEX_ALLOCATION:
      break;

    case BITMAP:
      break;

    case REPARSE_POINT:
      break;
  }
  printf("\n");
}
