#ifndef _IIR
#define _IIR


#define MAX_COUNT_IIR_FLO_FILTER 1
#define MAX_COUNT_IIR_DEC_FILTER 6	//макс количество фильтров
#define MAX_COUNT_IIR_FLO_ORDER	 2
#define MAX_COUNT_IIR_DEC_ORDER	 2	//максимальный порядок фильтров для хранения коэффициентов



typedef struct
 {
  unsigned char  N;
  float  A[MAX_COUNT_IIR_FLO_ORDER+1];
  float  B[MAX_COUNT_IIR_FLO_ORDER+1];
  float  X[MAX_COUNT_IIR_FLO_ORDER+1];
  float  Y[MAX_COUNT_IIR_FLO_ORDER+1];
 } f_FILTER;

typedef struct
 {
  unsigned char  N;
  signed int  A[MAX_COUNT_IIR_DEC_ORDER+1];
  signed int  B[MAX_COUNT_IIR_DEC_ORDER+1];
  signed int  X[MAX_COUNT_IIR_DEC_ORDER+1];
  signed int  Y[MAX_COUNT_IIR_DEC_ORDER+1];
 } d_FILTER;

typedef struct
 {
  //переменные текущие
  unsigned long long SUM_RMS; //интегратор квадратов
  unsigned short 	 i;		  //служебная переменная
  unsigned short 	 period;  //период
  signed int 		 max;     //текущее максимальное значение амплитуды
  signed int	     min;	  //текущее минимальное значение амплитуды
  //переменные содержащие результат
  unsigned long long RES;	  //сумма квадратов за период
  signed int		 MAX_RES; //максимальное значение амплитуды за период
  signed int		 MIN_RES; //минимальное значение амплитуды за период
  unsigned char 	 over;	  //детектор границ диапазона
 } d_DETECTOR;

typedef struct
 {
  double B[MAX_COUNT_IIR_DEC_ORDER+1];   //енумератор
  double A[MAX_COUNT_IIR_DEC_ORDER+1];  // денумератор
 } d_COEFF_FILTER;	


//фильтра с плавающей точкой
f_FILTER 	*iir_CreateFilterFlo(float *A, float *B, unsigned char N);
float 		iir_FLO_FILTER_1ORD(f_FILTER *F, float newval);
float 		iir_FLO_FILTER_2ORD(f_FILTER *F, float newval);

//фильтра с целочисленными вычислением
d_FILTER 	*iir_CreateFilterDec(double *A, double *B, unsigned char N);
void 		 iir_DEC_FILTER_RESET(d_FILTER *F);								 		//сбросить значения фильтра
void 		 iir_DEC_FILTER_CHANGE(double *A, double *B, d_FILTER *F);				 //задать новые коэффициенты фильтру
signed int 	 iir_DEC_FILTER_1ORD(d_FILTER *F, signed int newval);	 				//фильтр 1-ого порядка
signed int 	 iir_DEC_FILTER_2ORD(d_FILTER *F, signed int newval);	 				//фильтра 2-ого порядка
signed int 	 iir_DEC_FILTER_4ORD(d_FILTER *F1, d_FILTER *F2, signed int newval);	//фильтр 4-ого порядка состоящий из двух секций (2 фильтра 2-ого порядка)

signed int   iir_DEC_INTEGRATOR_2ORD(d_FILTER *F, signed int newval);

#define iir_DEC_FILTER_CREATE(coeff,N)	 	iir_CreateFilterDec(&coeff.A[0],&coeff.B[0],N)
#define iir_DEC_FILTER_SET(coeff,filter)  iir_DEC_FILTER_CHANGE(&coeff.A[0],&coeff.B[0],filter);
//фильтр для расчета RMS
void 		  iir_DETECTOR_RESET(d_DETECTOR *F,unsigned short period); //сброс RMS фильтра
void 		  iir_DETECTOR(d_DETECTOR *F, signed int newval);		   //вызов детектора

float 		  iir_RETURN_RMS(d_DETECTOR *F); 		//Расчет RMS
float 		  iir_RETURN_MAX_PIK(d_DETECTOR *F);    //Максимальное значение за период
float 		  iir_RETURN_MIN_PIK(d_DETECTOR *F);	//Минимальное значение за период
float 		  iir_RETURN_PIK_PIK(d_DETECTOR *F);	//ПИК-ПИК (максимальное - минимальное) за период

unsigned char iir_RETURN_OVER(d_DETECTOR *F);//перегрузка входа


#endif





