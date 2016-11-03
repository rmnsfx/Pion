#include "stm32f10x.h"
#include <File_Config.h>
#include <stdio.h>
#include <string.h>
#include "Road.h"
#include "ff.h"
#include "diskio.h"
#include "vga_lib.h"
#include "Pin.h"
#include "ff.h"


#define MaxNumElement 500					  //???????????? ????? ????? ? ????????
#define SizeElement	  (sizeof(TNameElement))
#define MaxFileSize	  (SizeElement*500)
#define FileName	  "M:\\Road.0"
u8		t[512];
TNameElement NEl;
FILE  		*pRFile = 0;
FIL fil;
FATFS fat;
unsigned char res;
unsigned int byte_read;



s8 rod_INIT(void)
{
	char temp[20];
	
	
	if (pRFile == 0) pRFile = fopen ("M:\\Road.log","r");
	else 
	{
		fclose(pRFile);	
		pRFile = fopen ("M:\\Road.log","r");
	}
	
	if (pRFile!=0)
	{
	
		fscanf(pRFile, "%s", temp);			
		fclose(pRFile);	
		
		pRFile = fopen (temp,"r");
		
//		if (pRFile == 0)
//		{
//			pRFile = fopen ("M:\\Road.000","r");
//			if (pRFile == 0)
//			{
//				///rod_CreateFile();
//				//rod_CreateFile_edit();
//				pRFile = fopen ("M:\\Road.000","r");
//			}
//		}
//	return 0;
	}
	
	
// pRFile = fopen (FileName,"r");

// if (pRFile==0) return -1; //?????? ???????? ?????
//	}
//	
 return 0;
}


void rod_DEINIT ( void )
{
	if (pRFile != NULL) fclose (pRFile);
}


s8 rod_GET_NameElement(TNameElement * Element, unsigned char num)
{
 if (pRFile==0) return -1;


 memset(Element,' ',SizeElement);
 
 if (num==0)	return -1;

 fseek(pRFile, (u32)(num-1)*SizeElement, SEEK_SET);
	
 if (fread(Element,1,SizeElement,pRFile)==SizeElement) 
	 
  if (Element->StringName_1[0]==0)
	{
	 memset(Element,0x00,SizeElement);
	 return -1;
	}
   else return 0;
 
 return -1; 
}

//s8 rod_GET_NameElement(TNameElement * Element, unsigned char num)
//{

//	memset(Element,' ',SizeElement);
//	
//	if (num==0)	return -1;
//	
//	res = f_mount(&fat,"0:",1);
//	res = f_open(&fil, "Road.log", FA_OPEN_EXISTING | FA_READ);
//	res = f_lseek(&fil, (u32) (num-1)*SizeElement);
//	res = f_read(&fil, Element, SizeElement, &byte_read);
//	if (byte_read == SizeElement)
//	{
//		if (Element->StringName_1[0]==0)
//		{
//			memset(Element,0x00,SizeElement);
//			return -1;
//		}
//		else 
//		{
//			f_close(&fil);
//			res = f_mount(0,"0:",0);
//			return 0;
//		}
//	}
//	
//}


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
	

	__disable_irq();
	__disable_fiq(); 
		
	f_mount(&fls, "0:", 1);
				
	f_open(&Fil,"0:Road.0", FA_CREATE_ALWAYS | FA_WRITE);	
	f_printf(&Fil,"%s", "без маршрута   ");		
	f_putc(0,&Fil);
	
	IWDG_ReloadCounter();
	
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

	IWDG_ReloadCounter();	
	
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

