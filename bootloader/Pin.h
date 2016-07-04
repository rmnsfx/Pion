#include "stm32f10x.h"


#define HIGTH 1
#define LOW	  0

#define ON	  0
#define OFF	  1

#define CLK_72MHz	1
#define CLK_8MHz 	0

#define ST_MESUARE		0x0001	  //режим измерения
#define ST_CHARGE		0x0002	  //режим заряда
#define ST_USB_CONNECT	0x0004	  //подключен усб
#define ST_OVER			0x0008		

#define ST_ERR1			0x0100	  //ошибка часов реального времени
#define ST_ERR2			0x0200	  //ошибка FLASH

//FAST Выходы/входы
#define pin_ADC_CLK_SET	GPIOB->BSRR = GPIO_Pin_13
#define pin_ADC_CLK_RES	GPIOB->BRR =  GPIO_Pin_13

#define pin_ADC_CS_SET  GPIOB->BSRR = GPIO_Pin_12
#define pin_ADC_CS_RES  GPIOB->BRR  = GPIO_Pin_12

#define pin_ADC_DATA    ((GPIOB->IDR&0x4000)>0);

//ДИСКРЕТНЫЕ ВЫХОДЫ

#define pin_SPI_CS(a) 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)a)
#define pin_SPI_CLK(a) 	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)a)
#define pin_SPI_MOSI(a) GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)a)
#define pin_ADC_CS(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)a)
#define pin_ADC_CLK(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)a)

#define pin_3VA(a)	    GPIO_WriteBit(GPIOD,GPIO_Pin_2,(BitAction)a)
//#define pin_KEY(a)	    GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)a)
#define pin_DEBUG(a)	GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)a)		

//ssd1303
#define pin_SSD_CS(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)a)
#define pin_SSD_RES(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)a)
#define pin_SSD_DC(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)a)
#define pin_SSD_WR(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)a)
#define pin_SSD_RD(a)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)a)
#define pin_SSS_DATA(a)	GPIO_Write(GPIOC,(GPIO_ReadOutputData(GPIOC) & 0xFF00)|a)

#define pin_13V(a)		GPIO_WriteBit(GPIOC,GPIO_Pin_10,(BitAction)a)
#define pin_DISC(a)		GPIO_WriteBit(GPIOC,GPIO_Pin_11,(BitAction)a)	
#define pin_OFF()		GPIO_WriteBit(GPIOC,GPIO_Pin_12,(BitAction)0)
#define POWER_OFF() GPIO_ResetBits(GPIOC,GPIO_Pin_9)

#define DISPLAY_OFF() GPIO_ResetBits(GPIOC,GPIO_Pin_10)

//ДИСКРЕТНЫЕ ВХОДЫ

#define pin_USB_5V		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
//#define pin_CHARGE		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define pin_USB_DP()		GPIO_ResetBits(GPIOA,GPIO_Pin_12)
//#define pin_USB_DP()		GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)0)

#define pin_CHARGE		GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)0)
#define pin_CHARGE_OFF		GPIO_ResetBits(GPIOA,GPIO_Pin_3)

#define pin_SPI_MISO	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
//#define pin_ADC_DATA	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

#define pin_KEY1		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define pin_KEY2		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define pin_KEY3		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define pin_KEY4		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define pin_KEY5		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)


