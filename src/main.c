#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start;
  VBR* vbr    = malloc(1 * sizeof(VBR));
  MFT* mft    = malloc(1 * sizeof(MFT));
  MFT* mirror = malloc(1 * sizeof(MFT));

  if(argc != 2) {
    perror("USAGE: ./main [DEVICE]\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(vbr == NULL || mft == NULL) {
    perror("Failed to allocate memory\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("Failed to open device\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(mbr_check(fd) < 0) {
    perror("Failed to read MBR\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if((ntfs_start = get_ntfs_offset(fd)) < 0) {
    perror("Failed to find NTFS partition\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(load_vbr(fd, vbr, ntfs_start) < 0) {
    perror("Failed to read VBR\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(load_mft(fd, mft, vbr, ntfs_start) < 0) {
    perror("Failed to read MFT\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(load_mft_mirror(fd, mirror, vbr, ntfs_start) < 0) {
    perror("Failed to read MFT mirror\n");
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  #ifdef DEBUG
  printf("Partition type: %s\n", vbr->oem_id);
  printf("MFT main\n");
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

  free(vbr);
  free(mft);
  free(mirror);

  return 0;
}
