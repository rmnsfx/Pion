//---------------�������������� ������� ��� ����������� ssd1303 OLED ������� 128�64--------------------------------//
#include "stm32f10x.h"
#include "Pin.h"


void ssd_OLED_INIT(void);						  	 //������������� OLED �������
void ssd_RESET(void);							  	 //����� OLED �������
void ssd_CLS(unsigned char fon);				  	 //��������� ���������� fon
void ssd_SET_POS(unsigned int x, unsigned int y); 	 //���������� ������� �������, X-������� � ��������, Y-������ � ������
void ssd_SEND(unsigned char cmd, unsigned char type);//��������� 1 ���� ������ ��� �������


//�������
#define SSD_CMD					0
#define SSD_DATA				1

#define SD_SET_LINE             0xb0
#define SD_SET_COL_LO           0x00
#define SD_SET_COL_HI           0x10
#define SD_SET_OFFSET           0xd3


#define ssd_CLEAR_OLED()		ssd_CLS(0)
#define ssd_SET_OLED()			ssd_CLS(0xFF)
#define ssd_SEND_DATA8(data8)	ssd_SEND(data8, SSD_DATA);


//��� ������� ������
void ssd_PRINT(unsigned char *text, unsigned char x, unsigned char y);
void ssd_TEST(void);

//---------------�������������� ������� ��� ����������� ssd1303 OLED �������--------------------------------------//

