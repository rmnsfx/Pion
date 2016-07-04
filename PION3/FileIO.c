
#include "stm32f10x.h"
#include <stdio.h>
#include "type.h"
#include "FileIO.h"
 


FILE_HANDEL_TypeDef FH;




void READ_MEM(DWORD adres, void *dest, DWORD count)
{

 while (count>0)
  {
   *(BYTE *)dest = Memory[adres];
   adres++;
   dest = (BYTE *)dest+1;
   count--;
  }
}

void WRITE_MEM(DWORD adres, void *sourse, DWORD count)
{
 while (count>0)
  {
   Memory[adres] = *(BYTE *)sourse;
   adres++;
   sourse = (BYTE *)sourse+1;
   count--;
  }
}


BYTE fat_OPEN(FILE_HANDEL_TypeDef *FH, BYTE NUMFILE)
{
 if ((NUMFILE>15)||(NUMFILE<1)) return 1;

 //читаем корневую директорию
 READ_MEM(START_MAINROOT_AREA+32*NUMFILE, FH->dir, 32);
 
 //проверяем есть ли файл
 if ((FH->dir->name[0]!=0xE5)&&(FH->dir->name[0]!=0x00)) //файл есть
  {
   FH->open = 1;
   FH->seek = 0;
   FH->cur_claster = FH->dir->firstClusterLO;
   return 0;
  }
 else //файла нет
  {
   return 1;
  }

 //return 0; //ошибок нет
}


void fat_CLOSE(FILE_HANDEL_TypeDef *FH)
{
   FH->open = 0;
   FH->seek = 0;
   FH->cur_claster = 0;
   FH->dir = 0;
}

BYTE fat_NEXT_CLUSTER(WORD *CLASTER)
{
 CLASTER++;
 return 0;
}


BYTE fat_WRITE(FILE_HANDEL_TypeDef *FH, void *DATA, DWORD LEN)
{
  DWORD A;
  DWORD L;

  
  while (LEN>0)
   {
    if (FH->cur_claster<2) return 1;

    L = FH->seek%SIZE_CLASTER; //смещение внутри кластера

    A = START_DATA_AREA+(FH->cur_claster-2)*SIZE_CLASTER+L; 

    L = SIZE_CLASTER-L;

    if (LEN<L)
	 {
	  WRITE_MEM(A,DATA,LEN);
	  FH->seek+=LEN;
	  LEN = 0;
	 }
	else
	 {
	  WRITE_MEM(A,DATA,L);
	  FH->seek+=L;
	  LEN-= L;
	  if (fat_NEXT_CLUSTER(&FH->cur_claster)) return 1;
	  
	 }

   }
 

  return 0;
}


