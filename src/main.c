#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ntfs.h"

#define NTFS_PARTITION "/dev/sda2"

int main(void) {
  int fd;
  Boot_Sector* boot = calloc(1, sizeof(Boot_Sector));

  if((fd = open(NTFS_PARTITION, O_RDWR)) < 0) {
    printf("Failed to open: " NTFS_PARTITION "\n");
    return -1;
  }

  load_vbr(fd, boot);

  free(boot);

  return 0;
}
