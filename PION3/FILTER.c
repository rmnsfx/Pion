//#include "Pin.h"

//коэффициенты интегрирующего фильтра при частоте 800√ц
//#define  GAIN_IN 0.0039332726069243422
//#define  B0 	 1
//#define  B1 	 1
//#define  A0		 1
//#define  A1 	-0.99213345478615134

//коэффициенты интегрирующего фильтра при частоте 400√ц
#define  lp_GAIN_IN 0.0082178362763896638
#define  lp_B0 	 	1
#define  lp_B1 	 	1
#define  lp_A0		1
#define  lp_A1 	   -0.98356432744722067

//коэффициенты фильтра высоких частот
/*#define  hp_GAIN_IN 0.96219524582910354
#define  hp_B0 	 	1
#define  hp_B1 	   -1
#define  hp_A0		1
#define  hp_A1 	   -0.92439049165820708*/

//коэффициенты фильтра высоких частот
#define  hp_GAIN_IN 0.42080777983773182//0.0082178362763896638
#define  hp_B0 	 	1
#define  hp_B1 	    1
#define  hp_A0		1
#define  hp_A1 	  -0.15838444032453636//-0.98356432744722067


//коэффициенты фильтра высоких частот
//#define  hpGAIN_IN 0.96219524582910354
#define  hpB0 	 	68936//269
#define  hpB1 	    68936//269
#define  hpA0		8388608//32768
#define  hpA1 	   -8250735//-32229



//float temp_val;
float lp_X0,lp_X1,lp_Y0,lp_Y1;
float hp_X0,hp_X1,hp_Y0,hp_Y1;

signed long hpX0,hpX1,hpY0,hpY1;


void iir_lp_INIT_FILTER(void)
{
 lp_X0 = 0;
 lp_Y0 = 0;
 lp_X1 = 0;
 lp_Y1 = 0;
}

void iir_hp_INIT_FILTER(void)
{
 hp_X0 = 0;
 hp_Y0 = 0;
 hp_X1 = 0;
 hp_Y1 = 0;
}


void iir_hp_INIT_FILTER_INT(void)
{
 hpX0 = 0;
 hpY0 = 0;
 hpX1 = 0;
 hpY1 = 0;
}
/*
signed int iir_FILTER(signed int IN_VAL)
{
 temp_val = (float)IN_VAL*GAIN_IN;

 X1 = X0;
 Y1 = Y0;
 X0 = temp_val;
 //Y0 = 0;

 //temp_val = B0*X0+B1*X1-A0*Y0-A1*Y1;
 Y0 = B0*X0+B1*X1-(A1*Y1);

 
 return Y0;

}*/

float iir_lp_FILTER(float IN_VAL)
{
 //temp_val = IN_VAL*GAIN_IN;

 lp_X1 = lp_X0;
 lp_Y1 = lp_Y0;
 lp_X0 = IN_VAL*lp_GAIN_IN;

 //Y0 = 0;
 //temp_val = B0*X0+B1*X1-A0*Y0-A1*Y1;
 lp_Y0 = lp_B0*lp_X0+lp_B1*lp_X1-(lp_A1*lp_Y1);

 return lp_Y0;

}

float iir_hp_FILTER(float IN_VAL)
{
 //float temp_val;// = IN_VAL*GAIN_IN;

 hp_X1 = hp_X0;
 hp_Y1 = hp_Y0;
 hp_X0 = IN_VAL*hp_GAIN_IN;

 //Y0 = 0;
 //temp_val = B0*X0+B1*X1-A0*Y0-A1*Y1;
 hp_Y0 = hp_B0*hp_X0+hp_B1*hp_X1-(hp_A1*hp_Y1);

 return hp_Y0;

}


int iir_hp_FILTER_INT(int IN_VAL)
{
 //signed long temp;
 hpX1 = hpX0;
 hpY1 = hpY0;
 hpX0 = IN_VAL<<8;
 hpY0 = hpX0*hpB0+hpX1*hpB1-hpY1*hpA1;
 
 hpY0 = hpY0>>15; 

 return hpY0>>8;
}


int iir_hp_FILTER_INT64(int IN_VAL)
{
 static __int64 X0,X1,Y0,Y1;
 //signed long temp;
 X1 = X0;
 Y1 = Y0;
 X0 = (unsigned int)IN_VAL<<16;
 Y0 = X0*hpB0+X1*hpB1-Y1*hpA1;
 
 Y0 = Y0>>23;//15; 

 return Y0>>16;
}






