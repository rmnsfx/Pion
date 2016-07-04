#ifndef _IIR
#define _IIR


#define MAX_COUNT_IIR_FLO_FILTER 1
#define MAX_COUNT_IIR_DEC_FILTER 6	//���� ���������� ��������
#define MAX_COUNT_IIR_FLO_ORDER	 2
#define MAX_COUNT_IIR_DEC_ORDER	 2	//������������ ������� �������� ��� �������� �������������



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
  //���������� �������
  unsigned long long SUM_RMS; //���������� ���������
  unsigned short 	 i;		  //��������� ����������
  unsigned short 	 period;  //������
  signed int 		 max;     //������� ������������ �������� ���������
  signed int	     min;	  //������� ����������� �������� ���������
  //���������� ���������� ���������
  unsigned long long RES;	  //����� ��������� �� ������
  signed int		 MAX_RES; //������������ �������� ��������� �� ������
  signed int		 MIN_RES; //����������� �������� ��������� �� ������
  unsigned char 	 over;	  //�������� ������ ���������
 } d_DETECTOR;

typedef struct
 {
  double B[MAX_COUNT_IIR_DEC_ORDER+1];   //����������
  double A[MAX_COUNT_IIR_DEC_ORDER+1];  // �����������
 } d_COEFF_FILTER;	


//������� � ��������� ������
f_FILTER 	*iir_CreateFilterFlo(float *A, float *B, unsigned char N);
float 		iir_FLO_FILTER_1ORD(f_FILTER *F, float newval);
float 		iir_FLO_FILTER_2ORD(f_FILTER *F, float newval);

//������� � �������������� �����������
d_FILTER 	*iir_CreateFilterDec(double *A, double *B, unsigned char N);
void 		 iir_DEC_FILTER_RESET(d_FILTER *F);								 		//�������� �������� �������
void 		 iir_DEC_FILTER_CHANGE(double *A, double *B, d_FILTER *F);				 //������ ����� ������������ �������
signed int 	 iir_DEC_FILTER_1ORD(d_FILTER *F, signed int newval);	 				//������ 1-��� �������
signed int 	 iir_DEC_FILTER_2ORD(d_FILTER *F, signed int newval);	 				//������� 2-��� �������
signed int 	 iir_DEC_FILTER_4ORD(d_FILTER *F1, d_FILTER *F2, signed int newval);	//������ 4-��� ������� ��������� �� ���� ������ (2 ������� 2-��� �������)

signed int   iir_DEC_INTEGRATOR_2ORD(d_FILTER *F, signed int newval);

#define iir_DEC_FILTER_CREATE(coeff,N)	 	iir_CreateFilterDec(&coeff.A[0],&coeff.B[0],N)
#define iir_DEC_FILTER_SET(coeff,filter)  iir_DEC_FILTER_CHANGE(&coeff.A[0],&coeff.B[0],filter);
//������ ��� ������� RMS
void 		  iir_DETECTOR_RESET(d_DETECTOR *F,unsigned short period); //����� RMS �������
void 		  iir_DETECTOR(d_DETECTOR *F, signed int newval);		   //����� ���������

float 		  iir_RETURN_RMS(d_DETECTOR *F); 		//������ RMS
float 		  iir_RETURN_MAX_PIK(d_DETECTOR *F);    //������������ �������� �� ������
float 		  iir_RETURN_MIN_PIK(d_DETECTOR *F);	//����������� �������� �� ������
float 		  iir_RETURN_PIK_PIK(d_DETECTOR *F);	//���-��� (������������ - �����������) �� ������

unsigned char iir_RETURN_OVER(d_DETECTOR *F);//���������� �����


#endif





