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
#define reg_OFFSET				0x0000					//�������� 
#define reg_OFFSET_DATA			(reg_SIZE_REG)


//callback �������
#define _reg_SETUP()			_OK//fram_SETUP()
//#define _reg_READ(adr,b,c)  	AT45DB_READ_DATA(FILEDATA_OFFSET,adr + reg_OFFSET,(unsigned char *)b,c)
//#define _reg_WRITE(adr,b,c)		AT45DB_WRITE_DATA(FILEDATA_OFFSET,adr + reg_OFFSET,(unsigned char *)b,c)

//#define _reg_READ(adr,b,c)  	AT45DB_READ_DATA(0,adr + reg_OFFSET,(unsigned char *)b,c)
//#define _reg_WRITE(adr,b,c)		AT45DB_WRITE_DATA(0,adr + reg_OFFSET,(unsigned char *)b,c)

#define _reg_READ(adr,b,c)  	READ_REG_FROM_SD(1,adr + reg_OFFSET,(unsigned char *)b,c)
#define _reg_WRITE(adr,b,c)		WRITE_REG_TO_SD(1,adr + reg_OFFSET,(unsigned char *)b,c)
	

TStatus 	   reg_SETUP(void);											  //������������� �������
TStatus 	   reg_WRITE(unsigned int num_reg, unsigned short data);	  //������ ������� � ����������������� ������ � ���������� ������
unsigned short reg_READ(unsigned int num_reg);							  //������ ������� �� ������
TStatus 	   reg_READ_ARRAY(unsigned int num_reg, unsigned short *data, unsigned short count); //������ ������� ������� �� ������

#define 	   REG_MAX(num)   PARAMS[num].Max
#define 	   REG_MIN(num)   PARAMS[num].Min
#define 	   REG_DEF(num)   PARAMS[num].Def

#define 	   REG(num)			   reg_READ(num)
#define 	   REGW(num, data)	   reg_WRITE(num, data)
#define 	   REGWDEF(num)		   reg_WRITE(num, REG_DEF(num))	  //�������� ������� �� �������� �� ���������

//unsigned short reg_READ(unsigned int offset);
//void 		   reg_WRITE(unsigned int offset, unsigned short data);



