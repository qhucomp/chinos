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
/* Definitions of physical drive number for each drive */
#define M0 0 /* Example: Map MMC/SD card to physical drive 0 */

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
    if(sd_read_sector(buff, sector, count) == 0)
        return RES_OK;
    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(uint8_t pdrv, const uint8_t *buff, uint32_t sector, uint32_t count)
{
    if(sd_write_sector((uint8_t *)buff, sector, count) == 0)
        return RES_OK;
    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(uint8_t pdrv, uint8_t cmd, void *buff)
{
    DRESULT res = RES_ERROR;

    switch(cmd)
    {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = RES_OK;
            break;
        /* Get number of sectors on the disk (uint32_t) */
        case GET_SECTOR_COUNT:
            *(uint32_t *)buff = (cardinfo.SD_csd.DeviceSize + 1) << 10;
            res = RES_OK;
            break;
        /* Get R/W sector size (WORD) */
        case GET_SECTOR_SIZE:
            *(uint32_t *)buff = cardinfo.CardBlockSize;
            res = RES_OK;
            break;
        /* Get erase block size in unit of sector (uint32_t) */
        case GET_BLOCK_SIZE:
            *(uint32_t *)buff = cardinfo.CardBlockSize;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
    }
    return res;
}

DRESULT disk_init(void) {
    printk("/******************sdcard test*****************/\n");
    uint8_t status = sd_init();
    if (status != 0) {
        printk("sd init %d",status);
        return RES_ERROR;
    }
   printk("card info status %d\n", status);
   printk("CardCapacity:%ld\n", cardinfo.CardCapacity);
   printk("CardBlockSize:%d\n", cardinfo.CardBlockSize);
  return RES_OK;
}