
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

extern void 	SET_CLOCK_SPEED(unsigned char CLOCK); 
extern TStatus  FORMAT(void);
extern float  k_reg;
//extern float  k_reg_mul;
#define VER 270

#endif
