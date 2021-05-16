#ifndef __FS_H
#define __FS_H
#include <stdint.h>
#include <stddef.h>
#include "vfs.h"
#undef wchar_t
#define wchar_t uint16_t
typedef struct __boot_sector {
    uint8_t bs_jmpboot[3];
    uint8_t bs_oem_name[8];
    uint16_t bpb_bytes_per_sec;
    uint8_t bpb_sec_per_clus;
    uint16_t bpb_reserved_sec_cnt;
    uint8_t bpb_num_fats;
    uint16_t bpb_root_ent_cnt;
    uint16_t bpb_total_sector;
    uint8_t bpb_media;
    uint16_t bpb_fatsz;
    uint16_t bpb_sec_per_trk;
    uint16_t bpb_num_heads;
    uint32_t bpb_hiddsec;
    uint32_t bpb_total_sector_32;
    uint32_t bpb_fatsz32;
    uint16_t bpb_ext_flags;
    uint16_t bpb_fsver;
    uint32_t bpb_root_clus;
    uint16_t bpb_fs_info;
    uint16_t bpb_bk_boot_sector;
    uint8_t bpb_reserved[12];
    uint8_t bs_drv_num;
    uint8_t bs_reserved1;
    uint8_t bs_boot_sig;
    uint32_t bs_vol_id;
    uint8_t bs_vol_label[11];
    uint8_t bs_fs_type[8];
    uint8_t bootcode[420];
    uint16_t end_flag;
} __attribute__((packed)) boot_sector;

typedef struct __fsinfo {
    uint32_t fsi_leadsig;
    uint8_t fsi_reserved1[480];
    uint32_t fsi_structsig;
    uint32_t fsi_free_count;
    uint32_t fsi_next_free;
    uint8_t fsi_reserved2[12];
    uint32_t fsi_trail_sig;
} __attribute__((packed)) FSInfo;

typedef struct __short_dir_entry {
    char dir_name[11];
    uint8_t dir_attr;
    uint8_t dir_ntres;
    uint8_t dir_crt_time_tenth;
    uint16_t dir_crt_time;
    uint16_t dir_crt_date;
    uint16_t dir_last_acc_date;
    uint16_t dir_fst_clus_hi;
    uint16_t dir_wrt_time;
    uint16_t dir_wrt_date;
    uint16_t dir_fst_clus_lo;
    uint32_t dir_file_size;
} __attribute((packed)) short_dir_entry;

typedef struct __long_dir_entry {
    uint8_t ldir_ord;
    wchar_t ldir_name_1[5];
    uint8_t ldir_attr;
    uint8_t ldir_type;
    uint8_t ldir_chksum;
    wchar_t ldir_name_2[6];
    uint16_t ldir_fst_clus_lo;
    wchar_t ldir_name_3[2];
} __attribute((packed)) long_dir_entry;

typedef struct __directory_entry {
    short_dir_entry short_entry;
    long_dir_entry long_entry;
} fat32_entry;

typedef struct __fat32 {
    boot_sector boot;
    FSInfo fs_info;
    uint32_t *fat1;
    uint32_t temp_fat[512];    //缓存4个扇区
    uint32_t fat1_start_sector;
    uint32_t start_fat_sector;
    uint32_t count;
    uint32_t data_sector;
    size_t (*write)(struct __fat32 *,const char *,const void *,size_t);
    size_t (*read)(struct __fat32 *,const char *,void *,size_t);
} fat32_fs;

typedef struct __part {
    uint8_t flag;
    uint8_t chs[3];
    uint8_t type;
    uint8_t end_chs[3];
    uint32_t lba;
    uint32_t count;
} __attribute((packed)) part;

typedef struct __mbr {
    uint8_t bootcode[446];
    part parts[4];
    uint16_t end_flag;
} __attribute__((packed)) mbr_struct;

fat32_fs *fat32_init(void);
//size_t fat32_write(fat32_fs *fat32,const char *path,const void *data,size_t size);
typedef struct __dentry dentry_struct;
size_t fat32_read(dentry_struct *p,void *buf,size_t size);
dentry_struct *fat32_open(const char *path);
#define FILE_NAME_END (0x40)
#define NO_CHAR ((wchar_t)0xffff)
#define FAT_MASK (0xfffffff)
#define FILE_END (0xffffff8)
#define is_short_entry(e) (!((e)->dir_attr == 0xf || (e)->dir_name[0] == 0xe5 || (e)->dir_name[0] == 0x00 || (e)->dir_name[0] == 0x05))
#define DEFAULT_LOAD_SECTOR 128
#define MAX_NAME_LEN 256
#define EMPTY_CLUSNO (0xf0000000)
#define IS_EMPTY_DIR_ENTRY(e) ((e)->dir_name[0] == 0xe5 || (e)->dir_name[0] == 0x00 || (e)->dir_name[0] == 0x05)
extern fat32_fs *fs;
#endif