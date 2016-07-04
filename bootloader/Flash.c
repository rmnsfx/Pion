#include <File_Config.h>
#include <string.h>
//#include "type.h"
#include "stm32f10x.h"
#include "mscuser.h"
//#include "m25p80.h"
#include "FM25V10.h"
#include "FLASH_AT45DB.h"
#include "memory.h"

//unsigned char Disk[4096];
//unsigned char D2[5]={0xA0,0x78,0x12,0x13,0xFE};


BOOL mmc_init (void)
{
 AT45DB_INIT();
 return (__TRUE);
}

BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt)
{
 if (sect<MSC_BlockCount)
  {
   //memcpy(&Memory[sect*512],buf,cnt*512);
   AT45DB_WRITE_PAGE(sect+DISK_OFFSET,buf);
   //fram_WRITE(sect*MSC_BlockSize + MSC_Offset,buf,cnt*MSC_BlockSize);
   return (__TRUE);
  }
 else 
  return (__FALSE);
}

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt)
{
 if (sect<MSC_BlockCount)
  {
   AT45DB_READ_PAGE(sect+DISK_OFFSET,buf);
   //fram_READ(sect*MSC_BlockSize + MSC_Offset,buf,cnt*MSC_BlockSize);
   return (__TRUE);
  }
 else
  return (__FALSE);///memset(buf,0,cnt*512);

}

BOOL mmc_read_config (MMCFG *cfg)
{ 
 return (__TRUE);
}	

BOOL mmc_format(void)
{
 U32 i;

 //AT45DB_CHIP_ERASE();

 //AT45DB_CHIP_SET(0);

 //AT45DB_PAGE_ERASE(0);


 //*(unsigned short *)(DiskImage+19) = ClusterPerDisk;  //число кластеров в диске
 //*(unsigned short *)(DiskImage+22) = ClusterPerFat;  //дисло сектаров на фат
	 //*(uint32_t *)(DiskImage+32) = 3839480;  //дисло сектаров на фат	
 AT45DB_WRITE_PAGE(DISK_OFFSET,&DiskImage[0]);				 //пишем бутрекорд

 i = 600;

 while (i--)
   AT45DB_WRITE_PAGE(FAT_OFFSET+i,&DiskImage[512]);	  //стераем таблицу фат

 if (ClusterPerFat>0)
  {
   //FAT12
   //DiskImage[512+0]=0xF8;	   //первые 2 кластера всегда заняты
   //DiskImage[512+1]=0xFF;	   //первые 2 кластера всегда заняты
   //DiskImage[512+2]=0xFF;	   //первые 2 кластера всегда заняты
   //FAT16
   DiskImage[512+0]=0xF8;	   //первые 2 кластера всегда заняты
   DiskImage[512+1]=0xFF;	   //первые 2 кластера всегда заняты
   DiskImage[512+2]=0xFF;	   //первые 2 кластера всегда заняты
   DiskImage[512+3]=0xFF;	    //первые 2 кластера всегда заняты

   
   AT45DB_WRITE_PAGE(FAT_OFFSET,&DiskImage[512]);
  }
 //корневая директория
 AT45DB_WRITE_PAGE(ROOT_OFFSET,&DiskImage[1024]);

 
 return (__TRUE);
}



	  
