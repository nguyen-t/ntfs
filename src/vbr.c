#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "vbr.h"

VBR* vbr_read(int fd, off_t offset) {
  VBR* vbr;
  off_t current;

  // Save current file pointer
  if((current = lseek(fd, 0, SEEK_CUR)) < 0) {
    return NULL;
  }
  // Read VBR starting at offset address
  if(lseek(fd, offset, SEEK_SET) < 0) {
    return NULL;
  }
  if((vbr = malloc(sizeof(VBR))) == NULL) {
    return NULL;
  }
  if(read(fd, vbr, sizeof(VBR)) < 0) {
    free(vbr);
    return NULL;
  }
  // Reset file pointer
  if(lseek(fd, current, SEEK_SET) < 0) {
    free(vbr);
    return NULL;
  }
  if(!vbr_check(vbr)) {
    free(vbr);
    return NULL;
  }

  return vbr;
}

off_t vbr_mft_offset(VBR* vbr) {
  uint16_t sector_size = vbr->bpb.bytes_per_sector;
  uint8_t cluster_size = vbr->bpb.sectors_per_cluster;
  uint64_t mft_cluster = vbr->ebpb.mft_cluster_number;
  off_t offset = vbr->bpb.hidden_sectors;

  return sector_size * (cluster_size * mft_cluster + offset);
}

int vbr_check(VBR* vbr) {
  // Pointer magic
  return *(uint64_t*) vbr->oem_id == *(uint64_t*) VBR_MAGIC;
}

void vbr_print(VBR* vbr) {
  printf("Volume Boot Record\n");
  printf("OEM ID:               %.8s\n", vbr->oem_id);
  printf("End of sector marker: 0x%04x\n", vbr->end_of_sector_marker);
  printf("BIOS parameter block\n");
  printf("    Bytes per sector:          %d\n", vbr->bpb.bytes_per_sector);
  printf("    Sectors per cluster:       %d\n", vbr->bpb.sectors_per_cluster);
  printf("    Sectors per track:         %d\n", vbr->bpb.sectors_per_track);
  printf("    Number of heads:           %d\n", vbr->bpb.number_of_heads);
  printf("    Media descriptor:          0x%02x\n", vbr->bpb.media_descriptor);
  printf("    Hidden sectors:            0x%08x\n", vbr->bpb.hidden_sectors);
  printf("Extended BIOS parameter block\n");
  printf("    Total sectors:             %ld\n", vbr->ebpb.total_sectors);
  printf("    MFT cluster number:        %ld\n", vbr->ebpb.mft_cluster_number);
  printf("    Mirror MFT cluster number: %ld\n", vbr->ebpb.mft_mirror_cluster_number);
  printf("    Volume serial number       0x%16lx\n", vbr->ebpb.volume_serial_number);
  printf("    Checksum:                  0x%08x\n", vbr->ebpb.checksum);
  printf("\n");
}
