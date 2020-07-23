#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  int partition;
  MBR* mbr;
  VBR* vbr;
  MFT* mft;
  Attribute* attr;

  switch(argc) {
    case 2:
      partition = 0;
      break;
    case 3:
      partition = strtol(argv[2], NULL, 10);
      break;
    default:
      perror("USAGE: ./main [DEVICE] [PARTITION]\n");
      return -1;
  }
  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("Failed to open device\n");
    return -1;
  }
  if((mbr = mbr_read(fd)) == NULL) {
    perror("Failed to read MBR\n");
    close(fd);
    return -1;
  }
  // Ask for partition type
  if(partition < 1 || partition > 4) {
    do {
      mbr_partition_list(mbr);
      printf("Which partition? ");
    } while(scanf(" %d", &partition) != 1 || partition < 1 || partition > 4);
  }
  if((vbr = vbr_read(fd, mbr_partition_offset(mbr, partition))) == NULL) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    close(fd);
    return -1;
  }

  #ifdef DEBUG
    printf("\n");
    mbr_print(mbr);
    vbr_print(vbr);
  #endif

  mft = mft_next(fd, vbr);

  do {
    #ifdef DEBUG
      mft_print(mft);
    #endif

    attr = attribute_next(mft);

    do {
      #ifdef DEBUG
        if(attr->header.type == FILE_NAME) {
          print_0030h(attr);
        } else {
          attribute_print(attr);
        }
      #endif
      free(attr);
    } while((attr = attribute_next(NULL)) != NULL);

    free(mft);
  } while((mft = mft_next(fd, NULL)) != NULL);

  free(mbr);
  free(vbr);
  close(fd);

  return 0;
}
