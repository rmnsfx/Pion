/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

//#include "diskio.h"

#include <string.h> // memcpy
#include "FLASH_AT45DB.h"
#include "stm32f10x.h"
//#include "SD_card_functions.h"
#include "ffconf.h"
#include "diskio.h"
#include "main.h"
#include "RealTime.h"
#include "Reg.h"
#include "vga_lib.h"


//#include "regs.h"
//#include "vga_lib.h"
//#include "menu.h"

#define BLOCK_SIZE            512 /* Block Size in Bytes */

static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static volatile
DWORD Timer1, Timer2;	/* 100Hz decrement timers */

//static BYTE CardType;			/* Card type flags */

#define SOCKPORT	1			/* Socket contact port */
#define SOCKWP		0			/* Write protect switch (PB5) */
#define SOCKINS		0			/* Card detect switch (PB4) */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
        BYTE drv                                /* Physical drive nmuber (0..) */
)
{
 
	
	
	DSTATUS stat = 0;
	//SD_CardInfo SDCardInfo;

	AT45DB_INIT();
 
	/*
  if (drv)
  {
    stat |= STA_NOINIT;
  }

  if (SD_Init() !=SD_OK)
  {

    stat |= STA_NOINIT;
  }
SD_GetCardInfo(&SDCardInfo);
	return(stat);
	*/
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
        BYTE drv                /* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = 0;
	
	//if (SD_Detect() != SD_PRESENT)
	//	stat |= STA_NODISK;

	// STA_NOTINIT - Subsystem not initailized
	// STA_PROTECTED - Write protected, MMC/SD switch if available
	
	return(stat);
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
        BYTE drv,               /* Physical drive nmuber (0..) */
        BYTE *buff,             /* Data buffer to store read data */
        DWORD sector,   				/* Sector address (LBA) */
        UINT count              /* Number of sectors to read (1..255) */
)
{
 /* uint16_t i;
  for (i=0; i<count; i++) SD_ReadBlock((uint8_t *)buff+512*i, sector+i, 512);
  */
	  //memset(buff2, 0, sizeof(buff2));
	/*
	if(count==1)
        {
          SD_ReadBlock(buff, sector*512, 512);
          //memcpy(buff,buff2,SECTOR_SIZE);
	}
	else
        {
          SD_ReadMultiBlocks(buff, sector*512,512,count);
          //memcpy(buff,buff2,SECTOR_SIZE * count);
	}
*/
	
	
	AT45DB_READ_PAGE(sector+1,buff);
	

/*
SD_ReadMultiBlocks ((uint8_t *)buff, 
                          (uint32_t)sector*512, 
                          512,
                          (uint16_t)count);*/
	return RES_OK;
	/*
	SD_Error Status;


	
	if (SD_Detect() != SD_PRESENT)
		return(RES_NOTRDY);

	if ((DWORD)buff & 3) // DMA Alignment failure, do single up to aligned buffer
	{
		DRESULT res = RES_OK;
		DWORD scratch[BLOCK_SIZE / 4]; // Alignment assured, you'll need a sufficiently big stack

		while(count--)
		{
			res = disk_read(drv, (void *)scratch, sector++, 1);

			if (res != RES_OK)
				break;

			memcpy(buff, scratch, BLOCK_SIZE);

			buff += BLOCK_SIZE;
		}

		return(res);
	}

  Status = SD_ReadMultiBlocks(buff, sector, BLOCK_SIZE, count); // 4GB Compliant

	if (Status == SD_OK) return(RES_OK);
	else return(RES_ERROR);*/
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
        BYTE drv,                       /* Physical drive nmuber (0..) */
        const BYTE *buff,       /* Data to be written */
        DWORD sector,           /* Sector address (LBA) */
        UINT count                      /* Number of sectors to write (1..255) */
)
{
	uint16_t i;
/*  uint16_t i;
  for (i=0; i<count; i++) SD_WriteBlock((uint8_t *)buff+512*i, sector+i, 512);
  return RES_OK;*/

/*
        if(count==1)
        {
          //memcpy(buff,0x00,512);
          SD_WriteBlock((uint8_t *)buff, sector*512, 512);
          //
	}
	else
        {

          SD_WriteMultiBlocks((uint8_t *)buff, sector*512,512,count);
          //memcpy(buff,buff2,SECTOR_SIZE * count);
          //		while(count--)
	//	{
           //             SD_WriteBlock((uint8_t *)buff, sector*512, 512);
          //              sector++;
//
	//	}
	}
	*/			
	
	if(count==1)
	{
	AT45DB_WRITE_PAGE(sector+1,buff);
	}
	else 
	{
		for (i=0; i<count; i++)  AT45DB_WRITE_PAGE(sector+1+i,buff+512*i);  //SD_WriteBlock((uint8_t *)buff+512*i, sector+i, 512);
	}
	
	
  return RES_OK; 
  /*
	SD_Error Status;

	if (SD_Detect() != SD_PRESENT)
		return(RES_NOTRDY);

	if ((DWORD)buff & 3) // DMA Alignment failure, do single up to aligned buffer
	{
		DRESULT res = RES_OK;
		DWORD scratch[BLOCK_SIZE / 4]; // Alignment assured, you'll need a sufficiently big stack

		while(count--)
		{
			memcpy(scratch, buff, BLOCK_SIZE);

			res = disk_write(drv, (void *)scratch, sector++, 1);

			if (res != RES_OK)
				break;

			buff += BLOCK_SIZE;
		}

		return(res);
	}

  Status = SD_WriteMultiBlocks((uint8_t *)buff, sector, BLOCK_SIZE, count); // 4GB Compliant

	if (Status == SD_OK) return(RES_OK);
	else return(RES_ERROR);*/
}
#endif /* _READONLY */




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
  BYTE drv, // Physical drive number (0)
  BYTE ctrl, // Control code
  void *buff // Buffer to send/receive control data
  )
{
  DRESULT res = 1;
  uint32_t status = 0;
//  SD_CardInfo SDCardInfo;
  //uint32_t status = NAND_READY;

//vga_SET_POS_TEXT(10,10); 

  switch (ctrl) {
  case CTRL_SYNC : /// Make sure that no pending write process
    //status = SD_GetTransferState();
		//status = SD_TRANSFER_OK;
   // if (status == SD_TRANSFER_OK)
      //status = FSMC_NAND_GetStatus();
      //if (status == NAND_READY)
         // vga_SET_POS_TEXT(10,10);  

   // {res = RES_OK;}
   // else{res = RES_ERROR;}
		res = RES_OK;
//vga_PRINT_STR("141",&FONT_6x8);
//vga_UPDATE();	    
    break;

  case GET_SECTOR_COUNT :   // Get number of sectors on the disk (DWORD)
    //SD_GetCardInfo(&SDCardInfo);
    //*(DWORD*)buff = SDCardInfo.CardCapacity / 512;  
    *(DWORD*)buff = 0x3A9700; // 4*1024*32 = 131072
    res = RES_OK;
        //vga_SET_POS_TEXT(10,10);  
//vga_PRINT_STR("151",&FONT_6x8);
//vga_UPDATE();	
    break;

  case GET_SECTOR_SIZE :   // Get R/W sector size (WORD)
    *(WORD*)buff = 512;
    res = RES_OK;
        //vga_SET_POS_TEXT(10,10);  
//vga_PRINT_STR("161",&FONT_6x8);
//vga_UPDATE();	
    break;

  case GET_BLOCK_SIZE :     // Get erase block size in unit of sector (DWORD)
    *(DWORD*)buff = 32;
    res = RES_OK;
        //vga_SET_POS_TEXT(10,10);  
//vga_PRINT_STR("171",&FONT_6x8);
//vga_UPDATE();	
	
	case CTRL_ERASE_SECTOR :	 
	
	res = RES_OK;
  }

  return res;
}


/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void){
 
	char temp[25];
	int yearft =0;
	int monthft = 0;
	int mdayft = 0;
	int hourft = 0;
	int minft = 0;
	int secft = 0;
		
	
	yearft = rtc_READ(rtc_YEAR);
  monthft = rtc_READ(rtc_MON);
  mdayft = rtc_READ(rtc_DAY);
  hourft = rtc_READ(rtc_HOUR);
  minft = rtc_READ(rtc_MIN);  
	secft = rtc_READ(rtc_SEC);

	
	
////  RTC_TimeTypeDef time;
////  RTC_DateTypeDef date;
////  RTC_GetTime(RTC_Format_BIN,&time);
////  RTC_GetDate(RTC_Format_BIN,&date);
////  
////          	return	  ((DWORD)(/*rtc.year*/date.RTC_Year+20) << 25)
////			| ((DWORD)/*rtc.month*/date.RTC_Month << 21)
////			| ((DWORD)/*rtc.mday*/date.RTC_Date << 16)
////			| ((DWORD)/*rtc.hour*/time.RTC_Hours << 11)
////			| ((DWORD)/*rtc.min*/time.RTC_Minutes << 5)
////			| ((DWORD)/*rtc.sec*/time.RTC_Seconds >> 1);
                
//     return	  ((DWORD)(/*rtc.year*/2010 - 1980) << 25)
//			| ((DWORD)/*rtc.month*/10 << 21)
//			| ((DWORD)/*rtc.mday*/7 << 16)
//			| ((DWORD)/*rtc.hour*/1 << 11)
//			| ((DWORD)/*rtc.min*/26 << 5)
//			| ((DWORD)/*rtc.sec*/6 >> 1);

return	  ((DWORD)(/*rtc.year*/yearft-60) << 25)
			| ((DWORD)/*rtc.month*/monthft << 21)
			| ((DWORD)/*rtc.mday*/mdayft << 16)
			| ((DWORD)/*rtc.hour*/hourft << 11)
			| ((DWORD)/*rtc.min*/minft << 5)
			| ((DWORD)/*rtc.sec*/secft >> 1);

}

/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */
/*
RAMFUNC void disk_timerproc (void)
{
	static BYTE pv;
	BYTE n, s;

	n = Timer1;						
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;

	n = pv;
	pv = SOCKPORT & (SOCKWP | SOCKINS);	

	if (n == pv) {					
		s = Stat;

		if (pv & SOCKWP)			
			s |= STA_PROTECT;
		else						
			s &= ~STA_PROTECT;

		if (pv & SOCKINS)			
			s |= (STA_NODISK | STA_NOINIT);
		else						
			s &= ~STA_NODISK;

		Stat = s;
	}
}

*/

DRESULT data_erase (
				
				
        DWORD sector,           /* Sector address (LBA) */
        UINT count              /* Number of sectors to write (1..255) */
)
{
	uint16_t i;

	

	if(count==1)
	{
		AT45DB_PAGE_ERASE(sector+1);
	}
	else 
	{
		for (i=0; i<count; i++)  AT45DB_PAGE_ERASE(sector+1+i);  
	}
	
	  return RES_OK; 
	
}