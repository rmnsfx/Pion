#include <File_Config.h>
#include <string.h>
//#include "type.h"
#include "stm32f10x.h"
#include "mscuser.h"
//#include "m25p80.h"
#include "FM25V10.h"
#include "FLASH_AT45DB.h"
#include "memory.h"
//#include "fs_fat.c"
//unsigned char Disk[4096];
//unsigned char D2[5]={0xA0,0x78,0x12,0x13,0xFE};
#include "ff.h"
#include "diskio.h"
#include "stm32f10x_iwdg.h"
#include "vga_lib.h"


   extern FATFS   fls;            
   extern FIL     FileTmp;          
	 extern FRESULT res_t; 
	 extern	FILINFO fno;

BOOL mmc_init (void)
{
//	u8 i = 0;
////	while ((AT45DB_INIT() != 0) || (i < 10)) i++;
//	while (i++ < 10)
//		if (AT45DB_INIT() == 0) break;
//	
// //AT45DB_INIT();
// return (__TRUE);
}

BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt)
{
	U8 res;
	
	if (disk_status(0) != 0) disk_initialize(0);	
	 						
	__disable_irq();
	__disable_fiq();
	
	res = disk_write(0, buf, sect, 1);
	
	__enable_irq();
	__enable_fiq();


   return (__TRUE);
}

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt)
{
	U8 res;
		
	if (disk_status(0) != 0) disk_initialize(0);	
	
		__disable_irq();
		__disable_fiq();

	res = disk_read(0, buf, sect, 1);
	
		__enable_irq();
		__enable_fiq();

	
   return (__TRUE);
}

BOOL mmc_read_config (MMCFG *cfg)
{ 
 return (__TRUE);
}	

BOOL mmc_format(void)
{
 U32 i;

 
 
__disable_irq();
__disable_fiq(); 
 IWDG_ReloadCounter();
 f_mount(&fls, "0:", 1);
 f_mkfs("0:", 0, 0);
 IWDG_ReloadCounter();
 f_setlabel("PION-3");
 IWDG_ReloadCounter();
 f_mount(0,"0:", 0);
__enable_irq();
__enable_fiq(); 
 

 
 return (__TRUE);
}



	  
