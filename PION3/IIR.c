#include "IIR.h"
#include "math.h"

f_FILTER IIRF[MAX_COUNT_IIR_FLO_FILTER];//массив фильтров с плавающей точкой
d_FILTER IIRD[MAX_COUNT_IIR_DEC_FILTER];//массив фильтров 



f_FILTER *iir_CreateFilterFlo(float *A, float *B, unsigned char N)
{
 static unsigned char num_fil = 0;
 f_FILTER *F;
// signed long long SUM_COEFF;
 float 	SUM_COEFF;

 if (num_fil==MAX_COUNT_IIR_FLO_FILTER) return 0;
 if ((N>MAX_COUNT_IIR_FLO_ORDER)||(!N))		return 0;

 F = &IIRF[num_fil++];

 F->N = N;
 //F->Temp = 0;

 SUM_COEFF = 0;
 do 
  {
   F->X[N] = 0;
   F->Y[N] = 0;
   F->A[N] = A[N];//lrint(A[N]*65536*32);
   F->B[N] = B[N];//lrint(B[N]*65536*32);
   if (N!=0) SUM_COEFF+= F->A[N];
   SUM_COEFF+= F->B[N];
  }
 while (N-- > 0);

 //if (SUM_COEFF>0x10000) return 0;

  //F->A[1] = F->A[1]+1; 
 return F;
}									  

									  
float iir_FLO_FILTER_1ORD(f_FILTER *F, float newval)
{
  float SUM = 0;

  F->X[1]=F->X[0];
  F->Y[1]=F->Y[0];

  F->X[0] = newval;

  SUM = (F->X[0]*F->B[0]+F->X[1]*F->B[1])+(F->Y[1]*F->A[1]);

  F->Y[0] = (SUM);

  return F->Y[0];

}

float iir_FLO_FILTER_2ORD(f_FILTER *F, float newval)
{
  float SUM = 0;

  F->X[2]=F->X[1];
  F->Y[2]=F->Y[1];
  F->X[1]=F->X[0];
  F->Y[1]=F->Y[0];

  F->X[0] = newval;

  SUM = (F->X[0]*F->B[0]+F->X[1]*F->B[1]+F->X[2]*F->B[2])+(F->Y[1]*F->A[1]+F->Y[2]*F->A[2]);

  F->Y[0] = (SUM);

  return F->Y[0];

}


d_FILTER *iir_CreateFilterDec(double *A, double  *B, unsigned char N)
{
 static unsigned char num_fil = 0;
 d_FILTER *F;
 signed long long SUM_COEFF;
 
 if (num_fil==MAX_COUNT_IIR_DEC_FILTER) return 0;
 if ((N>MAX_COUNT_IIR_DEC_ORDER)||(!N))		return 0;

 F = &IIRD[num_fil++];

 F->N = N;
 //F->Temp = 0;

 SUM_COEFF = 0;
 do 
  {
   F->X[N] = 0;
   F->Y[N] = 0;
   if ((A[N]<2)&&(A[N]>-2))
   							F->A[N] =lrint(A[N]*65536*16384);	  //lrint
   if ((B[N]<2)&&(B[N]>-2))
   							F->B[N] =lrint(B[N]*65536*16384);
   if (N!=0) SUM_COEFF+= F->A[N];
   SUM_COEFF+= F->B[N];
  }
 while (N-- > 0);

 //if (SUM_COEFF>0x10000) return 0;

  //F->A[1] = F->A[1]+1; 
 return F;
}

void iir_DEC_FILTER_RESET(d_FILTER *F)								 //сбросить значения фильтра
{
 unsigned char i;
 for (i=0;i<(MAX_COUNT_IIR_DEC_ORDER+1);i++)
  {
   F->X[i] = 0;
   F->Y[i] = 0;
  }
}

void iir_DEC_FILTER_CHANGE(double *A, double *B, d_FILTER *F)		 //задать новые коэффициенты фильтру
{
 unsigned char i;
 for (i=0;i<(F->N+1);i++)
  {
   F->A[i] =lrint(A[i]*65536*16384);	  //lrint
   F->B[i] =lrint(B[i]*65536*16384);
   F->X[i] = 0;
   F->Y[i] = 0;
  }
}


signed int iir_DEC_INTEGRATOR_2ORD(d_FILTER *F, signed int newval)
{
  signed long long SUM = 0;

  F->X[2]=F->X[1];
  F->Y[2]=F->Y[1];
  F->X[1]=F->X[0];
  F->Y[1]=F->Y[0];

  F->X[0] = newval;

  SUM = ((signed long long)F->X[0]*F->B[0]+(signed long long)F->X[1]*F->B[1]+(signed long long)F->X[2]*F->B[2])+((signed long long)F->Y[1]*F->A[1]+(signed long long)F->Y[2]*F->A[2]);

  F->Y[0] = ((SUM+536870912)>>30);
  //F->Y[0] = ((SUM)>>30);

  
  /*if (F->Y[0]>32767*32768)  return 32767;
  else
  if (F->Y[0]<-32768*32768) return -32768; */ 

  return F->Y[0];

}



signed long long iir_DEC_FILTER_1ORD(d_FILTER *F, signed long long newval)
{
  signed long long SUM = 0;

  F->X[1]=F->X[0];
  F->Y[1]=F->Y[0];

  //F->X[0] = ((signed int)newval)<<15;
  F->X[0] = newval;

  SUM = ((signed long long)F->X[0]*F->B[0]+(signed long long)F->X[1]*F->B[1])+((signed long long)F->Y[1]*F->A[1]);

  F->Y[0] = ((SUM+536870912)>>30);

  /*if (F->Y[0]>32767*32768)  return 32767;
  else
  if (F->Y[0]<-32768*32768) return -32768;  

  return (F->Y[0]+16384)>>15;  */

  if (F->Y[0]>32767*32768)  return 32767*32768;
  else
  if (F->Y[0]<-32768*32768) return -32768*32768;  

  return F->Y[0];

 

}

signed long long iir_DEC_FILTER_2ORD(d_FILTER *F, signed long long newval)
{
  signed long long SUM = 0;

  F->X[2]=F->X[1];
  F->Y[2]=F->Y[1];
  F->X[1]=F->X[0];
  F->Y[1]=F->Y[0];

  //F->X[0] = ((signed int)newval)<<15;
  F->X[0] = newval;

  SUM = ((signed long long)F->X[0]*F->B[0]+(signed long long)F->X[1]*F->B[1]+(signed long long)F->X[2]*F->B[2])+((signed long long)F->Y[1]*F->A[1]+(signed long long)F->Y[2]*F->A[2]);

  F->Y[0] = ((SUM+536870912)>>30);

  /*if (F->Y[0]>32767*32768)  return 32767;
  else
  if (F->Y[0]<-32768*32768) return -32768;  

  return (F->Y[0]+16384)>>15;  */

  if (F->Y[0]>32767*32768)  return 32767*32768;
  else
  if (F->Y[0]<-32768*32768) return -32768*32768;  

  return F->Y[0];

 

}

signed long long iir_DEC_FILTER_4ORD(d_FILTER *F1, d_FILTER *F2, signed long long newval)
{
  signed long long SUM = 0;
  //первая секция фильтра
  F1->X[2]=F1->X[1];
  F1->Y[2]=F1->Y[1];
  F1->X[1]=F1->X[0];
  F1->Y[1]=F1->Y[0];

  F1->X[0] = newval;
  //F1->X[0] = ((signed int)newval)<<15;

  SUM = ((signed long long)F1->X[0]*F1->B[0]+(signed long long)F1->X[1]*F1->B[1]+(signed long long)F1->X[2]*F1->B[2])+((signed long long)F1->Y[1]*F1->A[1]+(signed long long)F1->Y[2]*F1->A[2]);

  F1->Y[0] = ((SUM+536870912)>>30);
  //вторая секция фильтра
  SUM = 0;

  F2->X[2]=F2->X[1];
  F2->Y[2]=F2->Y[1];
  F2->X[1]=F2->X[0];
  F2->Y[1]=F2->Y[0];

  F2->X[0]=F1->Y[0];

  
  SUM = ((signed long long)F2->X[0]*F2->B[0]+(signed long long)F2->X[1]*F2->B[1]+(signed long long)F2->X[2]*F2->B[2])+((signed long long)F2->Y[1]*F2->A[1]+(signed long long)F2->Y[2]*F2->A[2]);

  F2->Y[0] = ((SUM+536870912)>>30);  


  /*if (F2->Y[0]>32767*32768)  return 32767;
  else
  if (F2->Y[0]<-32768*32768) return -32768;  

  return (F2->Y[0]+16384)>>15;		 */

  if (F2->Y[0]>32767*32768)  return 32767*32768;
  else
  if (F2->Y[0]<-32768*32768) return -32768*32768;  

  return F2->Y[0];

}
//---функции детектора--------------------------------------------------------------------------------//

void iir_DETECTOR_RESET(d_DETECTOR *F,unsigned short period)
{
 F->period = period; //первое измерение с двойным периодом
 F->SUM_RMS = 0;
 F->max = 0;
 F->min = 0;
 F->RES	= 0;
 F->i = 0;
 F->over = 0;
 F->MAX_RES = 0;
 F->MIN_RES = 0;
}

void iir_DETECTOR(d_DETECTOR *F, signed int newval)
{
 if (newval>F->max) F->max = newval;
 if (newval<F->min) F->min = newval;
 //для скз
 newval = (newval+16384)>>15; 

 F->SUM_RMS+=((signed long long)newval*newval);//>>15;

 //if ((newval==32767)||(newval==-32768))  F->over = 1;
 
 F->i++;

 if (F->i >= F->period)
  {
   F->RES = F->SUM_RMS;//>>15; //запоминаме результат
   F->MAX_RES = F->max;
   F->MIN_RES = F->min;
   F->max = 0;
   F->min = 0;
   F->SUM_RMS = 0;
   F->i = 0;
   F->over = 0;
  }


 //return F->RES;
}
/*
void iir_DETECTOR(d_DETECTOR *F, signed short newval)
{
 F->SUM_RMS+=(signed int)newval*newval;

 //if ((newval==32767)||(newval==-32768))  F->over = 1;
 
 F->i++;

 if (F->i >= F->period)
  {
   F->RES = F->SUM_RMS;
   F->SUM_RMS = 0;
   F->i = 0;
   F->over = 0;
  }


 //return F->RES;
} */

float iir_RETURN_RMS(d_DETECTOR *F)
{
 return sqrt((double)F->RES/F->period);
}

float iir_RETURN_MAX_PIK(d_DETECTOR *F)
{
 return ((float)F->MAX_RES)/32768;
}

float iir_RETURN_MIN_PIK(d_DETECTOR *F)
{
 return ((float)F->MIN_RES)/32768;
}

float iir_RETURN_PIK_PIK(d_DETECTOR *F)
{
 return iir_RETURN_MAX_PIK(F)-iir_RETURN_MIN_PIK(F);
}


unsigned char iir_RETURN_OVER(d_DETECTOR *F)//перегрузка входа
{
 return (((F)->over)&0x01); 
}








