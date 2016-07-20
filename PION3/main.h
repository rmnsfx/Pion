
#include "stdio.h"

#ifndef _main_h
#define _main_h





	 typedef enum 
	  {
	   _OK 		= 1,
	   _ERR		= 0
	  } TStatus;

	 /*typedef unsigned char 		TBIT;

	 typedef unsigned char		_U8;
	 typedef unsigned short		_U16;
	 typedef unsigned long		_U32;
	 typedef unsigned long long _U64;

	 typedef signed char		_S8;
	 typedef signed short 		_S16;
	 typedef signed long		_S32;
	 typedef signed long long	_S64;

	 typedef float				_F32;	*/
void Delay(long i);
extern void 	SET_CLOCK_SPEED(unsigned char CLOCK); 
extern TStatus  FORMAT(void);
extern float  k_reg;
		
void CONTROL_RTC(void);		 
			 
extern int USB_CONNECT;

extern int first_flag; /// флаг первого включения

extern int POWER_OFF;
extern int GLOBAL_TIMER_1ms;

extern int timer1;
extern int timer2;
extern int timer3;

extern int frzbat1;
extern int frzbat2;

extern int usb_addr;

extern int id_akb;

float CAPACITY ();

extern float akbemk_count;
extern float akbtemp;
extern float akbtimer1;
extern float akbemk_volt;
extern float akbemk_percent;

extern unsigned int usb_charge_state;


//Выключатель usb для отладки 
#define USB_SWITCH 1



//extern float  k_reg_mul;
#define VER 339

#endif
