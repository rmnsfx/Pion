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

 

void Timer_1ms_CallBack(void)
{

 GLOBAL_TIMER_1ms++;
 
 key_1ms();
 men_1ms();


 if (POWER_OFF>0) POWER_OFF--;
 if (Sec_timer==0) {
 					Sec_timer = 1000; 
					Flag_1S   = 1;
				   }
					else Sec_timer--;

}

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
   //pin_3VA(OFF);
	 pin_3VA(1);
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
TStatus FORMAT(void)
{
  FILE * pFile;
  unsigned char temp_data[512];

  memset(temp_data,0,512);

  //форматирование
  mmc_format();//очистка флэш, создание таблицы FAT

  if (finit()!=0)
	return _ERR;   //ошибка, форматирование не помогло
  else	    
	{   
	
 	 rod_CreateFile();

	 return _OK;
	}
}

TStatus FAT_Init(void)
{
  u32 res;
  
  //тест FAT
  res = finit();

  if (res==1)
    //ошибка Flash
	return _ERR;
  else
  	if (res>1) 
	  return FORMAT();	 //пытаемся отформатировать

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


void CONTROL_POWER(u8 RESET)
{
 
 if (RESET) POWER_OFF = TIME_POWER_OFF; 
 else
  if (POWER_OFF==0) 	pin_OFF(); //отключаем	
}




void JumpToApplication(uint32_t addr)
{
  typedef  void (*pFunction)(void);
  pFunction Jump_To_Application;
  uint32_t JumpAddress;

    JumpAddress = *(uint32_t*) (addr + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(vu32*) addr);
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, addr-0x8000000);
    Jump_To_Application();

}


int main(void)

{
	FILE  		*pRFile = 0;
  uint32_t bt[5];
	uint8_t bf[2];

	unsigned int j,carry_flag,a;
	uint16_t crc = 0xffff;
	char FileName[15];
	uint32_t chsumm;
  
  unsigned int i = 0;
  unsigned char num;
  uint32_t mask;
	uint8_t first;
	uint8_t second;
	uint8_t third;
	uint8_t four;
	char str [10];
	int aa;
	
	int *var = (int*)0x8010000;
	
	
	
  Delay(100000);
  Delay(100000);
  Delay(100000);

  GPIO_SETUP();
  Delay(100000);
  Delay(100000);
  Delay(100000);
  

  pin_13V(HIGTH);
  Delay(100000);
  Delay(100000);
  Delay(100000);
  

  
  SystemInit();
  SET_CLOCK_SPEED(CLK_72MHz);   
  adc_SETUP();
	
	vga_INIT();	
	
	FAT_Init();
	



	if (key_CHECK_EV(key_EVENT_PRESSED_MESUARE))
	{
			vga_SET_POS_TEXT(1,1);
			vga_PRINT_STR("Bootloader v 2.7",&FONT_6x8);	
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
			 vga_PRINT_STR("Подождите.",&FONT_6x8);	
			 vga_SET_POS_TEXT(1,25);
			 vga_PRINT_STR("Идет обновление ПО.",&FONT_6x8);			
			 vga_UPDATE();
			
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
								vga_PRINT_STR("Файлы ПО повреждены",&FONT_6x8);
								vga_SET_POS_TEXT(1,34);
								vga_PRINT_STR("Нажмите ENTER",&FONT_6x8);
								vga_UPDATE();								
								goto JumpToApplication;

							}
							crc = 0xffff;
					}

			}

			FLASH_Unlock();
			
			for(i = 0;  i<191; i++)
			{
				FLASH_ErasePage(0x8010000+i*0x800);
			}
			
			FLASH_Lock();
			
			i=0;

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
					}

			}
		
			
						
			FLASH_Lock();
			vga_CLEAR();
			vga_SET_POS_TEXT(1,25);
			vga_PRINT_STR("ПО успешно обновлено",&FONT_6x8);
			vga_SET_POS_TEXT(1,34);
			vga_PRINT_STR("Нажмите ENTER",&FONT_6x8);
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
