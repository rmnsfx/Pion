/**
  ******************************************************************************
  * @file    Project/Template/main.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "FILTER.h"
#include "File_Config.h"
#include <string.h>
#include <math.h>
#include "stm32f10x_exti.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h" 
#include "mscuser.h"
#include "memory.h"
#include "type.h"
#include "FileIO.h"
#include "wav.h"
#include "IIR.h"
#include "ADC.h"
#include "Pin.h"
#include "ssd1303.h"
#include "FM25V10.h"
#include "FLASH_AT45DB.h"
#include "EXT_ADC.h"
#include "vga_lib.h"
#include "Types.h"
#include "Keyboard.h"
#include "Menu.h"
#include "Reg.h"
#include "RealTime.h"
#include "Road.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_crc.h"


//#include "Backup.h"
//#include "m25p80.h"
 #define TIME_POWER_OFF	(10*60*1000)
 
 

 u32 GLOBAL_TIMER_1ms = 0;
 u32 POWER_OFF		  = TIME_POWER_OFF;

 //s16 DELAY_MESUARE = -1;
 u32 GLOBAL_ERROR;
 u8  USB_CONNECT = 0;
 extern u8	measure_stat = 0;
 extern unsigned int	Num_of_Signals = 0;
 u16 test_reg;
 u16 Sec_timer;
 u16 Moto_Sec;		//мотосекунды
 u8  Flag_1S;
 float		   k_reg;

 u16 T;

 #define GET_SYS_TIMER() 	GLOBAL_TIMER_1ms;
 #define SET_SYS_TIMER(set)	GLOBAL_TIMER_1ms = set;


	

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 USART_InitTypeDef USART_InitStructure;
 GPIO_InitTypeDef  GPIO_InitStructure;
 DMA_InitTypeDef   DMA;
 ADC_InitTypeDef   ADC_InitStructure;

 //ISingleChFrame SIM[16000];
 

 u8				buf[10];			

 //IDubleChFrame  DUB[1000];


 
//BYTE  Memory[MSC_MemorySize];  /* MSC RAM */


//BYTE Memory[MSC_MemorySize];                  // MSC Memory in RAM

 //BYTE Memory[RAM_SIZE];     



//#define MEM2 (BYTE *)0x20000000

//BYTE MEM[MSC_ImageSize];

//калбак функция

void Timer_1ms_CallBack(void)
{
// pin_DEBUG(HIGTH);
 GLOBAL_TIMER_1ms++;
 
 key_1ms();
 men_1ms();

 //if (DELAY_MESUARE>0) DELAY_MESUARE--;
 if (POWER_OFF>0) POWER_OFF--;
 if (Sec_timer==0) {
 					Sec_timer = 1000; 
					Flag_1S   = 1;
				   }
					else Sec_timer--;

 /*pin_KEY(HIGTH);
 pin_KEY(LOW);
 pin_KEY(HIGTH); */
 //pin_DEBUG(LOW);

}
/*
void msDELAY(unsigned int msTIME)
{
  SET_SYS_TIMER(0);
  while (GET_SYS_TIMER
} */

/* Private functions ---------------------------------------------------------*/

void InitAll(void)
{
   RCC->APB2ENR	|= RCC_APB2ENR_IOPCEN;		// Подали такты на порт. Без этого работать не будет. 
						// Жоские вилы STM32 =))) А в симуляторе без этого работает на ура
 


   GPIOC->CRH   |= GPIO_CRH_MODE12_0;  		// Настроили порт на выход
   return;
}

void Delay(long i)
{
 while (i>0) i--;
}

void SET_CLOCK_SPEED(unsigned char CLOCK)
{
 if (CLOCK)
  if ((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL)
   {
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	
   }
  else;
 else
  if ((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSI)  
   {
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));

	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
   }
 
 if (CLOCK)	SysTick_Config(9000*8);	//настройка 1ms таймера  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
 else		SysTick_Config(8000); //настройка 1ms таймера  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
     
}


void GPIO_SETUP()
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

   //включаем порт A
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
   //настраиваем выход P0
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOA,GPIO_Pin_4);
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //настраиваем вход аналоговый вход PFI
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //настраиваем вход USB5V
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //настраиваем вход CHARGE
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //настраиваем выход SPI MCS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOA,GPIO_Pin_4);
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //настраиваем выход SPI MCLK,MISO,MOSI
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOA,GPIO_Pin_5);
   GPIO_SetBits(GPIOA,GPIO_Pin_7);
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   //входы A8-A15 пропускаем
	 
	 
	 //настраиваем вход USB5V_DP
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	 

   //включаем порт B
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
   //настраиваем вход KEY1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем вход KEY2
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //входы B2-B4 пропускаем
   //настраиваем выход CS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;															  
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_5);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем выход RES
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   pin_SSD_RES(LOW);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем выход D/C
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_7);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем выход WR
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_8);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем выход RD
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_9);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем вход KEY3
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем вход KEY4
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure); 	
   //внешний АЦП
   //настраиваем ADC_CS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOB,GPIO_Pin_12);
   pin_ADC_CS(LOW);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем ADC_CLK
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   pin_ADC_CLK(LOW);
   //GPIO_SetBits(GPIOB,GPIO_Pin_12);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //настраиваем ADC_DATA
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_IPU; //наверно нужно настроить как плавающий вход
   GPIO_Init(GPIOB, &GPIO_InitStructure);


   //включаем порт C
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
   //настраиваем выход D0-D7
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   //настраиваем вход KEY5
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   //настраиваем выход DISC
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_11);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
    //настраиваем выход OFF
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_12);
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   //включаем порт D
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
   //включаем порт 3.3VA_ON
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOD,GPIO_Pin_2);
   pin_3VA(OFF);
   GPIO_Init(GPIOD, &GPIO_InitStructure);

//**добавлено для кового варианта*******
//вкл. /выкл.
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_9);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	 Delay(50000);
	 
	 //настраиваем выход 13V_ON (вкл. дисплея)
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_SetBits(GPIOC,GPIO_Pin_10);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	 
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
	 RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	 
	 AFIO->EXTICR[0] &= ~AFIO_EXTICR3_EXTI9_PA;
	 AFIO->EXTICR[0] |= AFIO_EXTICR3_EXTI9_PA;
	 EXTI->FTSR |= EXTI_FTSR_TR9;
	 EXTI->IMR |= EXTI_IMR_MR9;
	 NVIC_SetPriority(EXTI9_5_IRQn, 15);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	 //************
}

void EXTI9_5_IRQHandler ( void )
{
	pin_DISC(1);
	DISPLAY_OFF();
	POWER_OFF();
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


U16 FIL(U16 val)
{
 static U16 buf[8]={0,0,0,0,0,0,0,0};
 static U8  i = 0;
 static U32 SUM = 0;

 buf[i++] = val;
 if (i==4) i = 0;

 SUM = (buf[0]+buf[1]+buf[2]+buf[3]);

 return	 (U16)SUM;

}

U16 TINGULAR(U16 max,U16 Freq)
{
  static U32 val = 0 ;


  val+=(unsigned long)max*65536/Freq;

  if (val>=(unsigned long)max*65536) val = 0;

  return val>>16;
}



//процедура форматирования дискаа
//TStatus FORMAT(void)
//{
//  FILE * pFile;
//  unsigned char temp_data[512];

//  memset(temp_data,0,512);

//  //форматирование
//  mmc_format();//очистка флэш, создание таблицы FAT

//  if (finit()!=0)
//	return _ERR;   //ошибка, форматирование не помогло
//  else	    
//	{   
//	 //создаем нужные нам директории и файлы
//	 //создаем файл уставок размером 1 сектор, пустой
//	 /*pFile = fopen ("M:\\Setings.000","w");
//	 if (pFile==0) return _ERR;
//     else
//      if (fwrite(&temp_data,1,512,pFile)!=512)
//	  										  {
//											   fclose(pFile);
//											   return _ERR;
//											  }
//	 fclose(pFile);*/

// 	 rod_CreateFile();
//	 //pFile = fopen ("M:\\DIR\\","w");
//     //fclose(pFile);
//	 return _OK;
//	}
//}

TStatus FORMAT(void)
{
  FILE * pFile;
  unsigned char temp_data[512];
	s8 i;
	char str_out[5];
	int i2=0;
	long res = 0;

	finit();
	Delay(100000);
	fat_init();
	
		
	vga_CLEAR();
	men_SHOW_MESSAGE("Форматирование...","",0);
	
	
	/// Прогресс-бар
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_NO_FILL);
	vga_UPDATE();	
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,20,33,drRECT_FILL);
	vga_UPDATE();	
	
	memset(temp_data,0,512);
	
  //форматирование
  mmc_format();//очистка флэш, создание таблицы FAT


	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_NO_FILL);
	vga_UPDATE();		
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,60,33,drRECT_FILL);
	vga_UPDATE();	
	
	
// 	REGW(NUMFILE,1);
//	REGW(NUMFILE_CURENT,0);
//	REGW(ROUTE_NUM,0);
//	REGW(BEYOND_ROAD,1);


	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_NO_FILL);
	vga_UPDATE();	
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,100,33,drRECT_FILL);
	vga_UPDATE();	

	
	//res = rod_CreateFile_edit();	

	
	vga_CLEAR();
	men_SHOW_MESSAGE("Форматирование...","",0);
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_NO_FILL);
	vga_UPDATE();	
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_FILL);
	vga_UPDATE();	

	Delay(1400000);
	
	vga_CLEAR();
	vga_SET_POS_TEXT(1,1);
	sprintf(str_out,"Форматирование");		 								
	vga_PRINT_STR(str_out,&FONT_6x8);
	//vga_UPDATE();						
	vga_SET_POS_TEXT(1,12);
	sprintf(str_out,"завершено.");		 								
	vga_PRINT_STR(str_out,&FONT_6x8);
	vga_UPDATE();	

	Delay(1500000);

	vga_CLEAR();
  vga_SET_POS_TEXT(28,25);  
	vga_PRINT_STR("ВЫКЛЮЧЕНИЕ",&FONT_7x10_bold);
	vga_UPDATE();
	
	Delay(1500000);
		
	pin_OFF();
	
	return _OK;
	
	
}

TStatus FAT_Init(void)
{
  u32 res;
	unsigned int i = 0;
  
  //тест FAT
  res = finit();
	
	if (res != 0) 
	while(i++ < 5)
	{		
		res = finit();		
		fat_init();
		if (res == 0) break;
		Delay(100000);
	}
		
		
  //ошибка Flash
  if (res==1)	return _ERR;
  else
  	if (res>1) 
		{
			 vga_CLEAR();		
			 vga_SET_POS_TEXT(1,1);
			 vga_PRINT_STR("Ошибка FAT.",&FONT_6x8);
			 vga_SET_POS_TEXT(1,25);
			 vga_PRINT_STR("Форматировать...?",&FONT_6x8);
			 vga_UPDATE();
					
			while(1)	
			if (key_CHECK_EV(key_EVENT_PRESSED_ENTER)) 
			{ 
				vga_CLEAR();
				vga_UPDATE();
				return FORMAT();				   				
			}
			else
			{
					if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU)) 
					{			
						vga_CLEAR();
						vga_SET_POS_TEXT(28,25);  
						vga_PRINT_STR("ВЫКЛЮЧЕНИЕ",&FONT_7x10_bold);
						vga_UPDATE();
						pin_OFF();
					}
			}			
		}
	  //return FORMAT();	 //пытаемся отформатировать

  return _OK;	 
}

void CONTROL_BAT(unsigned char MIN_VAL_BAT)
{
  //измерение заряда бата
  REGW(BAT,adc_BAT_PERCENT()); //записываем в регист, измеренное значение
  if (REG(BAT)<=MIN_VAL_BAT)   //если заряд меньше 5%
   {
    pin_SSD_RES(LOW);
    pin_OFF(); //отключаем
   }
 
}

void UPDATE_MODE_REG()
{
unsigned int 		temp_reg;
static unsigned int old_mode_reg = 0;

	temp_reg = REG(CHANEL_MESUARE);

 	switch (temp_reg)
						 {
						  case 0x01: temp_reg = (temp_reg<<8)|REG(FILTER_A);	   //ускорение
						  			 break;
						  case 0x02: temp_reg = (temp_reg<<8)|REG(FILTER_V);	   //скорость
						  			 break;
						  case 0x04: temp_reg = (temp_reg<<8)|REG(FILTER_S);	   //перемещение
						  			 break;
						  case 0x08: temp_reg = (temp_reg<<8);
						  			 break;
						  default  : temp_reg = 0;
						 }
  if (old_mode_reg!=temp_reg)
   {
    REGW(MODE_REG,temp_reg);
	old_mode_reg = temp_reg;
	//очистить регистры
	REGWDEF(RMS_VAL);
	REGWDEF(PIK_VAL);
	REGWDEF(PIK_PIK_VAL);
	REGWDEF(PIK_FACTOR_VAL);

   }
  
}

/*
void CONTROL_RTC(void)
{
//событие 1 секунда
  if (rtc_CHECK_EVENT_1S())
	 {
	  rtc_COPY_RTC_TO_BUFER();
	  REGW(YEAR,rtc_READ(rtc_YEAR));
	  REGW(DATA,rtc_READ(rtc_MON)*100+rtc_READ(rtc_DAY));
	  REGW(TIME,rtc_READ(rtc_HOUR)*100+rtc_READ(rtc_MIN));
	  REGW(SECOND,rtc_READ(rtc_SEC));
	 }     
}	*/

void START_MESUARE(void)
{
  //ext_adc_SAMPLING(ext_adc_SIM,ext_SAMPLING_SIZE); //сбросить выборку
	  	  
  //питание аналоговой части
  pin_3VA(ON);
  ext_adc_START();	//старт внешнего АЦП
	  

  //reg_WRITE(PION_STATUS,reg_READ(PION_STATUS)|ST_MESUARE);	  //установить бит в регистре
  //DELAY_MESUARE = 0;//2000;//1500; //задержка в мс
  REGW(SAMPLING_STATUS,0); //присваиваем состояние ожидания
  REGW(PION_STATUS,REG(PION_STATUS)|ST_MESUARE);	  //установить бит в регистре				    
  ext_adc_SAMPLING(ext_adc_SIM,ext_SAMPLING_SIZE);  //задаем параметры выборки
  
}

void STOP_MESUARE(void)
{
 
 pin_3VA(OFF);
 ext_adc_STOP();	//стоп внешнего АЦП

 REGW(PION_STATUS,REG(PION_STATUS)&(~ST_MESUARE));  //сбросить бит в регистре
 if (REG(SAMPLING_STATUS)==0) REGWDEF(SAMPLING_STATUS); //присваиваем состояние ожидания

 //DELAY_MESUARE = -1;

 
}

void CONTROL_CHARGE()
{
 //если зарядка то отключаем
 //if (pin_CHARGE==0) pin_OFF();
}

void CONTROL_POWER(u8 RESET)
{
 
 if (RESET) POWER_OFF = TIME_POWER_OFF; 
 else
  if (POWER_OFF==0) 	pin_OFF(); //отключаем	
}

void MESUARE_PROCESS(void)
{
  //unsigned int temp_reg;
  float		   reg_1;
  float		   reg_2;
  float		   rms_reg;
 
  	  //расчет RMS
		  k_reg = ((float)REG(K_VIBRO)/1000);  //коэффициент
			
		  //rms_reg = iir_RETURN_RMS(&DETECTOR)*((float)REG(K_VIBRO)/1000);

      	  switch (REG(MODE_REG))
		   {
		    //ускорение
		    case 0x0101: //канал ускорения
						 k_reg = k_reg/3;//30
						 break; 
			case 0x0102: //канал ускорения
						 k_reg = k_reg/3;//30
						 break;
			case 0x0104: //канал ускорения
						 k_reg = k_reg/3;//30
						 break; 
			case 0x0108: //канал ускорения
						 k_reg = k_reg/3;//30
						 break;
			case 0x0110: //канал ускорения
						 k_reg = k_reg/3;//30
						 break;
			//скорость
			case 0x0201: k_reg = k_reg*114/3;	//канал скорости, 2-1000Гц	  //114
						 //temp_reg = sqrt(rms_reg*rms_reg-3*3); //вычитаем шум 0,3мм/с
						 //temp_reg = rms_reg;
						 break;
			case 0x0202: k_reg = k_reg*20.916/3;	//канал скорости, 10-1000Гц // 14,925
						 break;
			case 0x0204: k_reg = k_reg*20.916/3;	//канал скорости, 10-2000Гц
						 break;
			case 0x0208: k_reg = k_reg*20.916/3;	//канал скорости, 10-5000Гц
						 break;
		    //перемещение
			case 0x0401: k_reg = k_reg*395.570/300;	//канал перемещения, 10-300Гц //208.986
						 break;
			default: 	 //temp_reg = 0; 
						 break;		
		   }
		  //расчет СКЗ
		  rms_reg = iir_RETURN_RMS(&DETECTOR)*k_reg;  
		  if (REG(MODE_REG)==0x0201) rms_reg = sqrt(rms_reg*rms_reg-20*20); //вычитаем шум для диапазон 2-1000 Гц	  //шум = 0.20 мм/с

		  REGW(RMS_VAL,rms_reg);						 	//заносим результат измерения в регистр
		  //расчет пик
		  reg_1 = iir_RETURN_MAX_PIK(&DETECTOR);
		  reg_2 = iir_RETURN_MIN_PIK(&DETECTOR)*(-1);
		  if (reg_1<reg_2) reg_1 = reg_2; 
		  reg_1 = reg_1*k_reg;

			if (REG(MODE_REG)==0x0101) 
		   if (reg_1>4) reg_1-=4; else reg_1=0;	
			if (REG(MODE_REG)==0x0102) 
		   if (reg_1>4) reg_1-=4; else reg_1=0;			
			if (REG(MODE_REG)==0x0104) 
		   if (reg_1>7) reg_1-=7; else reg_1=0;	
			if (REG(MODE_REG)==0x0108) 
		   if (reg_1>14) reg_1-=14; else reg_1=0;	
			
			if (REG(MODE_REG)==0x0201) 
		   if (reg_1>70) reg_1-=70; else reg_1=0;	
			if (REG(MODE_REG)==0x0202) 
		   if (reg_1>8) reg_1-=8; else reg_1=0;	
			if (REG(MODE_REG)==0x0204) 
		   if (reg_1>8) reg_1-=8; else reg_1=0;	
			if (REG(MODE_REG)==0x0208) 
		   if (reg_1>10) reg_1-=10; else reg_1=0;
			
		  if (REG(MODE_REG)==0x0401) 
		   if (reg_1>5) reg_1-=5; else reg_1=0;			//вычитаем амплитудный шум = 2 мкм
		  
			
		  REGW(PIK_VAL,reg_1);						   	//заносим результат измерения в регистр

		  //расчет пик-фактора
		  if (rms_reg>5) reg_1 = reg_1/rms_reg;
		  else			  reg_1 = 0;
		  REGW(PIK_FACTOR_VAL,reg_1*100);

		  //расчет пик-пик
		  reg_1 = iir_RETURN_PIK_PIK(&DETECTOR)*k_reg;
		  if (reg_1<0) reg_1 = 0;
			
			if (REG(MODE_REG)==0x0101) 
		   if (reg_1>8) reg_1-=8;else reg_1=0;	
			if (REG(MODE_REG)==0x0102) 
		   if (reg_1>8) reg_1-=8;else reg_1=0;	
			if (REG(MODE_REG)==0x0104) 
		   if (reg_1>14) reg_1-=14;else reg_1=0;	
			if (REG(MODE_REG)==0x0108) 
		   if (reg_1>30) reg_1-=30;else reg_1=0;	
			
			if (REG(MODE_REG)==0x0201) 
		   if (reg_1>130) reg_1-=130;else reg_1=0;	
			if (REG(MODE_REG)==0x0202) 
		   if (reg_1>20) reg_1-=20;else reg_1=0;	
			if (REG(MODE_REG)==0x0204) 
		   if (reg_1>20) reg_1-=20;else reg_1=0;	
			if (REG(MODE_REG)==0x0208) 
		   if (reg_1>20) reg_1-=20;else reg_1=0;
			
		  if (REG(MODE_REG)==0x0401) 
		   if (reg_1>10) reg_1-=10;else reg_1=0;			//вычитаем амплитудный шум = 4 мкм
			
		  REGW(PIK_PIK_VAL,reg_1);					  	//заносим результат измерения в регистр
	  	  
	  	  
}


void JumpToApplication(uint32_t addr)
{
  typedef  void (*pFunction)(void);
  pFunction Jump_To_Application;
  uint32_t JumpAddress;
  //if(addr>=0x08005000)
  //{
    JumpAddress = *(uint32_t*) (addr + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(vu32*) addr);
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, addr-0x8000000);
    Jump_To_Application();
 // }
}


int main(void)

{
	FILE  		*pRFile = 0;
  //unsigned char data;
  uint32_t bt[5];
	uint8_t bf[2];
  //volatile int   x1,y1,y2;
  //DWORD n;
//void (*JumptoApp)(void);
//JumpToApplication(0x8020000);
//JumptoApp=(void(*)(void))0x08020004;   //?????? ???? ?????????
//JumptoApp();
	unsigned int j,carry_flag,a;
	uint16_t crc = 0xffff;
	char FileName[15];
  //unsigned int temp_reg;
	uint32_t chsumm;
  
  unsigned int i = 0;
  unsigned char num;
  //FILE * pFile;
  uint32_t mask;
	uint8_t first;
	uint8_t second;
	uint8_t third;
	uint8_t four;
	char str [10];
	int aa;
	
	int *var = (int*)0x8010000;
	//int x4 __attribute__((at(0x8010000))) = 123;
	
	
	
	
	
  Delay(100000);
  Delay(100000);
  Delay(100000);

  GPIO_SETUP();
  //CONTROL_CHARGE();
  Delay(100000);
  Delay(100000);
  Delay(100000);
  
  //???????? ???????
 // CONTROL_CHARGE();
   //?????? 13?
  pin_13V(HIGTH);
  Delay(100000);
  Delay(100000);
  Delay(100000);
  

  
  SystemInit();
  SET_CLOCK_SPEED(CLK_72MHz); 
  //????????????? ??????????? ??? ? ???????? ?????? ????-----------------------------//
  adc_SETUP();
  //if (adc_BAT_PERCENT()<=5)  pin_OFF(); //?????????	???? <5%
	
	vga_INIT();
	
	FAT_Init();
	
	
	
	
			
			
	
/*	vga_SET_POS_TEXT(1,1);
	vga_PRINT_STR("Bootloader v 2.0",&FONT_6x8);	
	vga_UPDATE();

while (!pin_USB_5V);		
	USB_Init();
	USB_CONNECT = pin_USB_5V;
	USB_Connect(USB_CONNECT);	
	//FORMAT();*/


if (key_CHECK_EV(key_EVENT_PRESSED_MESUARE))
	{
	vga_SET_POS_TEXT(1,1);
	vga_PRINT_STR("Bootloader v 2.9",&FONT_6x8);	
	vga_UPDATE();

  while (!pin_USB_5V);		
		
	USB_Init();
	USB_CONNECT = pin_USB_5V;
	USB_Connect(USB_CONNECT);	
	
	while (pin_USB_5V);		
		
	}
	else
	{

		
		if ((pRFile = fopen ("M:\\prog1.bin","rb")) != NULL)
		{
			fclose(pRFile);
			
			 vga_SET_POS_TEXT(1,1);
			 vga_PRINT_STR("Подождите...",&FONT_6x8);	
			 vga_SET_POS_TEXT(1,25);
			 vga_PRINT_STR("идет обновление ПО.",&FONT_6x8);			
			 vga_UPDATE();
			// checksum
			
			for (num = 1; num < 3; num++)
			{
			
				sprintf(FileName, "%s%d%s", "M:\\prog", num, ".bin");
				
					if ((pRFile = fopen (FileName,"rb")) != NULL)
					{
						
								while (fread(bf, sizeof(uint8_t), 1, pRFile) == 1)
								{
									crc=crc^bf[0];
									for (j=0;j<8;j++)
									{
										a=crc;
										carry_flag=a&0x0001;
										crc=crc>>1;
										if (carry_flag==1)
											crc=crc^0xa001;
									}
								}


							fclose(pRFile);
							if (crc != 0) 
								{

								vga_CLEAR();
								vga_SET_POS_TEXT(1,25);
								vga_PRINT_STR("Файлы ПО повреждены.",&FONT_6x8);
								vga_SET_POS_TEXT(1,34);
								vga_PRINT_STR("Нажмите ENTER.",&FONT_6x8);
								vga_UPDATE();								
								goto JumpToApplication;

							}
							crc = 0xffff;
					}

			}
//vga_SET_POS_TEXT(1,34);	vga_PRINT_STR("1111111",&FONT_6x8);	vga_UPDATE();	
		//fclose(pRFile);	

	//???????	
			FLASH_Unlock();
			for(i = 0;  i<191; i++)
			{
			//sprintf(FileName, "%s%d", "prog", i);
				//vga_SET_POS_TEXT(1,44);
				//vga_PRINT_STR(FileName,&FONT_6x8);	vga_UPDATE();	
			FLASH_ErasePage(0x8010000+i*0x800);
			}
			FLASH_Lock();
			//vga_SET_POS_TEXT(1,44);	vga_PRINT_STR("222222222",&FONT_6x8);	vga_UPDATE();	
			i=0;
	//???????
			FLASH_Unlock();
			for (num = 1; num < 10; num++)
			{
				sprintf(FileName, "%s%d%s", "M:\\prog", num, ".bin");
				
					if ((pRFile = fopen (FileName,"rb")) != NULL)
					{
							while (fread(bt, sizeof(unsigned int), 1, pRFile) == 1)
							{
								FLASH_ProgramWord(0x8010000+i*4,bt[0]);
								i++;
							}
							fclose(pRFile);
							//unlink(FileName);
							//remove(FileName);
					}

			}
		
			
			
			
			
			
			FLASH_Lock();
			vga_CLEAR();
			vga_SET_POS_TEXT(1,25);
			vga_PRINT_STR("ПО успешно обновлено!",&FONT_6x8);
			vga_SET_POS_TEXT(1,34);
			vga_PRINT_STR("Нажмите ENTER.",&FONT_6x8);
			vga_UPDATE();	
			

			
			
			

			
			JumpToApplication:		
			for (num = 1; num < 10; num++)
			{
					sprintf(FileName, "%s%d%s", "M:\\prog", num, ".bin");
					unlink(FileName);
			}
			while (!key_CHECK_EV(key_EVENT_PRESSED_ENTER));
			JumpToApplication(0x8010000);			
	}
	vga_SET_POS_TEXT(1,1);
	vga_PRINT_STR("*",&FONT_6x8);		
	vga_UPDATE();

///	pin_CHARGE_OFF;
		
		if (*var == 0xffffffff) 
		{									
					
				while (1)
				{
						if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU))
						POWER_OFF();
				}			
				
		}
		
	
	

		
		
			for (num = 1; num < 10; num++)
			{
					sprintf(FileName, "%s%d%s", "M:\\prog", num, ".bin");
					unlink(FileName);
			}
	
			JumpToApplication(0x8010000);
		
	}

JumpToApplication(0x8010000);
	

}//конец программы

 /*   if (offset<50) offset++;else offset = 0;

	if ((pin_CHARGE==FALSE)&&(offset>25))
	 {
	  //ssd_PRINT("CHARGE",0,5);
	 }
	else
	 {
	  //ssd_PRINT("      ",0,5);
	 }	  
	
	//Delay(0xFFFF);
  */


/*
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
/*
PUTCHAR_PROTOTYPE
{
  // Place your implementation of fputc here 
  // e.g. write a character to the USART 
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  // Loop until the end of transmission 
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
} */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
