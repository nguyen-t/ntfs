#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ntfs.h"

#define NTFS_PARTITION "/dev/sda2"

int main(void) {
  int fd;
  VBR* boot = malloc(1 * sizeof(VBR));
  char* mft = malloc(1024 * sizeof(char));

  if((fd = open(NTFS_PARTITION, O_RDWR)) < 0) {
    perror("Failed to open " NTFS_PARTITION "\n");
    return -1;
  }

  if(load_vbr(fd, boot) < 0) {
    perror("Failed to read VBR\n");
    return -1;
  }

  if(load_mft(fd, mft, boot) < 0) {
    perror("Failed to read MFT\n");
    return -1;
  }

  printf("Partition type: %s\n", boot->oem_id);
  for(int i = 0; i < 1024; i += 64) {
    for(int j = 0; j < 64 && (i + j) < 1024; j++) {
      printf("%c", mft[i + j]);
    }
    printf("\n");
  }

  free(mft);
  free(boot);

  return 0;
}
