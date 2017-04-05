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
//nclude <io.h>
#include "FILTER.h"
#include "File_Config.h"
#include <string.h>
#include <math.h>
#include "stm32f10x_exti.h"
#include "stm32f10x_iwdg.h"
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
#include "ff.h"
#include "diskio.h"
#include "Flash.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_crc.h"






  extern  FATFS   fls=0;            
  extern  FIL     FileTmp=0;          
	extern  FRESULT res_t=0; 
	extern 	FILINFO fno=0;

 #define TIME_POWER_OFF	(10*60*1000)


int GLOBAL_TIMER_1ms = 0;
int POWER_OFF = TIME_POWER_OFF;


 unsigned int GLOBAL_ERROR;
 extern int USB_CONNECT=0;
 extern u8	measure_stat=0;
 extern u8	usb_stat=0; 
 extern unsigned int	Num_of_Signals=0;
 u16 test_reg;
 u16 Sec_timer;
 u16 Moto_Sec;		//�����������
 u8  Flag_1S;
 float		   k_reg;
 
unsigned int prgb_bat = 0;
bool usb_transit=0;


 u16 T;
 
 
int first_flag = 0; /// ���� ������� ���������
int message_status = 0;

int timer1=0;
int timer2=0;
int timer3=0;
int timer4=0;
int timer5=0;
 
int frzbat1=0;
int frzbat2=0; 

int usb_addr=0; 
int usb_count=0;

int id_akb = 0; //�� ������� (1-������, 0-�����)

unsigned int usb_charge_state = 1; //���������� ��������� ������� (0 - ����, 1- ���)

float akbtemp = 0;
float akbtimer1 = 0;
float akbtimer2 = 0;
float akbemk = 0;
float akbemk_count = 0;
float akbemk_tek = 0;
float akbemk_percent = 0;
float akbemk_volt = 0;
int akbstatus = -1;
float akbemk_menu = 0;
FATINFO sdinfo;
unsigned char SD_SWITCH; 
unsigned char autooff = 0; 




USB_DEVICE_DESCRIPTOR USB_D;

 #define GET_SYS_TIMER() 	GLOBAL_TIMER_1ms;
 #define SET_SYS_TIMER(set)	GLOBAL_TIMER_1ms = set;

#define FULL_BATTERY_VOLT 4500
	

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 USART_InitTypeDef USART_InitStructure;
 GPIO_InitTypeDef  GPIO_InitStructure;
 DMA_InitTypeDef   DMA;
 ADC_InitTypeDef   ADC_InitStructure;

 

u8				buf[10];			
extern Tmen_status				men_STATUS;

extern unsigned int road_pos;
extern unsigned int road_cursor_pos;



//������ �������

void Timer_1ms_CallBack(void)
{
	
	
	
			IWDG_ReloadCounter();

			GLOBAL_TIMER_1ms++;			 
			
			men_1ms();			
			key_1ms();

			if(timer4 == 5)
			{
				
				timer4=0;	
			}
			
			if (autooff == 1 && pin_USB_5V == 0)	
			POWER_OFF--;
				
			 if (Sec_timer==0) {
								Sec_timer = 1000; 
								Flag_1S   = 1;
								 }
								else Sec_timer--;


			
			if (timer1 == 1000) 
			{				
				timer1 = 0;
				timer2++;
				timer3++;
			}
			else timer1++;
			
			if (timer2 == 30) timer2 = 0; ///30 ������
			if (timer3 == 900000) timer3 = 0; ///15 ���.
			timer4++;
									
			if(timer5 == 10)
			{
				disk_timerproc();
				timer5=0;	
			}
			else timer5++;
			
}


/* Private functions ---------------------------------------------------------*/

void InitAll(void)
{
   RCC->APB2ENR	|= RCC_APB2ENR_IOPCEN;		// ������ ����� �� ����. ��� ����� �������� �� �����. 
						// ������ ���� STM32 =))) � � ���������� ��� ����� �������� �� ���
 


   GPIOC->CRH   |= GPIO_CRH_MODE12_0;  		// ��������� ���� �� �����
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
 
 if (CLOCK)	SysTick_Config(9000*8);	//��������� 1ms �������  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
 else		SysTick_Config(8000); //��������� 1ms �������  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
     
}


void GPIO_SETUP()
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

   //�������� ���� A
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
   //����������� ����� P0
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOA,GPIO_Pin_4);
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //����������� ���� ���������� ���� PFI
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //����������� ���� USB5V
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //����������� ���� DISCHARGE
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   //����������� ����� SPI MCS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOA,GPIO_Pin_4);
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //����������� ����� SPI MCLK,MISO,MOSI
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOA,GPIO_Pin_5);
   GPIO_SetBits(GPIOA,GPIO_Pin_7);
   GPIO_Init(GPIOA, &GPIO_InitStructure);	 
	 //�������
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   //����� A8-A15 ����������

   //�������� ���� B
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
   //����������� ���� KEY1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);   	 
	 //����������� ���� KEY2
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);	  
	 
   //����� B2-B4 ����������
   //����������� ����� CS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;															  
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_5);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ����� RES
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   pin_SSD_RES(LOW);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ����� D/C
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_7);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ����� WR
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_8);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ����� RD
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOB,GPIO_Pin_9);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ���� KEY3
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ���� KEY4
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure); 	


	 

	 
	 
   //������� ���
   //����������� ADC_CS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOB,GPIO_Pin_12);
   pin_ADC_CS(LOW);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ADC_CLK
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   pin_ADC_CLK(LOW);
   //GPIO_SetBits(GPIOB,GPIO_Pin_12);
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //����������� ADC_DATA
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_IPU; //������� ����� ��������� ��� ��������� ����
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //������������� ��� (1-������, 0-�����)
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   //�������� ���� C
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
   //����������� ����� D0-D7
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   //����������� ���� KEY5
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   //����������� ����� 13V_ON
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   pin_13V(LOW);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   //����������� ����� DISC
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_11);
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
    //����������� ����� OFF
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	 
	 GPIO_ResetBits(GPIOC,GPIO_Pin_13);
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   //�������� ���� D
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
   //�������� ���� 3.3VA_ON
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_SetBits(GPIOD,GPIO_Pin_2);
   pin_3VA(ON);
   GPIO_Init(GPIOD, &GPIO_InitStructure);


//**��������� ��� ������ ��������*******
//���. /����.
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_SetBits(GPIOC,GPIO_Pin_9);
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	 Delay(50000);
	 
	 // ���/���� ���������
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
	 
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_BKP,ENABLE);
	
//	//���� ������ �����., ����������� �� �����
//	if ( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 1) 
//	{		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);		
//	}
	
}

void ShowPowerOffForm ( void )
{
  vga_CLEAR();
  //vga_SET_POS_TEXT(28,25);
	//vga_PRINT_STR("����������",&FONT_7x10_bold);
	vga_UPDATE();
}

void EXTI9_5_IRQHandler ( void )
{
	uint32_t i;
	
	
  
	
	if ( (pin_USB_5V) || (usb_transit) )
	{
		
		EXTI->PR |= EXTI_PR_PR1; //����� ����� ����������

		
	}
	else
	{			
	
			rod_DEINIT();
			SET_CLOCK_SPEED(CLK_72MHz); 
			
			ShowPowerOffForm();
			for(i=0;i<0x1FFFFF;i++){__NOP();}	

			pin_DISC(1);
			DISPLAY_OFF();
			pin_OFF();	
			while(1) IWDG_ReloadCounter();
	}
	
	
	
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


U16 TINGULAR(U16 max,U16 Freq)
{
  static U32 val = 0 ;


  val+=(unsigned long)max*65536/Freq;

  if (val>=(unsigned long)max*65536) val = 0;

  return val>>16;
}

void progressbar_percent(void)
{
	 char str_out2[5];
	 int ii = 1000;
	
	 while (ii--)
	 {
			vga_CLEAR();
			vga_SET_POS_TEXT(1,1);
			vga_PRINT_STR("��������������...",&FONT_6x8);
		 
			vga_SET_POS_TEXT(1,1);
			sprintf(str_out2,"%.1f%%", (1000 - ii)/10.0);		 								
			vga_PRINT_STR(str_out2,&FONT_6x8);
			vga_UPDATE();			
		 
	 }	 
	 
}

void progressbar(unsigned int value)
{
	int ii = 0;
	char str_out2[5];
	float k = 0;
	
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	vga_RECTANGLE(1,30,vga_GET_WIDTH_DISPLAY-3,33,drRECT_NO_FILL);

	k = value * 1.24;
	
	while (ii++ <= k)
	{		
		vga_SET_DRAW_MODE(drMODE_NORMAL);				 	
		vga_RECTANGLE(1,30, ii,33,drRECT_FILL);		
	}		
		
	vga_SET_POS_TEXT(1,1);
  vga_PRINT_STR("��������������:", &FONT_6x8);	
	vga_SET_POS_TEXT(103,1);
	sprintf(str_out2,"%d%%", value);
	vga_PRINT_STR(str_out2, &FONT_6x8);
	vga_UPDATE();	
	
}


unsigned int FAT_Init(void)
{
  u32 res;
	unsigned int e = 0;
  
	SPI_SETUP();		
	
	res = disk_initialize(0);
	
	if (res != 0) 
	{
		while(e++ < 5)
		{
			res = disk_initialize(0);	
			if (res == 0) break;
			Delay(10000);
		}
	}
	
	finit();   

  return res;	 
}

//��������� �������������� ������
TStatus FORMAT(void)
{
  FILE * pFile;
  unsigned char temp_data[512];
	s8 i;
	char str_out[5];
	int i2=0;
	unsigned char res = 0;
	uint8_t buf[512];

	FAT_Init();
	
	SET_CLOCK_SPEED(CLK_72MHz);

	IWDG_ReloadCounter();	
	
	vga_CLEAR();
		
  res = mmc_format();//������� ����, �������� ������� FAT

//	while (i++ < 3 || res == 0)		
//	res = mmc_format();			
	
	SET_CLOCK_SPEED(CLK_8MHz);
		
	IWDG_ReloadCounter();
	
 	REGW(NUMFILE,1);
	REGW(NUMFILE_CURENT,0);
	REGW(ROUTE_NUM,0);
	REGW(BEYOND_ROAD,1);

  for(i = 60; i<80; i++) 
	{
		progressbar(i);
		Delay(500);
	}
	
	FAT_Init();
	
	res = rod_CreateFile_edit();	
	
	for(i = 80; i<=100; i++) 
	{
		progressbar(i);
		Delay(500);
	}
	
	Delay(1500000);		
	
	vga_CLEAR();
	vga_SET_POS_TEXT(1,1);
	sprintf(str_out,"��������������");		 								
	vga_PRINT_STR(str_out,&FONT_6x8);
	//vga_UPDATE();						
	vga_SET_POS_TEXT(1,12);
	sprintf(str_out,"���������.");		 								
	vga_PRINT_STR(str_out,&FONT_6x8);
	vga_UPDATE();				

	IWDG_ReloadCounter();
	
//	vga_SET_POS_TEXT(10,1);
//	sprintf(str_out,"%d",res);		 								
//	vga_PRINT_STR(str_out,&FONT_6x8);
//	vga_UPDATE();		

	Delay(1500000);		
	
//	vga_CLEAR();
//  vga_SET_POS_TEXT(28,25);  
//	vga_PRINT_STR("����������",&FONT_7x10_bold);
//	vga_UPDATE();
//	
//	Delay(1500000);		
//	
	BKP_WriteBackupRegister(BKP_DR12, 0); ///��������� A,V
//		
//	pin_OFF();	
	
	return _OK;
	
}



void CONTROL_BAT(unsigned char MIN_VAL_BAT)
{

	char t_str[20];
	
	
	if (id_akb == 0) //����� ���
	{
					 if (akbemk_percent < 10 && akbemk_percent > 2 && message_status == 0 && measure_stat == 0)   //���� ����� ������ 10%		
					 {								 
							vga_CLEAR();					
							vga_SET_POS_TEXT(5,20);
							vga_PRINT_STR("����� ������������", &FONT_6x8);
							vga_SET_POS_TEXT(5,30);
							vga_PRINT_STR("������ 10% ...",&FONT_6x8);		
							vga_UPDATE();		
							
							message_status = 1; ///���� ������������ ������ 
							 
							Delay (2500000);													 
								 						
					 }
					 
					 
					 
					 if ( measure_stat == 0 && akbemk_volt <= 2.6 )   
					 {	
							vga_CLEAR();					
							vga_SET_POS_TEXT(5,20);
							vga_PRINT_STR("�����������", &FONT_6x8);
							vga_SET_POS_TEXT(5,30);
							vga_PRINT_STR("��������.",&FONT_6x8);								
							vga_UPDATE();						
						 
							Delay(3500000);
																				 
							vga_CLEAR();
							vga_SET_POS_TEXT(28,25);  
							vga_PRINT_STR("����������",&FONT_7x10_bold);
							vga_UPDATE();
						
							Delay(2500000);						


							message_status = 1;						 						 
							
							pin_OFF(); //���������			
							
							
					 }
					 
					 
	}
	else
	{	
	
					 if (adc_BAT_PERCENT_edit() <= 10 && adc_BAT_PERCENT_edit() > 2 && message_status == 0 && measure_stat == 0)   //���� ����� ������ 10		

					 {
								 
						vga_CLEAR();					
						vga_SET_POS_TEXT(5,20);
						vga_PRINT_STR("����� ������������", &FONT_6x8);
						vga_SET_POS_TEXT(5,30);
						vga_PRINT_STR("������ 10% ...",&FONT_6x8);		
						vga_UPDATE();		
						
						message_status = 1; ///���� ������������ ������ 
						 
						Delay (2500000);						
						
					 }

						
					if (adc_BAT_PERCENT_edit() <= 1 && measure_stat == 0 && message_status == 0)   
					{	
						vga_CLEAR();					
						vga_SET_POS_TEXT(5,20);
						vga_PRINT_STR("�����������", &FONT_6x8);
						vga_SET_POS_TEXT(5,30);
						vga_PRINT_STR("��������.",&FONT_6x8);		

					
						vga_UPDATE();						
						 
						//Delay(5000000);
						//for(i=0;i<0x2FFFFF;i++){__NOP();}

						//ShowPowerOffForm();
						 
						Delay(5000000);
						 
						//pin_SSD_RES(LOW);		 
						vga_CLEAR();
						vga_UPDATE();
						pin_OFF(); //���������
						
						
					
					}
										
	}
 
}

void UPDATE_MODE_REG()
{
unsigned int 		temp_reg;
static unsigned int old_mode_reg = 0;

	temp_reg = REG(CHANEL_MESUARE);

 	switch (temp_reg)
						 {
						  case 0x01: temp_reg = (temp_reg<<8)|REG(FILTER_A);	   //���������
						  			 break;
						  case 0x02: temp_reg = (temp_reg<<8)|REG(FILTER_V);	   //��������
						  			 break;
						  case 0x04: temp_reg = (temp_reg<<8)|REG(FILTER_S);	   //�����������
						  			 break;
						  case 0x08: temp_reg = (temp_reg<<8);
						  			 break;
						  default  : temp_reg = 0;
						 }
  if (old_mode_reg!=temp_reg)
   {
    REGW(MODE_REG,temp_reg);
	old_mode_reg = temp_reg;
	//�������� ��������
	REGWDEF(RMS_VAL);
	REGWDEF(PIK_VAL);
	REGWDEF(PIK_PIK_VAL);
	REGWDEF(PIK_FACTOR_VAL);

   }
  
}


void CONTROL_RTC(void)
{
//������� 1 �������
  if (rtc_CHECK_EVENT_1S())
	 {
	  rtc_COPY_RTC_TO_BUFER();
	  REGW(YEAR,rtc_READ(rtc_YEAR));
	  REGW(DATA,rtc_READ(rtc_MON)*100+rtc_READ(rtc_DAY));
	  REGW(TIME,rtc_READ(rtc_HOUR)*100+rtc_READ(rtc_MIN));
	  REGW(SECOND,rtc_READ(rtc_SEC));
	 }     
}	

void START_MESUARE(void)
{
  
	//������� ���������� �����
	pin_3VA(ON);
  
  ext_adc_START();	//����� �������� ���
	
  REGW(SAMPLING_STATUS,0); //����������� ��������� ��������
  REGW( PION_STATUS,REG(PION_STATUS)|ST_MESUARE );	  //���������� ��� � ��������				    		    
  ext_adc_SAMPLING(ext_adc_SIM,ext_SAMPLING_SIZE);  //������ ��������� �������
  
}

void STOP_MESUARE(void)
{
 
 //pin_3VA(OFF);
 ext_adc_STOP();	//���� �������� ���

 REGW(PION_STATUS,REG(PION_STATUS)&(~ST_MESUARE));  //�������� ��� � ��������
 if (REG(SAMPLING_STATUS)==0) REGWDEF(SAMPLING_STATUS); //����������� ��������� ��������

 //DELAY_MESUARE = -1;

 
}



void CONTROL_POWER(u8 RESET)
{
		 if (key_STATE > 0) POWER_OFF = TIME_POWER_OFF; 

		 if (pin_USB_5V == 1) POWER_OFF = TIME_POWER_OFF; 	
	
		 ///if (measure_stat == 2) POWER_OFF = TIME_POWER_OFF; 
		
		 if (autooff == 1) 	
		 if (POWER_OFF <= 0) 	
		 {
				pin_OFF();
		 }
	
}



void MESUARE_PROCESS(void)
{
  //unsigned int temp_reg;
  float		   reg_1;
  float		   reg_2;
  float		   rms_reg;
 
  	  //������ RMS
		  k_reg = ((float)REG(K_VIBRO)/1000);  //�����������
			
		  //rms_reg = iir_RETURN_RMS(&DETECTOR)*((float)REG(K_VIBRO)/1000);

      	  switch (REG(MODE_REG))
		   {
		    //���������
		    case 0x0101: //����� ���������
						 k_reg = k_reg/3;//30
						 break; 
			case 0x0102: //����� ���������
						 k_reg = k_reg/3;//30
						 break;
			case 0x0104: //����� ���������
						 k_reg = k_reg/3;//30
						 break; 
			case 0x0108: //����� ���������
						 k_reg = k_reg/3;//30
						 break;
			case 0x0110: //����� ���������
						 k_reg = k_reg/3;//30
						 break;
			//��������
			case 0x0201: k_reg = k_reg*114/3;	//����� ��������, 2-1000��	  //114
						 //temp_reg = sqrt(rms_reg*rms_reg-3*3); //�������� ��� 0,3��/�
						 //temp_reg = rms_reg;
						 break;
			case 0x0202: k_reg = k_reg*20.916/3;	//����� ��������, 10-1000�� // 14,925
						 break;
			case 0x0204: k_reg = k_reg*20.916/3;	//����� ��������, 10-2000��
						 break;
			case 0x0208: k_reg = k_reg*20.916/3;	//����� ��������, 10-5000��
						 break;
		    //�����������
			case 0x0401: k_reg = k_reg*395.570/300;	//����� �����������, 10-300�� //208.986
						 break;
			default: 	 //temp_reg = 0; 
						 break;		
		   }
		  //������ ���
		  rms_reg = iir_RETURN_RMS(&DETECTOR)*k_reg;  
		  if (REG(MODE_REG)==0x0201) rms_reg = sqrt(rms_reg*rms_reg-20*20); //�������� ��� ��� �������� 2-1000 ��	  //��� = 0.20 ��/�

		  REGW(RMS_VAL,rms_reg);						 	//������� ��������� ��������� � �������
		  //������ ���
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
		   if (reg_1>5) reg_1-=5; else reg_1=0;			//�������� ����������� ��� = 2 ���
		  
			
		  REGW(PIK_VAL,reg_1);						   	//������� ��������� ��������� � �������

		  //������ ���-�������
		  if (rms_reg>5) reg_1 = reg_1/rms_reg;
		  else			  reg_1 = 0;
		  REGW(PIK_FACTOR_VAL,reg_1*100);

		  //������ ���-���
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
		   if (reg_1>10) reg_1-=10;else reg_1=0;			//�������� ����������� ��� = 4 ���
			
		  REGW(PIK_PIK_VAL,reg_1);					  	//������� ��������� ��������� � �������
	  	  
	  	  
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

//������� ������� ������ ������ (�����/����).
float CAPACITY ()
{
	
						//���������� ���, �����.
						akbtemp = (float) adc_BAT_MEASURE_NEW_AMPER() / 4096 * 3.3;								
						akbtemp = (float) 1.65 - akbtemp;
						//akbtemp = (float) sqrt(akbtemp*akbtemp) / (float)(60 * 0.082);
						akbtemp = (float) akbtemp / (float)(60 * 0.082) * 1.03; //����. �������� ������. � ������������� �����. + �����������.
						//akbtemp = (float) akbtemp * 1.3;//��� ���� �����������, ��� ������������ �� ���������
		
						//���������� ����������, �����.
						akbemk_volt = (float) adc_BAT_MEASURE_NEW_VOLT() / 4096 * 3.3 * 2.11;		
				
						//����� �����, �� �����. �� �����., ���.
						akbtimer1 = (GLOBAL_TIMER_1ms - akbtimer2) / 1000;
						akbtimer2 = GLOBAL_TIMER_1ms;
						
						//���������� �������, �����/���.
						akbemk = akbtemp * akbtimer1 / 3600;		
						
		
						//������� �������, �����
						if (usb_charge_state == 1) akbemk_count += akbemk;
						
						//������� �������, ������
						if (usb_charge_state == 0) akbemk_count -= akbemk;
		
						//if (akbemk_count <= 0) akbemk_count = 0;

						//������ �������� ������� �� �������� ����
						if (REG(AKB_EMK_COUNT) == 0) akbemk_menu = 0.6;
						else akbemk_menu = 1.2;
						
						//�������� � ������ ����������
						if (akbemk_volt <= 2.6) akbemk_percent = 0;
							else akbemk_percent = (akbemk_count * 100) / (float) akbemk_menu;
		
		
						//��������� ������ � ������� BKP 
						//PWR->CR |=  PWR_CR_DBP;
		
						//���������� �������, ������������ ��� ������ � BKP ����� ����. �������������� (1,2 / 65535 = 0.00001831082)
						BKP_WriteBackupRegister(BKP_DR10, (int) ceil(akbemk_count / 0.00001831082)); 						
									

						//��������� ������ � BKP
						//PWR->CR &= ~PWR_CR_DBP;

						return akbemk_count;
}

///������� ���-�� ��������� �� sd-�����
unsigned int ROADS_COUNTING(void)
{
			unsigned int i = 0,j = 0;
			char 		  FileName[25];
			FIL Fil;
			
//			f_mount(&fls, "0:", 1);
//							
//			//for (j=0;j<=255;j++)
//			for (j=0; j<10; j++)
//			{
//				sprintf(FileName,"0:Road.%03d",j);						
//				if (f_stat(FileName, &fno) == FR_OK) i++;						
//			}

//			f_mount(0,"0:", 0);
//			
//			return i + 1;
	
//			if ((pRFile = fopen ("Roads.txt","r")) != NULL)
//			{
//				fseek(pRFile,9*(road_pos),SEEK_SET);
//				if (fscanf(pRFile, "%s", temp) != 1) 
//				{
//					road_pos--;
//					exist = 0;
//				}
//				else exist = 1;
//				fclose(pRFile);
//			}	
	
	
			f_mount(&fls, "0:", 1);
			res_t = f_open(&Fil,"Roads.txt", FA_OPEN_ALWAYS | FA_READ);
			if (res_t == 0) 
			{
				for (j=0; j<10; j++)
				{
					res_t = f_lseek(&Fil, 9*j);
					f_gets(FileName,9,&Fil);
					if (f_stat(FileName, &fno) == FR_OK) i++;	
				}
			}
			f_close(&Fil);			
			f_mount(0,"0:", 0);
	
	
			return i;
	
}


void flashing_bootloader(void)
{
	
		unsigned int i = 0, j = 0, res = 0, a, carry_flag, f_res;
		FATFS fatfs;
		FIL Fil;
		uint8_t bf[2];
		unsigned int br = 1;
		uint16_t crc = 0xffff;
		uint32_t bt[4];
		char t_str[20];
		uint32_t calculated_crc;
		
	
		vga_CLEAR();
		vga_SET_POS_TEXT(1,1);			
		vga_PRINT_STR("����������",&FONT_6x8);			
		vga_SET_POS_TEXT(1,15);
		vga_PRINT_STR("����������...",&FONT_6x8);
		vga_SET_POS_TEXT(1,30);
		vga_PRINT_STR("����������",&FONT_6x8);
		vga_SET_POS_TEXT(1,45);
		vga_PRINT_STR("�������� ����������.",&FONT_6x8);
		vga_UPDATE();							
					
		__disable_fiq();
		__disable_irq();
					
		while (1)
		{
				if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU)) JumpToApplication(0x8000000);
				if (pin_USB_5V == 1) break; 															
				IWDG_ReloadCounter();	
		}
					
		
		res = f_mount(&fatfs, "0:", 1);		
		res = f_open(&Fil, "boot1.bin", FA_OPEN_ALWAYS | FA_READ);
		
		while (1)
		{			
			res = f_read(&Fil, bf, sizeof(uint8_t), &br);
			if (br == 0) break;
				
			crc = crc^bf[0];
				
			for (j = 0; j < 8; j++)
			{
					a = crc;
					carry_flag = a & 0x0001;
					crc = crc >> 1;
					if (carry_flag == 1) crc = crc ^ 0xA001;
			}			

			IWDG_ReloadCounter();				
		}			
			

		if (crc != 0) 
		{
				vga_CLEAR();
				vga_SET_POS_TEXT(1,1);
				vga_PRINT_STR("���� ���������, ",&FONT_6x8);
				vga_SET_POS_TEXT(1,15);
				vga_PRINT_STR("������ CRC.",&FONT_6x8);
				vga_SET_POS_TEXT(1,30);
				vga_PRINT_STR("��������� ��������",&FONT_6x8);
				vga_SET_POS_TEXT(1,45);
				vga_PRINT_STR("����������.",&FONT_6x8);			
				vga_UPDATE();								
				
				while(pin_USB_5V) IWDG_ReloadCounter();	
				
				JumpToApplication(0x8000000);						
					
		}						
		else
		{			
			
			vga_CLEAR();
			vga_SET_POS_TEXT(1,1);			
			vga_PRINT_STR("����������",&FONT_6x8);			
			vga_SET_POS_TEXT(1,20);
			vga_PRINT_STR("����������...",&FONT_6x8);
			vga_UPDATE();							
			
			FLASH_Unlock();		

			
			for(i = 0; i < 32; i++)
			{			
				FLASH_ErasePage(0x8000000+i*0x800);
			}			
			
			
			res = f_open(&Fil, "boot1.bin", FA_OPEN_ALWAYS | FA_READ);
			
			if (res == 0)
			{
				i = 0;
				
				while (1)
				{						
						res = f_read( &Fil, bt, sizeof(unsigned int), &br );		
						
						if (br == 0) break;
					
						if  (f_res = FLASH_ProgramWord(0x8000000+i*4,bt[0]) == FLASH_COMPLETE)	i++;						
						else break;
						
						IWDG_ReloadCounter();	
				}
			}
			
			res = f_close(&Fil);				
						
			FLASH_Lock();				
			
			vga_CLEAR();
			vga_SET_POS_TEXT(1,1);
			vga_PRINT_STR("���������� ���������.",&FONT_6x8);
			vga_SET_POS_TEXT(1,20);
			vga_PRINT_STR("��������� ��������",&FONT_6x8);
			vga_SET_POS_TEXT(1,35);
			vga_PRINT_STR("����������.",&FONT_6x8);			
			vga_UPDATE();	
			
			while (pin_USB_5V) IWDG_ReloadCounter(); 
						
			res = f_unlink("boot1.bin");						
			res = f_mount(0, "0:", 0);		
			
			JumpToApplication(0x8000000);			

		}
		
		__enable_fiq();
		__enable_irq();	
			
		res = f_mount(0, "0:", 0);		
}





int main(void)

{
  unsigned int fl=0;
  volatile unsigned int temp_reg;  
	volatile unsigned int temp_reg2 = 0;
  unsigned int i = 0, j = 0;
  char 		  FileName[25];
	char temp[25];
	FIL Fil;
  FILE * pRFile;
	unsigned char persent_bat_char = 5;	
	uint16_t Number,Num;	
	unsigned char result;
	TStatus Err;
	volatile unsigned int tempreg=0;
	unsigned int res = 1;
	
	measure_stat = 0;		
		
  GPIO_SETUP();

  Delay(300000);
  	
  //������ 13�
  pin_13V(HIGTH);
  
	Delay(300000);
    
  SystemInit();
  
	SET_CLOCK_SPEED(CLK_72MHz); 
  
	//������������� ����������� ��� -----------------------------//
  adc_SETUP();
  
  USB_Init(); // USB Initialization                                        
  
	vga_INIT();
	
	///���� ������� ���������
  first_flag = 1; 
	
	///������������� �����
	Err = rtc_SETUP();
	if (Err == _ERR) GLOBAL_ERROR |= 0x01;
	
	///������������� sd �����
	result = FAT_Init();	
	if (result != 0) 
	{
			SD_SWITCH = 0;
		
			vga_CLEAR();					
			vga_SET_POS_TEXT(5,10);
			vga_PRINT_STR("���������� ������", &FONT_6x8);
			vga_SET_POS_TEXT(5,20);
			vga_PRINT_STR("������� ���������.",&FONT_6x8);
			vga_SET_POS_TEXT(5,40);
			vga_PRINT_STR("���������� �",&FONT_6x8);					
			vga_SET_POS_TEXT(5,50);
			vga_PRINT_STR("��������� �����.",&FONT_6x8);					
			vga_UPDATE();	
			Delay(50000000);
		
			GLOBAL_ERROR |= 0x02;
	}
	else
	{
			///�������� ������ ��������� ��������� � SD �����
			SD_SWITCH = 1;
	}
	
	///������������� ��������� ���������
	Err = reg_SETUP();
	if (Err == _ERR) GLOBAL_ERROR |= 0x04;
		
	///��������� �������� ����
	BKP->RTCCR = REG(TIME_EDIT);	
	
	///�������� ��������� sd-����� � ����. ���������� ��� ����. ����� �� usb 
	sdinfo = get_mmc();	
	

	
//  //����������� �����������----------------------------------------------------------//
//  if (rtc_SETUP()==_ERR) GLOBAL_ERROR|=0x01;	//���� �����
//  if (FAT_Init() ==_ERR) GLOBAL_ERROR|=0x04;;	//���� ���
//  if (reg_SETUP()==_ERR) GLOBAL_ERROR|=0x02;	//��������� ������������� ���������
//GLOBAL_ERROR = 0;
	
//	if (GLOBAL_ERROR>0) 	 //���� ���� ������
//	{	   
//						 temp_reg = 0;
//						 //������� ��������� �� ������ 
//						 vga_PRINT_STR("ERROR:",&FONT_6x8);
//						 
//						 if (GLOBAL_ERROR&0x01)   {vga_SET_POS_TEXT(5,++temp_reg*10);vga_PRINT_STR("����",&FONT_6x8);}
//						 if (GLOBAL_ERROR&0x02)   {vga_SET_POS_TEXT(5,++temp_reg*10);vga_PRINT_STR("������������� FLASH",&FONT_6x8);}
//						 if (GLOBAL_ERROR&0x04)   {vga_SET_POS_TEXT(5,++temp_reg*10);vga_PRINT_STR("������������� FAT",&FONT_6x8);}
//						 
//						 vga_UPDATE();						 
//						 
////						 Delay(700000); 
////						 ShowPowerOffForm();
////						 Delay(700000); 						 
////						 pin_OFF();
//	}

		
	
	MakeTIK();
	vga_UPDATE();		
	for(i=0;i<0x2FFFFF;i++){__NOP();}

  ext_adc_SETUP(20);//16 - 62.5 ���//20 - 50���  
	
	men_SETUP();  

	///������� ���-�� ��������� �� sd
	Num_of_Signals = ROADS_COUNTING();
	///��������� ����� ��� ������
	if (Num_of_Signals > 5) exist = 1;
	else exist = 0;
	
  SET_CLOCK_SPEED(CLK_8MHz); 

  //������ ����� ������������ ����, ���� ������ ������ "����","����"
  if ((key_CHECK_ST(key_UP))&&(key_CHECK_ST(key_DOWN))) 
   men_SET_CONFIG(0x80);	

  k_reg = ((float)REG(K_VIBRO)/1000);
  k_reg = k_reg/(30*10);

  //��������� �����������
  Moto_Sec = BKP_ReadBackupRegister(BKP_DR3);		
		
	///���������� ����� �������� ��� ����������� A � V	
	road_indicator = BKP_ReadBackupRegister(BKP_DR12); 
	
	///�������� ������������� ���
	id_akb = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);	
		
	///���������� ������� ���	
	akbemk_count = (float) BKP_ReadBackupRegister(BKP_DR10) * 0.00001831082;
	
	///������ �������� ������� �� �������� ����
	if (REG(AKB_EMK_COUNT) == 0) akbemk_menu = 0.6;
	else akbemk_menu = 1.2;
	
	///����������� �������� ��� ��������� ���������
	akbemk_percent = (akbemk_count * 100) / (float) akbemk_menu;	
	
	/// ��������� ��������� ��� ������ ���������
	if (id_akb == 0) frzbat1 = akbemk_percent; 			
	else frzbat1 = adc_BAT_PERCENT_edit(); 			
	
	///���� ������ �����., PA3 ����������� �� �����
	if ( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 1) 
	{		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	 
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
	}
	
	//tempreg = REG(K_VIBRO);
 
	
	while (1) //������ ��������� �����
  {		
		
		

   //------------- ��������� -------------------------------------------------//
		if (Flag_1S)
		{
				if (Moto_Sec==3600) ///���
				{
					 Moto_Sec = REG(MOTO_REG);
					 if (Moto_Sec<65000) REGW(MOTO_REG,++Moto_Sec);
					 Moto_Sec = 0;
				}
		else
				Moto_Sec++;
				Flag_1S = 0;
				BKP_WriteBackupRegister(BKP_DR3, Moto_Sec);
		}

	
	
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  //IWDG_SetReload(0xFF);
	IWDG_SetReload(0x400);
  IWDG_Enable();

		
	///������� ������� ���
	CAPACITY();	
		
	///������ ������� ��������������
	autooff = REG(AUTOPOWEROFF); 

	if (USB_SWITCH == 1)
	{
			
		 /// ����� ����������� ������ ��� ����������� USB
		 usb_addr = USB_DeviceAddress; 
		
		 //----------- USB ---------------------------------------------------------//
		 if (USB_CONNECT != pin_USB_5V)
		 {				
			 
			 USB_CONNECT = pin_USB_5V;	 

			 
			if (USB_CONNECT)  //���� ���� ������� ��������� USB
			{								
					
					STOP_MESUARE();				
					measure_stat = 0;
					rod_DEINIT();	
					Delay(200000);				   //��������������� ��������								
					
//					//������� ������� ))
//					if (REG(LOCK_REG) == 100) REGW(LOCK_REG,99);
//					else 
//					{
//						REGW(LOCK_REG,100);
//						__enable_irq();
//						__enable_fiq();
//						NVIC_SystemReset();
//					}			
				 
				 
				 SET_CLOCK_SPEED(CLK_72MHz);								
			
				 usb_transit = 0;	
								 
			}
			else 			   
				{
									
										
						usb_transit = 1; /// ���� ��� ���� ������ "����."
					
						//Alex	
						usb_stat = 0;				
						vga_CLEAR();
						vga_SET_POS_TEXT(1,1);
						vga_PRINT_STR("���������...",&FONT_6x8);
						vga_UPDATE();
						temp_reg = 0;
						temp_reg2 = 0;
							
					
						__disable_irq();
						__disable_fiq();
						
					
						f_mount(&fls, "0:", 1);
					
						for (j=0;j<=255;j++)
						{
							sprintf(FileName,"0:%03u.%03u/Signal %d.dat",0,0,j);						
							if (f_stat(FileName, &fno) == FR_OK) temp_reg++;						
						}
						
						
						IWDG_ReloadCounter();	
						
						f_open(&FileTmp,"Roads.txt", FA_CREATE_ALWAYS | FA_WRITE);
						
						sprintf(FileName,"Road.%01u  ",0);
						f_printf(&FileTmp,FileName);
						f_putc('\n',&FileTmp);
		
						
						for(i=1;i<255;i++)
						{
							sprintf(FileName,"Road.%03d",i);
							if (f_stat(FileName, &fno) == FR_OK)
							{
								f_printf(&FileTmp,FileName);
								f_putc('\n',&FileTmp);
								temp_reg2++;
							}						

							IWDG_ReloadCounter();							
						}
						
		
						f_close(&FileTmp);

						f_getlabel("", FileName, 0);
						if(strcmp(FileName, "PION"))
						{
							f_setlabel("PION");
						}
							
						f_mount(0,"0:", 0);
							
						__enable_irq();
						__enable_fiq();
							
						
						//Num_of_Signals = temp_reg;
						Num_of_Signals = ROADS_COUNTING();
						
						REGW(BEYOND_ROAD,temp_reg+1);
						i = 0;
						j = 0;
							
							
						//*Alex
				if ((pRFile = fopen ("M:\\prog1.bin","rb")) != NULL) 
				{
						JumpToApplication(0x8000000);
						road_pos = 0;
						road_cursor_pos = 0;
				}	

				
				f_mount(&fls, "0:", 1);		
				res = f_stat("boot1.bin", &fno);
				if (res == 0)
				{		
						road_pos = 0;
						road_cursor_pos = 0;												
						res = f_mount(0, "0:", 0);			
						flashing_bootloader();	
				}
				
				f_mount(0, "0:", 0);				
		
				SET_CLOCK_SPEED(CLK_8MHz); 
				Delay(200000);				   //��������������� ��������
							
		
				rod_INIT();
				
				men_SHOW_MAINFORMS(form_MESUARE); 			
					 

			}		
				
				///��������� ��������� ���				
			
				if (id_akb == 0 && measure_stat == 0) 
				{
//						frzbat1 = akbemk_percent; 
//						frzbat2 = akbemk_percent;
				}
				else
				{
						frzbat1 = adc_BAT_PERCENT_edit(); 
						frzbat2 = adc_BAT_PERCENT_edit();
				}
				
				USB_Connect(USB_CONNECT); 			
			
		 }


	}
 
	 
   //-------------------------------------------------------------------------//
   //USB ��������, �������� � ���������� ������
   //-------------------------------------------------------------------------//
	if (!USB_CONNECT)  
	{
		
						//�������� ������� ������� ��� 
						usb_charge_state = 1;
						
						usb_transit = 0; 
								
						///��������������
						if (REG(AUTOPOWEROFF) == 1) CONTROL_POWER(1);	
		
						LED_CHARGE_OFF();
						CHARGE_OFF();

						if (measure_stat == 0) CONTROL_BAT(0); ///�������� ��� �� ������ (��� ������ ���������)						
		
						//Alex
						if ((measure_stat == 0)&&key_CHECK_EV(key_EVENT_PRESSED_MESUARE)) measure_stat = 1; 
						///if ((measure_stat == 1)&&key_CHECK_EV(key_EVENT_RELEASED_MESUARE)) measure_stat = 2;
		
						if ((measure_stat == 1)&&key_CHECK_EV(key_EVENT_RELEASED_MESUARE)) 
						{
							measure_stat = 2;
							
							if (men_STATUS==men_MAIN)
										{											
												SET_CLOCK_SPEED(CLK_72MHz);
												START_MESUARE();																												
										}							
						}
						
						if ((measure_stat == 2)&&key_CHECK_EV(key_EVENT_PRESSED_MESUARE)) measure_stat = 3;
						if ((measure_stat == 3)&&key_CHECK_EV(key_EVENT_RELEASED_MESUARE)) measure_stat = 0;
						
						//����� ������� ������� ��. "���������"
						if (measure_stat == 0)
						{										
								STOP_MESUARE();

								if (ext_adc_GET_STATUS()==ext_adc_STATUS_STOP)
								{
										i++;
								}

								SET_CLOCK_SPEED(CLK_8MHz);							
						}				 
					 
						//������ ��� ��� �������
						if ((REG(PION_STATUS)&ST_MESUARE)>0)
						{
								if (REG(MODE_REG)==0x0800) //�������
								{
										REGW(SAMPLING_STATUS,ext_adc_GET_PROCESS());	 	//�������� �������
								}
								else MESUARE_PROCESS();								    //������� ����� ���������
										
						}

						//�������� ���������� ������
						if (ext_adc_OVER>0)
								REGW(PION_STATUS,REG(PION_STATUS)|ST_OVER);	  //���������� ��� ���������� � ��������
						else
								REGW(PION_STATUS,REG(PION_STATUS)&(~ST_OVER)); //�������� ��� ���������� � ��������
							
						ext_adc_OVER = 0;										 //���������� ������� ���������� ������
					
						//�������� ������
						men_MENU_CONTROL();
						
						
						//�������� �����
						UPDATE_MODE_REG();


	}	//����� - "�������� � ���������� ������"
		
	else  
	{		
	
			if (USB_Configuration == 0) 
				{ 			
								
					Delay(10000000);
					
					if (USB_Configuration == 0)  
					{
						men_SHOW_MAINFORMS(form_CHARGE); 
						
					}
					else 
					{
						men_SHOW_MAINFORMS(form_USB);
						usb_charge_state = 0; 
					}
				}			
			else men_SHOW_MAINFORMS(form_USB);			
		
				
		
	}		
	
 } //����� ��������� �����
	
	

}//����� ���������

 


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
