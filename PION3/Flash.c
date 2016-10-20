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
// if (sect < MSC_BlockCount)
//  {
//   //memcpy(&Memory[sect*512],buf,cnt*512);
//		GPIO_SetBits(GPIOA,GPIO_Pin_12);
//										
//		__disable_irq();
//		__disable_fiq();
//   AT45DB_WRITE_PAGE(sect+DISK_OFFSET,buf);
//		__enable_irq();
//		__enable_fiq();
//		GPIO_ResetBits(GPIOA,GPIO_Pin_12);										
//   //fram_WRITE(sect*MSC_BlockSize + MSC_Offset,buf,cnt*MSC_BlockSize);
		
	 disk_write(0, buf, sect, 1);
   return (__TRUE);
//  }
// else 
//  return (__FALSE);
}

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt)
{
 //if (sect<MSC_BlockCount)
//  {
//		__disable_irq();
//		__disable_fiq();
//   AT45DB_READ_PAGE(sect+DISK_OFFSET,buf);
//		__enable_irq();
//		__enable_fiq();
//   //fram_READ(sect*MSC_BlockSize + MSC_Offset,buf,cnt*MSC_BlockSize);
		
		disk_read(0, buf, sect, 1);
   return (__TRUE);
//  }
// else
//  return (__FALSE);///memset(buf,0,cnt*512);

}

BOOL mmc_read_config (MMCFG *cfg)
{ 
 return (__TRUE);
}	

BOOL mmc_format(void)
{
 U32 i;


// //AT45DB_CHIP_ERASE();
// //AT45DB_CHIP_SET(0);
// //AT45DB_PAGE_ERASE(0); 

// //*(unsigned short *)(DiskImage+19) = ClusterPerDisk;  //����� ��������� � �����
// //*(unsigned short *)(DiskImage+22) = ClusterPerFat;  //����� �������� �� ���
//	 //*(uint32_t *)(DiskImage+32) = 3839480;  //����� �������� �� ���	
// 
//IWDG_ReloadCounter();
//	
//__disable_irq();
//__disable_fiq();
//	
//IWDG_ReloadCounter();
//	
//	AT45DB_WRITE_PAGE(DISK_OFFSET,&DiskImage[0]);				 //����� ���������
// 	
//__enable_irq();
//__enable_fiq(); 

//IWDG_ReloadCounter();

// i = 1000;
//	
// while (i--)
// {
//	 
//	 IWDG_ReloadCounter();
//	 
//__disable_irq();
//__disable_fiq();
//	 
//	 IWDG_ReloadCounter();
//	 
//		AT45DB_WRITE_PAGE(FAT_OFFSET+i,&DiskImage[512]);	  //������� ������� ���
//__enable_irq();
//__enable_fiq(); 
//	 
////	vga_CLEAR();
////  vga_SET_POS_TEXT(1,1);
////  vga_PRINT_STR("��������������...",&FONT_6x8);
////												
////  vga_SET_POS_TEXT(1,20);
////								sprintf(str_out,"%.1f%%", (1000 - i)/10.0);		 								
////								vga_PRINT_STR(str_out,&FONT_6x8);
////								
////								vga_UPDATE();			
//	 
//	 
//	IWDG_ReloadCounter();
//	 
// }
//	
		 
 
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
 
/*	vga_CLEAR();
  vga_SET_POS_TEXT(1,1);
  vga_PRINT_STR("Done",&FONT_6x8);
												
								
								vga_UPDATE();*/			
 /*if (ClusterPerFat>0)
  {
   //FAT12
   //DiskImage[512+0]=0xF8;	   //������ 2 �������� ������ ������
   //DiskImage[512+1]=0xFF;	   //������ 2 �������� ������ ������
   //DiskImage[512+2]=0xFF;	   //������ 2 �������� ������ ������
   //FAT16
   DiskImage[512+0]=0xF8;	   //������ 2 �������� ������ ������
   DiskImage[512+1]=0xFF;	   //������ 2 �������� ������ ������
   DiskImage[512+2]=0xFF;	   //������ 2 �������� ������ ������
   DiskImage[512+3]=0xFF;	    //������ 2 �������� ������ ������

   
   AT45DB_WRITE_PAGE(FAT_OFFSET,&DiskImage[512]);
  }*/
 //�������� ����������
 //AT45DB_WRITE_PAGE(473,&BlockName[0]);
//	write_label("TTTT",3);
 
 return (__TRUE);
}



	  
