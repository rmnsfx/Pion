

#define MAX_ITEMS     50		//���������� ��������� ����
#define COUNT_ENUM    6		//���������� ������������� ���������
#define last_enum     COUNT_ENUM - 1




//�����
//#define   O_REF	   0x01 //1 - ���������/0 - �� ���������
#define   O_LEFT   0x02 //1 - ������������ �� ������ ����/0 - ������������ �� ������� ����
#define   O_SZ	   0x04 //1 - ���������� �� �������� ����/0 - ���
#define   O_HR	   0x08 //1 - �������(HOLD)/0 - ����������(INPUT)
#define   O_RW	   0x10 //1 - �������� ��������������/0 - ������ ������
		
//������ �������
#define   L_ACCES0 0    //������� ������� 0
#define   L_ACCES1 1    //������� ������� 1
#define   L_ACCES2 2    //������� ������� 2

//39 ����
typedef struct { 
  unsigned char Id_group;                    //������������� ������
  		   char Text_0[21];                  //��� ���������, ������
		   char Text_1[21];                  //��� ���������, ������
  		   char Caption[21];					 //����������� ��� ���������
  		   char MeasureText[4];  			 //������� ���������
  //unsigned char Data_reg;                    //����� ���������
  unsigned char Options;                     //����� ��������� 
  unsigned char Typedata;                    /*
                                             FFh - �������� 
                                             FEh - ����
                                             FDh 
											 
											 
                                             0 - xxxxx  
				                             1 - xxxx.x
				                             2 - xxx.xx
											 3 - xx.xxx
											 4 - (+/-)xxxx   //�������� �������� (0x0000 - ����������� ������������;0xFFFF - ������������ ������������� 0x8000- ����
                                             5 - (+/-)xxx.x
											 6 - xxxx ����� � �������������� ������
											 7 - xxxx ���� ������

											 10 - �������

											 16 - xx�xx   //���������� ����� ��������� � ����/������     
											 17 - xx�xx   //���������� ������ ��������� � ������/�������
                                             18 - �����   //����,������
											 19 - ����    //�����,�����
											 
				                             25-150 - ������������� ���

											*/
                    
  unsigned char LevelAcces;                 /*������� ������� � ����������
      					                     00 - �������� ��� �������������� �� ���� �������
					                         01 - �������� ��� �������������� �� 1 ������
					                         02 - �������� ��� �������������� �� 2 ������
  											
				                             */
  unsigned char Config;                      //����� ������������ 
  unsigned char Data_reg;                 //��������� �� ������ ��� ����� ���������
  } TItem;

typedef struct  {                         //���������� ��������� ��� �������������� ����
  unsigned char Id;                          //
  unsigned char V;
  		   char S[20];
  } TEnum_val;


extern const TItem     Items[MAX_ITEMS];
extern const TEnum_val Enum_val[COUNT_ENUM];



