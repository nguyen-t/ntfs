#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mbr.h"
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start;
  VBR* vbr = malloc(1 * sizeof(VBR));
  uint8_t* mft = malloc(1024 * sizeof(char)); // MFT size unknown for now

  if(argc != 2) {
    perror("USAGE: ./main [DEVICE]\n");
    free(mft);
    free(vbr);
    return -1;
  }

  if(vbr == NULL || mft == NULL) {
    perror("Failed to allocate memory\n");
    free(mft);
    free(vbr);
    return -1;
  }

  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("Failed to open device\n");
    free(mft);
    free(vbr);
    return -1;
  }

  if(mbr_check(fd) < 0) {
    perror("Failed to read MBR\n");
    free(mft);
    free(vbr);
  }

  if((ntfs_start = get_ntfs_offset(fd)) < 0) {
    perror("Failed to find NTFS partition\n");
    free(mft);
    free(vbr);
  }

  if(load_vbr(fd, vbr, ntfs_start) < 0) {
    perror("Failed to read VBR\n");
    free(mft);
    free(vbr);
    return -1;
  }

  if(load_mft(fd, mft, vbr, ntfs_start) < 0) {
    perror("Failed to read MFT\n");
    free(mft);
    free(vbr);
    return -1;
  }

  #ifdef DEBUG
  printf("Partition type: %s\n", vbr->oem_id);
  for(int i = 0; i < 1024; i += 64) {
    for(int j = 0; j < 64 && (i + j) < 1024; j++) {
      printf("%c", mft[i + j]);
    }
    printf("\n");
  }
  #endif

  free(mft);
  free(vbr);

  return 0;
}
