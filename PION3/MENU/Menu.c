#include "types.h"
#include "Menu.h"
#include "Keyboard.h"
#include "vga_lib.h"
#include "Items.h"
#include "string.h"
#include "Reg.h"
#include "regmap.h"
#include "Params.h"
#include "Pin.h"
#include "stdio.h"
#include "EXT_ADC.h"
#include "wav.h"
#include "main.h"
#include "Road.h"
#include "fft_calc.h" 
#include "ff.h"
#include "diskio.h"
#include "RealTime.h"
#include "stm32f10x_iwdg.h"
#include "ADC.h"
#include "math.h"
#include "diskio.h"		
#include <cerrno>







   extern FATFS   fls;            
   extern FIL     FileTmp;          
	 extern FRESULT res_t; 
	 extern	FILINFO fno;
	 
	 extern int USB_CONNECT;
	
#define men_SEPARATOR_1 			' '//0x04  //������
#define men_SEPARATOR_2 			' '//':'//0x04 //':'//��������������
#define men_SEPARATOR_3 			'!'//0x06 //������� ������
#define men_SEPARATOR_4 			0x05 //������� ��� ����
#define men_CUR_SYMBOL				'^'

#define men_SELECT_STRING			"\1"//"\13"
#define men_NO_SELECT_STRING		" "//"\12"

#define men_MAX_LEVEL       		3								//������������ ���������� 
#define men_REFRASH_TIME			50								//������ ���������� ���������� ��
#define men_TIME_RES_PAROL  		65000							//����� ������ ������ 1,2

#define mes_PAROL2					9999
#define mes_PAROL1					2000

//�������
#define men_EVENT_REF 		  		0x01
#define men_EVENT_TIME_MESAGE 		0x02


#define men_CHECK_EV(mask)			(men_CHECK_EVENT(mask)==1)	

#define men_COUNT_STR				5								//���-�� ������������ ����� ����
#define men_FONT_DEFAULT			(&FONT_6x8)						//������������ �����
#define men_HEIGHT_SYMBOL			men_FONT_DEFAULT->H				//������ ������
#define men_WIDTH_SYMBOL			men_FONT_DEFAULT->W				//������ ������
#define men_OFFSET					1								//�������� 
#define men_HEIGHT_STR				(men_HEIGHT_SYMBOL+men_OFFSET*2)//������ ������
#define men_X0						1								//���������� � - �������� ������ ���� ����
#define men_Y0						11								//���������� Y - �������� ������ ���� ����




//uint16_t Agl, Vgl;
bool crtflag = 0;
bool BeyondRoad = 0;
bool SampleAlreadyExist = 0;
extern u8	measure_stat;
extern unsigned int	Num_of_Signals;

unsigned int Num_of_Road = 0;


Tmen_status				men_STATUS;
Tmen_status				men_SAVE_STATUS;

volatile unsigned char 	men_EVENT 		=	0;
Tmen_form 				men_ID_FORM 	=	form_MESUARE;
unsigned short			men_TIMER		=	1000;
unsigned short 			men_TIME_REF 	=	men_REFRASH_TIME;
signed   short  		men_TIME_MES 	=	-1;
signed   short  		men_TIME_PAROL	= 	-1;
unsigned char 			men_LEVEL_ACCES	= 	1;    					//0 - ������ ������, 1 - ������ ��� ���������, 2 - ������ ��� ��������

unsigned short		    men_POINTER		= 	0;					    //����� ���������� ��������
unsigned short      	men_START_POINTER= 	0;						//����� �������� � �������� �������� ��������
unsigned char 			men_cp 			= 	0; 						//������������ ������� ������� ��� �������������� ���������
unsigned char 			men_cc 			= 	0; 						//�������������� ������� ������� ��� �������������� ���������
unsigned char 			men_s			= 	0;						//������ � ������� �������� ��������
signed char   			men_CURSOR_STR	= 	0; 						//???������ � ������� ���������� ������
unsigned char 			men_CONFIG_SYS  = 	0x7F;//all;		   		//������� ������� ������������
unsigned short		    men_LEVEL_POINT[men_MAX_LEVEL];	       		//������ ����� �������� 
unsigned char 			men_LEVEL_CUR[men_MAX_LEVEL];
unsigned char 			men_LEVEL		= 	0;	

unsigned int 			men_temp = 8;
//char					men_Message[128];
unsigned int road_pos = 0;
unsigned int road_pos_int = 0;
unsigned int road_indicator = 0;
char road_pos_edit[25];
char road_pos_edit_2[25];
unsigned int road_cursor_pos = 0;
char Road_Name[15]; 
unsigned char Road_Number; // ����� ��������
unsigned int route = 0; // ����� �������

unsigned int sample_pos = 0;
unsigned int sample_cursor_pos = 0;
char current_sample_num = 0;

void men_SHOW_MAINFORMS(Tmen_form form);
void men_ES_MENU(void);
void men_DW_MENU(void);
void men_UP_MENU(void);
void men_SHOW_MENU(void);
void men_SHOW_CURSOR(void);
void men_EN_MENU(void);
void men_SHOW_RECT(const char *text);
void men_SET_CONFIG(unsigned char conf);


	char h_out[5];
	char m_out[5];
	char s_out[5];
	char str_out[5];		
	char bat_out[5];
	float bat_float = 0;
	unsigned int hour = 0;			
	unsigned int min = 0;
	unsigned int sec = 0;
	unsigned int flag_bat = 0;
	unsigned int flag_charge = 0;
	unsigned int freezbat = 0;
	unsigned int freezbat2 = 0;
	unsigned int iout = 0;  		
	FIL Fil;
	FIL Fil_2;
	FRESULT result;	
	//const TCHAR savefiledirTCHAR[50];
	const TCHAR * savefiledirTCHAR;
	const TCHAR * savefilenameTCHAR;
	const TCHAR * savefilenameTCHAR2;
	char savefilename[25];
	char savefiledir[25];
	char temp_str3[25];
	bool flag_charge2 = 0;
	unsigned int old_state_usb = 0; 
	unsigned int old_state_pa8 = 0;
	unsigned char VALUE=0;
	unsigned long count_menu_items = 0; ///���-�� ��������� � ����
	unsigned int exist; ///�������� ����. ��-�� ����




extern void FR_print_error( FRESULT fresult )
{
	/* File function return code (FRESULT) */
	const char *FRErrorStr[] = {
		"FR_OK",					/* (0) Succeeded */
		"FR_DISK_ERR",				/* (1) A hard error occurred in the low level disk I/O layer */
		"FR_INT_ERR",				/* (2) Assertion failed */
		"FR_NOT_READY",				/* (3) The physical drive cannot work */
		"FR_NO_FILE",				/* (4) Could not find the file */
		"FR_NO_PATH",				/* (5) Could not find the path */
		"FR_INVALID_NAME",			/* (6) The path name format is invalid */
		"FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
		"FR_EXIST",					/* (8) Access denied due to prohibited access */
		"FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
		"FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
		"FR_INVALID_DRIVE",			/* (11) The logical drive number is invalid */
		"FR_NOT_ENABLED",			/* (12) The volume has no work area */
		"FR_NO_FILESYSTEM",			/* (13) There is no valid FAT volume */
		"FR_MKFS_ABORTED",			/* (14) The f_mkfs() aborted due to any parameter error */
		"FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
		"FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
		"FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
		"FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_SHARE */
		"FR_INVALID_PARAMETER"		/* (19) Given parameter is invalid */
	};
	
		
			vga_CLEAR();
			vga_SET_POS_TEXT(0,20);	
			vga_PRINT_STR(FRErrorStr[fresult],&FONT_6x8);
			vga_UPDATE();	
								 
			Delay(3000000); 
}


FRESULT set_timestamp (const TCHAR *obj )
{
    
	FILINFO fno2;
	int year=0;
	int month = 0;
	int mday = 0;
	int hour = 0;
	int sec = 0;
	
	CONTROL_RTC();
	
	  year = rtc_READ(rtc_YEAR);
    month = rtc_READ(rtc_MON);
    mday = rtc_READ(rtc_DAY);
    hour = REG(TIME)/100;
    min = REG(TIME)%100; 
    sec = REG(SECOND);
			
		

    fno2.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno2.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno2);
}



//----------------------------------------------------------------//
// ����� �������. ���� ������� ���������, �� ������� ���������� 1
//----------------------------------------------------------------//
unsigned char men_CHECK_EVENT(unsigned char mask_event)
{
 if (men_EVENT & mask_event)
  {
   men_EVENT&=~mask_event;
   return 1;
  }
 else
   return 0;
}

//----------------------------------------------------------------//
// ������ ����. ��������� ������ ���������� � 1�� ����������
//----------------------------------------------------------------//
void men_1ms(void)
{
	
 if (men_TIME_REF) men_TIME_REF--;
 else 
  {
   men_EVENT|=men_EVENT_REF;
   men_TIME_REF = men_REFRASH_TIME;
  }

 if (men_TIME_MES>0) men_TIME_MES--;
 else
  if (men_TIME_MES==0) 
   {
    men_EVENT|=men_EVENT_TIME_MESAGE;
	men_TIME_MES = -1;
   }   
 

  if (men_TIMER>0) men_TIMER--;
  if (!men_TIMER)  men_TIMER=1000; 
	
}

//----------------------------------------------------------------//
// ������������� ����
//----------------------------------------------------------------//
void men_SETUP(void)
{
// men_LEVEL_ACCES = (men_PAROL1==0);
// men_TIME_PAROL  = NIL;
 //men_STATUS      = men_MAIN;
 //vga_INIT();
	FILE *file;	
	
	men_SET_CONFIG(REG(CHANEL_MESUARE));		 //������ ������������ ���� ���� ������� ������ ����� ��������� 
		
	if (rod_INIT() != 0) 
	{
		//men_SHOW_MESSAGE("������ ������","����������� �����",800);
		//men_SHOW_MESSAGE("**","",100);
		return;
	}	 
 	
	rod_GET_NameElement(&NEl,1);
	//Alex
	//Road_Name = NEl.StringName_1;
	memcpy(Road_Name, NEl.StringName_1, 15);
	Road_Number = NEl.Number;
	//rod_GET_NameElement(&NEl,REG(NUMFILE_CURENT));
 
 
	if (Road_Number == 0) 
	{
		//REGW(NUMFILE_CURENT,REG(BEYOND_ROAD));
		rod_GET_NameElement(&NEl,1+REG(BEYOND_ROAD));
		REGW(NUMFILE_CURENT,1+REG(BEYOND_ROAD));
	}
	else 
	{
		REGW(NUMFILE_CURENT,2);
		rod_GET_NameElement(&NEl,2);
	}
	
	//*Alex
 
 //men_SHOW_MAINFORMS(form_MESUARE);
// while (!key_CHECK_EV(key_EVENT_PRESSED_ENTER));
	
	if ( first_flag == 1)	REGW(ROUTE_NUM, 0); /// ���� ������ ��������� ���������� ������� �������� �������
	
	if ( first_flag == 2 && Road_Number != 0)
	{		
		if (Road_Number != Num_of_Road) { REGW(ROUTE_NUM, 0); route=0; }	/// ���������� ������� �������� �������			 
		else { route++; REGW(ROUTE_NUM,route);	} /// ����������� ������� �������
	}
	
	first_flag = 2;
}

//----------------------------------------------------------------//
// ��������� �������� �� ������ �������� ����
//----------------------------------------------------------------//
void men_READ_VALUE_PARAM(unsigned int point)
{
 typVALUE_PARAM.val = REG(Items[point].Data_reg);
 typVALUE_PARAM.max = REG_MAX(Items[point].Data_reg);
 typVALUE_PARAM.min = REG_MIN(Items[point].Data_reg);
 typVALUE_PARAM.def = REG_DEF(Items[point].Data_reg);
}

//----------------------------------------------------------------//
// ��������� �������� �� ������ ��������
//----------------------------------------------------------------//
void men_READ_PARAM(unsigned int num_reg)
{
 typVALUE_PARAM.val = REG(num_reg);
 typVALUE_PARAM.max = REG_MAX(num_reg);
 typVALUE_PARAM.min = REG_MIN(num_reg);
 typVALUE_PARAM.def = REG_DEF(num_reg);
}


void men_WRITE_VALUE_PARAM(unsigned int point)
{
 if (Items[point].Data_reg==CHANEL_MESUARE) 
  {
   men_SET_CONFIG(typVALUE_PARAM.val&0xFF);		 //������ ������������ ���� ���� ������� ������ ����� ���������
  }

 REGW(Items[point].Data_reg,typVALUE_PARAM.val);
}

void men_SHOW_BAT(unsigned char X, unsigned char Y, unsigned char VAL)
{
 if (VAL>100) return;

 vga_RECTANGLE(X,Y,X+13,Y+6,drRECT_NO_FILL);
 vga_LINE(X+14,Y+2,X+14,Y+4);

 if (VAL>=10)
  vga_RECTANGLE(X+2,Y+2,X+(VAL/10)+1,Y+4,drRECT_FILL);	

}

void men_SHOW_BAT_edit(unsigned char X, unsigned char Y, unsigned char VAL)
{
 
 //if (VAL<0) return; 

 vga_RECTANGLE(X,Y,X+13,Y+6,drRECT_NO_FILL);
 vga_LINE(X+14,Y+2,X+14,Y+4);
 
 
 if (VAL>=10) vga_RECTANGLE(X+2,Y+2,X+2,Y+4,drRECT_FILL);
 if (VAL>=20) vga_RECTANGLE(X+2,Y+2,X+3,Y+4,drRECT_FILL);
 if (VAL>=30) vga_RECTANGLE(X+2,Y+2,X+5,Y+4,drRECT_FILL); 
 if (VAL>=40) vga_RECTANGLE(X+2,Y+2,X+6,Y+4,drRECT_FILL); 
 if (VAL>=50) vga_RECTANGLE(X+2,Y+2,X+7,Y+4,drRECT_FILL); 
 if (VAL>=60) vga_RECTANGLE(X+2,Y+2,X+8,Y+4,drRECT_FILL); 
 if (VAL>=70) vga_RECTANGLE(X+2,Y+2,X+9,Y+4,drRECT_FILL); 
 if (VAL>=80) vga_RECTANGLE(X+2,Y+2,X+10,Y+4,drRECT_FILL); 
 if (VAL>=90) vga_RECTANGLE(X+2,Y+2,X+11,Y+4,drRECT_FILL); 
	
}




void men_SHOW_LARGE_BAT (unsigned char VAL)
{
	
//if (usb_charge_state != 0)	
{
	if (VALUE > 100) 
	{
		VALUE = 0;
		vga_CLEAR();		
	}
	else VALUE += 10;
	
	
	
	vga_RECTANGLE(17+10,11,85+10,49,drRECT_NO_FILL);
	vga_RECTANGLE(18+10,12,84+10,48,drRECT_NO_FILL);
	vga_RECTANGLE(84+10,20,90+10,40,drRECT_FILL);
	
	
	if (VALUE>5) vga_RECTANGLE(21+10,15,27+10,45,drRECT_ARC_FILL);
	if (VALUE>20) vga_RECTANGLE(30+10,15,36+10,45,drRECT_ARC_FILL);
	if (VALUE>35) vga_RECTANGLE(39+10,15,45+10,45,drRECT_ARC_FILL);
	if (VALUE>50) vga_RECTANGLE(48+10,15,54+10,45,drRECT_ARC_FILL);
	if (VALUE>65) vga_RECTANGLE(57+10,15,63+10,45,drRECT_ARC_FILL);
	if (VALUE>80) vga_RECTANGLE(66+10,15,72+10,45,drRECT_ARC_FILL);
	if (VALUE>99) vga_RECTANGLE(75+10,15,81+10,45,drRECT_ARC_FILL);
}


}


//----------------------------------------------------------------//
// �������� ������� �����.
// form - ����� ������� �����
//----------------------------------------------------------------//

void men_SHOW_REFRESH(void)
{
 char t_str[20];
 char t_str2[20];
 char *temp_str;
 unsigned int temp_reg;
 unsigned int temp_val;
 FILE  		*pRFile = 0;
 //static float A,V,S;
 uint16_t A, V, S, cn;
 float* result;
 unsigned int sch=0;
 unsigned int flag_charge = 0;
	

	
 switch (men_ID_FORM)
 {
  case form_MESUARE:
		
  					vga_CLEAR();

						if (REG(AKB_EMK) == 1)
						{
										//������� �������
										vga_SET_POS_TEXT(21, 1);						
										sprintf(t_str,"%0.5f", (float) akbemk_count);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//����������
										vga_SET_POS_TEXT(52, 1);						
										sprintf(t_str,"%0.1f", (float) akbemk_volt);						
										vga_PRINT_STR(t_str,&FONT_4x7);							
							
										//�������� ��� ����������� ���������
										vga_SET_POS_TEXT(67, 1);						
										sprintf(t_str,"%d", (int) akbemk_percent);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//���
										vga_SET_POS_TEXT(21, 7);						
										sprintf(t_str,"%0.5f", (float) akbtemp);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//���������� BKP ��������
										vga_SET_POS_TEXT(55, 7);						
										sprintf(t_str,"%d", BKP_ReadBackupRegister(BKP_DR10));						
										vga_PRINT_STR(t_str,&FONT_4x7);
						}
						else
						{
										men_READ_PARAM(TIME); //��������� �����
										temp_str = typ_SHOW_TIME_VALUE(&typVALUE_PARAM);
										vga_SET_POS_TEXT(21,1);	
										vga_PRINT_STR(temp_str,&FONT_4x7);
						}
	
//�������� ��������������						
//										vga_SET_POS_TEXT(1, 7);						
//										sprintf(t_str,"%d", REG(AUTOPOWEROFF));						
//										vga_PRINT_STR(t_str,&FONT_4x7);
//										vga_SET_POS_TEXT(20, 7);						
//										sprintf(t_str,"%d", POWER_OFF);						
//										vga_PRINT_STR(t_str,&FONT_4x7);
//										vga_SET_POS_TEXT(55, 7);						
//										sprintf(t_str,"%d", key_STATE);						
//										vga_PRINT_STR(t_str,&FONT_4x7);
						
						
						
						
					  
						///��������� �������� ���������� �� ������� ��� ��������� ������					
						//if (timer2 == 0 && measure_stat == 0) 
						if (timer2 == 0) 
						{
							if (id_akb == 0) frzbat1 = akbemk_percent;
							else frzbat1 = adc_BAT_PERCENT_edit();
						}
							
						///������ ���������		
						if (frzbat1 >= 0) men_SHOW_BAT_edit(1,1,frzbat1);
						else men_SHOW_BAT_edit(1,1,0);										
	
						//������ �������
						vga_SET_POS_TEXT(1,vga_GET_HEIGHT_DISPLAY-7);
						switch (REG(CHANEL_MESUARE))
						{
						  case 0x01: temp_reg = REG(FILTER_A);	   //���������
						  			 if (temp_reg==0x01)  vga_PRINT_STR("5-1000 ��",&FONT_4x7);
									 if (temp_reg==0x02)  vga_PRINT_STR("10-1000 ��",&FONT_4x7);
									 if (temp_reg==0x04)  vga_PRINT_STR("10-2000 ��",&FONT_4x7);
									 if (temp_reg==0x08)  vga_PRINT_STR("10-5000 ��",&FONT_4x7);
									 if (temp_reg==0x10)  vga_PRINT_STR("10-10000 ��",&FONT_4x7);
						  			 break;
						  case 0x02: temp_reg = REG(FILTER_V);	   //��������
						  			 if (temp_reg==0x01)  vga_PRINT_STR("5-1000 ��",&FONT_4x7);
									 if (temp_reg==0x02)  vga_PRINT_STR("10-1000 ��",&FONT_4x7);
									 if (temp_reg==0x04)  vga_PRINT_STR("10-2000 ��",&FONT_4x7);
									 if (temp_reg==0x08)  vga_PRINT_STR("10-5000 ��",&FONT_4x7);
						  			 break;
						  case 0x04: temp_reg = REG(FILTER_S);	   //�����������
						  			 if (temp_reg==0x01)  vga_PRINT_STR("10-300 ��",&FONT_4x7);
						  			 break;
						  case 0x08: vga_PRINT_STR("10-10000 ��",&FONT_4x7);	   //�������
						  			 break;
						  default  : temp_reg = 0;
						 }
						
								
						//���������� ��� ���������
						switch (REG(CHANEL_MESUARE))
						 {
						  case 0x01: temp_reg = REG(DETECT_A);
						  			 break;
						  case 0x02: temp_reg = REG(DETECT_V);
						  			 break;
						  case 0x04: temp_reg = REG(DETECT_S);
						  			 break;
						  case 0x08: temp_reg = 0x00;
						  default  : temp_reg = 0;
						 }

 						switch (temp_reg)
						 {
						  case 0x01: //���
						  			 men_READ_PARAM(RMS_VAL); //��������� ��������
									 if (REG(CHANEL_MESUARE)==0x04) //����������������
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 ���� ����� �������
									 else 
									  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 ���� ����� �������
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 ���� ����� �������

						   			  
						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //����������� ��������
						   			 vga_SET_POS_TEXT(3,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 //vga_PRINT_STR("���-������",&FONT_6x8);
						   			 vga_PRINT_STR("       ���",&FONT_6x8);
									 break;
						  case 0x02: //���
						   			 men_READ_PARAM(PIK_VAL); //��������� ��������
									 if (REG(CHANEL_MESUARE)==0x04) //����������������
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 ���� ����� �������
									 else 
						   			  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 ���� ����� �������
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 ���� ����� �������

									 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //����������� ��������
									 vga_SET_POS_TEXT(3,24);
									 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("       ���",&FONT_6x8);
									 break;
						  case 0x04: //���-���
						  			 men_READ_PARAM(PIK_PIK_VAL); //��������� ��������
									 if (REG(CHANEL_MESUARE)==0x04) //����������������
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 ���� ����� �������
									 else 
						   			  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 ���� ����� �������
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 ���� ����� �������

						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //����������� ��������
						   			 vga_SET_POS_TEXT(3,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("   ���-���",&FONT_6x8);
									 break;
						  case 0x08: //���-������
						  			 men_READ_PARAM(PIK_FACTOR_VAL); //��������� ��������
									 temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 ���� ����� �������
						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //����������� ��������
						   			 vga_SET_POS_TEXT(16,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("���-������",&FONT_6x8);
									 break;
						  case 0x10: break;
						  	
						  case 0x00: //�������

							

							

////������ 10 ���. 							
//							vga_SET_POS_TEXT(57,1);								
//							  sprintf(t_str,"%d", REG(ROUTE_NUM)+1);
//									 vga_PRINT_STR(t_str,&FONT_6x8);

//							
//							vga_SET_POS_TEXT(50,20);								
//							  sprintf(t_str,"%d", NEl.Number);
//									 vga_PRINT_STR(t_str,&FONT_6x8);
//							vga_UPDATE();
							
							
///���������
//							vga_SET_POS_TEXT(75,8);
//								sprintf(t_str,"%d ", measure_stat);
//							  //sprintf(t_str,"%d ", adc_BAT_MEASURE_edit());
//							  //sprintf(t_str,"%d ", measure_stat);
//									 vga_PRINT_STR(t_str,&FONT_4x7);

//// ��������							
//							vga_SET_POS_TEXT(42,7);
//							 sprintf(t_str,"%d",route);
//									 vga_PRINT_STR(t_str,&FONT_4x7);
//							
//							vga_SET_POS_TEXT(47,7);
//							 sprintf(t_str,"%d",REG(ROUTE_NUM));
//									 vga_PRINT_STR(t_str,&FONT_4x7);
//							
//							vga_SET_POS_TEXT(56,7);
//							 sprintf(t_str,"%d",Road_Number);
//									 vga_PRINT_STR(t_str,&FONT_4x7);
//							vga_SET_POS_TEXT(60,7);
//							 sprintf(t_str,"%d",Num_of_Road);
//									 vga_PRINT_STR(t_str,&FONT_4x7);
							
							
							
									 vga_SET_POS_TEXT(82,1);		
									 vga_PRINT_STR("��",&FONT_4x7);				
									 vga_LINE(91,2,91,6);
									 
									 vga_SET_POS_TEXT(92,1);											 
									 sprintf(t_str,"����� %03d",REG(ROUTE_NUM)+1);									 
									 vga_PRINT_STR(t_str,&FONT_4x7);


									 //vga_RECTANGLE(8,22,20+100+2,33,drRECT_NO_FILL);
									 vga_SET_POS_TEXT(2,14);
									 temp_val = REG(SAMPLING_STATUS);	
								
									
									 vga_RECTANGLE(2,25,2+122,29,drRECT_NO_FILL);
									 if (temp_val==0xFF) temp_val = 0;

									 if (BeyondRoad) vga_PRINT_STR(Road_Name,&FONT_6x8);
									 else vga_PRINT_STR(Road_Name,&FONT_6x8);
									 
									 
									 vga_SET_POS_TEXT(95,14);
									 sprintf(t_str,"%3d %%",temp_val);
									 vga_PRINT_STR(t_str,&FONT_6x8);
								     
									 vga_SET_POS_TEXT(2,33);
									 sprintf(t_str,"����� %03d             ",REG(BEYOND_ROAD));
									 

									if (BeyondRoad) vga_PRINT_TEXT(NEl.StringName_1,15,&FONT_6x8);
									else vga_PRINT_TEXT(NEl.StringName_1,15,&FONT_6x8);


								
								
						/////////////////////////////////////////////////////								

						
//						//���������� ���, ������� �������
//						vga_SET_POS_TEXT(35,45);						
//						sprintf(t_str,"%d", adc_BAT_MEASURE_edit());						
//						vga_PRINT_STR(t_str,&FONT_4x7);
//		
//						//���������� �������
//						vga_SET_POS_TEXT(45,45);						
//						sprintf(t_str,"%0.5f", (float) akbtemp);						
//						vga_PRINT_STR(t_str,&FONT_4x7);
//							
//												
//						//PA8
//						vga_SET_POS_TEXT(85,45);				
//						sprintf(t_str,"%d", GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8));
//						vga_PRINT_STR(t_str,&FONT_4x7);
//						
//						//��������
//						vga_SET_POS_TEXT(95,45);				
//						sprintf(t_str,"%d", (int) ceil(akbemk_count * 100 / 0.6));
//						vga_PRINT_STR(t_str,&FONT_4x7);
//						
//						//����������
//						vga_SET_POS_TEXT(115,45);				
//						sprintf(t_str,"%0.3f", (float) akbemk_volt);
//						vga_PRINT_STR(t_str,&FONT_4x7);
//						
//						//��_���
//						vga_SET_POS_TEXT(38,45);				
//						sprintf(t_str,"%d", usb_charge_state);
//						vga_PRINT_STR(t_str,&FONT_4x7);

						
						
						/////////////////////////////////////////////////////
						

							if (crtflag == 1)
							{
								
							
										///���������� ��� ����� �������� �� ������� 
										pRFile = fopen ("Roads.txt","r");
										if (pRFile != NULL)
										{
												fseek(pRFile,9*road_pos,SEEK_SET);
												fscanf(pRFile, "%s", road_pos_edit);
												fclose(pRFile);
										}

										IWDG_ReloadCounter();			
																										
										strncpy(road_pos_edit_2,road_pos_edit+5,8-5); ///�������� ��������� ������ ����� � ��������			
										road_pos_int = atoi(&road_pos_edit_2); /// � int
										
									
										__disable_irq();
										__disable_fiq();
										
										
										sprintf(t_str,"M:\\%03u.%03u\\Signal %d.dat",Road_Number,REG(ROUTE_NUM),NEl.Number);
										//sprintf(t_str,"0:%03u.%03u/Signal %d.dat",road_pos_int,REG(ROUTE_NUM),NEl.Number);								
																		
										A = calc_from_dat_A(t_str);
										V = calc_from_dat_V(t_str);


										sprintf(t_str2,"0:%03u.%03u/Signal %d.dat",road_pos_int,REG(ROUTE_NUM),NEl.Number);
										savefilenameTCHAR2 = t_str2;
										
										f_mount(&fls, "0:", 1);
										res_t = f_open(&Fil,savefilenameTCHAR2, FA_OPEN_ALWAYS | FA_WRITE);  
										res_t = f_lseek(&Fil, f_size(&Fil));
										res_t = f_write(&Fil,&A,2,&iout);		
										res_t = f_write(&Fil,&V,2,&iout);		
										f_close(&Fil);			
										f_mount(0,"0:", 0);							
										
										__enable_irq();
										__enable_fiq();	
										

										crtflag = 0;
										temp_reg = REG(NUMFILE_CURENT);
										temp_reg++;
										REGW(NUMFILE_CURENT,temp_reg);
										rod_GET_NameElement(&NEl,temp_reg);
										if (NEl.Number == 0) 
											{
											 temp_reg--;
											 REGW(NUMFILE_CURENT,temp_reg);
											 rod_GET_NameElement(&NEl,temp_reg);
											}									
									
								}
							
								
								///����������� A,V � �������
								
							 //sprintf(t_str,"M:\\%03u.%03u\\Signal %d.dat",Road_Number,REG(ROUTE_NUM),NEl.Number);	
							 sprintf(t_str,"M:\\%03u.%03u\\Signal %d.dat",road_indicator,REG(ROUTE_NUM),NEl.Number);	
							 
							
									 if (pRFile = fopen (t_str,"r"))
										 {
												fseek(pRFile,-4,SEEK_END);
												fread(&A,2,1,pRFile);
												fread(&V,2,1,pRFile);
											 
												memset(t_str,'\0', 20);
												sprintf(t_str,"��� A = %3.02f �/�%c",(float)A/100,0xbd);
												vga_SET_POS_TEXT(2,45);
												vga_PRINT_TEXT(t_str,20,&FONT_6x8);

												if ( (V/100) > 99 )
												sprintf(t_str,"��� V = %d ��/� ", (int)V/100);
												else
												sprintf(t_str,"��� V = %3.01f ��/�", (float)V/100);
												vga_SET_POS_TEXT(2,57);
												vga_PRINT_TEXT(t_str,20,&FONT_6x8);
//												memset(t_str,' ', 20);
//												vga_SET_POS_TEXT(2,60);
//												vga_PRINT_TEXT(t_str,20,&FONT_6x8);
											 
												fclose(pRFile);
												SampleAlreadyExist = 1;
										 }
										 else SampleAlreadyExist = 0;
												vga_RECTANGLE(2,25,2+122*temp_val/100,29,drRECT_FILL);
										 
										 //if (!BeyondRoad)
										 {
												vga_SET_POS_TEXT(109,56);
												vga_SET_DRAW_MODE(drMODE_XOR);
												sprintf(t_str,"%03d",NEl.Number);
												vga_PRINT_TEXT(t_str,3,&FONT_6x8);
												vga_RECTANGLE(108,55,128,64,drRECT_FILL); 
												vga_SET_DRAW_MODE(drMODE_NORMAL);
										 }											 
									 //Alex
										if (temp_val == 0x64) 
										{measure_stat = 0; 
											men_EN_MENU();}
								
									 
						  default:	 break;		
						 }

						//������� ��.��������� ������ ��� ���,���,���-���
						if ((temp_reg==0x01)||(temp_reg==0x02)||(temp_reg==0x04))
						 switch (REG(CHANEL_MESUARE))
						 {
						  case 0x01: //��������������
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("�/�",&FONT_6x8);
							 		 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-27,29);
						     		 vga_PRINT_STR("2",&FONT_4x7);
						  			 break;
						  case 0x02: //�������������
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("��/�",&FONT_6x8);
						  			 break;
						  case 0x04: //����������������
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("���",&FONT_6x8);
						  			 break;
						  default  : temp_reg = 0;
						 }
							
						
  					 //������� ������ "���������"	
						 
						 if ( (measure_stat == 2) && REG(PION_STATUS) )
						 {
						  vga_SET_POS_TEXT(53,0);
						  vga_PRINT_STR("���",&FONT_6x8);
						  vga_SET_DRAW_MODE(drMODE_XOR);
						  vga_RECTANGLE(51,0,73,8,drRECT_FILL);
						  vga_SET_DRAW_MODE(drMODE_NORMAL);
						  
						  if ((REG(PION_STATUS) & ST_OVER)>0)
						   {
									//������� ��������� ��������
									vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-25,14);
									vga_PRINT_STR("*",&FONT_6x8);
									vga_SET_DRAW_MODE(drMODE_XOR);
									vga_RECTANGLE(vga_GET_WIDTH_DISPLAY-25,14,vga_GET_WIDTH_DISPLAY-19,20,drRECT_ARC_FILL);
									vga_SET_DRAW_MODE(drMODE_NORMAL);
						   }
						  
						 }

			   
						
						break;
  case form_USB:	
						
						
	
						vga_CLEAR();
						men_READ_PARAM(TIME); //��������� �����
						temp_str = typ_SHOW_TIME_VALUE(&typVALUE_PARAM);
						vga_SET_POS_TEXT(21,1);	
						vga_PRINT_STR(temp_str,&FONT_4x7);	
	
						if (id_akb == 0) men_SHOW_BAT_edit(1,1, akbemk_percent);	
							else men_SHOW_BAT_edit(1,1,adc_BAT_PERCENT_edit_charge());	
	
						vga_SET_POS_TEXT(45,24);
						vga_PRINT_STR("USB",&FONT_12x16);												
						//vga_UPDATE();
	
						if (REG(AKB_EMK) == 1)
						{
										//������� �������
										vga_SET_POS_TEXT(21, 1);	
										sprintf(t_str,"%0.5f", (float) akbemk_count);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//����������
										vga_SET_POS_TEXT(52, 1);						
										sprintf(t_str,"%0.1f", (float) akbemk_volt);						
										vga_PRINT_STR(t_str,&FONT_4x7);							
							
										
										vga_SET_POS_TEXT(67, 1);						
										sprintf(t_str,"%d", (int) frzbat1);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//���
										vga_SET_POS_TEXT(83, 1);						
										sprintf(t_str,"%0.4f", (float) akbtemp);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										vga_SET_POS_TEXT(110, 1);						
										sprintf(t_str,"%d", usb_charge_state);						
										vga_PRINT_STR(t_str,&FONT_4x7);																	
							
						}
						
						
												
						
						///��������� ��� ������� �� ����������� ������ ���
						if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )
						{												
							LED_CHARGE_ON();
							usb_charge_state = 1;															
						}	
						
						CHARGE_ON();
						
						Delay(100000);
						
						if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 1 && old_state_pa8 == 0 && pin_USB_5V == 1 && old_state_usb == 1 )
						{							
								LED_CHARGE_OFF();	
							
								BKP_WriteBackupRegister(BKP_DR10, (int) ceil(akbemk_menu / 0.00001831082)); 
								akbemk_count = akbemk_menu;
								akbemk_percent = 100;	

								usb_charge_state = 2;										
						}			
						
						
						///���������� ��������� usb � pa8
						old_state_usb = pin_USB_5V;
						old_state_pa8 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
												
						frzbat1 = akbemk_percent;
						
						
						break;
	
	case form_CHARGE:		
	
	
	if (id_akb == 0) //����� ���					
	{					
						
						///��������� ��� ������� �� ����������� ������ ���
						if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )
						{												
							LED_CHARGE_ON();
							usb_charge_state = 1;								
						}					
						
						CHARGE_ON();
						
						men_SHOW_LARGE_BAT(0);

						/////////////////////////////////////////////////////
						if (REG(AKB_EMK) == 1)
						{
										//������� �������
										vga_SET_POS_TEXT(21, 1);											
										sprintf(t_str,"%0.5f", (float) akbemk_count);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										//����������
										vga_SET_POS_TEXT(52, 1);						
										sprintf(t_str,"%0.1f", (float) akbemk_volt);						
										vga_PRINT_STR(t_str,&FONT_4x7);							
							
										
										vga_SET_POS_TEXT(67, 1);						
										sprintf(t_str,"%d", (int) frzbat1);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										vga_SET_POS_TEXT(82, 1);						
										sprintf(t_str,"%f", (float) akbtemp);						
										vga_PRINT_STR(t_str,&FONT_4x7);
							
										vga_SET_POS_TEXT(120, 1);						
										sprintf(t_str,"%d", usb_charge_state);						
										vga_PRINT_STR(t_str,&FONT_4x7);
						}
						
								
						//////////////////////////////////////////////////////		
						Delay(100000);
								
						if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 1 && old_state_pa8 == 0 && pin_USB_5V == 1 && old_state_usb == 1 )
						{							
								LED_CHARGE_OFF();	
							
								BKP_WriteBackupRegister(BKP_DR10, (int) ceil(akbemk_menu / 0.00001831082)); 
								akbemk_count = akbemk_menu;
								akbemk_percent = 100;	

								usb_charge_state = 2;	
							
								vga_CLEAR();
								vga_SET_POS_TEXT(25,24);
								vga_PRINT_STR("����� ��������",&FONT_6x8);																												
								vga_UPDATE();
													
								while (pin_USB_5V) {}						
									
						}
						
						
						
						//���������� ��������� usb � pa8
						old_state_usb = pin_USB_5V;
						old_state_pa8 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);							
							
						frzbat1 = akbemk_percent;
						
						
	}
	else //������ ���
	{
						vga_CLEAR();						
						men_SHOW_LARGE_BAT( adc_BAT_PERCENT_edit_charge() );
						vga_UPDATE();
	
						LED_CHARGE_ON();
						CHARGE_ON();
							
						
						if (adc_BAT_PERCENT_edit_charge() < 100) 
						{
							LED_CHARGE_ON();
							CHARGE_ON();							
							
								
						}
						else 
						{
							
							while (pin_USB_5V) 
							{
												
												LED_CHARGE_ON();
												vga_CLEAR();																					
												IWDG_ReloadCounter();									
												sec = REG(SECOND);												 
												if (sec == 0) { min++; Delay(10000000); }												
												if (min == 60) 
												{
														hour++;
														min = 0;
												}

												if ( hour >= 3 ) 	
												{ 
														CHARGE_OFF(); 
														LED_CHARGE_OFF(); 														
														flag_bat = 1;	
																																								
														vga_CLEAR();
														vga_SET_POS_TEXT(25,24);
														vga_PRINT_STR("����� ��������",&FONT_6x8);																												
														vga_UPDATE();
													
														while (pin_USB_5V) {}
													
												}	
												
												vga_CLEAR();
												men_SHOW_LARGE_BAT(adc_BAT_PERCENT_edit_charge());
												vga_UPDATE();
												Delay(1000000);

							}
							
							
							LED_CHARGE_OFF();
							CHARGE_OFF();	
						}
	
	}
							
  					break;		
		default: 
		measure_stat = 0;
		return;
 }

 vga_UPDATE();
 
}

//----------------------------------------------------------------//
// ������� ���������
//----------------------------------------------------------------//
unsigned char men_SHOW_MESSAGE(char *mes1,char *mes2, signed int time_mes)
{
  vga_CLEAR();

  vga_SET_POS_TEXT(1,1);
  vga_PRINT_STR(mes1,&FONT_6x8);

  vga_SET_POS_TEXT(1,20);
  vga_PRINT_STR(mes2,&FONT_6x8);

  vga_UPDATE();

  men_TIME_MES = time_mes;

  while (1)
  {
   if (key_CHECK_EV(key_EVENT_PRESSED_ENTER)) 	 return 1;
   if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU)) return 0;
   if (men_CHECK_EV(men_EVENT_TIME_MESAGE))		 return 0;
   key_CHECK_EV(key_EVENT_PRESSED_UP|key_EVENT_PRESSED_DOWN|key_EVENT_PRESSED_MESUARE);
  }

 //return 0;
}


//----------------------------------------------------------------//
// ���������� ������� �����.
// form - ����� ������� �����
//----------------------------------------------------------------//
void men_SHOW_MAINFORMS(Tmen_form form)
{
 men_STATUS = men_MAIN;

 if (men_ID_FORM!=form) 
 						{
						 vga_CLEAR();
						 men_ID_FORM = form;
						}

 //lcd_SET_NON_CURSOR;

 
 men_SHOW_REFRESH();

 //vga_UPDATE();

}

//----------------------------------------------------------------//
// ��������� ������������ �������� ����,
//----------------------------------------------------------------//
unsigned char men_GET_CONFIG(unsigned short id_)                //�������� ������������ ��������
{

 unsigned short reg = REG(CHANEL_MESUARE);
 
	
 if (men_CONFIG_SYS == 0x80) 
 {	 
	 if (Items[id_].Config == 0x80) return 1;	
	 
	 if (reg == 0x1) //���������
	 {
		 if (Items[id_].Config == 0x81) return 1;	
		 if (Items[id_].Config == 0x82) return 0;
		 if (Items[id_].Config == 0x84) return 0;
	 }
	 
	 if (reg == 0x2) //��������
	 {
		 if (Items[id_].Config == 0x81) return 0;	
		 if (Items[id_].Config == 0x82) return 1;
		 if (Items[id_].Config == 0x84) return 0;
	 }
	 
	 if (reg == 0x4) //�����������
	 {
		 if (Items[id_].Config == 0x81) return 0;	
		 if (Items[id_].Config == 0x82) return 0;
		 if (Items[id_].Config == 0x84) return 1;
	 }
	 
	 if (reg == 0x8) //�������
	 {
		 if (Items[id_].Config == 0x87) return 0;			 
		 if (Items[id_].Config == 0x78) return 1;	
	 }
	 	
 }
 	
 if (men_CONFIG_SYS & Items[id_].Config) 
 {	 
	 if ( (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0) && (Items[id_].Config == 0xC7) ) return 0;
	 else return 1;	 
 }
 else 
 {	 
	 if ( (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 1) && (Items[id_].Config == 0xC7) ) return 1;
	 else return 0;	
 }
                      
}

//----------------------------------------------------------------//
// ������ ����� ������������ ����
//----------------------------------------------------------------//
void men_SET_CONFIG(unsigned char conf)
{
 men_CONFIG_SYS = conf;
}

//----------------------------------------------------------------//
// ������� ����� ���������� �������� � ������
//----------------------------------------------------------------//
unsigned short men_GET_NEXT_ITEM(unsigned short point)    //���������� ����� ���������� �������� � ������
{
	unsigned char id;
	
	id = Items[point].Id_group;
	
	if ((Items[point].Data_reg == 0xFF)||(Items[point].Data_reg == 0xFE)||(Items[men_POINTER].Data_reg == 0xFB)||(Items[men_POINTER].Data_reg == 0xFC))
	{		
		return point;
	}
	
	//*Alex
	while (point<(MAX_ITEMS-1))
  {
		point++;
		//if (Items[point-1].Id_group == Items[point].Id_group) 
		//if (Items[point].Data_reg == 0xFF) return point--;
		
		if (id == Items[point].Id_group)			
    if (men_GET_CONFIG(point)) return point; 
    //else return NIL;
  }
	
	return NIL;
	
}


//----------------------------------------------------------------//
// ������� ����� ����������� �������� � ������
//----------------------------------------------------------------//
unsigned short men_GET_PREV_ITEM(unsigned short point)
{
unsigned char id;
 id = Items[point].Id_group;
	//Alex
	if ((Items[point].Data_reg == 0xFF)||(Items[point].Data_reg == 0xFE)||(Items[men_POINTER].Data_reg == 0xFB)||(Items[men_POINTER].Data_reg == 0xFC))
	{	
	//	if (road_pos_tmp > 0) road_pos_tmp--;
		return point;	
	}			
	
		//*Alex
 while (point>0)
  {
  point--;
   //if (Items[point+1].Id_group==Items[point].Id_group) 
  if (id==Items[point].Id_group)
    if (men_GET_CONFIG(point)) return point;else;  
    //else return NIL;
  }
 return NIL;
}

//----------------------------------------------------------------//

//----------------------------------------------------------------//
unsigned men_RETURN_SUB_ITEM(unsigned pp)   //������� ����� 1-�� �������� � ������� � �������� Id_goup ����� Id_gr
{
 unsigned char Id_gr;
 Id_gr=Items[pp].Data_reg;
 while (pp++<MAX_ITEMS)
   if (Items[pp].Id_group==Id_gr) 
     if (men_GET_CONFIG(pp)) return pp;
 
 return NIL;
}


//----------------------------------------------------------------//
// ������� �������
//----------------------------------------------------------------//
void men_CALL_CUB_ITEM()                          
{
 unsigned short pp;
 
	
 pp = men_RETURN_SUB_ITEM(men_POINTER);
 
 if (pp==NIL) return ;
 
 if (men_LEVEL==men_MAX_LEVEL) return;
	
 men_LEVEL_POINT[men_LEVEL] = men_POINTER;                    //��������� ����� ��������
 
 men_LEVEL++;
 men_POINTER = pp;
 men_CURSOR_STR = 0;
	
 
	
 men_SHOW_MENU();
 
     
}

//----------------------------------------------------------------//
// ��������� � ���������� ����
//----------------------------------------------------------------//

void men_CALLBACK()                               
{
	
	if (!men_LEVEL) return;
	//������� �������� ������������� ����  � ������� ������������//

	men_LEVEL--;
	if (Items[men_POINTER].Data_reg == 0xFE) men_POINTER = 27;	
	//	else if (Items[men_POINTER].Data_reg == 0xF0) men_POINTER = 0x28;
	else if (Items[men_POINTER].Data_reg == 0xF0) men_POINTER = 0x26;	
	else if (Items[men_POINTER].Data_reg == 0xF2) men_POINTER = 0x26;	
	else if (Items[men_POINTER].Data_reg == 0xFB) men_POINTER = 40;
	else if (Items[men_POINTER].Data_reg == 0xFC) men_POINTER = 40;
	else if (Items[men_POINTER].Data_reg == 0xFF) men_POINTER = 0;	
	else if (Items[men_POINTER].Data_reg == 0x11 || Items[men_POINTER].Data_reg == 0x14) men_POINTER = 0x0001;	///���������
	else if (Items[men_POINTER].Data_reg == 0x12 || Items[men_POINTER].Data_reg == 0x15) men_POINTER = 0x0001;	///��������
	else if (Items[men_POINTER].Data_reg == 0x13 || Items[men_POINTER].Data_reg == 0x16) men_POINTER = 0x0001;	///�����������
	else men_POINTER = 0;//men_LEVEL_POINT[men_LEVEL];
 
	men_CURSOR_STR = 0;//men_LEVEL_CUR[men_LEVEL];
	men_SHOW_MENU();
 
}

//----------------------------------------------------------------//
// ���������� �������� � ��������� ���� ���� 
//----------------------------------------------------------------//
char* men_SHOW_ITEM_VALUE(unsigned item_num)
{
 char Options;
 

 if (men_STATUS==men_EDIT_ITEM) Options = O_SZ;
 else Options = Items[item_num].Options;

 if (Items[item_num].Typedata<4) 
  return typ_FORMAT_STR(typ_SHOW_VALUE_0(&typVALUE_PARAM,Items[item_num].Typedata,(TBit)(Options&O_SZ)),(TBit)(Options&O_LEFT),6);
 
 
 if (Items[item_num].Typedata==4) 
  return typ_FORMAT_STR(typ_SHOW_ZNAK(&typVALUE_PARAM,0,(TBit)(Options&O_SZ)),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==5) 
  return typ_FORMAT_STR(typ_SHOW_ZNAK(&typVALUE_PARAM,1,(TBit)(Options&O_SZ)),(TBit)(Options&O_LEFT),6);


 if (Items[item_num].Typedata==6) //������
  return typ_FORMAT_STR(typ_SHOW_PAROL(&typVALUE_PARAM,men_LEVEL_ACCES),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==7)//���� ������
  if (men_STATUS==men_EDIT_ITEM) return typ_FORMAT_STR(typ_SHOW_PAROL(&typVALUE_PARAM,1),(TBit)(Options&O_LEFT),6);
  else 
   {
    strcpy(typVALUE_PARAM.valstr,"----");
    return typ_FORMAT_STR(typVALUE_PARAM.valstr,(TBit)(Options&O_LEFT),6);
   }


 if (Items[item_num].Typedata==10)//�������
   {
    if (men_STATUS==men_EDIT_ITEM) strcpy(typVALUE_PARAM.valstr," ��/���?");
    else strcpy(typVALUE_PARAM.valstr,"");
    
	return typ_FORMAT_STR(typVALUE_PARAM.valstr,(TBit)0,6);
   }
  
 if (Items[item_num].Typedata==16)
  return typ_FORMAT_STR(typ_SHOW_COUNTTIME_VALUE(&typVALUE_PARAM,'�'),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==17)
  return typ_FORMAT_STR(typ_SHOW_COUNTTIME_VALUE(&typVALUE_PARAM,'�'),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==18)
  return typ_FORMAT_STR(typ_SHOW_TIME_VALUE(&typVALUE_PARAM),(TBit)(Options&O_LEFT),6);
   
 if (Items[item_num].Typedata==19)
  return typ_FORMAT_STR(typ_SHOW_DATA_VALUE(&typVALUE_PARAM,(TBit)0),(TBit)(Options&O_LEFT),6);
 
 if ((Items[item_num].Typedata>24)&&(Items[item_num].Typedata<128))
  return typ_FORMAT_STR(typ_SHOW_ENUM_VALUE(&typVALUE_PARAM,Items[item_num].Typedata),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata>=128)
  if (typVALUE_PARAM.val & ((unsigned long)1<<((Items[item_num].Typedata-128)&0x0F)))
   return men_SELECT_STRING;
  else
   return men_NO_SELECT_STRING;
  
 return "err_type";
}


//----------------------------------------------------------------//
// ������ ������� (���������)
//----------------------------------------------------------------//

void men_SHOW_ARROW(int way)
{
	vga_SET_DRAW_MODE(drMODE_NORMAL);
	
	///0 - ����, 1 - �����
	if (way == 0)
	{			
		vga_LINE(vga_GET_WIDTH_DISPLAY-8,4,vga_GET_WIDTH_DISPLAY-3,4);
		vga_LINE(vga_GET_WIDTH_DISPLAY-7,5,vga_GET_WIDTH_DISPLAY-4,5);
		vga_LINE(vga_GET_WIDTH_DISPLAY-6,6,vga_GET_WIDTH_DISPLAY-5,6);
	}
	else
	{
		vga_LINE(vga_GET_WIDTH_DISPLAY-8,2,vga_GET_WIDTH_DISPLAY-3,2);
		vga_LINE(vga_GET_WIDTH_DISPLAY-7,1,vga_GET_WIDTH_DISPLAY-4,1);
		vga_LINE(vga_GET_WIDTH_DISPLAY-6,0,vga_GET_WIDTH_DISPLAY-5,0);
	}
	
	//vga_LINE(120,63,128,63);
	
	//vga_UPDATE();
}



//----------------------------------------------------------------//
// ���������� ���� ������� ����
//----------------------------------------------------------------//
void men_SHOW_ITEM(unsigned short item_num,unsigned char str_num)
{
	unsigned char digit;
	char temp[20];
	

	vga_SET_POS_TEXT(men_X0, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);


	//Alex
  if (Items[item_num].Data_reg == 0xFF) 
  {
		sprintf(temp,"Signal %u  %u",road_pos,road_pos);
    vga_PRINT_TEXT(temp,21,men_FONT_DEFAULT);
  }
	else
	 
	if (Items[item_num].Typedata>0xFD) 
  {   
		vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
  }
 else
 if (Items[item_num].Typedata>=0x80) 
 {
   //������� �������� ���������/�������
   if ((Items[item_num].Options&O_SZ))
   	{
			vga_SET_POS_TEXT(men_X0 + men_WIDTH_SYMBOL*2-5, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
			vga_PRINT_TEXT(Items[item_num].Text_0,20,men_FONT_DEFAULT);
		}
   else vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
 }
 else
 if (Items[item_num].Typedata==10)
 {
   	 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
 } 	
	
 if (Items[item_num].Typedata==11)
 {
   	 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
 } 	
 else  	
 {
		if (Items[item_num].Typedata == 26 || Items[item_num].Typedata == 27) vga_PRINT_TEXT(Items[item_num].Caption,14,men_FONT_DEFAULT);
		else
		//������� ������� �������� ���������
		vga_PRINT_TEXT(Items[item_num].Caption,12,men_FONT_DEFAULT); //���� 9
		
		//������� ����������� ������  
		if ((Items[item_num].Options&O_RW)==0) vga_PRINT_CHAR(men_SEPARATOR_1,men_FONT_DEFAULT);
		else 
    if (men_LEVEL_ACCES<Items[item_num].LevelAcces) vga_PRINT_CHAR(men_SEPARATOR_3,men_FONT_DEFAULT);//������ ������
    else 
		if (Items[item_num].Typedata!=10) 
	 								  //�������
									   vga_PRINT_CHAR(men_SEPARATOR_2,men_FONT_DEFAULT); //�������
    else 							   vga_PRINT_CHAR(men_SEPARATOR_4,men_FONT_DEFAULT);//�������


	 
		if (Items[item_num].Typedata == 26 || Items[item_num].Typedata == 27) 
		{
			sprintf(temp,""); 
			vga_PRINT_TEXT(temp,0,men_FONT_DEFAULT);	/// ������������ � ���� "������" 		
		}
		else 
		{
			sprintf(temp,"     "); 
			vga_PRINT_TEXT(temp,2,men_FONT_DEFAULT);	/// ������������ � ���� "������" 		//5
		}
		
		
		vga_PRINT_TEXT(men_SHOW_ITEM_VALUE(item_num),0,men_FONT_DEFAULT);
		if (Items[item_num].MeasureText[0]!=0) 
		{
				vga_SET_POS_TEXT(men_X0+17*men_WIDTH_SYMBOL,men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
   			vga_PRINT_TEXT(Items[item_num].MeasureText,4,men_FONT_DEFAULT);
		} 
		//���� ����� �������������� �� ������� ������
		if (men_STATUS == men_EDIT_ITEM) 
		{
			digit = typ_DIGIT;
			//������� ������ ��������������
			if ((Items[item_num].Typedata<=3)&&(Items[item_num].Typedata>0))
			if (typ_DIGIT>=Items[item_num].Typedata) digit = typ_DIGIT+1;
										   

			if ((Items[item_num].Typedata==18)||(Items[item_num].Typedata==19))
			if (typ_DIGIT>0) digit = typ_DIGIT+2;
										   
										  

			vga_SET_POS_TEXT(men_X0+(20-digit)*men_WIDTH_SYMBOL, men_Y0 + men_OFFSET + (unsigned short)(str_num+1) * men_HEIGHT_STR);
			vga_PRINT_CHAR(men_CUR_SYMBOL,men_FONT_DEFAULT);

		}
  }
 
	
	
}

//----------------------------------------------------------------//
// �������� ������
//----------------------------------------------------------------//
void men_CLEAR_ITEM(unsigned char str_num)
{
 unsigned char w;
 //lcd_SET_POS(str_num*lcd_COUNT_COL+1);
 
 vga_SET_POS_TEXT(men_X0, men_Y0 + (unsigned short)str_num * men_HEIGHT_STR);

 w = vga_GET_WIDTH_DISPLAY/men_WIDTH_SYMBOL + 1;

 vga_FILL_STR(' ',w,men_FONT_DEFAULT);
 //lcd_PRINT_LINE("",19);
}




//----------------------------------------------------------------//
//����������� ������� ����
//----------------------------------------------------------------//
void men_INC_POINT()
{
 unsigned short pp = 0;
	uint8_t i = 0;
FILE* pRFile;
	char temp[10];
	
 pp = men_GET_NEXT_ITEM(men_POINTER);
//if (Items[men_POINTER].Data_reg == 0xFF) road_pos++;	
 if (pp==NIL) return;
//Alex
	if ((Items[men_POINTER].Data_reg == 0xFF)||(Items[men_POINTER].Data_reg == 0xFE))
	{
		if (road_cursor_pos<4) road_cursor_pos++; 
		if (road_pos<255) road_pos++;
		
		if ((pRFile = fopen ("Roads.txt","r")) != NULL)
			{
				fseek(pRFile,9*(road_pos),SEEK_SET);
				if (fscanf(pRFile, "%s", temp) != 1) 
				{
					road_pos--;
					exist = 0;
				}
				else exist = 1;
				fclose(pRFile);
			}
			

//men_CURSOR_STR++;
		men_SHOW_MENU();
		return;
	}
	
	

//*Alex
 men_SHOW_CURSOR();

 men_CURSOR_STR++;
	
 men_POINTER = pp;

 if (men_GET_NEXT_ITEM(men_POINTER)==NIL) 
 if (men_CURSOR_STR>(men_COUNT_STR-1)) 
 {
		men_CURSOR_STR = men_COUNT_STR-1;
		men_SHOW_MENU();
 }
 else
 {
		men_SHOW_CURSOR();
		vga_UPDATE();
 }
 else 
 if (men_CURSOR_STR>(men_COUNT_STR-1)) 
 {
		men_CURSOR_STR = men_COUNT_STR-1;
    men_SHOW_MENU(); 
 }
 else 								 
 {
		men_SHOW_CURSOR();
		vga_UPDATE();
 }
 
 
 
 
}

//----------------------------------------------------------------//
//����������� ������� �����
//----------------------------------------------------------------//
void men_DEC_POINT()
{
 unsigned short pp = 0;

//if (Items[men_POINTER].Data_reg == 0xFF) if (road_pos > 0) road_pos--;	
 pp = men_GET_PREV_ITEM(men_POINTER);

 if (pp==NIL) return;
	
	//Alex
	if ((Items[men_POINTER].Data_reg == 0xFF)||(Items[men_POINTER].Data_reg == 0xFE)) 
	{
		//if (road_cursor_pos>0) road_cursor_pos--; 
		if (road_pos>0)road_pos--;
		if ((road_pos<road_cursor_pos)&&(road_cursor_pos>0)) road_cursor_pos--; 

		if (road_pos>5) exist = 1;
		else
		{
			if (Num_of_Signals > 5) exist = 1;
		}
		
		men_SHOW_MENU();
		return;	
	}
	
	if ((Items[men_POINTER].Data_reg == 0xFB)||(Items[men_POINTER].Data_reg == 0xFC))
	{
		if (sample_pos>0) sample_pos--;
		if ((sample_pos<sample_cursor_pos)&&(sample_cursor_pos>0)) sample_cursor_pos--; 

		men_SHOW_MENU();
		return;			
	}
	//*Alex	
	
 men_SHOW_CURSOR();

 men_CURSOR_STR--;
//if (road_cursor_pos>0) road_cursor_pos--;		//Alex
 men_POINTER = pp;

 if (men_CURSOR_STR<0)
 {
		men_CURSOR_STR = 0;
    men_SHOW_MENU();								 
 }
 else
 {
		men_SHOW_CURSOR();	 
		vga_UPDATE();		
 }	 
 
  
}


//----------------------------------------------------------------//
// ������ ������
//----------------------------------------------------------------//

void men_SHOW_CURSOR(void)
{
 unsigned short h;

 h = men_Y0 + men_OFFSET + (unsigned short)men_CURSOR_STR * men_HEIGHT_STR;
 //if (h>0) h-=1;

 vga_SET_DRAW_MODE(drMODE_XOR);

 vga_RECTANGLE(men_X0-1,h-men_OFFSET,vga_GET_WIDTH_DISPLAY-1,h+men_HEIGHT_SYMBOL+men_OFFSET-1,drRECT_FILL);

 vga_SET_DRAW_MODE(drMODE_NORMAL);
	

}


//----------------------------------------------------------------//

//----------------------------------------------------------------//
void men_SHOW_ONEITEM(unsigned item_num)
{
 vga_CLEAR();
 if ((Items[item_num].Options&O_RW)>0)
 	men_SHOW_RECT("��������������");
 else
    men_SHOW_RECT("��������");
  

 vga_SET_POS_TEXT(men_X0,men_Y0 + men_OFFSET + 0 * men_HEIGHT_STR);
 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
 vga_SET_POS_TEXT(men_X0,men_Y0 + men_OFFSET + 1 * men_HEIGHT_STR);
 vga_PRINT_TEXT(Items[item_num].Text_1,21,men_FONT_DEFAULT);


 if ((Items[item_num].Options&O_RW)>0)
  if (Items[item_num].LevelAcces>men_LEVEL_ACCES)	return;
   
  
 
 //lcd_SET_POS(60);
 //lcd_SEND_CHAR(0x01);


  men_SHOW_ITEM(item_num,3);
  if (Items[item_num].Typedata!=10) 
   if (men_STATUS==men_EDIT_ITEM)
    {
     //lcd_SET_POS(70+5-mem_RETURN_CURSOR_POS(Items[item_num].Typedata));
     //lcd_SET_BLINC_CURSOR;
    }
  

}
//----------------------------------------------------------------//
// ���������� ����
//----------------------------------------------------------------//
unsigned int men_SHOW_ITEMS(void)                   //������� ����
{
 
 signed char 	sn;
 signed char 	ss;
 unsigned short pp;
 
		
 sn = men_s;
 ss = men_s;
 pp = men_START_POINTER;
	
 count_menu_items = 0;	

 while (sn>0) men_CLEAR_ITEM(--sn);

 while (ss<men_COUNT_STR)              
 if (pp!=NIL)  //��������� ��������//
 {    
    men_READ_VALUE_PARAM(pp);
    men_SHOW_ITEM(pp,ss);
    ss++;
		pp = men_GET_NEXT_ITEM(pp);	 	
		//count_menu_items++;	 
 }
 else
 {
	 men_CLEAR_ITEM(ss++);	 
 }
 
 ///������� ���-�� ��������� � ����
 ss = men_s;
 pp = men_START_POINTER;
 while (ss<MAX_ITEMS)              
 if (pp!=NIL)  
 {    
    //men_READ_VALUE_PARAM(pp);
    //men_SHOW_ITEM(pp,ss);
    ss++;
		pp = men_GET_NEXT_ITEM(pp);	 	
		count_menu_items++;	 
 }
 else
 {
	 //men_CLEAR_ITEM(ss++);	 
	 ss++;
 } 
 
	
 
}

//----------------------------------------------------------------//
// ���������� ����� ����
//----------------------------------------------------------------//
void men_SHOW_RECT(const char *text)
{
 //const char *s  = 0;
 char len = 0;
 //vga_RECTANGLE(0,0,vga_GET_WIDTH_DISPLAY-1,63,drRECT_NO_FILL);
 //vga_LINE(0,9,vga_GET_WIDTH_DISPLAY-1,9);	 men_FONT_DEFAULT &FONT_7x10_bold
 

 vga_SET_POS_TEXT(0,1);
 vga_PRINT_STR(text,men_FONT_DEFAULT);

 vga_SET_DRAW_MODE(drMODE_XOR);

 len = strlen(text)*men_WIDTH_SYMBOL;
 vga_RECTANGLE(0,0,len,8,drRECT_FILL);
 vga_LINE(0,9,vga_GET_WIDTH_DISPLAY-1,9);

 vga_SET_DRAW_MODE(drMODE_NORMAL);
 //vga_LINE(len+1,0,len+1+9,9);
 vga_LINE(len+1,1,len+1+8,9);
 vga_LINE(len+1,2,len+1+7,9);
 vga_LINE(len+1,3,len+1+6,9);
 vga_LINE(len+1,4,len+1+5,9);
 vga_LINE(len+1,5,len+1+4,9);
 vga_LINE(len+1,6,len+1+3,9);
 vga_LINE(len+1,7,len+1+2,9);
 vga_LINE(len+1,8,len+1+1,9);

 vga_SET_POS_TEXT(0,vga_GET_HEIGHT_DISPLAY-8);
 //vga_PRINT_STR("[\36|\37]",men_FONT_DEFAULT);
 vga_LINE(0,vga_GET_HEIGHT_DISPLAY-2,vga_GET_WIDTH_DISPLAY-1,vga_GET_HEIGHT_DISPLAY-2);
 ///vga_LINE(0,vga_GET_HEIGHT_DISPLAY-10,vga_GET_WIDTH_DISPLAY-1,vga_GET_HEIGHT_DISPLAY-10);
 
}


//----------------------------------------------------------------//
// ���������� ����
//----------------------------------------------------------------//
void men_SHOW_MENU(void)                    
{
	char temp[25];
	char temp2[9];
	char temp3[3];
	char temp4[25];
	char temp5[10];
	char procent[10];
	char temppath[25];
	char err[25];
	char 		  FileName[25];
	FILE  		*pRFile = 0;
	FILE  		*file = 0;
	
	FIL fil;  
	TCHAR *fn_ptr;
	char* fn;
	FRESULT result;	
	DIR dir;
	uint16_t a =0;
	uint16_t b =0;
	uint16_t c =0;
	uint16_t d =0;
	volatile unsigned int res;
	uint8_t ip = 0;
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t i2 = 0;
	uint16_t i3 = 0;
	uint16_t compare_status = 0;
	uint8_t NumberOfFiles = 8;
	uint16_t AddPos = 0;
  FATFS f;
	unsigned int count = 0;	
	unsigned int total_road = 0;	
	
	
	men_STATUS 	   = men_MULTI_ITEM;
  men_s			   = men_CURSOR_STR;


//Alex
	
	if ((Items[men_POINTER].Data_reg == 0xFF)||(Items[men_POINTER].Data_reg == 0xFE)) ///����� ��������
	{		
		
		vga_CLEAR();
		
		men_SHOW_RECT(Items[men_LEVEL_POINT[men_LEVEL-1]].Text_0);
		
		if (road_pos>4) AddPos = road_pos-4;
		else AddPos = 0;
		
		ip = 0;
		NumberOfFiles = 0;
		
		while (ip<5)
		{
			vga_SET_POS_TEXT(men_X0, men_Y0 + men_OFFSET + (unsigned short)ip * men_HEIGHT_STR);			
			
			pRFile = fopen ("Roads.txt","r");
						
			sprintf(err, "%s", strerror(errno));
			
			if (pRFile != NULL)
			{
				fseek(pRFile,9*(ip+AddPos),SEEK_SET);
				fscanf(pRFile, "%s", temp);
				fclose(pRFile);
				if ((pRFile = fopen (temp,"r")) != NULL) 
				{
					memset(temp,0,20);
					fread(temp,1,15,pRFile);
					fclose(pRFile);
					vga_PRINT_TEXT(temp,20,men_FONT_DEFAULT);
					NumberOfFiles++;
					
				}				
			}
			else 
			{
				memset(temp,0,20);
				sprintf(temp,"Road.%03u",ip+AddPos);
				pRFile = fopen (temp,"r");
				if (pRFile != NULL) 
				{
					fread(temp,1,15,pRFile);
					fclose(pRFile);
					vga_PRINT_TEXT(temp,20,men_FONT_DEFAULT);
					//	road_cursor_pos--;
					NumberOfFiles++;						
				}
				
			}
			
		ip++;
			
		}

		if (road_cursor_pos >= NumberOfFiles) 
		{
			road_cursor_pos--;		
		}
		
		
		men_CURSOR_STR = road_cursor_pos;
		
		men_SHOW_CURSOR();		
				
		//count = AddPos + road_pos;
		
		///������� ����������
		if (Num_of_Signals > 5 && exist == 1) men_SHOW_ARROW(0); ///���� 		
		if (road_pos >= 5) men_SHOW_ARROW(1); ///����� 
		
		vga_UPDATE();		
		
		return;
	}
	
	

	//*Alex
	
 //men_START_POINTER = men_POINTER;
 while (!men_GET_CONFIG(men_POINTER))
 {		
		if ((men_POINTER = men_GET_NEXT_ITEM(men_POINTER))==NIL) break;		
 }
  
 men_START_POINTER = men_POINTER;
  
 while (men_s>0) ///��������� s,p
 {
		men_START_POINTER = men_GET_PREV_ITEM(men_START_POINTER);
		
		if (men_START_POINTER==NIL) 
		{ 
			men_START_POINTER = men_POINTER;
			break;
		}
		
		men_s--;
 }

 //lcd_SET_NON_CURSOR;//cursor(0,0);
 vga_CLEAR();
 
 if (men_LEVEL==0)
 {
	 men_SHOW_RECT("����");
 } 
 else
 {
	 men_SHOW_RECT(Items[men_LEVEL_POINT[men_LEVEL-1]].Text_0);
 }
 
 men_SHOW_ITEMS();//show_element();//(s,p);
 
 men_SHOW_CURSOR();
  
 ///������ ������� ���������� 
 if (men_LEVEL > 0 && count_menu_items > 5 ) men_SHOW_ARROW(0); ///���� 
 if (men_LEVEL > 0 && men_CURSOR_STR == 4) men_SHOW_ARROW(1); ///����� 
 if (men_LEVEL > 0 && men_START_POINTER != 0x001B 
									 && men_START_POINTER != 0x0001 
									 && men_START_POINTER != 0x0006 
									 && men_START_POINTER != 0x000A
									 && men_START_POINTER != 0x0014
									 && men_START_POINTER != 0x000E
									 && men_START_POINTER != 0x0018  
									 && men_START_POINTER != 0x002D 
									 && men_START_POINTER != 0x0010) men_SHOW_ARROW(1); ///����� � �������� �����������
  
 vga_UPDATE();
}


//----------------------------------------------------------------//
// ������� ��������� �������
//----------------------------------------------------------------//
u8 men_MENU_CONTROL(void)
{
u8 key_event = 0;
 if (men_CHECK_EV(men_EVENT_REF))
  {
   //�������� �������� ���������� � ������ ������
   //if (men_STATUS==men_MULTI_ITEM) men_SHOW_ITEMS(); 
   //�������� ��������� ��������� � ��������� ������
   /*if (men_STATUS==men_ONE_ITEM)  {
    							   men_READ_VALUE_PARAM(men_POINTER);
								   men_SHOW_ONEITEM(men_POINTER);
								   //mem_SHOW_ITEM(mem_POINTER,3);
								  }*/
   //�������� ������� �����
   if (men_STATUS==men_MAIN)       
   									men_SHOW_REFRESH();//mem_SHOW_MAINFORMS(mem_ID_FORM);

  }

 /*if (men_CHECK_EV(men_EVENT_TIME_MESAGE))	//������� ����� ����
  {
   men_STATUS = men_SAVE_STATUS;
   //�������� ����
   //men_UPDATE(); 
  }*/
 
 //mem_CONTROL_CONFIG();

 //��������� ������� ����������
 if (!key_CHECK_ST(key_MESUARE))
  {
   /*if (key_CHECK_EV(key_EVENT_PRESSED_ENTER))	 	{SET_CLOCK_SPEED(CLK_72MHz);men_EN_MENU();SET_CLOCK_SPEED(CLK_8MHz);} 
   if (key_CHECK_EV(key_EVENT_PRESSED_UP))	 	    {SET_CLOCK_SPEED(CLK_72MHz);men_UP_MENU();SET_CLOCK_SPEED(CLK_8MHz);} 
   if (key_CHECK_EV(key_EVENT_PRESSED_DOWN))	 	{SET_CLOCK_SPEED(CLK_72MHz);men_DW_MENU();SET_CLOCK_SPEED(CLK_8MHz);} 
   if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU))	{SET_CLOCK_SPEED(CLK_72MHz);men_ES_MENU();SET_CLOCK_SPEED(CLK_8MHz);} */
		
		
   if (key_CHECK_EV(key_EVENT_PRESSED_ENTER))	 	{key_event = 1; measure_stat = 0; men_EN_MENU();}
   if (key_CHECK_EV(key_EVENT_PRESSED_UP))	 	    {key_event = 1;measure_stat = 0;men_UP_MENU();}
   if (key_CHECK_EV(key_EVENT_PRESSED_DOWN))	 	{key_event = 1;measure_stat = 0;men_DW_MENU();}
   if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU))	{key_event = 1;measure_stat = 0;men_ES_MENU();}
  }			
//if (men_STATUS!=men_MAIN)&&key_event ==   measure_stat = 0; 	
return  key_event;  
}

//----------------------------------------------------------------//
// ������� ��������� ������� ������� �����
//----------------------------------------------------------------//
void men_UP_MENU(void)
{
 unsigned int temp_reg;
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL; 
 BeyondRoad = 0;
 switch (men_STATUS)
   {
    case men_MAIN:      
//						if (Road_Number != 0)
						if (REG(MODE_REG)==0x0800 && SD_SWITCH == 1)
						{	
								temp_reg = REG(NUMFILE_CURENT);
								if (temp_reg<REG_MAX(NUMFILE_CURENT)) 
									{
										temp_reg++;
										REGW(NUMFILE_CURENT,temp_reg);
										rod_GET_NameElement(&NEl,temp_reg);
										//Alex
										if (NEl.Number == 0) 
										{
											temp_reg--;
											REGW(NUMFILE_CURENT,temp_reg);
											rod_GET_NameElement(&NEl,temp_reg);
										}
										//*Alex	 
										men_SHOW_REFRESH();
									}; 
							}
						
							
							
							
						 break;
    case men_MULTI_ITEM: 
					     men_DEC_POINT();
					     break;
						 
	case men_EDIT_ITEM:  
	 					 typ_INC_VALUR(&typVALUE_PARAM,Items[men_POINTER].Typedata);
						 men_SHOW_ITEM(men_POINTER,3);
						 //men_SHOW_CURSOR_EDIT();
						 //lcd_SET_POS(70+5-mem_RETURN_CURSOR_POS(Items[mem_POINTER].Typedata));
						 //cd_SET_BLINC_CURSOR;
						 vga_UPDATE();
						 break;
						

	default:return;
   }
   
}

//----------------------------------------------------------------//
// ������� ��������� ������� ������� ����
//----------------------------------------------------------------//
void men_DW_MENU(void)
{
 unsigned int temp_reg;
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL;
BeyondRoad = 0;
 switch (men_STATUS)
   {
    case men_MAIN:       
//			if (Road_Number != 0)
			if (REG(MODE_REG)==0x0800 && SD_SWITCH == 1)
			{
								temp_reg = REG(NUMFILE_CURENT);
								if (temp_reg>REG_MIN(NUMFILE_CURENT)) {
									temp_reg--;
									REGW(NUMFILE_CURENT,temp_reg);
									rod_GET_NameElement(&NEl,temp_reg);
									//Alex
									if ((NEl.Number == 0) || (temp_reg == 1))
									{
										temp_reg++;
										REGW(NUMFILE_CURENT,temp_reg);
										rod_GET_NameElement(&NEl,temp_reg);
									}
									//*Alex
									men_SHOW_REFRESH();
								}; 
			}
						 break;
    case men_MULTI_ITEM: 
   	 				     men_INC_POINT();
					     break;

    case men_EDIT_ITEM:  
	 					 typ_DEC_VALUR(&typVALUE_PARAM,Items[men_POINTER].Typedata);
						 men_SHOW_ITEM(men_POINTER,3);
						 //men_SHOW_CURSOR_EDIT();
						 //lcd_SET_POS(70+5-mem_RETURN_CURSOR_POS(Items[mem_POINTER].Typedata));
						 //cd_SET_BLINC_CURSOR;
						 vga_UPDATE();
						 break;
						 

    default:return;
   }   
  
}

//----------------------------------------------------------------//
// ������� "���������� �������"
//----------------------------------------------------------------//

void SERVICE_ACC2( )
{

		
		u16 status=0;
		bool flag1=0;		
		bool flag2=0;		
		int flag3=0;	
		int bat=0;
		char * str;		
		int currentmonth =0;
		int currentyear =0;
		int month=0;
		int year=0;
	
		unsigned int hour = 0;			
		unsigned int min = 0;
		unsigned int sec = 0;
	
		int starthour = 0;
		int startmin = 0;
		int startsec = 0;
	
		int currenthour = 0;
		int currentmin = 0;
		int currentsec = 0;
		
		int revhour = 0;
		int revmin = 0;
		int revsec = 0;
		
		char h_out[5];
		char m_out[5];
		char s_out[5];
		char str_out[5];
		char t_str [15];
		
		char bat_out[5];
		char* str2;
	
		int h,m,s = 0;
		
		int old_pa8 = 0;
		int old_usb = 0;
		
		int prev_charge_status = -1;
		
		
		 
//		PWR_BackupAccessCmd(ENABLE);
//		BKP_ClearITPendingBit();
//		BKP_ClearFlag();
//	
//		RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
//		PWR->CR |= PWR_CR_DBP;	
//		BKP->CSR &= ~((1<<9)||(1<<8));
		
//		currentmonth = rtc_READ(rtc_MON);
//		currentyear = rtc_READ(rtc_YEAR);
	
		if (id_akb == 0) //����� ���	
		{
					
			
					while(flag1==0)
					{				
						
										IWDG_ReloadCounter();
						
										vga_CLEAR();					
										vga_SET_POS_TEXT(5,20);
										vga_PRINT_STR("����������", &FONT_6x8);
										vga_SET_POS_TEXT(5,30);
										vga_PRINT_STR("����������",&FONT_6x8);
										vga_SET_POS_TEXT(5,40);
										vga_PRINT_STR("�������?",&FONT_6x8);					
										vga_UPDATE();					
				
											
										if (key_CHECK_EV(key_EVENT_PRESSED_ENTER)) 
										{							
															
											flag1=1; flag2=1; 		
									
										}
									
										if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU)) 
										{
										
											flag1=1; flag2=0; 
											men_SHOW_MENU();
											
										}									
						}		
			
			
						if (flag2==1)
						{													
							
							while(flag2==1)
							{	
								IWDG_ReloadCounter();			
								
								//CAPACITY();
								
								CHARGE_ON();
								
								if ( (USB_CONNECT != pin_USB_5V) )
								{
																				
											vga_CLEAR();
											vga_SET_POS_TEXT(5,30);
											vga_PRINT_STR("�����...",&FONT_6x8);																								
											vga_UPDATE();			

//											//������� �������
//											vga_SET_POS_TEXT(5,55);						
//											sprintf(t_str,"%0.5f", (float) akbemk_count);						
//											vga_PRINT_STR(t_str,&FONT_4x7);
//							
//											//���������� �������
//											vga_SET_POS_TEXT(45,55);						
//											sprintf(t_str,"%0.5f", (float) akbtemp);						
//											vga_PRINT_STR(t_str,&FONT_4x7);
//												
//																	
//											//PA8
//											vga_SET_POS_TEXT(85,55);				
//											sprintf(t_str,"%d", GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8));
//											vga_PRINT_STR(t_str,&FONT_4x7);
//									
//											
//											//��������
//											vga_SET_POS_TEXT(90,55);				
//											sprintf(t_str,"%d", (int) ceil(akbemk_count * 100 / 0.6));
//											vga_PRINT_STR(t_str,&FONT_4x7);
//											vga_UPDATE();										
									
											
									
											CHARGE_ON();
																						
									
											if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )
											{
												LED_CHARGE_ON();												
												
												usb_charge_state = 1;
											}
											

											if ( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 1 & old_pa8 == 0 & pin_USB_5V & old_usb )
											{	
													BKP_WriteBackupRegister(BKP_DR10, (int) ceil(akbemk_menu * 100000)); 
													akbemk_count = 0.6;
													akbemk_percent = 100;	

													usb_charge_state = 0;	
												
													LED_CHARGE_OFF();
												
													flag2 = 0; flag3 = 1;		
											}
											
											//���������� ��������� usb � pa8
											old_usb = pin_USB_5V;
											old_pa8 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);

								}
								else
								{
															
											vga_CLEAR();
											vga_SET_POS_TEXT(5,20);
											vga_PRINT_STR("����������",&FONT_6x8);																								
											vga_SET_POS_TEXT(5,30);
											vga_PRINT_STR("��������",&FONT_6x8);																								
											vga_SET_POS_TEXT(5,40);
											vga_PRINT_STR("����������...",&FONT_6x8);																								
											vga_UPDATE();			
									
											LED_CHARGE_OFF();									

								}	
							}									
	
						}
						
						
						if (flag3==1)	
						{			
							
							IWDG_ReloadCounter();
													
							
							vga_CLEAR();					
							vga_SET_POS_TEXT(5,30);
							vga_PRINT_STR("������� ����������", &FONT_6x8);							
							vga_SET_POS_TEXT(5,40);
							vga_PRINT_STR("������� ��������.",&FONT_6x8);									
							vga_UPDATE();												
												
								
							while (USB_CONNECT != pin_USB_5V);
								
							men_SHOW_MENU();									
							
						}					
						
						
			

		}
		else
		{
	
			CONTROL_RTC(); //����� �����
		
		
			starthour = REG(TIME)/100;
			startmin = REG(TIME)%100; 
			//startmin = 0;
			startsec = REG(SECOND);
			
		
					
					while(flag1==0)
					{				
										IWDG_ReloadCounter();
						
										vga_CLEAR();					
										vga_SET_POS_TEXT(5,20);
										vga_PRINT_STR("����������", &FONT_6x8);
										vga_SET_POS_TEXT(5,30);
										vga_PRINT_STR("����������",&FONT_6x8);
										vga_SET_POS_TEXT(5,40);
										vga_PRINT_STR("�������?",&FONT_6x8);					
										vga_UPDATE();					
				
											
										if (key_CHECK_EV(key_EVENT_PRESSED_ENTER)) 
										{							
															
											flag1=1; flag2=1; 		
									
										}
									
										if (key_CHECK_EV(key_EVENT_PRESSED_ESC_MENU)) 
										{
										
											flag1=1; flag2=0; 
											men_SHOW_MENU();
											
										}									
						}		
											
										
						if (flag2==1)
						{													
							CHARGE_OFF(); 	// ��������� �����							
							DISCHARGE_ON();	// �������� ������
							
							while(flag2==1)
							{	
								IWDG_ReloadCounter();			
								
								if (USB_CONNECT == pin_USB_5V)
								{
									vga_CLEAR();
									vga_SET_POS_TEXT(5,20);
									vga_PRINT_STR("����������",&FONT_6x8);																								
									vga_SET_POS_TEXT(5,30);
									vga_PRINT_STR("��������",&FONT_6x8);																								
									vga_SET_POS_TEXT(5,40);
									vga_PRINT_STR("����������...",&FONT_6x8);																								
									vga_UPDATE();				
								}
								else
								{
								
								
											vga_CLEAR();
											vga_SET_POS_TEXT(5,30);
											vga_PRINT_STR("������...",&FONT_6x8);																								
											vga_UPDATE();				
											
											if (adc_BAT_MEASURE_edit() < 3200) 
											{
												flag2=0;
												flag3=1;
											}
									
																
											
											LED_CHARGE_ON();
											Delay(500000);
											LED_CHARGE_OFF();
											Delay(500000);

										

								}	
							}		
							
							DISCHARGE_OFF();	// ��������� ������					
						}
						
						if (flag3==1)
						{
							
							CHARGE_ON(); //�������� ����� 
							vga_CLEAR();
							//pin_SSD_RES(LOW);		
							vga_UPDATE();	
							
							while(flag3==1)
							{																					
											IWDG_ReloadCounter();
								
											vga_CLEAR();
											vga_SET_POS_TEXT(5,30);
											vga_PRINT_STR("������ ��������.",&FONT_6x8);																								
											vga_SET_POS_TEXT(5,40);
											vga_PRINT_STR("������� �����....",&FONT_6x8);	
											vga_UPDATE();			

											if (adc_BAT_MEASURE_edit() > 4000) 	flag3=2;			
								
											LED_CHARGE_ON();										
								
							}
							
//							CHARGE_OFF(); //��������� ����� 	
							
				
						}
						
						
						if (flag3==2)	
						{			
							
							
							while(flag3==2)
							{
								
								IWDG_ReloadCounter();
								
								//������� �����
								//hour = REG(TIME)/100;
								//min = REG(TIME)%100;
								sec = REG(SECOND);
								
								if (sec == 0) { min++; Delay(3000000); };
								if (min == 59) 
									{
										hour++;
										min = 0;
										Delay(3000000);
									}
								

								
								vga_CLEAR();
								vga_SET_POS_TEXT(5,20);
								vga_PRINT_STR("����� ������������..",&FONT_6x8);																								
												
								vga_SET_POS_TEXT(5,35);		 										 
								sprintf(h_out,"�����: %02d:%02d:%02d", 2-hour, 59-min, 59-sec);		 								
								//sprintf(h_out,"�����: %02d:%02d:%02d", revhour, revmin, revsec);		 								
								vga_PRINT_STR(h_out,&FONT_6x8);
							
								vga_SET_POS_TEXT(5,50);		 
								sprintf(bat_out,"����.���: %d (��)", adc_BAT_MEASURE_edit());		 
								vga_PRINT_STR(bat_out,&FONT_6x8);
								
								vga_UPDATE();			

								if ( hour == 3 ) 	flag3=3;	
								
								Delay(1000000);
							}
							
						}
						
						if (flag3==3)	
						{					
							IWDG_ReloadCounter();
						
							CHARGE_OFF(); //��������� ����� 	
							
							//����� � BKP ������� ������� ����
							//BKP_WriteBackupRegister(BKP_DR12, currentmonth);
							//BKP_WriteBackupRegister(BKP_DR6, currentyear);

							vga_CLEAR();					
							vga_SET_POS_TEXT(5,30);
							vga_PRINT_STR("������� ����������", &FONT_6x8);							
							vga_SET_POS_TEXT(5,40);
							vga_PRINT_STR("������� ��������.",&FONT_6x8);			
							
							vga_UPDATE();												
											
							Delay(10000000);							
						
							 
						}					
		}

		
} //��������� SERVICE_ACC



//----------------------------------------------------------------//
// ������� ��������� ������� ������� ����/���
//----------------------------------------------------------------//
void men_ES_MENU(void)
{
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL;
BeyondRoad = 0;
	
	
 switch (men_STATUS)
  {
   case men_MAIN:      
                     
					   	//vga_CLEAR();
					   men_SHOW_MENU();
					   break;
					  
   case men_MESSAGE:	  
					   if (men_TIME_MES!=NIL) men_TIME_MES = 0;
					   break;
					  
   case men_MULTI_ITEM:
                     
					   if (men_LEVEL!=0)
					     men_CALLBACK();
 					   else 
					     {
						  //lcd_CLEAR;
					      men_SHOW_MAINFORMS(form_MESUARE); //������� �����
						 }
					    

					   break;
					  

   case men_ONE_ITEM: 
					   men_SHOW_MENU();
					   break;
  
   case men_EDIT_ITEM:
   					   //lcd_CLEAR;
					   //mem_READ_VALUE_PARAM(mem_POINTER);
					   men_SHOW_MENU();
					   return;
   /*case memVIEWARH:  {
                       lcd_CLEAR;
					   mem_SHOW_MENU();
					   return;
					 }	*/
  
   default:return;
  }  
}


void men_EN_MENU(void)
{
  char 		  FileName[25];
  unsigned int temp_reg; 
	uint16_t A=0, V=0;
	uint16_t Aa=0,Vv=0;	
	uint8_t ip = 0;	
	unsigned int i = 0, j = 0;
	uint8_t NumberOfFiles = 8;
	uint16_t AddPos = 0;
	//float fft_res[128]; 
	FILE  		*pRFile = 0;	
  int k=0;
	int d=0;
	DWORD sector[2];
	uint8_t ffarr1[50000];
	TCHAR* ffarr2;
	char temp[25];
	char temp2[9];
	char temp3[3];
	char temp4[25];
	char temp5[10];
	uint16_t a =0;
	uint16_t b =0;
	uint16_t c =0;	
	volatile unsigned int res;
	char temppath[25];
	
	
	
	
	
 
	switch (men_STATUS)
  {
   case men_MAIN:       
   						if (REG(CHANEL_MESUARE)==0x08) //�������
							{
								if (REG(SAMPLING_STATUS)!=0xFF)
								{						 

								
								temp_reg = REG(NUMFILE_CURENT);
								sprintf(FileName,"Signal %d.dat",NEl.Number);								
				  
								men_SHOW_MESSAGE("����������...","",0);			

									
								
								SET_CLOCK_SPEED(CLK_72MHz); 			
						
								
							  REGW(NUMFILE,0); //������ ���-����
								

								
									
																											
											///���������� ��� ����� �������� �� ������� 
											pRFile = fopen ("Roads.txt","r");
											if (pRFile != NULL)
											{
													fseek(pRFile,9*road_pos,SEEK_SET);
													fscanf(pRFile, "%s", road_pos_edit);
													fclose(pRFile);
											}

											IWDG_ReloadCounter();			
																								
											strncpy(road_pos_edit_2,road_pos_edit+5,8-5); ///�������� ��������� ������ ����� � ��������			
											road_pos_int = atoi(&road_pos_edit_2); /// � int
																											
																											
											if (Road_Number == 0)
											{
													//temp_reg = REG(BEYOND_ROAD);
													//temp_reg++;
													REGW(BEYOND_ROAD,REG(BEYOND_ROAD)+1);
											}
									

											sprintf(FileName,"M:\\%03u.%03u\\Signal %d.dat",road_pos_int,REG(ROUTE_NUM),NEl.Number);											
																			
											sprintf(savefiledir,"0:%03u.%03u",road_pos_int,REG(ROUTE_NUM));		
											sprintf(savefilename,"0:%03u.%03u/Signal %d.dat",road_pos_int,REG(ROUTE_NUM),NEl.Number);
														
											Num_of_Road = Road_Number;				


															
							
				
								/// ��������� ������ �� SD ����� (������� DAT ����)
								
								savefiledirTCHAR = savefiledir;
								savefilenameTCHAR = savefilename;

								IWDG_ReloadCounter();
											
								__disable_fiq();
								__disable_irq();
								
								finit();
								f_mount(&fls, "0:", 1);
								res_t = f_mkdir(savefiledirTCHAR);								
								res_t = f_open(&Fil, savefilenameTCHAR, FA_WRITE | FA_CREATE_ALWAYS);
								res_t = f_write(&Fil,&k_reg,4,&iout);										
								res_t = f_write(&Fil,ext_adc_SIM,25000*2,&iout);		
								f_close(&Fil);
								f_mount(0,"0:", 0);		
								
								__enable_fiq();
								__enable_irq();								
								
								
								if (iout < 50000)	
								{
									vga_CLEAR();					
									vga_SET_POS_TEXT(1,1);
									vga_PRINT_STR("���� ����������", &FONT_6x8);	
									vga_UPDATE();
									
									GLOBAL_ERROR |= 0x10;
								}
								
								if (res_t != 0x00 || iout != 50000)	
								{
									//GLOBAL_ERROR |= 0x10;
								}																
						
								
								/// ��������� ��������� � ��������
								A = calc_from_dat_A(FileName);								
								V = calc_from_dat_V(FileName);


								f_mount(&fls, "0:", 1);
								res_t = f_open(&Fil_2,savefilenameTCHAR, FA_WRITE);  
																
								res_t = f_lseek(&Fil_2, f_size(&Fil_2));
																	
								res_t = f_write(&Fil_2,&A,2,&iout);		
																	
								res_t = f_write(&Fil_2,&V,2,&iout);		
																
								f_close(&Fil_2);			
								f_mount(0,"0:", 0);								
								
								crtflag = 1;

								
					
								//*Alex
							  REGW(NUMFILE,temp_reg); //������� ���-����
							  SET_CLOCK_SPEED(CLK_8MHz);
							   
							  REGWDEF(SAMPLING_STATUS);
							
								if (!SampleAlreadyExist)
								{
								
								}
								
								REGW(NUMFILE,temp_reg); //������� ���-����

							
						  }
							else 
							{
								if(Road_Number) BeyondRoad = !BeyondRoad;
								//men_SHOW_MESSAGE("����","",15000);
								

								
							}
						}
							
						
   						break;

					  
   case men_MULTI_ITEM:

		 
		 if (Items[men_POINTER].Data_reg == 0xFF) ///"��� ��������" � ����
		 {
					
			sample_pos = 0;
			sample_cursor_pos = 0;

			if(road_pos == 0)
			{
				if ((pRFile = fopen ("Roads.txt","r")) != NULL)
				{
					fseek(pRFile,9*(road_pos),SEEK_SET);
					fscanf(pRFile, "%s", FileName);
					fclose(pRFile);
					
					res = f_mount(&fls, "0:", 1);		
					res = f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
					if (res == 0)
					{	
						f_printf(&Fil,"%s", FileName);
						f_close(&Fil);
					}
					res = f_mount(0,"0:", 0);	
					
//					if ((pRFile = fopen ("M:\\Road.log","w")) != NULL)
//					{
//						fprintf(pRFile,"%s",FileName);
//						fclose(pRFile);
//					}
				}
				else
				{
						res = f_mount(&fls, "0:", 1);		
						res = f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
						if (res == 0)
						{	
							f_printf(&Fil,"Road.%03u", road_pos);
							f_close(&Fil);
						}
						res = f_mount(0,"0:", 0);	
						
//					if ((pRFile = fopen ("M:\\Road.log","w")) != NULL)
//					{
//						fprintf(pRFile,"Road.%03u",road_pos);
//						fclose(pRFile);
//					}				
				}
				REGW(NUMFILE_CURENT,0x00);
				REGW(ROUTE_NUM,0);

				men_SETUP();
				men_STATUS=men_MAIN;
				
				road_indicator = 0;
				
				BKP_WriteBackupRegister(BKP_DR12, road_indicator);

				 return;
			}
			else men_CALL_CUB_ITEM();
			return;
		 }
		 
		 
		 
  
		 
		 if(Items[men_POINTER].Data_reg == 0xf0) ///�������� ��������
		 {
			 
			 uint8_t ii = 0; /// ����� �������			 
			
			if ((pRFile = fopen ("Roads.txt","r")) != NULL)
			{
				fseek(pRFile,9*(road_pos),SEEK_SET);
				fscanf(pRFile, "%s", FileName);
				fclose(pRFile);
				
				res = f_mount(&fls, "0:", 1);		
				res = f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
				if (res == 0)
				{	
					f_printf(&Fil,"%s", FileName);
					f_close(&Fil);
				}
				res = f_mount(0,"0:", 0);	
					
//				if ((pRFile = fopen ("M:\\Road.log","w")) != NULL)
//				{						
//					fprintf(pRFile,"%s",FileName);
//					fclose(pRFile);					
//				}
			}

			else
			{
				
				res = f_mount(&fls, "0:", 1);		
				res = f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
				if (res == 0)
				{							
					f_close(&Fil);
				}
				res = f_mount(0,"0:", 0);	
				
				
//				if ((pRFile = fopen ("M:\\Road.log","w")) != NULL)
//				{
//					fclose(pRFile);
//				}				
			}

								
			 REGW(NUMFILE_CURENT,0x00);			 	
			
			 men_SETUP();
			 men_STATUS=men_MAIN;



			///���������� ������� �� ������� 
			pRFile = fopen ("Roads.txt","r");
			if (pRFile != NULL)
			{
					fseek(pRFile,9*road_pos,SEEK_SET);
					fscanf(pRFile, "%s", road_pos_edit);
					fclose(pRFile);
			}

			IWDG_ReloadCounter();			
																								
			strncpy(road_pos_edit_2,road_pos_edit+5,8-5); ///�������� ��������� ������ ����� � ��������			
			road_indicator = atoi(&road_pos_edit_2); /// � int


			BKP_WriteBackupRegister(BKP_DR12, road_indicator);
			
			
			//����������� ������ ������� ������� (*.???), ���� ������ ��� ���� (�.�. �� �������� ������ �������).
			f_mount(&fls, "0:", 1);			
			
			sprintf(FileName,"0:%03u.%03u\\Signal %d.dat", road_indicator, REG(ROUTE_NUM), 1);
			
			while ( f_stat(FileName, &fno) == FR_OK ) 																								
			{													
					REGW(ROUTE_NUM, REG(ROUTE_NUM)+1);													
					sprintf(FileName,"0:%03u.%03u\\Signal %d.dat", road_indicator, REG(ROUTE_NUM), 1);
			}
											 
			f_mount(0,"0:", 0);
	
			
			 return;			 
		 }
		 
		 
		 
		 
		 
		/// �������� 	
		if (Items[men_POINTER].Data_reg == 0xf2)
		{

			IWDG_ReloadCounter();
		
			SET_CLOCK_SPEED(CLK_72MHz);
			
			vga_CLEAR();
			vga_SET_POS_TEXT(1,1);
			vga_PRINT_STR("��������...",&FONT_6x8);				
			vga_UPDATE();
			
			

			///���������� ��� ����� �������� �� ������� 
			pRFile = fopen ("Roads.txt","r");
			if (pRFile != NULL)
			{
				fseek(pRFile,9*road_pos,SEEK_SET);
				fscanf(pRFile, "%s", temp2);
				fclose(pRFile);
			}

			f_mount(&fls, "0:", 1);
			
			//fdelete(temp2);
			res = f_unlink(temp2); ///������� ���� ��������
						
			strncpy(temp3,temp2+5,8-5); ///�������� ��������� ������ ����� � ��������			
			b = atoi(&temp3); /// � int					
			
			do ///���������� ���������� ������� � ��������� ��������
			{
				sprintf(temppath,"%03u.%03u", b, d);	
				d++;
			} while ( (result = f_stat(temppath, &fno)) == FR_OK );
			
			for (a=0;a<=d;a++) ///�������
			{
				for (c=0;c<=255;c++) ///������
				{
					sprintf(temp4,"0:%03u.%03u\\Signal %d.dat", b, a, c);
					result = f_unlink(temp4);
					//fdelete(temp4);
					
				}
				
				///������� �����
				sprintf(temp5,"0:%03u.%03u", b, a);
				result = f_unlink(temp5);
				//fdelete(temp5);
							
//				vga_CLEAR();
//				vga_SET_POS_TEXT(1,1);
//				vga_PRINT_STR("��������: ",&FONT_6x8);
//				vga_SET_POS_TEXT(70,1);										
//				vga_PRINT_STR(temp4,&FONT_6x8);
//				vga_UPDATE();
				
			}
			
				
			///��������� Roads.txt
			//f_mount(&fls, "0:", 1);											
			f_open(&FileTmp,"Roads.txt", FA_CREATE_ALWAYS | FA_WRITE);
					
			sprintf(FileName,"Road.0  ");
			f_printf(&FileTmp,FileName);
			f_putc('\n',&FileTmp);
			
			j = 1;
			for(i=1;i<255;i++)
			{
					sprintf(FileName,"Road.%03u",i);
				
					if (f_stat(FileName, &fno) == FR_OK)
					{
							f_printf(&FileTmp,FileName);
							f_putc('\n',&FileTmp);
							j++;
					}
						
			}
			
			f_close(&FileTmp);
			
			
			///��������� Roads.log
			
			f_open(&Fil,"0:Road.log", FA_CREATE_ALWAYS | FA_WRITE);
			f_printf(&Fil,"%s", "Road.0");
			f_close(&Fil);
			
						
			f_mount(0,"0:", 0);	

			
			vga_CLEAR();
			vga_SET_POS_TEXT(1,1);
			vga_PRINT_STR("�������� ���������.",&FONT_6x8);				
			vga_UPDATE();
			
			Delay (10000000);
			
			///��������� A,V
			BKP_WriteBackupRegister(BKP_DR12, 0); 			
			
			///�������� �������� 
			REGW(NUMFILE,1);
			REGW(NUMFILE_CURENT,0);
			REGW(ROUTE_NUM,0);
			REGW(BEYOND_ROAD,1);			
			
			men_SETUP();
			men_LEVEL = 0;
			men_POINTER = 0x0;
			men_CURSOR_STR = 0;
			road_pos = 0;
			road_cursor_pos = 0;
			Num_of_Signals = j;
			SET_CLOCK_SPEED(CLK_8MHz);
			men_SHOW_MENU();			
			
			return;
		}
		 
		 
		 
		 ///���������� ������� 
		 if(Items[men_POINTER].Data_reg == 0xfd) 
		 { 			
			 SERVICE_ACC2();			 
		 }
		 
		 

		 //*Alex
   					 if (Items[men_POINTER].Typedata==10)//���� �������
					    {
								if (Items[men_POINTER].Data_reg==1) //��������������
								if (men_SHOW_MESSAGE("������ ����� �������.","�������������?",15000)) //���� ������ ������ ���������
								{
								 //men_SHOW_MESSAGE("��������������...","",0);
								 //REGW(NUMFILE,0); //������ ���-����
								 //��������� ������������� ��������
								 //BKP_WriteBackupRegister(BKP_DR2, REG(K_VIBRO));
									
									FORMAT();
									
									JumpToApplication(0x8010000);						
								}
								else 
						  	men_SHOW_MENU();
						  break;
							}
	

             //���� � ����� ��������������
					   if (Items[men_POINTER].Typedata<0x80)//���� ��������
					   {  
							men_STATUS = men_ONE_ITEM;
							men_READ_VALUE_PARAM(men_POINTER);
								
						 if (((Items[men_POINTER].Options&O_RW)>0)&&(Items[men_POINTER].LevelAcces<=men_LEVEL_ACCES))
						  {
						   men_STATUS = men_EDIT_ITEM;//����� ��������������
						   if (typVALUE_PARAM.val==NIL) typVALUE_PARAM.val = typVALUE_PARAM.def;
						   if (typVALUE_PARAM.val<typVALUE_PARAM.min) typVALUE_PARAM.val=typVALUE_PARAM.min;
						   if (typVALUE_PARAM.val>typVALUE_PARAM.max) typVALUE_PARAM.val=typVALUE_PARAM.max;
						   if (Items[men_POINTER].Typedata==7) typVALUE_PARAM.val = 0;
						   typ_DIGIT  = 0;
						  }
 						 //lcd_CLEAR;
						 men_SHOW_ONEITEM(men_POINTER);
						 vga_UPDATE();
						 break;
						}
						 
						
					   //��������� �������� �������� ��������������� �� ����,
 					   if ((Items[men_POINTER].Typedata>=0x80)&&(Items[men_POINTER].Typedata<0xFD))
					    {  
						 //men_STATUS = men_ONE_ITEM;
						 men_READ_VALUE_PARAM(men_POINTER);
						 if (((Items[men_POINTER].Options&O_RW)>0)&&(Items[men_POINTER].LevelAcces<=men_LEVEL_ACCES))
						  {
								typVALUE_PARAM.val = ((unsigned long)1<<((Items[men_POINTER].Typedata-128)&0x0F));
								men_WRITE_VALUE_PARAM(men_POINTER);
						  }
 						 //lcd_CLEAR;
						 men_SHOW_MENU();
						 //men_SHOW_ITEMS();
						 vga_UPDATE();
						 break;
						}
					   
						//����� �������
						if (Items[men_POINTER].Typedata==0xFE)
					  if (Items[men_POINTER].LevelAcces<=men_LEVEL_ACCES) 
						{
								men_CALL_CUB_ITEM();								
								break;
						}


						 
					   break;
					 


   case men_EDIT_ITEM:
		 {
   					   //lcd_CLEAR;
					   //��������� ���� ���� ������
					   if (Items[men_POINTER].Typedata==7)
					     if (typVALUE_PARAM.val==mes_PAROL2) 
						 	{
						  	 men_LEVEL_ACCES = 2;
						     men_STATUS = men_MULTI_ITEM;
						     men_TIME_PAROL = men_TIME_RES_PAROL;
						     //men_SHOW_MESSAGE("������� ������� - 2",4000);
						    }
 						 else
						 if (typVALUE_PARAM.val==mes_PAROL1)
						    {
						     men_LEVEL_ACCES = 1;
						     men_STATUS = men_MULTI_ITEM;
						     men_TIME_PAROL = men_TIME_RES_PAROL;
						     //mem_SHOW_MESSAGE("������� ������� - 1",4000);
						    }
 						 else ;//men_SHOW_MESSAGE("������������ ������!",4000);
					   else
					   if (Items[men_POINTER].Typedata==10)
					    {
						 //�������
						 //mem_MAKE_COMAND(Items[mem_POINTER].Data_reg);
						 men_STATUS = men_MULTI_ITEM;
						 men_SHOW_MENU();
						}
					   else
						 if (typ_INC_DIGIT(Items[men_POINTER].Typedata)==typ_TRUE)
						  {
					   	   //���������� ���������
						   men_WRITE_VALUE_PARAM(men_POINTER);
						   //writeholdreg(Items[mem_POINTER].Data_reg,typVALUE_PARAM.val);
					       men_STATUS = men_MULTI_ITEM;
						  
					       men_SHOW_MENU();
					      }
						 else
						  {
						   men_SHOW_ONEITEM(men_POINTER);
						   vga_UPDATE();
						  }
					   
					   return;
		 }
		 
		 
		 
		 
		 

   default:return;
  } // ����� men_STATUS  

	
} //����� men_EN_MENU



