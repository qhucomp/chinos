#ifndef __DEVICE_H
#define __DEVICE_H
#include "fat32.h"
#include "sdcard.h"

#define FAT32_FS 1
typedef struct __device {
    union {
        fat32_fs *fat32;
    }
    SD_CardInfo carinfo;
    int device_type;
} device_t;
#endif