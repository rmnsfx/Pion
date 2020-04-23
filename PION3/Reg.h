#include "FM25V10.h"
#include "FLASH_AT45DB.h"
#include "main.h"
#include "regmap.h"
#include "mscuser.h"
#include "params.h"



typedef struct
 {
  unsigned short 	Signature;
  unsigned short 	Data[COUNT_REG];
 } TData_reg;

//extern TData_reg 				DATA_REG;

#define reg_SIGNATURE			0x1A60
#define reg_SIZE_REG			2
#define reg_SIZE				sizeof(TData_reg)
#define reg_OFFSET				0x0000					//смещение 
#define reg_OFFSET_DATA			(reg_SIZE_REG)


//callback функции
#define _reg_SETUP()			_OK//fram_SETUP()
//#define _reg_READ(adr,b,c)  	AT45DB_READ_DATA(FILEDATA_OFFSET,adr + reg_OFFSET,(unsigned char *)b,c)
//#define _reg_WRITE(adr,b,c)		AT45DB_WRITE_DATA(FILEDATA_OFFSET,adr + reg_OFFSET,(unsigned char *)b,c)

//#define _reg_READ(adr,b,c)  	AT45DB_READ_DATA(0,adr + reg_OFFSET,(unsigned char *)b,c)
//#define _reg_WRITE(adr,b,c)		AT45DB_WRITE_DATA(0,adr + reg_OFFSET,(unsigned char *)b,c)

///Чтение / запись регистров с/на флешку
#define _reg_READ(adr,b,c)  	READ_REG_FROM_SD(1,adr + reg_OFFSET,(unsigned char *)b,c)
#define _reg_WRITE(adr,b,c)		WRITE_REG_TO_SD(1,adr + reg_OFFSET,(unsigned char *)b,c)
	

TStatus 	   reg_SETUP(void);											  //инициализация уставок
TStatus 	   reg_WRITE(unsigned int num_reg, unsigned int data);	  //запись уставки в энергонезависимую память и обновление буфера
unsigned int reg_READ(unsigned int num_reg);							  //чтение уставки из буфера
TStatus 	   reg_READ_ARRAY(unsigned int num_reg, unsigned short *data, unsigned short count); //чтение массива уставок из буфера

#define 	   REG_MAX(num)   PARAMS[num].Max
#define 	   REG_MIN(num)   PARAMS[num].Min
#define 	   REG_DEF(num)   PARAMS[num].Def

#define 	   REG(num)			   reg_READ(num)
#define 	   REGW(num, data)	   reg_WRITE(num, data)
#define 	   REGWDEF(num)		   reg_WRITE(num, REG_DEF(num))	  //сбросить регистр на значение по умолчанию

//unsigned short reg_READ(unsigned int offset);
//void 		   reg_WRITE(unsigned int offset, unsigned short data);



