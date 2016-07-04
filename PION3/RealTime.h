
#ifndef _rtc_h
#define _rtc_h

#define rtc_YEAR		1	//���
#define rtc_MON			2	//�����
#define rtc_DAY			3	//����
#define rtc_HOUR		4	//����
#define rtc_MIN			5	//������
#define rtc_SEC			6	//������


TStatus rtc_SETUP(void);

void rtc_COPY_RTC_TO_BUFER(void); //����������� ���� ����� � ����� �� RTC
void rtc_COPY_BUFER_TO_RTC(void); //�������� ���� � ����� � RTC �� ������

//------- ������ �� ������ --------------------------------------------------------//
unsigned short rtc_READ(unsigned char ID);

																	   
//------ ������ � ����� -----------------------------------------------------------//
//�������� ��� � ���������� ��������� (>1900)
//�������� ����� (1-12)
//�������� ����  (1-31)
//�������� ����  (0-23)
//�������� ��� (0-59)
//�������� ��� (0-59)
void rtc_WRITE(unsigned char ID,unsigned short new_val);

//------ ������� 1 ��� ------------------------------------------------------------//
//���������� 1 ���� ������� ��������� ����� 0
unsigned char rtc_CHECK_EVENT_1S(void);



#endif
