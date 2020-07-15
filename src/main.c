#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start, mft_start;
  MBR* mbr;
  VBR* vbr;
  MFT* mft;
  Attribute* attr;

  printf("Attribute size: %ld\n", sizeof(Attribute));

  if(argc != 2) {
    perror("USAGE: ./main [DEVICE]\n");
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
  if((ntfs_start = mbr_partition_offset(mbr, NTFS_PARTITION_ID)) < 0) {
    perror("Failed to find NTFS partition\n");
    free(mbr);
    close(fd);
    return -1;
  }
  if((vbr = vbr_read(fd, ntfs_start)) == NULL) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    close(fd);
    return -1;
  }
  if((mft_start = vbr_mft_offset(vbr, ntfs_start)) < 0) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    close(fd);
    return -1;
  }

  mft = mft_next(fd, &mft_start);

  do {
    #ifdef DEBUG
      mft_print(mft);
    #endif

    attr = attribute_next(mft);

    do {
      #ifdef DEBUG
        attribute_print(attr);
      #endif

      if(attr->header.type == FILE_NAME) {
        print_0030h(attr);
      }
      free(attr);
    } while((attr = attribute_next(NULL)) != NULL);

    free(attr);
    free(mft);
  } while((mft = mft_next(fd, NULL)) != NULL);

  free(mbr);
  free(vbr);
  free(mft);

  close(fd);

  return 0;
}
