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
#include "stm32f10x_rcc.h"
#include "main.h"
#include "Flash.h"

#include "stm32f10x.h"


#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

   extern FATFS   fls;            
   extern FIL     FileTmp;          
	 extern FRESULT res_t; 
	 extern	FILINFO fno;


///Работа с SD-картой

BOOL mmc_init (void)
{

 return (__TRUE);
}

BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt)
{
		unsigned int res;
	
		res = disk_status(0);	
		if (res != 0) res = f_mount(&fls, "0:", 1);	
										
		__disable_irq();
		__disable_fiq();
   
		res = disk_write(0, buf, sect, 1);
		
		__enable_irq();
		__enable_fiq();

		res = f_mount(0,"0:", 0);
		
		return (__TRUE);

}

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt)
{
	
		unsigned int res;
	
		res = disk_status(0);
		if (res != 0) res = f_mount(&fls, "0:", 1);	
		
		__disable_irq();
		__disable_fiq();

		res = disk_read(0, buf, sect, 1);
	
		__enable_irq();
		__enable_fiq();
		
		res = f_mount(0,"0:", 0);	

		return (__TRUE);
}

BOOL mmc_read_config (MMCFG *cfg)
{ 
	
 cfg->blocknr = sdinfo.DskSize;
 cfg->read_blen = sdinfo.BytesPerSec;
 cfg->write_blen = sdinfo.BytesPerSec;  
	
 return (__TRUE);
}	

unsigned int mmc_format(void)
{
	
	U32 i;
	unsigned int res;
	unsigned int res_format;
	//unsigned int disk_size = sdinfo.DskSize;
 
	IWDG_ReloadCounter();
	
	__disable_irq();
	__disable_fiq(); 
 
	Delay(100000);
			
	res = finit();
	
	res = f_mount(&fls, "0:", 1);
	res_format = f_mkfs("0:", 0, 0); 	
	IWDG_ReloadCounter();	
	res = f_setlabel("PION"); 
	res = f_mount(0,"0:",0);
	
	__enable_irq();
	__enable_fiq(); 
 
 return (res_format);
}

///Работа со встроенной flash памятью микроконтроллера ( 0x8000000 - 0x807FFFF )
void flash_unlock(void) 
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void flash_lock() 
{
  FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t flash_ready(void) 
{
  return !(FLASH->SR & FLASH_SR_BSY);
}

void flash_erase_page(uint32_t address) 
{
  FLASH->CR|= FLASH_CR_PER; 
  FLASH->AR = address; 
  FLASH->CR|= FLASH_CR_STRT; 
  while(!flash_ready());  
  FLASH->CR&= ~FLASH_CR_PER; 
}

uint32_t flash_read(uint32_t address) 
{
	return (*(__IO uint32_t*) address);
}

void flash_write(uint32_t address,uint32_t data) 
{
  FLASH->CR |= FLASH_CR_PG; 
 
	//while(!flash_ready());
  //*(__IO uint16_t*)address = (uint16_t)data; 
  //while(!flash_ready());
  //address+=2;
  //data>>=16;

	while((FLASH->SR&FLASH_SR_BSY));
  *(__IO uint16_t*)address = (uint16_t)data; 
  
  FLASH->CR &= ~(FLASH_CR_PG); 
	
}