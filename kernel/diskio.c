/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "include/diskio.h" /* FatFs lower layer API */
#include "include/sdcard.h"
#include "include/printk.h"
#include "include/fpioa.h"
#include "include/dmac.h"
/* Definitions of physical drive number for each drive */
#define M0 0 /* Example: Map MMC/SD card to physical drive 0 */
int rw_count;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(uint8_t pdrv)
{
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(uint8_t pdrv)
{
    if(sd_init() == 0)
        return 0;
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(uint8_t pdrv, uint8_t *buff, uint32_t sector, uint32_t count)
{
    if (buff == NULL) {
        printk("Error\n");
        return RES_ERROR;
    }
    int repeat_count;
    for(int c = 0;c < count;c++) {
        // if (rw_count >= 5000) {
        //     disk_init();
        //     rw_count = 0;
        // }
        repeat_count = 0xff;
start:
        if(sd_read_sector(buff + c*512,sector + c)) {
            disk_init();
            //printk("count:%d\n",repeat_count);
            if (repeat_count) {
                repeat_count -= 1;
                goto start;
            }
            return RES_ERROR;
        }
        rw_count++;
        //printk("rw_count:%d read %d address:%p\n",rw_count,c,buff + c*512);
    }
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(uint8_t pdrv, uint8_t *buff, uint32_t sector, uint32_t count)
{
    if (buff == NULL) {
        printk("Error\n");
        return RES_ERROR;
    }
    int repeat_count;
    for(int c = 0;c < count;c++) {
        repeat_count = 0xff;
disk_repeat:
        if(sd_write_sector(buff + c*512,sector + c)) {
            disk_init();
            if (repeat_count) {
                repeat_count -= 1;
                goto disk_repeat;
            }
            return RES_ERROR;
        }
        rw_count++;
    }
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

// DRESULT disk_ioctl(uint8_t pdrv, uint8_t cmd, void *buff)
// {
//     DRESULT res = RES_ERROR;

//     switch(cmd)
//     {
//         /* Make sure that no pending write process */
//         case CTRL_SYNC:
//             res = RES_OK;
//             break;
//         /* Get number of sectors on the disk (uint32_t) */
//         case GET_SECTOR_COUNT:
//             *(uint32_t *)buff = (cardinfo.SD_csd.DeviceSize + 1) << 10;
//             res = RES_OK;
//             break;
//         /* Get R/W sector size (WORD) */
//         case GET_SECTOR_SIZE:
//             *(uint32_t *)buff = cardinfo.CardBlockSize;
//             res = RES_OK;
//             break;
//         /* Get erase block size in unit of sector (uint32_t) */
//         case GET_BLOCK_SIZE:
//             *(uint32_t *)buff = cardinfo.CardBlockSize;
//             res = RES_OK;
//             break;
//         default:
//             res = RES_PARERR;
//     }
//     return res;
// }

DRESULT disk_init(void) {
    uint8_t status = sd_init();
    if (status != 0) {
        panic("sd init error");
        return RES_ERROR;
    }
//    printk("card info status %d\n", status);
//    printk("CardCapacity:%ld\n", cardinfo.CardCapacity);
//    printk("CardBlockSize:%d\n", cardinfo.CardBlockSize);
  return RES_OK;
}