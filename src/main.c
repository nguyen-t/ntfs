#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntfs.h"

#ifdef DEBUG
  // Keep print formatting consistent
  #define pad_print(s) printf("%-25s ", (s))
#endif

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start, mft_start;
  MBR* mbr;
  VBR* vbr;
  MFT* mft;

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

  if(!mbr_check(mbr)) {
    perror("Failed MBR validation check\n");
    free(mbr);
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

  if((mft = mft_read(fd, mft_start)) == NULL) {
    perror("Failed to read MFT\n");
    free(mbr);
    free(vbr);
    free(mft);
    close(fd);
    return -1;
  }

  if(!mft_check(mft)) {
    perror("Failed MFT validation check\n");
    free(mbr);
    free(vbr);
    free(mft);
    close(fd);
    return -1;
  }

  #ifdef DEBUG
    pad_print("MFT type:");
    printf("%s\n", (mft_directory(mft)) ? "Directory" : "File");
    pad_print("MFT deleted:");
    printf("%s\n", (mft_deleted(mft)) ? "Yes" : "No");
  #endif


  attr_next(mft);
  attr_next(NULL);
  attr_next(NULL);
  attr_next(NULL);
  attr_next(NULL);
  attr_next(NULL);

  free(mbr);
  free(vbr);
  free(mft);
  close(fd);

  return 0;
}
