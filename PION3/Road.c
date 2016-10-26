#include "stm32f10x.h"
#include <File_Config.h>
#include <stdio.h>
#include <string.h>
#include "Road.h"
#include "ff.h"
#include "diskio.h"
#include "vga_lib.h"
#include "Pin.h"


#define MaxNumElement 500					  //???????????? ????? ????? ? ????????
#define SizeElement	  (sizeof(TNameElement))
#define MaxFileSize	  (SizeElement*500)
#define FileName	  "M:\\Road.0"
u8		t[512];
TNameElement NEl;
FILE  		*pRFile = 0;
FATFS flsys;
FIL fil;


s8 rod_INIT(void)
{
	char t1[20];	
	unsigned char result;
	unsigned int br, bw;
	
	f_mount(&flsys, "0:", 1);
		
	result = f_open(&fil, "0:Road.log", FA_OPEN_EXISTING | FA_READ);
	
	if (result != 0) 
	{		
		f_close(&fil);
		result = f_open(&fil, "0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);		
	}
			
	result = f_read(&fil, t1, sizeof(t1), &br);    
	f_close(&fil);
	
	//result = f_open(&fil, t1, FA_OPEN_EXISTING | FA_READ);
	
	//result = f_sync(&fil);

	f_mount(0, "0:", 0);
	
	return result;
}


void rod_DEINIT ( void )
{
	f_close (&fil);
}


s8 rod_GET_NameElement(TNameElement * Element, unsigned char num)
{

	unsigned char result;
	unsigned int br, bw;
	char tr[20];	
	char t2[20];
	FATFS fls2;
	FIL fil2;
	
	if (num == 0)	return -1;	
	
	result = disk_status(0);		
	
	result = f_mount(&fls2, "0:", 1);	
	result = f_open(&fil2, "0:Road.log", FA_OPEN_EXISTING | FA_READ);	
	result = f_read(&fil2, tr, sizeof(tr), &br);    
	f_close(&fil2);
	result = f_open(&fil2, tr, FA_OPEN_EXISTING | FA_READ);	
	
	memset(Element,' ',SizeElement);	
	
	f_lseek(&fil, (u32)(num-1)*SizeElement);		
	
	result = f_read(&fil2, Element, SizeElement, &br);
	
	if (br == SizeElement) 	 
	{
		if (Element->StringName_1[0] == 0)
		{
			memset(Element, 0x00, SizeElement);		
			return -1;
		}
		
		result = f_close(&fil2);
		result = f_mount(0,"0:", 0);
		
		return 0;
	}
	
	result = f_close(&fil2);
	result = f_mount(0,"0:", 0);

}




long rod_CreateFile_edit (void)
{

  s32 i = 0;  		
	FIL Fil;
	FIL Fil2;
	FRESULT result;	
	FATFS   fls;          
	FILINFO fno;
	long res = 0;
	char t_str[25];
	
//	finit();
//	Delay(100000);
//	fat_init();

	__disable_irq();
	__disable_fiq(); 
		
	f_mount(&fls, "0:", 1);
				
	f_open(&Fil,"0:Road.0", FA_CREATE_ALWAYS | FA_WRITE);	
	f_printf(&Fil,"%s", "без маршрута   ");		
	f_putc(0,&Fil);
	
	
//	f_open(&Fil,"0:Road.000", FA_OPEN_ALWAYS | FA_WRITE);	
//	f_lseek(&Fil, 16);
	
	for(i=1;i<1000;i++)
	{
			{
					f_printf(&Fil,"%s %03u      ","точка",i);				
					f_putc(i,&Fil);
	
			}						
	}		
	//Получаем размер 
	res = f_size(&Fil);	
	
	f_close(&Fil);
		
	//Только чтение
	f_chmod("0:Road.0", AM_RDO, AM_RDO | AM_ARC);
		
	f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
	f_printf(&Fil,"%s", "Road.0");
	f_close(&Fil);
	
	f_open(&Fil2,"0:Roads.txt", FA_CREATE_ALWAYS | FA_WRITE);
	f_printf(&Fil2,"%s", "Road.0");
	f_close(&Fil2);
		
//	//Проверяем размер 
//	f_open(&Fil,"0:Road.100", FA_OPEN_ALWAYS);				
//	//if (f_size(&Fil) == 16000) res = 1;			
//	res = f_size(&Fil);
//	f_close(&Fil);
		
	f_mount(0,"0:", 0);
		
	__enable_irq();
	__enable_fiq();
	
  return res;		 
}

