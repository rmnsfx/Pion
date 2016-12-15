#include <time.h>
#include <string.h>
#include "stm32f10x.h"
#include "main.h"
#include "RealTime.h"
#include "vga_lib.h"


#define rtc_SIGNATURE 0xAAAA

struct tm RTC_TM;

void rtc_RESET(void);
/*
//---------------------------------------------------------------------------------//
//  Конфигурирование часов
//---------------------------------------------------------------------------------//
void rtc_Configuration(void)
{
  // Enable PWR and BKP clocks 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  // Allow access to BKP Domain 
  PWR_BackupAccessCmd(ENABLE);

  // Reset Backup Domain 
  BKP_DeInit();

  // Enable LSE 
  RCC_LSEConfig(RCC_LSE_ON);
  // Wait till LSE is ready 
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  // Select LSE as RTC Clock Source 
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  // Enable RTC Clock 
  RCC_RTCCLKCmd(ENABLE);

  // Wait for RTC registers synchronization 
  RTC_WaitForSynchro();

  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();

  // Enable the RTC Second 
  //RTC_ITConfig(RTC_IT_SEC, ENABLE);

  // Wait until last write operation on RTC registers has finished 
  //RTC_WaitForLastTask();

  // Set RTC prescaler: set RTC period to 1sec 
  RTC_SetPrescaler(32767); // RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) 

  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();
}
*/


//---------------------------------------------------------------------------------//
// Инициализация часов реального времени, процедура возвращает _ERR 
// если часы сбросились иначе _OK
//---------------------------------------------------------------------------------//
TStatus rtc_SETUP(void)
{
 /* Enable PWR and BKP clocks */
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
 /* Allow access to BKP Domain */
	
	Delay(200000);
	
 PWR_BackupAccessCmd(ENABLE);

	///Коррекция точности хода
	BKP->RTCCR |= 36;

	
 if (BKP_ReadBackupRegister(BKP_DR1) != rtc_SIGNATURE)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

  	/* Reset Backup Domain */
  	BKP_DeInit();
		
  	/* Enable LSE */
 	RCC_LSEConfig(RCC_LSE_ON);
	
		
  	/* Wait till LSE is ready */
  	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  	{}

			
  	/* Select LSE as RTC Clock Source */
  	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  	/* Enable RTC Clock */
  	RCC_RTCCLKCmd(ENABLE);

  	/* Wait for RTC registers synchronization */
  	RTC_WaitForSynchro();

  	/* Wait until last write operation on RTC registers has finished */
  	RTC_WaitForLastTask();

  	/* Enable the RTC Second */
  	RTC_ITConfig(RTC_IT_SEC, ENABLE);

  	/* Wait until last write operation on RTC registers has finished */
  	//RTC_WaitForLastTask();

  	/* Set RTC prescaler: set RTC period to 1sec */
  	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  	/* Wait until last write operation on RTC registers has finished */
  	RTC_WaitForLastTask();

    rtc_RESET();

    BKP_WriteBackupRegister(BKP_DR1, rtc_SIGNATURE);


	return _ERR;
  }
  else
  {
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    //RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

	return _OK;
  }

}


void rtc_COPY_RTC_TO_BUFER(void)
{
struct tm  *timedata;
time_t 	   _time;


   _time = RTC_GetCounter();
   timedata = localtime(&_time);

   memcpy(&RTC_TM,timedata,sizeof(struct tm));	
}

void rtc_COPY_BUFER_TO_RTC(void)
{
//struct tm  *timedata;
time_t 	   _time;

   _time = mktime(&RTC_TM);

   //timedata = localtime(&_time);

   /*if (memcmp(&RTC_TM,timedata,24)!=0)
    return _ERR;
   else
    return _OK;*/

   /* Wait until last write operation on RTC registers has finished */
   RTC_WaitForLastTask();
   /* Change the current time */
   RTC_SetCounter(_time);
   /* Wait until last write operation on RTC registers has finished */
   RTC_WaitForLastTask();

   rtc_COPY_RTC_TO_BUFER();
}



void rtc_RESET(void)
{
//struct tm *my_tm;
time_t _time;
	
   RTC_TM.tm_sec = 0;
   RTC_TM.tm_min = 0;
   RTC_TM.tm_hour = 0;//1
   RTC_TM.tm_mday = 1;
   RTC_TM.tm_mon = 0;
   RTC_TM.tm_year = 70; //2000 год

   _time = mktime(&RTC_TM);

   /* Wait until last write operation on RTC registers has finished */
   RTC_WaitForLastTask();
   /* Change the current time */
   RTC_SetCounter(_time);
   /* Wait until last write operation on RTC registers has finished */
   RTC_WaitForLastTask();

}


void rtc_WRITE_YEAR(unsigned short new_year)
{
//time_t 	   _time;

  // _time = RTC_GetCounter();
  // timedata = localtime(&_time);

   if (new_year>=1900)
    RTC_TM.tm_year = new_year-1900;
   else
    RTC_TM.tm_year = 1900;

/*   _time = mktime(&RTC_TM);
 
   RTC_WaitForLastTask();
   RTC_SetCounter(_time);
   RTC_WaitForLastTask();


  */
}

void rtc_WRITE_MON(unsigned char new_mon)
{
   if (new_mon>12) new_mon = 12;
   else
   if (new_mon<1)  new_mon = 1;
  
   RTC_TM.tm_mon = new_mon-1; 

}

void rtc_WRITE_DAY(unsigned char new_day)
{
   
   if (new_day>31)  RTC_TM.tm_mday = 31;
   else
   if (new_day<1) 	RTC_TM.tm_mday = 1;
   else				RTC_TM.tm_mday = new_day;

}

void rtc_WRITE_HOUR(unsigned char new_hour)
{
   if (new_hour>23) RTC_TM.tm_hour = 23;
   else				RTC_TM.tm_hour = new_hour;
}

void rtc_WRITE_MIN(unsigned char new_min)
{
   if (new_min>59)  RTC_TM.tm_min = 59;
   else				RTC_TM.tm_min = new_min;
}

void rtc_WRITE_SEC(unsigned char new_sec)
{
   if (new_sec>59)  RTC_TM.tm_sec = 59;
   else				RTC_TM.tm_sec = new_sec;
}

void rtc_WRITE(unsigned char ID,unsigned short new_val)
{
 switch (ID)
	{
	 case rtc_YEAR:	if (new_val>=1900)
    					RTC_TM.tm_year = new_val-1900;
   					else
    					RTC_TM.tm_year = 1900;

					break;

	 case rtc_MON:	if (new_val>12) new_val = 12;
   					else
  					if (new_val<1)  new_val = 1;
  
   					RTC_TM.tm_mon = new_val-1; 
					
					break;

	 case rtc_DAY:	if (new_val>31) RTC_TM.tm_mday = 31;
   					else
   					if (new_val<1) 	RTC_TM.tm_mday = 1;
   					else			RTC_TM.tm_mday = new_val;

					break;

	 case rtc_HOUR: if (new_val>23) RTC_TM.tm_hour = 23;
   					else			RTC_TM.tm_hour = new_val;

					break;

	 case rtc_MIN:	if (new_val>59) RTC_TM.tm_min = 59;
   					else			RTC_TM.tm_min = new_val;

					break;

	 case rtc_SEC:	if (new_val>59) RTC_TM.tm_sec = 59;
   					else			RTC_TM.tm_sec = new_val;

					break;
	 default	 :  break;
	}
}

unsigned short rtc_READ(unsigned char ID)
{
 switch (ID)
	{
	 case rtc_YEAR:	return (RTC_TM.tm_year+1900);
	 case rtc_MON:	return (RTC_TM.tm_mon+1);
	 case rtc_DAY:	return (RTC_TM.tm_mday);
	 case rtc_HOUR: return (RTC_TM.tm_hour);
	 case rtc_MIN:	return (RTC_TM.tm_min);
	 case rtc_SEC:	return (RTC_TM.tm_sec); 
	 default	 :  return 0;
	} 
}

unsigned char rtc_CHECK_EVENT_1S(void)
{
  if (RTC_GetFlagStatus(RTC_FLAG_SEC)==SET)
   { 
    RTC_ClearFlag(RTC_FLAG_SEC);
	return 1;
   }
  else
   return 0;
}

