#include <unistd.h>
#include "ntfs.h"

// Keep functions calls clean
#define vbr_seek(offset) lseek(fd, offset, SEEK_SET)
#define vbr_read(content) read(fd, content, sizeof(content))

int load_vbr(int fd, Boot_Sector* bs) {
  // Header
  if(vbr_seek(JUMP_INSTRUCTION) < 0) return -1;
  if(vbr_read(bs->jump_instruction) < 0) return -1;
  if(vbr_seek(OEM_ID) < 0) return -1;
  if(vbr_read(bs->oem_id) < 0) return -1;

  // BIOS Parameter Block
  if(vbr_seek(BYTES_PER_SECTOR) < 0) return -1;
  if(vbr_read(bs->bpb.bytes_per_sector) < 0) return -1;
  if(vbr_seek(SECTORS_PER_CLUSTER) < 0) return -1;
  if(vbr_read(bs->bpb.sectors_per_cluster) < 0) return -1;
  if(vbr_seek(RESERVED_SECTORS) < 0) return -1;
  if(vbr_read(bs->bpb.reserved_sectors) < 0) return -1;
  if(vbr_seek(UNUSED0) < 0) return -1;
  if(vbr_read(bs->bpb.unused0) < 0) return -1;
  if(vbr_seek(UNUSED1) < 0) return -1;
  if(vbr_read(bs->bpb.unused1) < 0) return -1;
  if(vbr_seek(MEDIA_DESCRIPTOR) < 0) return -1;
  if(vbr_read(bs->bpb.media_descriptor) < 0) return -1;
  if(vbr_seek(UNUSED2) < 0) return -1;
  if(vbr_read(bs->bpb.unused2) < 0) return -1;
  if(vbr_seek(SECTORS_PER_TRACK) < 0) return -1;
  if(vbr_read(bs->bpb.sectors_per_track) < 0) return -1;
  if(vbr_seek(NUMBER_OF_HEADS) < 0) return -1;
  if(vbr_read(bs->bpb.number_of_heads) < 0) return -1;
  if(vbr_seek(HIDDEN_SECTORS) < 0) return -1;
  if(vbr_read(bs->bpb.hidden_sectors) < 0) return -1;
  if(vbr_seek(UNUSED3) < 0) return -1;
  if(vbr_read(bs->bpb.unused3) < 0) return -1;

  // Extended BIOS Parameter Block
  if(vbr_seek(UNUSED4) < 0) return -1;
  if(vbr_read(bs->ebpb.unused4) < 0) return -1;
  if(vbr_seek(TOTAL_SECTORS) < 0) return -1;
  if(vbr_read(bs->ebpb.total_sectors) < 0) return -1;
  if(vbr_seek(MFT_CLUSTER_NUMBER) < 0) return -1;
  if(vbr_read(bs->ebpb.mft_cluster_number) < 0) return -1;
  if(vbr_seek(MFT_MIRROR_CLUSTER_NUMBER) < 0) return -1;
  if(vbr_read(bs->ebpb.mft_mirror_cluster_number) < 0) return -1;
  if(vbr_seek(UNITS_PER_FILE_RECORD_SEGMENT) < 0) return -1;
  if(vbr_read(bs->ebpb.bytes_per_file_record_segment) < 0) return -1;
  if(vbr_seek(UNUSED5) < 0) return -1;
  if(vbr_read(bs->ebpb.unused5) < 0) return -1;
  if(vbr_seek(UNITS_PER_INDEX_BUFFER) < 0) return -1;
  if(vbr_read(bs->ebpb.bytes_per_index_buffer) < 0) return -1;
  if(vbr_seek(UNUSED6) < 0) return -1;
  if(vbr_read(bs->ebpb.unused6) < 0) return -1;
  if(vbr_seek(VOLUME_SERIAL_NUMBER) < 0) return -1;
  if(vbr_read(bs->ebpb.volume_serial_number) < 0) return -1;
  if(vbr_seek(CHECKSUM) < 0) return -1;
  if(vbr_read(bs->ebpb.checksum) < 0) return -1;

  // Footer
  if(vbr_seek(BOOTSTRAP_CODE) < 0) return -1;
  if(vbr_read(bs->bootstrap_code) < 0) return -1;
  if(vbr_seek(END_OF_SECTOR_MARKER) < 0) return -1;
  if(vbr_read(bs->end_of_sector_marker) < 0) return -1;

  return 0;
}
