#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start;
  MBR* mbr    = malloc(1 * sizeof(MBR));
  VBR* vbr    = malloc(1 * sizeof(VBR));
  MFT* mft    = malloc(1 * sizeof(MFT));
  MFT* mirror = malloc(1 * sizeof(MFT));

  #ifdef DEBUG
  printf("MBR size: %ld\n", sizeof(MBR));
  printf("VBR size: %ld\n", sizeof(VBR));
  printf("MFT size: %ld\n", sizeof(MFT));
  #endif

  if(argc != 2) {
    perror("USAGE: ./main [DEVICE]\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(vbr == NULL || mft == NULL) {
    perror("Failed to allocate memory\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("Failed to open device\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(mbr_read(fd, mbr) < 0) {
    perror("Failed to read MBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(!mbr_check(mbr)) {
    perror("Failed MBR validation check\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if((ntfs_start = mbr_partition_offset(mbr, NTFS_PARTITION_ID)) < 0) {
    perror("Failed to find NTFS partition\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(vbr_read(fd, vbr, ntfs_start) < 0) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(mft_read(fd, mft, vbr, ntfs_start) < 0) {
    perror("Failed to read MFT\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(mft_mirror_read(fd, mirror, vbr, ntfs_start) < 0) {
    perror("Failed to read MFT mirror\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  #ifdef DEBUG
  printf("Partition type: %s\n", vbr->oem_id);
  printf("NTFS start: %04lx\n", ntfs_start);
  printf("\n");
  for(int i = 0; i < 1024; i += 64) {
    for(int j = 0; j < 64 && (i + j) < 1024; j++) {
      printf("%c", mft->raw[i + j]);
    }
    printf("\n");
  }
  printf("MFT mirror\n");
  for(int i = 0; i < 1024; i += 64) {
    for(int j = 0; j < 64 && (i + j) < 1024; j++) {
      printf("%c", mirror->raw[i + j]);
    }
    printf("\n");
  }
  #endif

  free(mbr);
  free(vbr);
  free(mft);
  free(mirror);

  return 0;
}
