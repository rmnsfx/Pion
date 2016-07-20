#include "Keyboard.h"
#include "Pin.h"


volatile unsigned char key_STATE	  	  = key_NO;
volatile unsigned char key_OLD	  	  	  = key_NO;
volatile unsigned long key_EVENT		  = key_NO;
		 unsigned char key_bounce = 0;		//дребезг таймер

#define _key_UP			(pin_KEY4==0)
#define _key_DOWN		(pin_KEY5==0)
#define _key_MESUARE	(pin_KEY1==0)
#define _key_ESC_MENU	(pin_KEY3==0)
#define _key_ENTER		(pin_KEY2==0)

#define CHECK_RIS_EDGE(MASK)	 ((key_OLD&MASK)!=MASK) && ((key_STATE&MASK)==MASK)
#define CHECK_FAL_EDGE(MASK)	 ((key_OLD&MASK)==MASK) && ((key_STATE&MASK)!=MASK)


void key_1ms(void)
{
	
	
 if (_key_UP) 		key_STATE|= key_UP; 
 else 				key_STATE&= ~key_UP;

 if (_key_DOWN)		key_STATE|= key_DOWN;
 else				key_STATE&= ~key_DOWN;

 if (_key_MESUARE)	key_STATE|= key_MESUARE;
 else				key_STATE&= ~key_MESUARE;

 if (_key_ESC_MENU) key_STATE|= key_ESC_MENU; 
 else 				key_STATE&= ~key_ESC_MENU;

 if (_key_ENTER) 	key_STATE|= key_ENTER; 
 else 				key_STATE&= ~key_ENTER;

 if (key_STATE!=key_OLD)	//произошло событие
   if (key_bounce==0)  
   					{
					 if (CHECK_RIS_EDGE(key_UP))  		key_EVENT|= key_EVENT_PRESSED_UP;
					 if (CHECK_RIS_EDGE(key_DOWN))		key_EVENT|= key_EVENT_PRESSED_DOWN;
					 if (CHECK_RIS_EDGE(key_MESUARE))	key_EVENT|= key_EVENT_PRESSED_MESUARE;
					 if (CHECK_RIS_EDGE(key_ESC_MENU))	key_EVENT|= key_EVENT_PRESSED_ESC_MENU;
					 if (CHECK_RIS_EDGE(key_ENTER))		key_EVENT|= key_EVENT_PRESSED_ENTER;

					 if (CHECK_FAL_EDGE(key_UP))		key_EVENT|= key_EVENT_RELEASED_UP;
					 if (CHECK_FAL_EDGE(key_DOWN))		key_EVENT|=	key_EVENT_RELEASED_DOWN;
					 if (CHECK_FAL_EDGE(key_MESUARE))	key_EVENT|=	key_EVENT_RELEASED_MESUARE;
					 if (CHECK_FAL_EDGE(key_ESC_MENU))	key_EVENT|= key_EVENT_RELEASED_ESC_MENU;
					 if (CHECK_FAL_EDGE(key_ENTER))		key_EVENT|= key_EVENT_RELEASED_ENTER;

					 key_OLD 	= key_STATE;
					 key_bounce = key_TIME_BOUNCE;
					}

 if (key_bounce>0) key_bounce--;
					
}

TKEY_EVENT key_CHECK_EVENT(unsigned long mask_event)
{
 if (key_EVENT&mask_event) 
  {
   key_EVENT&=~mask_event;
   return KEY_EVENT;
  }
 else	
   return KEY_NO_EVENT;
}

void key_RES_EVENT(unsigned long mask_event)
{
 key_EVENT&=~mask_event;
}

TKEY_EVENT key_CHECK_STATUS(unsigned char mask_state)
{
 if ((key_STATE&mask_state)>0) return KEY_EVENT;
 else						   return KEY_NO_EVENT;
}

