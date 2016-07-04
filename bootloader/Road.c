#include "stm32f10x.h"
#include <File_Config.h>
#include <stdio.h>
#include <string.h>
#include "Road.h"



#define MaxNumElement 50					  //максимальное число точек в маршруте
#define SizeElement	  (sizeof(TNameElement))
#define MaxFileSize	  (SizeElement*50)
#define FileName	  "M:\\Road.000"
u8		t[512];
TNameElement NEl;
FILE  		*pRFile = 0;

s8 rod_INIT(void)
{
	char temp[20];
	
	pRFile = fopen ("M:\\Road.log","r");
	if (pRFile!=0)
	{
	//fread(temp,1,20,pRFile);
	fscanf(pRFile, "%s", temp);
		
	fclose(pRFile);	
	//sprintf(temp,"M:\\%s","Road.001");
	pRFile = fopen (temp,"r");
	return 0;
	}
	
	
 pRFile = fopen (FileName,"r");

 if (pRFile==0) return -1; //ошибка открытия файла

 return 0;
}

s8 rod_GET_NameElement(TNameElement * Element, unsigned char num)
{
 if (pRFile==0) return -1;

 memset(Element,' ',SizeElement);
 //if (num>MaxNumElement)	return -1;
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

s8 rod_CreateFile(void)
{
  FILE  		*pFile;
  //TNameElement 	NEl;
  s32 			i = MaxNumElement;
   u8			j;
   

  pFile = fopen (FileName,"w");
	 
  if (pFile==0) return -1;

  memset(&NEl,0,SizeElement);
  memset(&t,0,512);

  //задаем пустой файл, размер файла - 8 кластеров - 4096 байт
  
  while (i--)
   {
    //sprintf(NEl.StringName_1,"Точка %d", MaxNumElement-i);
    fwrite(&NEl,1,SizeElement,pFile);
   }

  i = 4096-MaxFileSize;
  j = i/512;

  while (j--)
   {
    fwrite(&t,1,512,pFile);
   }

  j = i%512;
  if (j>0) fwrite(&t,1,j,pFile);
	 
  fclose(pFile);
  
  return 0;		 
}


