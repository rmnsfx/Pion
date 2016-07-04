//---------------низкоуровневый драйвер для контроллера ssd1303 OLED дисплея 128х64--------------------------------//
#include "stm32f10x.h"
#include "Pin.h"


void ssd_OLED_INIT(void);						  	 //инициализация OLED дисплея
void ssd_RESET(void);							  	 //сброс OLED дисплея
void ssd_CLS(unsigned char fon);				  	 //заполнить значениями fon
void ssd_SET_POS(unsigned int x, unsigned int y); 	 //установить текущюю позицию, X-столбец в пикселах, Y-строка в байтах
void ssd_SEND(unsigned char cmd, unsigned char type);//отправить 1 байт данных или команду


//макросы
#define SSD_CMD					0
#define SSD_DATA				1

#define SD_SET_LINE             0xb0
#define SD_SET_COL_LO           0x00
#define SD_SET_COL_HI           0x10
#define SD_SET_OFFSET           0xd3


#define ssd_CLEAR_OLED()		ssd_CLS(0)
#define ssd_SET_OLED()			ssd_CLS(0xFF)
#define ssd_SEND_DATA8(data8)	ssd_SEND(data8, SSD_DATA);


//эти функции убрать
void ssd_PRINT(unsigned char *text, unsigned char x, unsigned char y);
void ssd_TEST(void);

//---------------низкоуровневый драйвер для контроллера ssd1303 OLED дисплея--------------------------------------//

