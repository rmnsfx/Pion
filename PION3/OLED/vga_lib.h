#include "font.h"

#define vga_COUNT_X_PIXELS	128
#define vga_COUNT_Y_PIXELS	64


typedef enum
 {
  drMODE_NORMAL,   //���������� ����� �����������
  drMODE_XOR	   //���������� �������� XOR
 } dr_mode_t;

typedef enum
 {
  drRECT_NO_FILL,
  drRECT_FILL,
  drRECT_ARC_NO_FILL,
  drRECT_ARC_FILL
 } dr_style_t;


//----------------�������� �������---------------------------------------------------------------------------------------//

void vga_INIT(void);
void vga_UPDATE(void);
void vga_CLEAR(void);

#define vga_GET_HEIGHT_DISPLAY		(unsigned char)vga_COUNT_Y_PIXELS
#define vga_GET_WIDTH_DISPLAY		(unsigned char)vga_COUNT_X_PIXELS


//----------------�������������� �������---------------------------------------------------------------------------------//

void vga_DRAW_DATA8(unsigned char x,unsigned char y,unsigned char data8);

//������� ��������� �������
void vga_DRAW_PIXEL(unsigned char x,unsigned char y);

//������ ����� ���������
void vga_SET_DRAW_MODE(dr_mode_t mode);

//��������� ������������� ����� ���������
dr_mode_t vga_GET_DRAW_MODE(void);

//----------------������� ��������� ����������� ��������-----------------------------------------------------------------//

//������� ��������� ������, �������� ���������� ������ � �����
void vga_LINE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

//������� ��������� ��������������, ���������� ���� �����-������� ����� ���� � ����� ������ � �������
void vga_RECTANGLE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, const dr_style_t style);

//������� ��������� ���������� � ������� xc,yc � �������� r
void vga_CIRCLE(unsigned char xc,unsigned char yc,unsigned char r);

//----------------������� ������ ������----------------------------------------------------------------------------------//

//���������� ������
void vga_SET_POS_TEXT(unsigned char x, unsigned char y);
//������� ���� ������
void vga_PRINT_CHAR(const char symbol, const font_t *font);
//������� �����, ���� len = 0, �� ����� ��������� ���� �� ��������� ������=0, ����� ��������� ����� �������� = len
void vga_PRINT_TEXT(const char *str, unsigned char len, const font_t *font);
//��������� ������ ����� ��������, ������ len
void vga_FILL_STR(const char fill_char, unsigned char len, const font_t *font);

void vga_SHOW_LOGO(void);
//������� ������
#define vga_PRINT_STR(s,f) 	vga_PRINT_TEXT(s,0,f)











