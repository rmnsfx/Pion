#include "Pin.h"
typedef enum {	
			  form_MESUARE,
			  form_USB
			 } Tmen_form;

void men_1ms(void);
void men_SETUP(void);
u8 men_MENU_CONTROL(void);

void men_SHOW_MAINFORMS(Tmen_form form);
void men_SET_CONFIG(unsigned char conf);
unsigned char men_SHOW_MESSAGE(char *mes1,char *mes2, signed int time_mes);


