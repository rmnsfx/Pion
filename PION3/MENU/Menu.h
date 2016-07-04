#include "Pin.h"
#include "ff.h"
#include "diskio.h"
#include "stm32f10x_iwdg.h"

typedef enum {	
			  form_MESUARE,
			  form_USB,
				form_CHARGE
			 } Tmen_form;

typedef enum {
			  men_MAIN,
			  men_INPUTPAROL,
			  men_MULTI_ITEM,
			  men_ONE_ITEM,
			  men_EDIT_ITEM,
			  men_MESSAGE,
			  men_VIEWARH
			 }  Tmen_status;

void men_1ms(void);
void men_SETUP(void);
u8 men_MENU_CONTROL(void);

void men_SHOW_MAINFORMS(Tmen_form form);
void men_SET_CONFIG(unsigned char conf);
unsigned char men_SHOW_MESSAGE(char *mes1,char *mes2, signed int time_mes);
void men_SHOW_LARGE_BAT (unsigned char VAL);

extern void FR_print_error( FRESULT fresult );
			 
extern unsigned int road_pos;
extern unsigned int road_pos_int;
extern unsigned int road_indicator;