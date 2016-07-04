/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    FS_MMC.C 
 *      Purpose: MultiMedia and SD Flash Card Interface
 *      Rev.:    V4.12
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "File_Config.h"

/* SD/MMC Commands */
#define GO_IDLE_STATE    (0x40 + 0)
#define SEND_OP_COND     (0x40 + 1)
#define SEND_IF_COND     (0x40 + 8)
#define SEND_CSD         (0x40 + 9)
#define SEND_CID         (0x40 + 10)
#define STOP_TRAN        (0x40 + 12)
#define SET_BLOCKLEN     (0x40 + 16)
#define READ_BLOCK       (0x40 + 17)
#define READ_MULT_BLOCK  (0x40 + 18)
#define WRITE_BLOCK      (0x40 + 24)
#define WRITE_MULT_BLOCK (0x40 + 25)
#define SD_SEND_OP_COND  (0x40 + 41)
#define APP_CMD          (0x40 + 55)
#define READ_OCR         (0x40 + 58)
#define CRC_ON_OFF       (0x40 + 59)

/* Wait timeouts, in multiples of 1 byte send over SPI */
#define WR_TOUT           500000        /* ~ 200 ms with SPI clk 20MHz */
#define STOP_TOUT         125000        /* ~  50 ms with SPI clk 20MHz */
#define CMD_TOUT          2500          /* ~   1 ms with SPI clk 20MHz */

/* Local variables */
static U8 CardType;

/*----------------------------------------------------------------------------
 *      MMC Driver Functions
 *----------------------------------------------------------------------------
 *  Required functions for MMC driver module:
 *   - BOOL mmc_init        ()
 *   - BOOL mmc_read_sect   (U32 sect, U8 *buf, U32 cnt)
 *   - BOOL mmc_write_sect  (U32 sect, U8 *buf, U32 cnt)
 *   - BOOL mmc_read_config (MMCFG *cfg)
 *---------------------------------------------------------------------------*/


/* Local Function Prototypes */
static U8   mmc_command     (U8 cmd, U32 arg, U8 *buf, U32 len);
static BOOL mmc_read_bytes  (U8 cmd, U32 arg, U8 *buf, U32 len);
static BOOL mmc_read_block  (U8 cmd, U32 arg, U8 *buf, U32 cnt);
static BOOL mmc_write_block (U8 cmd, U32 arg, U8 *buf, U32 cnt);

/*--------------------------- mmc_init --------------------------------------*/

BOOL mmc_init (void) {
  /* Initialize and enable the Flash Card. */
  U32 i,r1;
  U8  buf[4];

  /* Initialize SPI interface and enable Flash Card SPI mode. */
  spi_init ();

  spi_ss (1);
  spi_hi_speed (__FALSE);

  /* Send SPI Command with card not selected at 400 KBit. */
  for (i = 0; i < 16; i++) {
    spi_send (0xFF);
  }

  /* Reset the card, send CMD0+ (CMD0 with CS asserted ("0")). */
  spi_ss (0);
  r1 = mmc_command (GO_IDLE_STATE, 0, NULL, 0);
  for (i = 0; i < 100; i++) {
    if (r1 == 0x01) {
      break;
    }
    r1 = spi_send (0xFF);
  }
  spi_ss (1);
  if (r1 != 0x01) {
    /* Failed to Reset Flash Card. */
    return (__FALSE);
  }
  spi_hi_speed (__TRUE);

  /* Initialize CardType as unknown. */
  CardType = CARD_NONE;

  /* Check if card is V2.00 or later SD memory card (CMD8). */
  spi_ss (0);
  r1 = mmc_command (SEND_IF_COND, 0x1AA, buf, 4);
  if (r1 == 0x01) {
    if ((buf[0] == 0x00) && (buf[1] == 0x00) && (buf[2] == 0x01) && (buf[3] == 0xAA)) {
      CardType = CARD_SDV2;
    }
  }
  spi_ss (1);

  /* Check if SD card, send ACMD41. */
  for (i = 0; i < 50000; i++) {
    spi_ss (0);
    r1 = mmc_command (APP_CMD, 0, NULL, 0);
    spi_ss (1);
    if (r1 & 0x04) {
      /* Illegal Command. */
      break;
    }
    if (r1 == 0x01) {
      spi_ss (0);
      r1 = mmc_command (SD_SEND_OP_COND, ((CardType == CARD_SDV2) << 30), NULL, 0);
      spi_ss (1);
      if (r1 == 0x00) {
        /* OK, SD card initialized. */
        if (CardType != CARD_SDV2)
          CardType = CARD_SD;
        break;
      }
    }
  }

  /* Check if card is SDHC card (Read OCR (CMD58)). */
  if (CardType == CARD_SDV2) {
    spi_ss (0);
    r1 = mmc_command (READ_OCR, 0, buf, 4);
    spi_ss (1);
    /* Check Card Capacity Status (CCS). */
    if (r1 == 0x00) {
      if (buf[0] & (1 << (30-24))) {
        CardType = CARD_SDV2_HC;
      }
    }
  }

  /* If card is not SD check if it is MMC card. */
  if (CardType == CARD_NONE) {
    /* Check for MMC, send CMD1. */
    for (i = 0; i < 50000; i++) {
      spi_ss (0);
      r1 = mmc_command (SEND_OP_COND, 0, NULL, 0);
      spi_ss (1);
      if (r1 != 0x01) {
        break;
      }
    }
    if (r1 != 0x00) {
      /* Failed to Initialize the card. */
      return (__FALSE);
    }
    CardType = CARD_MMC;
  }

  /* Set block length in the Flash Card to 512 bytes. */
  spi_ss (0);
  r1 = mmc_command (SET_BLOCKLEN, 512, NULL, 0);
  spi_ss (1);
  if (r1 != 0x00) {
    return (__FALSE);
  }

  /* Turn Off CRC option. */
  spi_ss (0);
  r1 = mmc_command (CRC_ON_OFF, 0, NULL, 0);
  spi_ss (1);
  if (r1 != 0x00) {
    return (__FALSE);
  }

  /* Success, card initialized. */
  return (__TRUE);
}


/*--------------------------- mmc_CRC7 --------------------------------------*/

static U8 mmc_CRC7 (U8 cmd, U32 arg) {
  /* Calculate CRC7 (polynom: x^7 + x^3 + 1). */
  U32 i;
  U32 calc;
  U8  buf[5];

  buf[0] = cmd;
  buf[1] = arg >> 24;
  buf[2] = arg >> 16;
  buf[3] = arg >>  8;
  buf[4] = arg >>  0;
  calc   = 0;

  for (i = 0; i < 6*8; i++) {
    if (!(i & 0x07))
      if ((i >> 3) < 5) calc |= buf[i >> 3];
    if (calc & 0x8000)  calc ^= 0x8900;
    calc <<= 1;
  }

  return (calc >> 9);
}


/*--------------------------- mmc_command -----------------------------------*/

static U8 mmc_command (U8 cmd, U32 arg, U8 *buf, U32 len) {
  /* Send a Command to Flash card and get a Response. */
  U32 r1,i;
  U8  crc7;

  /* Calculate CRC7 for command. */
  crc7 = mmc_CRC7(cmd, arg);

  /* Send command with CRC. */
  spi_send (0xFF);
  spi_send (cmd);
  spi_send (arg >> 24);
  spi_send (arg >> 16);
  spi_send (arg >> 8);
  spi_send (arg);
  spi_send ((crc7 << 1) | 1);

  /* Response will come after 1 - 8 retries. */
  for (i = 0; i < 8; i++) {
    r1 = spi_send (0xFF);
    if (r1 != 0xFF) {
      break;
    }
  }
  /* Read response bytes after R1. */
  if (buf) {
    while (len--) {
      *buf++ = spi_send (0xFF);
    }
  }
  return (r1);
}


/*--------------------------- mmc_read_block --------------------------------*/

static BOOL mmc_read_block (U8 cmd, U32 arg, U8 *buf, U32 cnt) {
  /* Read a 'cnt' of data blocks from Flash Card. */
  U32 i;

  if (CardType == CARD_SDV2_HC) 
    arg >>= 9;

  if (mmc_command (cmd, arg, NULL, 0) != 0x00) {
    /* R1 status error. */
    return (__FALSE);
  }

  for (  ; cnt; buf += 512, cnt--) {
    for (i = CMD_TOUT; i; i--) {
      if (spi_send (0xFF) == 0xFE) {
        /* Data Start token. */
        break;
      }
    }
    if (i == 0) {
      /* Sector Read Tiomeout. */
      return (__FALSE);
    }

    for (i = 0; i < 512; i++) {
      buf[i] = spi_send (0xFF);
    }
    /* Read also a 16-bit CRC. */
    spi_send (0xFF);
    spi_send (0xFF);
  }
  return (__TRUE);
}


/*--------------------------- mmc_read_bytes --------------------------------*/

static BOOL mmc_read_bytes (U8 cmd, U32 arg, U8 *buf, U32 len) {
  /* Read a 'len' bytes from Flash Card. */
  U32 i;

  if (mmc_command (cmd, arg, NULL, 0) != 0x00) {
    /* R1 status error. */
    return (__FALSE);
  }

  for (i = CMD_TOUT; i; i--) {
    if (spi_send (0xFF) == 0xFE) {
      /* Data Start token. */
      break;
    }
  }
  if (i == 0) {
    /* Data Read Tiomeout. */
    return (__FALSE);
  }

  for (i = 0; i < len; i++) {
    buf[i] = spi_send (0xFF);
  }
  /* Read also a 16-bit CRC. */
  spi_send (0xFF);
  spi_send (0xFF);
  return (__TRUE);
}


/*--------------------------- mmc_CRC16 -------------------------------------*/

#if 0
static U16 mmc_CRC16 (U8 *buf, U32 cnt) {
  /* Calculate CRC16 (polynom: x^16 + x^12 + x^5 + 1). */
  U32 i;
  U32 bytes;
  U32 calc;

  bytes = (cnt    << 9);
  calc  = (buf[0] << 16) | (buf[1] << 8);

  for (i = 16; i < (bytes+2)*8; i++) {
    if (!(i & 0x07))
      if ((i >> 3) < bytes) calc |= buf[i >> 3];
    calc <<= 1;
    if (calc & 0x1000000)   calc ^= 0x1102100;
  }

  return (calc >> 8);
}
#endif


/*--------------------------- mmc_write_block -------------------------------*/

static BOOL mmc_write_block (U8 cmd, U32 arg, U8 *buf, U32 cnt) {
  /* Write a 'cnt' of data blocks to Flash Card. */
  U32 i;
//U16 crc16;
  U8  tkn;

  if (CardType == CARD_SDV2_HC) 
    arg >>= 9;

  if (mmc_command (cmd, arg, NULL, 0) != 0x00) {
    /* R1 status error. */
    return (__FALSE);
  }

  /* Select token, for single write or multiple write. */
  tkn = 0xFE;
  if (cnt > 1) {
    tkn = 0xFC;
  }

  for (  ; cnt; buf += 512, cnt--) {
//  /* Calculate CRC16 for data. */
//  crc16 = mmc_CRC16(buf, 1);

    /* Send Data Start token. */
    spi_send (tkn);
    /* Send data. */
    for (i = 0; i < 512; i++) {
      spi_send (buf[i]);
    }
    /* Send also a 16-bit CRC. */
    /* Dummy CRC used because of performance. */
    spi_send (0xFF);
    spi_send (0xFF);

//  /* If correct CRC would be used (degrades write performance). */
//  spi_send (crc16 >> 8);
//  spi_send (crc16);

    /* Check data response. */
    if ((spi_send (0xFF) & 0x0F) != 0x05) {
      return (__FALSE);
    }
    /* Wait while Flash Card is busy. */
    for (i = WR_TOUT; i; i--) {
      if (spi_send (0xFF) == 0xFF) {
        /* Sector Write finished. */
        break;
      }
    }
    if (i == 0) {
      /* Sector Write Timeout. */
      return (__FALSE);
    }
  }
  return (__TRUE);
}


/*--------------------------- mmc_read_sect ---------------------------------*/

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt) {
  /* Read single/multiple sectors from Flash Memory Card. */
  U32  i;
  BOOL retv;

  spi_ss (0);
  if (cnt > 1) {
    /* Multiple Block Read. */
    retv = mmc_read_block (READ_MULT_BLOCK, sect * 512, buf, cnt);

    mmc_command (STOP_TRAN, 0, NULL, 0);
    /* Wait while Flash Card is busy. */
    for (i = STOP_TOUT; i; i--) {
      if (spi_send (0xFF) == 0xFF) {
        goto x;
      }
    }
    retv = __FALSE;
  }
  else {
    /* Single Block Read. */
    retv = mmc_read_block (READ_BLOCK, sect * 512, buf, 1);
  }
x:spi_ss (1);
  return (retv);
}


/*--------------------------- mmc_write_sect --------------------------------*/

BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt) {
  /* Write a 512 byte sector to Flash Card. */
  U32  i;
  BOOL retv;

  spi_ss (0);
  if (cnt > 1) {
    /* Multiple Block Write. */
    retv = mmc_write_block (WRITE_MULT_BLOCK, sect * 512, buf, cnt);

    /* Send Stop Transmission token. */
    spi_send (0xFD);

    /* Wait one byte time before checking busy. */
    spi_send (0xFF);

    /* Wait while Flash Card is busy. */
    for (i = STOP_TOUT; i; i--) {
      if (spi_send (0xFF) == 0xFF) {
        goto x;
      }
    }
    retv = __FALSE;
  }
  else {
    /* Single Block Write. */
    retv = mmc_write_block (WRITE_BLOCK, sect * 512, buf, 1);
  }
x:spi_ss (1);
  return (retv);
}


/*--------------------------- mmc_read_config -------------------------------*/

BOOL mmc_read_config (MMCFG *cfg) {
  /* Read MMC/SD Card device configuration. */
  U8 buf[16],*bp;
  BOOL retv;
  U32 v,m;

  /* Read the CID - Card Identification. */
  spi_ss (0);
  retv = mmc_read_bytes (SEND_CID, 0, buf, 16);
  spi_ss (1);
  if (retv == __FALSE) {
    /* Read CID failed. */
    return (__FALSE);
  }
  /* CID register structure for SD is different than for MMC Card. */
  if (CardType == CARD_SD) {
    bp = &buf[9];
  }
  else {
    bp = &buf[10];
  }
  cfg->sernum = bp[0]<<24 | bp[1]<<16 | bp[2]<<8 | bp[3];

  /* Read the CSD - Card Specific Data. */
  spi_ss (0);
  retv = mmc_read_bytes (SEND_CSD, 0, buf, 16);
  spi_ss (1);
  if (retv == __FALSE) {
    /* Read CSD failed. */
    return (__FALSE);
  }
  /* Read Block length */
  v = buf[5] & 0x0F;
  cfg->read_blen = 1 << v;

  /* Write Block length */
  v = ((buf[12] << 8 | buf[13]) >> 6) & 0x0F;
  cfg->write_blen = 1 << v;

  /* Total Number of blocks */
  switch ((buf[0] >> 6) & 3) {          /* Bits 126..127      */
    case 0:                             /* SD card,  CSD v1.0 */
    case 2:                             /* MMC card, CSD v1.2 */
      v = ((buf[6] << 16 | buf[7] << 8 | buf[8]) >> 6) & 0x0FFF;
      m = ((buf[9] << 8  | buf[10]) >> 7) & 0x07;
      cfg->blocknr = (v + 1) << (m + 2);
      break;
    case 1:                             /* SD card,  CSD v2.0 */
      v =  (buf[7] << 16 | buf[8] << 8 | buf[9]) & 0xFFFF;
      cfg->blocknr = (v + 1) << 10;
      break;
    default:
      return (__FALSE);
  }

  return (__TRUE);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
