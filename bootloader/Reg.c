//#include "regmap.h"
#include "params.h"
#include "Reg.h"
#include "main.h"
#include "RealTime.h"


TData_reg DATA_REG;

unsigned short reg_READ(unsigned int num_reg)
{ 
unsigned short data;

 if (num_reg>=COUNT_REG) return 0;

 if (PARAMS[num_reg].CopyFlash==2)//Часы
  {
    rtc_COPY_RTC_TO_BUFER();
    if (num_reg==YEAR) return rtc_READ(rtc_YEAR);
	if (num_reg==DATA) return (rtc_READ(rtc_MON)*100+rtc_READ(rtc_DAY));
						
					
	if (num_reg==TIME) return (rtc_READ(rtc_HOUR)*100+rtc_READ(rtc_MIN));
	if (num_reg==SECOND) return rtc_READ(rtc_SEC);
					 
  }

 if (num_reg==VERNUM) return VER;

 return DATA_REG.Data[num_reg];
}
/*
TStatus reg_READ_ARRAY(unsigned int num_reg, unsigned short *data, unsigned short count)
{
TStatus status = _OK;
 while (count>0)
  {
   if (num_reg>=COUNT_REG) status = _ERR;
   *data = reg_READ(num_reg);
	data++;
	num_reg++;
	count--;
  }
 return status; 
}	*/

TStatus reg_WRITE(unsigned int num_reg, unsigned short data)
{
 if (num_reg>=COUNT_REG) return _ERR;

 if (data>PARAMS[num_reg].Max)  data = PARAMS[num_reg].Max;
 if (data<PARAMS[num_reg].Min)  data = PARAMS[num_reg].Min; 

 if (PARAMS[num_reg].CopyFlash==1) //копия во FLASH
  {
   _reg_WRITE(reg_OFFSET_DATA + num_reg*reg_SIZE_REG,&data, reg_SIZE_REG);
   _reg_READ(reg_OFFSET_DATA + num_reg*reg_SIZE_REG,&DATA_REG.Data[num_reg], reg_SIZE_REG);
  }
 else
  if (PARAMS[num_reg].CopyFlash==2) //часы реального времени
   {
	rtc_COPY_RTC_TO_BUFER();
    if (num_reg==YEAR) rtc_WRITE(rtc_YEAR,data);
	if (num_reg==DATA) {
						rtc_WRITE(rtc_MON,data/100);
						rtc_WRITE(rtc_DAY,data%100);
					   }
	if (num_reg==TIME) {
						rtc_WRITE(rtc_HOUR,data/100);
						rtc_WRITE(rtc_MIN,data%100);
						rtc_WRITE(rtc_SEC,0);
					   }
	if (num_reg==SECOND) 
					   {
					   	rtc_WRITE(rtc_SEC,data);
					   }
	rtc_COPY_BUFER_TO_RTC();
   }
  else
   DATA_REG.Data[num_reg] = data;
  

 if (data!=DATA_REG.Data[num_reg]) return _ERR; else return _OK;
} 


void reg_RESET()
{
 unsigned int i;

 for (i=0;i<COUNT_REG;i++)
  DATA_REG.Data[i] = PARAMS[i].Def;
  
}

TStatus reg_LOAD()
{
 unsigned int 	i;
 unsigned int 	adr;
 unsigned short temp_data;
 TStatus		result = _OK;

 for (i=0;i<COUNT_REG;i++)
  if (PARAMS[i].CopyFlash==1)
   {
    adr = reg_OFFSET_DATA + i*reg_SIZE_REG;
    _reg_READ(adr,&DATA_REG.Data[i],reg_SIZE_REG);

	if ((DATA_REG.Data[i]>PARAMS[i].Max)||(DATA_REG.Data[i]<PARAMS[i].Min))
	 {	  
	  _reg_WRITE(adr,&PARAMS[i].Def,reg_SIZE_REG);
	  _reg_READ(adr,&temp_data, reg_SIZE_REG);

	  if (PARAMS[i].Def!=temp_data) result = _ERR; else result = _OK;
	 }
	 	
   }

 return	result; 
}


TStatus reg_SETUP(void)
{
 //сброс регистров
 reg_RESET();

 if (_reg_SETUP()==_ERR) return _ERR;
  
 //читаем сигнатуру
 _reg_READ(0,&DATA_REG.Signature,reg_SIZE_REG);
 //если не наша сигнатура
 if (DATA_REG.Signature != reg_SIGNATURE) 
  {
   DATA_REG.Signature = reg_SIGNATURE;
   _reg_WRITE(0,&DATA_REG,reg_SIZE);   	   			//сбрасываем все уставки	
   _reg_READ(0,&DATA_REG.Signature,reg_SIZE_REG);   //снова читаем сигнатуру
   if (DATA_REG.Signature != reg_SIGNATURE) return _ERR;
  }
 
 //все OK
 
 return	reg_LOAD();
}
