#include "font.h"

#define vga_COUNT_X_PIXELS	128
#define vga_COUNT_Y_PIXELS	64


typedef enum
 {
  drMODE_NORMAL,   //нормальный режим отображения
  drMODE_XOR	   //логическая операция XOR
 } dr_mode_t;

typedef enum
 {
  drRECT_NO_FILL,
  drRECT_FILL,
  drRECT_ARC_NO_FILL,
  drRECT_ARC_FILL
 } dr_style_t;


//----------------основные функции---------------------------------------------------------------------------------------//

void vga_INIT(void);
void vga_UPDATE(void);
void vga_CLEAR(void);

#define vga_GET_HEIGHT_DISPLAY		(unsigned char)vga_COUNT_Y_PIXELS
#define vga_GET_WIDTH_DISPLAY		(unsigned char)vga_COUNT_X_PIXELS


//----------------низкоуровневые функции---------------------------------------------------------------------------------//

void vga_DRAW_DATA8(unsigned char x,unsigned char y,unsigned char data8);

//функция рисования пиксела
void vga_DRAW_PIXEL(unsigned char x,unsigned char y);

//задать режим рисования
void vga_SET_DRAW_MODE(dr_mode_t mode);

//запросить установленный режим рисования
dr_mode_t vga_GET_DRAW_MODE(void);

//----------------функции рисования графических объектов-----------------------------------------------------------------//

//Функция рисования прямой, задаются координаты начала и конца
void vga_LINE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

//Функция рисования прямоугольника, координаты двух точек-верхний левый угол и нижни правый и заливка
void vga_RECTANGLE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, const dr_style_t style);

//Функция рисования окружности с центром xc,yc и радиусов r
void vga_CIRCLE(unsigned char xc,unsigned char yc,unsigned char r);

//----------------функции вывода текста----------------------------------------------------------------------------------//

//установить курсор
void vga_SET_POS_TEXT(unsigned char x, unsigned char y);
//вывести один символ
void vga_PRINT_CHAR(const char symbol, const font_t *font);
//вывести текст, если len = 0, то текст выводится пока не встретися символ=0, иначе выводится число символов = len
void vga_PRINT_TEXT(const char *str, unsigned char len, const font_t *font);
//заполнить строку одним символом, длиной len
void vga_FILL_STR(const char fill_char, unsigned char len, const font_t *font);

//вывести строку
#define vga_PRINT_STR(s,f) 	vga_PRINT_TEXT(s,0,f)











