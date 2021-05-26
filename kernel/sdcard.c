#include "include/sdcard.h"

#include "include/printk.h"

#include "include/fpioa.h"
#include "include/gpiohs.h"
#include "include/spi.h"
#include "include/sysctl.h"
#include "include/assert.h"
#include "include/dmac.h"
// /*
//  * @brief  Start Data tokens:
//  *         Tokens (necessary because at nop/idle (and CS active) only 0xff is
//  *         on the data/command line)
//  */
// #define SD_START_DATA_SINGLE_BLOCK_READ 0xFE /*!< Data token start byte, Start Single Block Read */
// #define SD_START_DATA_MULTIPLE_BLOCK_READ 0xFE /*!< Data token start byte, Start Multiple Block Read */
// #define SD_START_DATA_SINGLE_BLOCK_WRITE 0xFE /*!< Data token start byte, Start Single Block Write */
// #define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFC /*!< Data token start byte, Start Multiple Block Write */

// /*
//  * @brief  Commands: CMDxx = CMD-number | 0x40
//  */
// #define SD_CMD0 0    /*!< CMD0 = 0x40 */
// #define SD_CMD8 8    /*!< CMD8 = 0x48 */
// #define SD_CMD9 9    /*!< CMD9 = 0x49 */
// #define SD_CMD10 10  /*!< CMD10 = 0x4A */
// #define SD_CMD12 12  /*!< CMD12 = 0x4C */
// #define SD_CMD16 16  /*!< CMD16 = 0x50 */
// #define SD_CMD17 17  /*!< CMD17 = 0x51 */
// #define SD_CMD18 18  /*!< CMD18 = 0x52 */
// #define SD_ACMD23 23 /*!< CMD23 = 0x57 */
// #define SD_CMD24 24  /*!< CMD24 = 0x58 */
// #define SD_CMD25 25  /*!< CMD25 = 0x59 */
// #define SD_ACMD41 41 /*!< ACMD41 = 0x41 */
// #define SD_CMD55 55  /*!< CMD55 = 0x55 */
// #define SD_CMD58 58  /*!< CMD58 = 0x58 */
// #define SD_CMD59 59  /*!< CMD59 = 0x59 */

#undef SPI_CHIP_SELECT_3
#define SPI_CHIP_SELECT_3 0
#define SPI_DEVICE_INDEX SPI_DEVICE_0

// SD_CardInfo cardinfo;

// void SD_CS_HIGH(void) { gpiohs_set_pin(7, GPIO_PV_HIGH); }

// void SD_CS_LOW(void) { gpiohs_set_pin(7, GPIO_PV_LOW); }

// void SD_HIGH_SPEED_ENABLE(void) {
//   //spi_set_clk_rate(SPI_DEVICE_INDEX, 10000000);
// }

// static void sd_lowlevel_init() {
//   gpiohs_set_drive_mode(7, GPIO_DM_OUTPUT);
//   //spi_set_clk_rate(SPI_DEVICE_INDEX, 200000); /*set clk rate*/
// }

// static void sd_write_data(uint8_t *data_buff, uint32_t length) {
//   spi_init(SPI_DEVICE_INDEX, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
//   spi_send_data_standard(SPI_DEVICE_INDEX, SPI_CHIP_SELECT_3, NULL, 0,
//                          data_buff, length);
// }

// static void sd_read_data(uint8_t *data_buff, uint32_t length) {
//   spi_init(SPI_DEVICE_INDEX, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
//   spi_receive_data_standard(SPI_DEVICE_INDEX, SPI_CHIP_SELECT_3, NULL, 0,
//                             data_buff, length);
// }

// static void sd_write_data_dma(uint8_t *data_buff) {
//   spi_init(SPI_DEVICE_INDEX, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
//   spi_send_data_standard_dma(DMAC_CHANNEL0, SPI_DEVICE_INDEX, SPI_CHIP_SELECT_3,
//                              NULL, 0, (uint8_t *)(data_buff), 128 * 4);
// }

// static void sd_read_data_dma(uint8_t *data_buff) {
//   spi_init(SPI_DEVICE_INDEX, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
//   spi_receive_data_standard_dma(-1, DMAC_CHANNEL0, SPI_DEVICE_INDEX,
//                                 SPI_CHIP_SELECT_3, NULL, 0, data_buff, 128 * 4);
// }

// /*
//  * @brief  Send 5 bytes command to the SD card.
//  * @param  Cmd: The user expected command to send to SD card.
//  * @param  Arg: The command argument.
//  * @param  Crc: The CRC.
//  * @retval None
//  */
// static void sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
//   uint8_t frame[6];
//   /*!< Construct byte 1 */
//   frame[0] = (cmd | 0x40);
//   /*!< Construct byte 2 */
//   frame[1] = (uint8_t)(arg >> 24);
//   /*!< Construct byte 3 */
//   frame[2] = (uint8_t)(arg >> 16);
//   /*!< Construct byte 4 */
//   frame[3] = (uint8_t)(arg >> 8);
//   /*!< Construct byte 5 */
//   frame[4] = (uint8_t)(arg);
//   /*!< Construct CRC: byte 6 */
//   frame[5] = (crc);
//   /*!< SD chip select low */
//   SD_CS_LOW();
//   /*!< Send the Cmd bytes */
//   sd_write_data(frame, 6);
// }

// /*
//  * @brief  Send 5 bytes command to the SD card.
//  * @param  Cmd: The user expected command to send to SD card.
//  * @param  Arg: The command argument.
//  * @param  Crc: The CRC.
//  * @retval None
//  */
// static void sd_end_cmd(void) {
//   uint8_t frame[1] = {0xFF};
//   /*!< SD chip select high */
//   SD_CS_HIGH();
//   /*!< Send the Cmd bytes */
//   sd_write_data(frame, 1);
// }

// /*
//  * @brief  Returns the SD response.
//  * @param  None
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// static uint8_t sd_get_response(void) {
//   uint8_t result;
//   uint16_t timeout = 0xFF;
//   /*!< Check if response is got or a timeout is happen */
//   while (timeout--) {
//     sd_read_data(&result, 1);
//     /*!< Right response got */
//     if (result != 0xFF) return result;
//   }
//   /*!< After time out */
//   return 0xFF;
// }

// /*
//  * @brief  Get SD card data response.
//  * @param  None
//  * @retval The SD status: Read data response xxx0<status>1
//  *         - status 010: Data accecpted
//  *         - status 101: Data rejected due to a crc error
//  *         - status 110: Data rejected due to a Write error.
//  *         - status 111: Data rejected due to other error.
//  */
// static uint8_t sd_get_dataresponse(void) {
//   uint8_t response;
//   /*!< Read resonse */
//   sd_read_data(&response, 1);
//   /*!< Mask unused bits */
//   response &= 0x1F;
//   if (response != 0x05) return 0xFF;
//   /*!< Wait null data */
//   sd_read_data(&response, 1);
//   while (response == 0) sd_read_data(&response, 1);
//   /*!< Return response */
//   return 0;
// }

// /*
//  * @brief  Read the CSD card register
//  *         Reading the contents of the CSD register in SPI mode is a simple
//  *         read-block transaction.
//  * @param  SD_csd: pointer on an SCD register structure
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// static uint8_t sd_get_csdregister(SD_CSD *SD_csd) {
//   uint8_t csd_tab[18];
//   /*!< Send CMD9 (CSD register) or CMD10(CSD register) */
//   sd_send_cmd(SD_CMD9, 0, 0);
//   /*!< Wait for response in the R1 format (0x00 is no errors) */
//   if (sd_get_response() != 0x00) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   if (sd_get_response() != SD_START_DATA_SINGLE_BLOCK_READ) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   /*!< Store CSD register value on csd_tab */
//   /*!< Get CRC bytes (not really needed by us, but required by SD) */
//   sd_read_data(csd_tab, 18);
//   sd_end_cmd();
//   /*!< Byte 0 */
//   SD_csd->CSDStruct = (csd_tab[0] & 0xC0) >> 6;
//   SD_csd->SysSpecVersion = (csd_tab[0] & 0x3C) >> 2;
//   SD_csd->Reserved1 = csd_tab[0] & 0x03;
//   /*!< Byte 1 */
//   SD_csd->TAAC = csd_tab[1];
//   /*!< Byte 2 */
//   SD_csd->NSAC = csd_tab[2];
//   /*!< Byte 3 */
//   SD_csd->MaxBusClkFrec = csd_tab[3];
//   /*!< Byte 4 */
//   SD_csd->CardComdClasses = csd_tab[4] << 4;
//   /*!< Byte 5 */
//   SD_csd->CardComdClasses |= (csd_tab[5] & 0xF0) >> 4;
//   SD_csd->RdBlockLen = csd_tab[5] & 0x0F;
//   /*!< Byte 6 */
//   SD_csd->PartBlockRead = (csd_tab[6] & 0x80) >> 7;
//   SD_csd->WrBlockMisalign = (csd_tab[6] & 0x40) >> 6;
//   SD_csd->RdBlockMisalign = (csd_tab[6] & 0x20) >> 5;
//   SD_csd->DSRImpl = (csd_tab[6] & 0x10) >> 4;
//   SD_csd->Reserved2 = 0; /*!< Reserved */
//   SD_csd->DeviceSize = (csd_tab[6] & 0x03) << 10;
//   /*!< Byte 7 */
//   SD_csd->DeviceSize = (csd_tab[7] & 0x3F) << 16;
//   /*!< Byte 8 */
//   SD_csd->DeviceSize |= csd_tab[8] << 8;
//   /*!< Byte 9 */
//   SD_csd->DeviceSize |= csd_tab[9];
//   /*!< Byte 10 */
//   SD_csd->EraseGrSize = (csd_tab[10] & 0x40) >> 6;
//   SD_csd->EraseGrMul = (csd_tab[10] & 0x3F) << 1;
//   /*!< Byte 11 */
//   SD_csd->EraseGrMul |= (csd_tab[11] & 0x80) >> 7;
//   SD_csd->WrProtectGrSize = (csd_tab[11] & 0x7F);
//   /*!< Byte 12 */
//   SD_csd->WrProtectGrEnable = (csd_tab[12] & 0x80) >> 7;
//   SD_csd->ManDeflECC = (csd_tab[12] & 0x60) >> 5;
//   SD_csd->WrSpeedFact = (csd_tab[12] & 0x1C) >> 2;
//   SD_csd->MaxWrBlockLen = (csd_tab[12] & 0x03) << 2;
//   /*!< Byte 13 */
//   SD_csd->MaxWrBlockLen |= (csd_tab[13] & 0xC0) >> 6;
//   SD_csd->WriteBlockPaPartial = (csd_tab[13] & 0x20) >> 5;
//   SD_csd->Reserved3 = 0;
//   SD_csd->ContentProtectAppli = (csd_tab[13] & 0x01);
//   /*!< Byte 14 */
//   SD_csd->FileFormatGrouop = (csd_tab[14] & 0x80) >> 7;
//   SD_csd->CopyFlag = (csd_tab[14] & 0x40) >> 6;
//   SD_csd->PermWrProtect = (csd_tab[14] & 0x20) >> 5;
//   SD_csd->TempWrProtect = (csd_tab[14] & 0x10) >> 4;
//   SD_csd->FileFormat = (csd_tab[14] & 0x0C) >> 2;
//   SD_csd->ECC = (csd_tab[14] & 0x03);
//   /*!< Byte 15 */
//   SD_csd->CSD_CRC = (csd_tab[15] & 0xFE) >> 1;
//   SD_csd->Reserved4 = 1;
//   /*!< Return the reponse */
//   return 0;
// }

// /*
//  * @brief  Read the CID card register.
//  *         Reading the contents of the CID register in SPI mode is a simple
//  *         read-block transaction.
//  * @param  SD_cid: pointer on an CID register structure
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// static uint8_t sd_get_cidregister(SD_CID *SD_cid) {
//   uint8_t cid_tab[18];
//   /*!< Send CMD10 (CID register) */
//   sd_send_cmd(SD_CMD10, 0, 0);
//   /*!< Wait for response in the R1 format (0x00 is no errors) */
//   if (sd_get_response() != 0x00) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   if (sd_get_response() != SD_START_DATA_SINGLE_BLOCK_READ) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   /*!< Store CID register value on cid_tab */
//   /*!< Get CRC bytes (not really needed by us, but required by SD) */
//   sd_read_data(cid_tab, 18);
//   sd_end_cmd();
//   /*!< Byte 0 */
//   SD_cid->ManufacturerID = cid_tab[0];
//   /*!< Byte 1 */
//   SD_cid->OEM_AppliID = cid_tab[1] << 8;
//   /*!< Byte 2 */
//   SD_cid->OEM_AppliID |= cid_tab[2];
//   /*!< Byte 3 */
//   SD_cid->ProdName1 = cid_tab[3] << 24;
//   /*!< Byte 4 */
//   SD_cid->ProdName1 |= cid_tab[4] << 16;
//   /*!< Byte 5 */
//   SD_cid->ProdName1 |= cid_tab[5] << 8;
//   /*!< Byte 6 */
//   SD_cid->ProdName1 |= cid_tab[6];
//   /*!< Byte 7 */
//   SD_cid->ProdName2 = cid_tab[7];
//   /*!< Byte 8 */
//   SD_cid->ProdRev = cid_tab[8];
//   /*!< Byte 9 */
//   SD_cid->ProdSN = cid_tab[9] << 24;
//   /*!< Byte 10 */
//   SD_cid->ProdSN |= cid_tab[10] << 16;
//   /*!< Byte 11 */
//   SD_cid->ProdSN |= cid_tab[11] << 8;
//   /*!< Byte 12 */
//   SD_cid->ProdSN |= cid_tab[12];
//   /*!< Byte 13 */
//   SD_cid->Reserved1 |= (cid_tab[13] & 0xF0) >> 4;
//   SD_cid->ManufactDate = (cid_tab[13] & 0x0F) << 8;
//   /*!< Byte 14 */
//   SD_cid->ManufactDate |= cid_tab[14];
//   /*!< Byte 15 */
//   SD_cid->CID_CRC = (cid_tab[15] & 0xFE) >> 1;
//   SD_cid->Reserved2 = 1;
//   /*!< Return the reponse */
//   return 0;
// }

// /*
//  * @brief  Returns information about specific card.
//  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD
//  *         card information.
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// static uint8_t sd_get_cardinfo(SD_CardInfo *cardinfo) {
//   if (sd_get_csdregister(&(cardinfo->SD_csd))) return 0xFF;
//   if (sd_get_cidregister(&(cardinfo->SD_cid))) return 0xFF;
//   cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 1024;
//   cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
//   cardinfo->CardCapacity *= cardinfo->CardBlockSize;
//   /*!< Returns the reponse */
//   return 0;
// }

// /*
//  * @brief  Initializes the SD/SD communication.
//  * @param  None
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// uint8_t sd_init(void) {
//   uint8_t frame[10], index, result;
//   /*!< Initialize SD_SPI */
//   sd_lowlevel_init(0);
//   /*!< SD chip select high */
//   SD_CS_HIGH();
//   /*!< Send dummy byte 0xFF, 10 times with CS high */
//   /*!< Rise CS and MOSI for 80 clocks cycles */
//   /*!< Send dummy byte 0xFF */
//   for (index = 0; index < 10; index++) frame[index] = 0xFF;
//   sd_write_data(frame, 10);
//   /*------------Put SD in SPI mode--------------*/
//   /*!< SD initialized and set to SPI mode properly */

//   index = 0xFF;
//   while (index--) {
//     sd_send_cmd(SD_CMD0, 0, 0x95);
//     result = sd_get_response();
//     sd_end_cmd();
//     if (result == 0x01) break;
//   }
//   if (index == 0) {
//     printk("SD_CMD0 is %X\n", result);
//     return 0xFF;
//   }

//   sd_send_cmd(SD_CMD8, 0x01AA, 0x87);
//   /*!< 0x01 or 0x05 */
//   result = sd_get_response();
//   sd_read_data(frame, 4);
//   sd_end_cmd();
//   if (result != 0x01) {
//     printk("SD_CMD8 is %X\n", result);
//     return 0xFF;
//   }
//   index = 0xFF;
//   while (index--) {
//     sd_send_cmd(SD_CMD55, 0, 0);
//     result = sd_get_response();
//     sd_end_cmd();
//     if (result != 0x01) return 0xFF;
//     sd_send_cmd(SD_ACMD41, 0x40000000, 0);
//     result = sd_get_response();
//     sd_end_cmd();
//     if (result == 0x00) break;
//   }
//   if (index == 0) {
//     printk("SD_CMD55 is %X\n", result);
//     return 0xFF;
//   }
//   index = 255;
//   while (index--) {
//     sd_send_cmd(SD_CMD58, 0, 1);
//     result = sd_get_response();
//     sd_read_data(frame, 4);
//     sd_end_cmd();
//     if (result == 0) {
//       break;
//     }
//   }
//   if (index == 0) {
//     printk("SD_CMD58 is %X\n", result);
//     return 0xFF;
//   }
//   if ((frame[0] & 0x40) == 0) return 0xFF;
//   SD_HIGH_SPEED_ENABLE();
//   return sd_get_cardinfo(&cardinfo);
// }

// /*
//  * @brief  Reads a block of data from the SD.
//  * @param  data_buff: pointer to the buffer that receives the data read from the
//  *                  SD.
//  * @param  sector: SD's internal address to read from.
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// uint8_t sd_read_sector(uint8_t *data_buff, uint32_t sector, uint32_t count) {
//   uint8_t frame[2], flag;
//   /*!< Send CMD17 (SD_CMD17) to read one block */
//   if (count == 1) {
//     flag = 0;
//     sd_send_cmd(SD_CMD17, sector, 0);
//   } else {
//     flag = 1;
//     sd_send_cmd(SD_CMD18, sector, 0);
//   }
//   /*!< Check if the SD acknowledged the read block command: R1 response (0x00:
//    * no errors) */
//   if (sd_get_response() != 0x00) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   while (count) {
//     if (sd_get_response() != SD_START_DATA_SINGLE_BLOCK_READ) break;
//     /*!< Read the SD block data : read NumByteToRead data */
//     sd_read_data_dma(data_buff);
//     /*!< Get CRC bytes (not really needed by us, but required by SD) */
//     sd_read_data(frame, 2);
//     data_buff += 512;
//     count--;
//   }
//   sd_end_cmd();
//   if (flag) {
//     sd_send_cmd(SD_CMD12, 0, 0);
//     sd_get_response();
//     sd_end_cmd();
//     sd_end_cmd();
//   }
//   /*!< Returns the reponse */
//   return count > 0 ? 0xFF : 0;
// }

// // uint8_t sd_read_sector(uint8_t *buf, uint32_t sectorno) {
// // 	uint8_t result;
// // 	uint32_t address = sectorno;
// // 	uint8_t dummy_crc[2];

// // 	// if (is_standard_sd) {
// // 	// 	address = sectorno << 9;
// // 	// }
// // 	// else {
// // 	// 	address = sectorno;
// // 	// }

// // 	// // enter critical section!
// // 	// acquiresleep(&sdcard_lock);

// // 	sd_send_cmd(SD_CMD17, address, 0);
// // 	result = sd_get_response();

// // 	if (0 != result) {
// // 		//releasesleep(&sdcard_lock);
// // 		printk("sdcard: fail to read");
// //         return 1;
// // 	}

// // 	int timeout = 0xffffff;
// // 	while (--timeout) {
// // 		sd_read_data(&result, 1);
// // 		if (0xfe == result) break;
// // 	}
// // 	if (0 == timeout) {
// // 		printk("sdcard: timeout waiting for reading");
// //         return 1;
// // 	}
// // 	sd_read_data_dma(buf);
// // 	sd_read_data(dummy_crc, 2);

// // 	sd_end_cmd();
// //     return 0;
// // 	//releasesleep(&sdcard_lock);
// // 	// leave critical section!
// // }

// /*
//  * @brief  Writes a block on the SD
//  * @param  data_buff: pointer to the buffer containing the data to be written on
//  *                  the SD.
//  * @param  sector: address to write on.
//  * @retval The SD Response:
//  *         - 0xFF: Sequence failed
//  *         - 0: Sequence succeed
//  */
// uint8_t sd_write_sector(uint8_t *data_buff, uint32_t sector, uint32_t count) {
//   uint8_t frame[2] = {0xFF};

//   if (count == 1) {
//     frame[1] = SD_START_DATA_SINGLE_BLOCK_WRITE;
//     sd_send_cmd(SD_CMD24, sector, 0);
//   } else {
//     frame[1] = SD_START_DATA_MULTIPLE_BLOCK_WRITE;
//     sd_send_cmd(SD_ACMD23, count, 0);
//     sd_get_response();
//     sd_end_cmd();
//     sd_send_cmd(SD_CMD25, sector, 0);
//   }
//   /*!< Check if the SD acknowledged the write block command: R1 response (0x00:
//    * no errors) */
//   if (sd_get_response() != 0x00) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   while (count--) {
//     /*!< Send the data token to signify the start of the data */
//     sd_write_data(frame, 2);
//     /*!< Write the block data to SD : write count data by block */
//     sd_write_data_dma(data_buff);
//     /*!< Put CRC bytes (not really needed by us, but required by SD) */
//     sd_write_data(frame, 2);
//     data_buff += 512;
//     /*!< Read data response */
//     if (sd_get_dataresponse() != 0x00) {
//       sd_end_cmd();
//       return 0xFF;
//     }
//   }
//   sd_end_cmd();
//   sd_end_cmd();
//   /*!< Returns the reponse */

//   printk("call sd_write_sector()");
//   return 0;
// }

// uint8_t sd_read_sector_dma(uint8_t *data_buff, uint32_t sector,
//                            uint32_t count) {
//   uint8_t frame[2], flag;

//   /*!< Send CMD17 (SD_CMD17) to read one block */
//   if (count == 1) {
//     flag = 0;
//     sd_send_cmd(SD_CMD17, sector, 0);
//   } else {
//     flag = 1;
//     sd_send_cmd(SD_CMD18, sector, 0);
//   }
//   /*!< Check if the SD acknowledged the read block command: R1 response (0x00:
//    * no errors) */
//   if (sd_get_response() != 0x00) {
//     sd_end_cmd();
//     return 0xFF;
//   }
//   while (count) {
//     if (sd_get_response() != SD_START_DATA_SINGLE_BLOCK_READ) break;
//     /*!< Read the SD block data : read NumByteToRead data */
//     sd_read_data_dma(data_buff);
//     /*!< Get CRC bytes (not really needed by us, but required by SD) */
//     sd_read_data(frame, 2);
//     data_buff += 512;
//     count--;
//   }
//   sd_end_cmd();
//   if (flag) {
//     sd_send_cmd(SD_CMD12, 0, 0);
//     sd_get_response();
//     sd_end_cmd();
//     sd_end_cmd();
//   }
//   /*!< Returns the reponse */
//   return count > 0 ? 0xFF : 0;
// }

// uint8_t sd_write_sector_dma(uint8_t *data_buff, uint32_t sector,
//                             uint32_t count) {
//   uint8_t frame[2] = {0xFF};
//   frame[1] = SD_START_DATA_SINGLE_BLOCK_WRITE;
//   uint32_t i = 0;
//   while (count--) {
//     sd_send_cmd(SD_CMD24, sector + i, 0);
//     /*!< Check if the SD acknowledged the write block command: R1 response
//      * (0x00: no errors) */
//     if (sd_get_response() != 0x00) {
//       sd_end_cmd();
//       return 0xFF;
//     }

//     /*!< Send the data token to signify the start of the data */
//     sd_write_data(frame, 2);
//     /*!< Write the block data to SD : write count data by block */
//     sd_write_data_dma(data_buff);
//     /*!< Put CRC bytes (not really needed by us, but required by SD) */
//     sd_write_data(frame, 2);
//     data_buff += 512;
//     /*!< Read data response */
//     if (sd_get_dataresponse() != 0x00) {
//       sd_end_cmd();
//       return 0xFF;
//     }
//     i++;
//   }
//   sd_end_cmd();
//   sd_end_cmd();
//   /*!< Returns the reponse */
//   return 0;
// }


#define BSIZE 512
void SD_CS_HIGH(void) {
    gpiohs_set_pin(7, GPIO_PV_HIGH);
}

void SD_CS_LOW(void) {
    gpiohs_set_pin(7, GPIO_PV_LOW);
}

void SD_HIGH_SPEED_ENABLE(void) {
    // spi_set_clk_rate(SPI_DEVICE_0, 10000000);
}

static void sd_lowlevel_init(uint8_t spi_index) {
    gpiohs_set_drive_mode(7, GPIO_DM_OUTPUT);
    // spi_set_clk_rate(SPI_DEVICE_0, 200000);     /*set clk rate*/
}

static void sd_write_data(uint8_t const *data_buff, uint32_t length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
    spi_send_data_standard(SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
}

static void sd_read_data(uint8_t *data_buff, uint32_t length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
    spi_receive_data_standard(SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
}

static void sd_write_data_dma(uint8_t const *data_buff, uint32_t length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
	spi_send_data_standard_dma(DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
}

// static void sd_read_data_dma(uint8_t *data_buff, uint32_t length) {
//     spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
// 	spi_receive_data_standard_dma(-1, DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
// }

/*
 * @brief  Send 5 bytes command to the SD card.
 * @param  Cmd: The user expected command to send to SD card.
 * @param  Arg: The command argument.
 * @param  Crc: The CRC.
 * @retval None
 */
static void sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
	uint8_t frame[6];
	frame[0] = (cmd | 0x40);
	frame[1] = (uint8_t)(arg >> 24);
	frame[2] = (uint8_t)(arg >> 16);
	frame[3] = (uint8_t)(arg >> 8);
	frame[4] = (uint8_t)(arg);
	frame[5] = (crc);
	SD_CS_LOW();
	sd_write_data(frame, 6);
}

static void sd_end_cmd(void) {
	uint8_t frame[1] = {0xFF};
	/*!< SD chip select high */
	SD_CS_HIGH();
	/*!< Send the Cmd bytes */
	sd_write_data(frame, 1);
}

/*
 * Be noticed: all commands & responses below 
 * 		are in SPI mode format. May differ from 
 * 		what they are in SD mode. 
 */

#define SD_CMD0 	0 
#define SD_CMD8 	8
#define SD_CMD58 	58 		// READ_OCR
#define SD_CMD55 	55 		// APP_CMD
#define SD_ACMD41 	41 		// SD_SEND_OP_COND
#define SD_CMD16 	16 		// SET_BLOCK_SIZE 
#define SD_CMD17 	17 		// READ_SINGLE_BLOCK
#define SD_CMD24 	24 		// WRITE_SINGLE_BLOCK 
#define SD_CMD13 	13 		// SEND_STATUS

/*
 * Read sdcard response in R1 type. 
 */
static uint8_t sd_get_response_R1(void) {
	uint8_t result;
	uint16_t timeout = 0xff;

	while (timeout--) {
		sd_read_data(&result, 1);
		if (result != 0xff)
			return result;
	}

	// timeout!
	return 0xff;
}

/* 
 * Read the rest of R3 response 
 * Be noticed: frame should be at least 4-byte long 
 */
static void sd_get_response_R3_rest(uint8_t *frame) {
	sd_read_data(frame, 4);
}

/* 
 * Read the rest of R7 response 
 * Be noticed: frame should be at least 4-byte long 
 */
static void sd_get_response_R7_rest(uint8_t *frame) {
	sd_read_data(frame, 4);
}

static int switch_to_SPI_mode(void) {
	int timeout = 0xff;

	while (--timeout) {
		sd_send_cmd(SD_CMD0, 0, 0x95);
		uint64_t result = sd_get_response_R1();
		sd_end_cmd();

		if (0x01 == result) break;
	}
	if (0 == timeout) {
		printk("SD_CMD0 failed\n");
		return 0xff;
	}

	return 0;
}

// verify supply voltage range 
static int verify_operation_condition(void) {
	uint64_t result;

	// Stores the response reversely. 
	// That means 
	// frame[2] - VCA 
	// frame[3] - Check Pattern 
	uint8_t frame[4];

	sd_send_cmd(SD_CMD8, 0x01aa, 0x87);
	result = sd_get_response_R1();
	sd_get_response_R7_rest(frame);
	sd_end_cmd();

	if (0x09 == result) {
		printk("invalid CRC for CMD8\n");
		return 0xff;
	}
	else if (0x01 == result && 0x01 == (frame[2] & 0x0f) && 0xaa == frame[3]) {
		return 0x00;
	}

	printk("verify_operation_condition() fail!\n");
	return 0xff;
}

// read OCR register to check if the voltage range is valid 
// this step is not mandotary, but I advise to use it 
static int read_OCR(void) {
	uint64_t result = 1;
	uint8_t ocr[4];

	int timeout;

	timeout = 0xff;
	while (--timeout) {
		sd_send_cmd(SD_CMD58, 0, 0);
		result = sd_get_response_R1();
		sd_get_response_R3_rest(ocr);
		sd_end_cmd();

		if (
			0x01 == result && // R1 response in idle status 
			(ocr[1] & 0x1f) && (ocr[2] & 0x80) 	// voltage range valid 
		) {
			return 0;
		}
	}

	// timeout!
	printk("read_OCR() timeout!\n");
	printk("result = %d\n", result);
	return 0xff;
}

// send ACMD41 to tell sdcard to finish initializing 
static int set_SDXC_capacity(void) {
	uint8_t result = 0xff;

	int timeout = 0xfff;
	while (--timeout) {
		sd_send_cmd(SD_CMD55, 0, 0);
		result = sd_get_response_R1();
		sd_end_cmd();
		if (0x01 != result) {
			printk("SD_CMD55 fail! result = %d\n", result);
			return 0xff;
		}

		sd_send_cmd(SD_ACMD41, 0x40000000, 0);
		result = sd_get_response_R1();
		sd_end_cmd();
		if (0 == result) {
			return 0;
		}
	}

	// timeout! 
	printk("set_SDXC_capacity() timeout!\n");
	printk("result = %d\n", result);
	return 0xff;
}

// Used to differ whether sdcard is SDSC type. 
static int is_standard_sd = 0;

// check OCR register to see the type of sdcard, 
// thus determine whether block size is suitable to buffer size
static int check_block_size(void) {
	uint8_t result = 0xff;
	uint8_t ocr[4];

	int timeout = 0xff;
	while (timeout --) {
		sd_send_cmd(SD_CMD58, 0, 0);
		result = sd_get_response_R1();
		sd_get_response_R3_rest(ocr);
		sd_end_cmd();

		if (0 == result) {
			if (ocr[0] & 0x40) {
				printk("SDHC/SDXC detected\n");
				if (512 != BSIZE) {
					printk("BSIZE != 512\n");
					return 0xff;
				}

				is_standard_sd = 0;
			}
			else {
				printk("SDSC detected, setting block size\n");

				// setting SD card block size to BSIZE 
				int timeout = 0xff;
				int result = 0xff;
				while (--timeout) {
					sd_send_cmd(SD_CMD16, BSIZE, 0);
					result = sd_get_response_R1();
					sd_end_cmd();

					if (0 == result) break;
				}
				if (0 == timeout) {
					printk("check_OCR(): fail to set block size");
					return 0xff;
				}

				is_standard_sd = 1;
			}

			return 0;
		}
	}

	// timeout! 
	printk("check_OCR() timeout!\n");
	printk("result = %d\n", result);
	return 0xff;
}

/*
 * @brief  Initializes the SD/SD communication.
 * @param  None
 * @retval The SD Response:
 *         - 0xFF: Sequence failed
 *         - 0: Sequence succeed
 */
uint8_t sd_init(void) {
	uint8_t frame[10];

	sd_lowlevel_init(0);
	//SD_CS_HIGH();
	SD_CS_LOW();

	// send dummy bytes for 80 clock cycles 
	for (int i = 0; i < 10; i ++) 
		frame[i] = 0xff;
	sd_write_data(frame, 10);

	if (0 != switch_to_SPI_mode()) 
		return 0xff;
	if (0 != verify_operation_condition()) 
		return 0xff;
	if (0 != read_OCR()) 
		return 0xff;
	if (0 != set_SDXC_capacity()) 
		return 0xff;
	if (0 != check_block_size()) 
		return 0xff;

	return 0;
}

//static struct sleeplock sdcard_lock;

void sdcard_init(void) {
	int result = sd_init();
	//initsleeplock(&sdcard_lock, "sdcard");

	if (0 != result) {
		printk("sdcard_init failed");
	}
	#ifdef DEBUG
	printk("sdcard_init\n");
	#endif
}

uint8_t sd_read_sector(uint8_t *buf, uint32_t sectorno) {
	uint8_t result;
	uint32_t address;
	uint8_t dummy_crc[2];

	#ifdef DEBUG
	printk("sdcard_read_sector()\n");
	#endif

	if (is_standard_sd) {
		address = sectorno << 9;
	}
	else {
		address = sectorno;
	}

	// enter critical section!
	//acquiresleep(&sdcard_lock);

	sd_send_cmd(SD_CMD17, address, 0);
	result = sd_get_response_R1();

	if (0 != result) {
		//releasesleep(&sdcard_lock);
		printk("sdcard: fail to read");
        return 1;
	}

	int timeout = 0xffffff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0xfe == result) break;
	}
	if (0 == timeout) {
		printk("sdcard: timeout waiting for reading");
        return 1;
	}
	sd_read_data(buf, BSIZE);
	sd_read_data(dummy_crc, 2);

	sd_end_cmd();
    return 0;
	//releasesleep(&sdcard_lock);
	// leave critical section!
}

uint8_t sd_write_sector(uint8_t *buf, uint32_t sectorno) {
	uint32_t address;
	static uint8_t const START_BLOCK_TOKEN = 0xfe;
	uint8_t dummy_crc[2] = {0xff, 0xff};

	#ifdef DEBUG
	printk("sdcard_write_sector()\n");
	#endif

	if (is_standard_sd) {
		address = sectorno << 9;
	}
	else {
		address = sectorno;
	}

	// enter critical section!
	//acquiresleep(&sdcard_lock);

	sd_send_cmd(SD_CMD24, address, 0);
	if (0 != sd_get_response_R1()) {
		//releasesleep(&sdcard_lock);
		printk("sdcard: fail to write");
	}

	// sending data to be written 
	sd_write_data(&START_BLOCK_TOKEN, 1);
	sd_write_data_dma(buf, BSIZE);
	sd_write_data(dummy_crc, 2);

	// waiting for sdcard to finish programming 
	uint8_t result;
	int timeout = 0xfff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0x05 == (result & 0x1f)) {
			break;
		}
	}
	if (0 == timeout) {
		//releasesleep(&sdcard_lock);
		printk("sdcard: invalid response token");
	}
	
	timeout = 0xffffff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0 != result) break;
	}
	if (0 == timeout) {
		//releasesleep(&sdcard_lock);
		printk("sdcard: timeout waiting for response");
	}
	sd_end_cmd();

	// send SD_CMD13 to check if writing is correctly done 
	uint8_t error_code = 0xff;
	sd_send_cmd(SD_CMD13, 0, 0);
	result = sd_get_response_R1();
	sd_read_data(&error_code, 1);
	sd_end_cmd();
	if (0 != result || 0 != error_code) {
		//releasesleep(&sdcard_lock);
		printk("result: %x\n", result);
		printk("error_code: %x\n", error_code);
		printk("sdcard: an error occurs when writing");
	}
	return result;
	//releasesleep(&sdcard_lock);
	// leave critical section!
}