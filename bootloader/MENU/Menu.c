#include "types.h"
#include "Menu.h"
#include "Keyboard.h"
#include "vga_lib.h"
#include "Items.h"
#include "string.h"
#include "Reg.h"
#include "Params.h"
#include "Pin.h"
#include "stdio.h"
#include "EXT_ADC.h"
#include "wav.h"
#include "main.h"
#include "Road.h"

#define men_SEPARATOR_1 			' '//0x04  //чтение
#define men_SEPARATOR_2 			' '//':'//0x04 //':'//редактирование
#define men_SEPARATOR_3 			'!'//0x06 //доаступ закрыт
#define men_SEPARATOR_4 			0x05 //команда или меню
#define men_CUR_SYMBOL				'^'

#define men_SELECT_STRING			"\1"//"\13"
#define men_NO_SELECT_STRING		" "//"\12"

#define men_MAX_LEVEL       		3								//махсимальная вложеность 
#define men_REFRASH_TIME			50								//период обновления параметров мс
#define men_TIME_RES_PAROL  		65000							//время сброса пароля 1,2

#define mes_PAROL2					9999
#define mes_PAROL1					2000

//события
#define men_EVENT_REF 		  		0x01
#define men_EVENT_TIME_MESAGE 		0x02


#define men_CHECK_EV(mask)			(men_CHECK_EVENT(mask)==1)	

#define men_COUNT_STR				5								//кол-во отображаемых строк меню
#define men_FONT_DEFAULT			(&FONT_6x8)						//используемый шрифт
#define men_HEIGHT_SYMBOL			men_FONT_DEFAULT->H				//Высота шрифта
#define men_WIDTH_SYMBOL			men_FONT_DEFAULT->W				//Ширина шрифта
#define men_OFFSET					1								//смещение 
#define men_HEIGHT_STR				(men_HEIGHT_SYMBOL+men_OFFSET*2)//Высота строки
#define men_X0						1								//Координата Х - верхнего левого угла меню
#define men_Y0						11								//Координата Y - верхнего левого угла меню



extern u8	measure_stat;
extern unsigned int	Num_of_Signals;


typedef enum {
			  men_MAIN,
			  men_INPUTPAROL,
			  men_MULTI_ITEM,
			  men_ONE_ITEM,
			  men_EDIT_ITEM,
			  men_MESSAGE,
			  men_VIEWARH
			 }  Tmen_status;


Tmen_status				men_STATUS;
Tmen_status				men_SAVE_STATUS;

volatile unsigned char 	men_EVENT 		=	0;
Tmen_form 				men_ID_FORM 	=	form_MESUARE;
unsigned short			men_TIMER		=	1000;
unsigned short 			men_TIME_REF 	=	men_REFRASH_TIME;
signed   short  		men_TIME_MES 	=	-1;
signed   short  		men_TIME_PAROL	= 	-1;
unsigned char 			men_LEVEL_ACCES	= 	1;    					//0 - доступ закрыт, 1 - доступ для оператора, 2 - доступ для инженера

unsigned short		    men_POINTER		= 	0;					    //номер выбранного элемента
unsigned short      	men_START_POINTER= 	0;						//номер элемента с которого начинаем рисовать
unsigned char 			men_cp 			= 	0; 						//вертикальная позиция курсора при редактировании параметра
unsigned char 			men_cc 			= 	0; 						//горизонтальная позиция курсора при редкатировании параметра
unsigned char 			men_s			= 	0;						//строка с которой начинаем рисовать
signed char   			men_CURSOR_STR	= 	0; 						//???строка в которой находиться курсов
unsigned char 			men_CONFIG_SYS  = 	0x7F;//all;		   		//регистр текущей конфигурации
unsigned short		    men_LEVEL_POINT[men_MAX_LEVEL];	       		//массив точек возврата 
unsigned char 			men_LEVEL_CUR[men_MAX_LEVEL];
unsigned char 			men_LEVEL		= 	0;	

unsigned int 			men_temp = 8;
//char					men_Message[128];
unsigned int road_pos = 0;
unsigned int road_cursor_pos = 0;
char Road_Name[15]; 
unsigned char Road_Number;

void men_SHOW_MAINFORMS(Tmen_form form);
void men_ES_MENU(void);
void men_DW_MENU(void);
void men_UP_MENU(void);
void men_SHOW_MENU(void);
void men_SHOW_CURSOR(void);
void men_EN_MENU(void);
void men_SHOW_RECT(const char *text);
void men_SET_CONFIG(unsigned char conf);

//----------------------------------------------------------------//
// Опрос события. Если событие наступило, то функция возвращает 1
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
// Таймер меню. Процедура должна вызываться в 1мс прерывании
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
 
/* if (mem_TIME_PAROL!=NIL)
  if (mem_TIME_PAROL) mem_TIME_PAROL--;
  else 
   {
    mem_LEVEL_ACCES	= (mes_PAROL1==0);
    mem_TIME_PAROL  = NIL;
   }
	   */

  if (men_TIMER>0) men_TIMER--;
  if (!men_TIMER)  men_TIMER=1000;
}

//----------------------------------------------------------------//
// Инициализация меню
//----------------------------------------------------------------//
void men_SETUP(void)
{
// men_LEVEL_ACCES = (men_PAROL1==0);
// men_TIME_PAROL  = NIL;
 //men_STATUS      = men_MAIN;

 //vga_INIT();
 men_SET_CONFIG(REG(CHANEL_MESUARE));		 //меняем конфигурацию меню если выбрали другой канал измерения 
 if (rod_INIT()!=0) 
  {
   men_SHOW_MESSAGE("Ошибка открытия","маршрутного файла",10000);
  }
 rod_GET_NameElement(&NEl,1);
	//Alex
	//Road_Name = NEl.StringName_1;
	memcpy(Road_Name, NEl.StringName_1, 15);
	Road_Number = NEl.Number;
	rod_GET_NameElement(&NEl,REG(NUMFILE_CURENT));
	//*Alex
 
 //men_SHOW_MAINFORMS(form_MESUARE);
// while (!key_CHECK_EV(key_EVENT_PRESSED_ENTER));
}

//----------------------------------------------------------------//
// Загрузить параметр по номеру элемента меню
//----------------------------------------------------------------//
void men_READ_VALUE_PARAM(unsigned int point)
{
 typVALUE_PARAM.val = REG(Items[point].Data_reg);
 typVALUE_PARAM.max = REG_MAX(Items[point].Data_reg);
 typVALUE_PARAM.min = REG_MIN(Items[point].Data_reg);
 typVALUE_PARAM.def = REG_DEF(Items[point].Data_reg);
}

//----------------------------------------------------------------//
// Загрузить параметр по номеру регистра
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
   men_SET_CONFIG(typVALUE_PARAM.val&0xFF);		 //меняем конфигурацию меню если выбрали другой канал измерения
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


//----------------------------------------------------------------//
// Обновить главную форму.
// form - номер главной формы
//----------------------------------------------------------------//

void men_SHOW_REFRESH(void)
{
 char t_str[20];
 char *temp_str;
 unsigned int temp_reg;
 unsigned int temp_val;
FILE  		*pRFile = 0;
	static float A,V,S;
	float* result;
	
 switch (men_ID_FORM)
 {
  case form_MESUARE:
  						vga_CLEAR();

					/*	men_READ_PARAM(RMS_VAL); //загружаем параметр
						temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);
						temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //форматируем параметр
						vga_SET_POS_TEXT(3,24);
						vga_PRINT_STR(temp_str,&FONT_12x16);*/
					  
						men_SHOW_BAT(1,1,REG(BAT));

						men_READ_PARAM(TIME); //загружаем время
						temp_str = typ_SHOW_TIME_VALUE(&typVALUE_PARAM);
						vga_SET_POS_TEXT(21,1);	
						vga_PRINT_STR(temp_str,&FONT_4x7);	

					    //выборка
						/*vga_SET_POS_TEXT(74,1);		
						vga_PRINT_STR("ВЬ",&FONT_4x7);				
						vga_LINE(82,2,82,6);
						vga_SET_POS_TEXT(83,1);		
						vga_PRINT_STR("БОРКА 10/25",&FONT_4x7);		  */
						//выводим канал
					   /* vga_SET_POS_TEXT(74,1);
						switch (REG(CHANEL_MESUARE))
						 {
						  case 0x01: vga_PRINT_STR("ускор",&FONT_6x8);		
						  case 0x02: vga_PRINT_STR("скор",&FONT_6x8);	
						  case 0x04: vga_PRINT_STR("перемещ",&FONT_6x8);	
						 } 	*/


						//выводи частоту
						vga_SET_POS_TEXT(1,vga_GET_HEIGHT_DISPLAY-7);
						switch (REG(CHANEL_MESUARE))
						 {
						  case 0x01: temp_reg = REG(FILTER_A);	   //ускорение
						  			 if (temp_reg==0x01)  vga_PRINT_STR("5-1000 Гц",&FONT_4x7);
									 if (temp_reg==0x02)  vga_PRINT_STR("10-1000 Гц",&FONT_4x7);
									 if (temp_reg==0x04)  vga_PRINT_STR("10-2000 Гц",&FONT_4x7);
									 if (temp_reg==0x08)  vga_PRINT_STR("10-5000 Гц",&FONT_4x7);
									 if (temp_reg==0x10)  vga_PRINT_STR("10-10000 Гц",&FONT_4x7);
						  			 break;
						  case 0x02: temp_reg = REG(FILTER_V);	   //скорость
						  			 if (temp_reg==0x01)  vga_PRINT_STR("5-1000 Гц",&FONT_4x7);
									 if (temp_reg==0x02)  vga_PRINT_STR("10-1000 Гц",&FONT_4x7);
									 if (temp_reg==0x04)  vga_PRINT_STR("10-2000 Гц",&FONT_4x7);
									 if (temp_reg==0x08)  vga_PRINT_STR("10-5000 Гц",&FONT_4x7);
						  			 break;
						  case 0x04: temp_reg = REG(FILTER_S);	   //перемещение
						  			 if (temp_reg==0x01)  vga_PRINT_STR("10-300 Гц",&FONT_4x7);
						  			 break;
						  case 0x08: vga_PRINT_STR("10-10000 Гц",&FONT_4x7);	   //выборка
						  			 break;
						  default  : temp_reg = 0;
						 }
						
								
						//определяем тип детектора
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
						  case 0x01: //СКЗ
						  			 men_READ_PARAM(RMS_VAL); //загружаем параметр
									 if (REG(CHANEL_MESUARE)==0x04) //виброперемещение
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 знак после запятой
									 else 
									  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 знак после запятой
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 знак после запятой

						   			  
						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //форматируем параметр
						   			 vga_SET_POS_TEXT(3,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 //vga_PRINT_STR("пик-фактор",&FONT_6x8);
						   			 vga_PRINT_STR("       скз",&FONT_6x8);
									 break;
						  case 0x02: //ПИК
						   			 men_READ_PARAM(PIK_VAL); //загружаем параметр
									 if (REG(CHANEL_MESUARE)==0x04) //виброперемещение
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 знак после запятой
									 else 
						   			  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 знак после запятой
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 знак после запятой

						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //форматируем параметр
						   			 vga_SET_POS_TEXT(3,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("       пик",&FONT_6x8);
									 break;
						  case 0x04: //ПИК-ПИК
						  			 men_READ_PARAM(PIK_PIK_VAL); //загружаем параметр
									 if (REG(CHANEL_MESUARE)==0x04) //виброперемещение
									  temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);		  //0 знак после запятой
									 else 
						   			  if (typVALUE_PARAM.val>1999) 
									   {
									    typVALUE_PARAM.val = (typVALUE_PARAM.val+5)/10;
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,1,(TBit)0);		  //1 знак после запятой
									   }
									  else
									    temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 знак после запятой

						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //форматируем параметр
						   			 vga_SET_POS_TEXT(3,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("   пик-пик",&FONT_6x8);
									 break;
						  case 0x08: //ПИК-Фактор
						  			 men_READ_PARAM(PIK_FACTOR_VAL); //загружаем параметр
									 temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,2,(TBit)0);		  //2 знак после запятой
						   			 temp_str = typ_FORMAT_STR(temp_str,(TBit)0,6); //форматируем параметр
						   			 vga_SET_POS_TEXT(16,24);
						   			 vga_PRINT_STR(temp_str,&FONT_12x16);
									 vga_SET_POS_TEXT(67,vga_GET_HEIGHT_DISPLAY-8);
						 			 vga_PRINT_STR("пик-фактор",&FONT_6x8);
									 break;
						  case 0x10: break;
						  	
						  case 0x00: //выборка
									 vga_SET_POS_TEXT(74,1);		
									 vga_PRINT_STR("ВЬ",&FONT_4x7);				
									 vga_LINE(82,2,82,6);
									 vga_SET_POS_TEXT(83,1);		
									 
									 sprintf(t_str,"БОРОК %02d",REG(15));
									 vga_PRINT_STR(t_str,&FONT_4x7);


									 //vga_RECTANGLE(8,22,20+100+2,33,drRECT_NO_FILL);
									 vga_SET_POS_TEXT(2,14);
									 temp_val = REG(SAMPLING_STATUS);	
								
									
									 vga_RECTANGLE(2,25,2+122,29,drRECT_NO_FILL);
									 if (temp_val==0xFF) temp_val = 0;
									 sprintf(t_str,"Буфер     %3d %%",temp_val);
									 //vga_PRINT_STR("Запись    ",&FONT_6x8);
									 //men_READ_PARAM(SAMPLING_STATUS); //загружаем параметр
									 //temp_str = typ_SHOW_VALUE_0(&typVALUE_PARAM,0,(TBit)0);
									 vga_PRINT_STR(Road_Name,&FONT_6x8);
									 vga_SET_POS_TEXT(92,14);
									 sprintf(t_str,"%3d %%",temp_val);
									 vga_PRINT_STR(t_str,&FONT_6x8);
								     //vga_PRINT_STR(" %",&FONT_6x8);			
									 vga_SET_POS_TEXT(2,33);
									 vga_PRINT_TEXT(NEl.StringName_1,15,&FONT_6x8);
									 //vga_SET_POS_TEXT(2,43);
									 //vga_PRINT_TEXT(NEl.StringName_2,20,&FONT_6x8);
									 sprintf(t_str,"M:\\DIR%d\\Signal %d.dat",Road_Number,NEl.Number);
									 if (pRFile = fopen (t_str,"r"))
										 {
											 result = calc_from_dat(pRFile);
											 A = *(result);
											 V = *(result+1);
									 
																										memset(t_str,'\0', 20);
																										sprintf(t_str,"A = %3.02f м/с%c",A,0xbd);
																										vga_SET_POS_TEXT(2,43);
																										vga_PRINT_TEXT(t_str,20,&FONT_6x8);

								
																									/*	sprintf(t_str,"S = %.02f ???",V);
																										vga_SET_POS_TEXT(2,39);
																										vga_PRINT_TEXT(t_str,20,&FONT_6x8);*/
								
																										sprintf(t_str,"V = %3.02f мм/с",V);
																										vga_SET_POS_TEXT(2,53);
																										vga_PRINT_TEXT(t_str,20,&FONT_6x8);
																										memset(t_str,' ', 20);
																										vga_SET_POS_TEXT(2,60);
																										vga_PRINT_TEXT(t_str,20,&FONT_6x8);
											 
											fclose(pRFile);
										 }
									 vga_RECTANGLE(2,25,2+122*temp_val/100,29,drRECT_FILL);
									 //Alex
										if (temp_val == 0x64) 
										{measure_stat = 0; 
											men_EN_MENU();}
														   										 
									 
						  default:	 break;		
						 }
						//выводим ед.измерения только для СКЗ,ПИК,ПИК-ПИК
						if ((temp_reg==0x01)||(temp_reg==0x02)||(temp_reg==0x04))
						 switch (REG(CHANEL_MESUARE))
						 {
						  case 0x01: //виброускорение
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("м/сек",&FONT_6x8);
							 		 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-15,29);
						     		 vga_PRINT_STR("2",&FONT_4x7);
						  			 break;
						  case 0x02: //виброскорость
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("мм/сек",&FONT_6x8);
						  			 break;
						  case 0x04: //виброперемещение
						  			 vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-45,31);
						     		 vga_PRINT_STR("мкм",&FONT_6x8);
						  			 break;
						  default  : temp_reg = 0;
						 }
							
						
  						//выводим кнопку "измерение"	
						if (measure_stat == 2)
						 {
						  vga_SET_POS_TEXT(49,0);
						  vga_PRINT_STR("изм",&FONT_6x8);
						  vga_SET_DRAW_MODE(drMODE_XOR);
						  vga_RECTANGLE(47,0,69,8,drRECT_FILL);
						  vga_SET_DRAW_MODE(drMODE_NORMAL);
						  
						  if ((REG(PION_STATUS) & ST_OVER)>0)
						   {
						    //выводим индикатор перегруз
							vga_SET_POS_TEXT(vga_GET_WIDTH_DISPLAY-25,14);
						    vga_PRINT_STR("*",&FONT_6x8);
							vga_SET_DRAW_MODE(drMODE_XOR);
						    vga_RECTANGLE(vga_GET_WIDTH_DISPLAY-25,14,vga_GET_WIDTH_DISPLAY-19,20,drRECT_ARC_FILL);
						    vga_SET_DRAW_MODE(drMODE_NORMAL);
						   }
						  
						 }


						
						//vga_SET_DRAW_MODE(drMODE_XOR);
						//vga_RECTANGLE(0,9,vga_GET_WIDTH_DISPLAY-1,vga_GET_HEIGHT_DISPLAY-11,drRECT_NO_FILL);
						//vga_RECTANGLE(0,vga_GET_HEIGHT_DISPLAY-20,127,vga_GET_HEIGHT_DISPLAY-1,drRECT_FILL);
						//-vga_RECTANGLE(0,0,127,8,drRECT_FILL);
						//vga_RECTANGLE(0,vga_GET_HEIGHT_DISPLAY-20,127,vga_GET_HEIGHT_DISPLAY-11,drRECT_FILL);
						//-vga_RECTANGLE(0,vga_GET_HEIGHT_DISPLAY-9,127,vga_GET_HEIGHT_DISPLAY-1,drRECT_FILL);
						//vga_SET_DRAW_MODE(drMODE_NORMAL);

					   
						
						break;
  case form_USB:	
						vga_CLEAR();
					  
						men_SHOW_BAT(1,1,REG(BAT));

						men_READ_PARAM(TIME); //загружаем время
						temp_str = typ_SHOW_TIME_VALUE(&typVALUE_PARAM);
						vga_SET_POS_TEXT(21,1);	
						vga_PRINT_STR(temp_str,&FONT_4x7);	


						vga_SET_POS_TEXT(45,24);
						vga_PRINT_STR("USB",&FONT_12x16);
							
  						break;
  default: return;
 }

 vga_UPDATE();
}

//----------------------------------------------------------------//
// вывести сообщение
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
// Нарисовать главную форму.
// form - номер главной формы
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
// Проверить конфигурацию элемента меню,
//----------------------------------------------------------------//
unsigned char men_GET_CONFIG(unsigned short id_)                //проверка конфигурации элемента
{
 if (men_CONFIG_SYS & Items[id_].Config) 
  return 1;
 else 
  return 0;                       
}

//----------------------------------------------------------------//
// Задать новую конфигурацию меню
//----------------------------------------------------------------//
void men_SET_CONFIG(unsigned char conf)
{
 men_CONFIG_SYS = conf;
}

//----------------------------------------------------------------//
// Вернуть номер следующего элемента в списке
//----------------------------------------------------------------//
unsigned short men_GET_NEXT_ITEM(unsigned short point)    //возвращает номер следующего элемента в списке
{
unsigned char id;
 id = Items[point].Id_group;
	//Alex
	if (Items[point].Data_reg == 0xFF)
	{	
	//	road_pos_tmp++;		
		return point;
	}
		//*Alex
 while (point<(MAX_ITEMS-1))
  {
  point++;
  //if (Items[point-1].Id_group == Items[point].Id_group) 
		//if (Items[point].Data_reg == 0xFF) return point--;
  if (id==Items[point].Id_group)
    if (men_GET_CONFIG(point)) return point;else;  
    //else return NIL;
  }
 return NIL;
}


//----------------------------------------------------------------//
// Вернуть номер предыдущего элемента в списке
//----------------------------------------------------------------//
unsigned short men_GET_PREV_ITEM(unsigned short point)
{
unsigned char id;
 id = Items[point].Id_group;
	//Alex
	if (Items[point].Data_reg == 0xFF)
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
unsigned men_RETURN_SUB_ITEM(unsigned pp)   //вернуть номер 1-го элемента в массиве у которого Id_goup равно Id_gr
{
 unsigned char Id_gr;
 Id_gr=Items[pp].Data_reg;
 while (pp++<MAX_ITEMS)
   if (Items[pp].Id_group==Id_gr) 
     if (men_GET_CONFIG(pp)) return pp;
 
 return NIL;
}

//----------------------------------------------------------------//
// Вызвать подменю
//----------------------------------------------------------------//
void men_CALL_CUB_ITEM()                          
{
 unsigned short pp;
 pp = men_RETURN_SUB_ITEM(men_POINTER);
 if (pp==NIL) return ;
 //CONTOLS_CONFIG();
 if (men_LEVEL==men_MAX_LEVEL) return;
 men_LEVEL_POINT[men_LEVEL] = men_POINTER;                    //сохранить точку возврата
 //men_LEVEL_CUR[men_LEVEL]   = men_CURSOR_STR;
 men_LEVEL++;
 men_POINTER = pp;
 men_CURSOR_STR = 0;
 men_SHOW_MENU();
 
 //refrash_cursor();    
}

//----------------------------------------------------------------//
// Вернуться в предыдущее меню
//----------------------------------------------------------------//

void men_CALLBACK()                               
{
 if (!men_LEVEL) return;
 //функция проверки существования меню  в которое возвращаемся//
 //CONTOLS_CONFIG();
 men_LEVEL--;
 men_POINTER = 0;//men_LEVEL_POINT[men_LEVEL];
 men_CURSOR_STR = 0;//men_LEVEL_CUR[men_LEVEL];
 men_SHOW_MENU();
 //refrash_cursor(); 
}

//----------------------------------------------------------------//
// Возвращает значение в строковом виде один 
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


 if (Items[item_num].Typedata==6) //пароль
  return typ_FORMAT_STR(typ_SHOW_PAROL(&typVALUE_PARAM,men_LEVEL_ACCES),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==7)//ввод пароля
  if (men_STATUS==men_EDIT_ITEM) return typ_FORMAT_STR(typ_SHOW_PAROL(&typVALUE_PARAM,1),(TBit)(Options&O_LEFT),6);
  else 
   {
    strcpy(typVALUE_PARAM.valstr,"----");
    return typ_FORMAT_STR(typVALUE_PARAM.valstr,(TBit)(Options&O_LEFT),6);
   }


 if (Items[item_num].Typedata==10)//команда
   {
    if (men_STATUS==men_EDIT_ITEM) strcpy(typVALUE_PARAM.valstr," Да/Нет?");
    else strcpy(typVALUE_PARAM.valstr,"");
    
	return typ_FORMAT_STR(typVALUE_PARAM.valstr,(TBit)0,6);
   }
  
 if (Items[item_num].Typedata==16)
  return typ_FORMAT_STR(typ_SHOW_COUNTTIME_VALUE(&typVALUE_PARAM,'ч'),(TBit)(Options&O_LEFT),6);

 if (Items[item_num].Typedata==17)
  return typ_FORMAT_STR(typ_SHOW_COUNTTIME_VALUE(&typVALUE_PARAM,'м'),(TBit)(Options&O_LEFT),6);

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
// Нарисовать один элемент меню
//----------------------------------------------------------------//
void men_SHOW_ITEM(unsigned short item_num,unsigned char str_num)
{
unsigned char digit;
	char temp[20];
 //lcd_SET_NON_CURSOR;
 vga_SET_POS_TEXT(men_X0, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
 //очистить строку
 /*vga_FILL_STR(' ',21,men_FONT_DEFAULT);
 vga_SET_POS_TEXT(men_X0, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);	*/
	//Alex
  if (Items[item_num].Data_reg == 0xFF) 
  {
   //lcd_PRINT_LINE(Items[item_num].LongCaption,18);
   //if (Items[item_num].LevelAcces>mem_LEVEL_ACCES) lcd_SEND_CHAR(mem_SEPARATOR_3);//?????? ??????
   //else lcd_SEND_CHAR(mem_SEPARATOR_4);
		sprintf(temp,"Signal %u  %u",road_pos,road_pos);
   vga_PRINT_TEXT(temp,21,men_FONT_DEFAULT);
  }
 else
	 //*Alex
 if (Items[item_num].Typedata>0xFD) 
  {
   //lcd_PRINT_LINE(Items[item_num].LongCaption,18);
   //if (Items[item_num].LevelAcces>mem_LEVEL_ACCES) lcd_SEND_CHAR(mem_SEPARATOR_3);//доступ закрты
   //else lcd_SEND_CHAR(mem_SEPARATOR_4);
   vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
  }
 else
 if (Items[item_num].Typedata>=0x80) 
  {
   //выводим название параметра/команды
   if ((Items[item_num].Options&O_SZ))
   	{
	 vga_SET_POS_TEXT(men_X0 + men_WIDTH_SYMBOL/2, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
	 vga_PRINT_TEXT(men_SHOW_ITEM_VALUE(item_num),1,men_FONT_DEFAULT);	 
	 vga_SET_POS_TEXT(men_X0 + men_WIDTH_SYMBOL*2, men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
	 vga_PRINT_TEXT(Items[item_num].Text_0,20,men_FONT_DEFAULT);
	}
   else 
	 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
  }
 else
 if (Items[item_num].Typedata==10)
  {
   	 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
  } 
 else  	
  {
   //выводим краткое название параметра
   vga_PRINT_TEXT(Items[item_num].Caption,9,men_FONT_DEFAULT);
   //выводим разделяющий символ
  
   if ((Items[item_num].Options&O_RW)==0) vga_PRINT_CHAR(men_SEPARATOR_1,men_FONT_DEFAULT);
   else 
    if (men_LEVEL_ACCES<Items[item_num].LevelAcces) vga_PRINT_CHAR(men_SEPARATOR_3,men_FONT_DEFAULT);//доступ закрты
    else 
	 if (Items[item_num].Typedata!=10) 
	 								  //уставка
									   vga_PRINT_CHAR(men_SEPARATOR_2,men_FONT_DEFAULT); //уставка
     else 							   vga_PRINT_CHAR(men_SEPARATOR_4,men_FONT_DEFAULT);//команда

   vga_PRINT_CHAR(' ',men_FONT_DEFAULT);//пробел
   vga_PRINT_TEXT(men_SHOW_ITEM_VALUE(item_num),0,men_FONT_DEFAULT);
   if (Items[item_num].MeasureText[0]!=0) {
										   vga_SET_POS_TEXT(men_X0+17*men_WIDTH_SYMBOL,men_Y0 + men_OFFSET + (unsigned short)str_num * men_HEIGHT_STR);
   										   vga_PRINT_TEXT(Items[item_num].MeasureText,4,men_FONT_DEFAULT);
									   	  } 
   //если режим редактирования то выводим курсор
   if (men_STATUS == men_EDIT_ITEM) 
										 {
										  digit = typ_DIGIT;
										  //выводим курсор редактирования
										  if ((Items[item_num].Typedata<=3)&&(Items[item_num].Typedata>0))
										   if (typ_DIGIT>=Items[item_num].Typedata) digit = typ_DIGIT+1;
										   

										  if ((Items[item_num].Typedata==18)||(Items[item_num].Typedata==19))
										   if (typ_DIGIT>0) digit = typ_DIGIT+2;
										   
										  
										  vga_SET_POS_TEXT(men_X0+(16-digit)*men_WIDTH_SYMBOL, men_Y0 + men_OFFSET + (unsigned short)(str_num+1) * men_HEIGHT_STR);
										  vga_PRINT_CHAR(men_CUR_SYMBOL,men_FONT_DEFAULT);

										 }
  }
  
}

//----------------------------------------------------------------//
// Очистить строку
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
//перемещение курсора вниз
//----------------------------------------------------------------//
void men_INC_POINT()
{
 unsigned short pp = 0;

	
 pp = men_GET_NEXT_ITEM(men_POINTER);
//if (Items[men_POINTER].Data_reg == 0xFF) road_pos++;	
 if (pp==NIL) return;

 men_SHOW_CURSOR();

 men_CURSOR_STR++;
	
 men_POINTER = pp;

 if (men_GET_NEXT_ITEM(men_POINTER)==NIL) 
   if (men_CURSOR_STR>(men_COUNT_STR-1)) {
   										  men_CURSOR_STR = men_COUNT_STR-1;
										  men_SHOW_MENU();
   										 }
   else   		   				   		 {
   										  men_SHOW_CURSOR();
										  vga_UPDATE();
										 }
 else 
   if (men_CURSOR_STR>(men_COUNT_STR-1)) 
       									 {
        								  men_CURSOR_STR = men_COUNT_STR-1;
        								  men_SHOW_MENU(); 
       									 }
   else 								 {
   		 								  men_SHOW_CURSOR();
		 								  vga_UPDATE();
										 }
//Alex
	if (Items[men_POINTER].Data_reg == 0xFF)
	{
		if (road_cursor_pos<4) road_cursor_pos++; 
		road_pos++;
		return;
	}
//*Alex
}

//----------------------------------------------------------------//
//перемещение курсора вверх
//----------------------------------------------------------------//
void men_DEC_POINT()
{
 unsigned short pp = 0;

//if (Items[men_POINTER].Data_reg == 0xFF) if (road_pos > 0) road_pos--;	
 pp = men_GET_PREV_ITEM(men_POINTER);

 if (pp==NIL) return;

 men_SHOW_CURSOR();

 men_CURSOR_STR--;
//if (road_cursor_pos>0) road_cursor_pos--;		//Alex
 men_POINTER = pp;

 if (men_CURSOR_STR<0)
        				{
        				 men_CURSOR_STR = 0;
        				 men_SHOW_MENU();
        				}
 else 					{
 						 men_SHOW_CURSOR();
						 vga_UPDATE();
						}
					//Alex
	if (Items[men_POINTER].Data_reg == 0xFF) 
	{
		if (road_cursor_pos>0) road_cursor_pos--; 
		if (road_pos>0)road_pos--;
		return;	
	}
	//*Alex		
}

void men_SHOW_CURSOR(void)
{
 unsigned short h;

 h = men_Y0 + men_OFFSET + (unsigned short)men_CURSOR_STR * men_HEIGHT_STR;
 //if (h>0) h-=1;

 vga_SET_DRAW_MODE(drMODE_XOR);

 vga_RECTANGLE(men_X0-1,h-men_OFFSET,vga_GET_WIDTH_DISPLAY-1,h+men_HEIGHT_SYMBOL+men_OFFSET-1,drRECT_FILL);

 vga_SET_DRAW_MODE(drMODE_NORMAL);
}

void men_SHOW_CURSOR_EDIT(void)
{
 
}

//----------------------------------------------------------------//

//----------------------------------------------------------------//
void men_SHOW_ONEITEM(unsigned item_num)
{
 vga_CLEAR();
 if ((Items[item_num].Options&O_RW)>0)
 	men_SHOW_RECT("Редактирование");
 else
    men_SHOW_RECT("Просмотр");
  
 //lcd_SET_NON_CURSOR;
 //lcd_SET_POS(0);
 vga_SET_POS_TEXT(men_X0,men_Y0 + men_OFFSET + 0 * men_HEIGHT_STR);
 vga_PRINT_TEXT(Items[item_num].Text_0,21,men_FONT_DEFAULT);
 vga_SET_POS_TEXT(men_X0,men_Y0 + men_OFFSET + 1 * men_HEIGHT_STR);
 vga_PRINT_TEXT(Items[item_num].Text_1,21,men_FONT_DEFAULT);

 //lcd_PRINT_LINE(Items[item_num].Text_0,20);

 //lcd_SET_POS(20);
 //lcd_PRINT_LINE(Items[item_num].Text_1,20);
 //lcd_SET_POS(40);
 //lcd_PRINT_LINE("",20);
 if ((Items[item_num].Options&O_RW)>0)
  if (Items[item_num].LevelAcces>men_LEVEL_ACCES)
   {
    //lcd_SET_POS(60);
	//if (Items[item_num].LevelAcces==1) lcd_PRINT_LINE("<Доступ закрыт,ур.1>",20);
	//if (Items[item_num].LevelAcces==2) lcd_PRINT_LINE("<Доступ закрыт,ур.2>",20);
	return;
   }
  
 
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
// Нарисовать меню
//----------------------------------------------------------------//
void men_SHOW_ITEMS(void)                   //вывести меню
{
 
 signed char 	sn;
 signed char 	ss;
 unsigned short pp;
 //unsigned long v;
 sn = men_s;
 ss = men_s;
 pp = men_START_POINTER;

 while (sn>0) men_CLEAR_ITEM(--sn);

 while (ss<men_COUNT_STR)              
  if (pp!=NIL)  //рисование элемента//
   {
    
    men_READ_VALUE_PARAM(pp);
    men_SHOW_ITEM(pp,ss);
    ss++;
	pp = men_GET_NEXT_ITEM(pp);
   }
  else  men_CLEAR_ITEM(ss++);
 
}

//----------------------------------------------------------------//
// Нарисовать рамку меню
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
// Нарисовать меню
//----------------------------------------------------------------//
void men_SHOW_MENU(void)                    
{
	char temp[20];
	FILE  		*pRFile = 0;
	uint8_t ip = 0;
 //unsigned char s = mem_STEP;
 //unsigned 	   start_point;
 men_STATUS 	   = men_MULTI_ITEM;

 men_s			   = men_CURSOR_STR;
	//Alex
		if (Items[men_POINTER].Data_reg == 0xFF)
	{
		vga_CLEAR();
		men_SHOW_RECT(Items[men_LEVEL_POINT[men_LEVEL-1]].Text_0);
		men_CURSOR_STR = road_cursor_pos;
		//men_SHOW_ITEMS();
		ip = 0;
		while (ip<5)
		{
			vga_SET_POS_TEXT(men_X0, men_Y0 + men_OFFSET + (unsigned short)ip * men_HEIGHT_STR);
			memset(temp,0,20);
			sprintf(temp,"Road.%03u",ip);
			pRFile = fopen (temp,"r");
			fread(temp,1,15,pRFile);
			fclose(pRFile);
			vga_PRINT_TEXT(temp,21,men_FONT_DEFAULT);
			ip++;
		}

		men_SHOW_CURSOR();
		vga_UPDATE();
		return;
	}
	//*Alex
	
 //men_START_POINTER = men_POINTER;
 while (!men_GET_CONFIG(men_POINTER))
   if ((men_POINTER = men_GET_NEXT_ITEM(men_POINTER))==NIL) break;
  
 //men_POINTER = men_START_POINTER;
 men_START_POINTER = men_POINTER;
  
 while (men_s>0) ///вычисляет s,p
  {
   men_START_POINTER = men_GET_PREV_ITEM(men_START_POINTER);
   if (men_START_POINTER==NIL) {men_START_POINTER = men_POINTER;break;}
   men_s--;
  }

 //lcd_SET_NON_CURSOR;//cursor(0,0);
 vga_CLEAR();
 if (men_LEVEL==0)	men_SHOW_RECT("Меню");
 else				men_SHOW_RECT(Items[men_LEVEL_POINT[men_LEVEL-1]].Text_0);
	men_SHOW_ITEMS();//show_element();//(s,p);
 
 men_SHOW_CURSOR();
 vga_UPDATE();
}


//----------------------------------------------------------------//
// Функция обработки событий
//----------------------------------------------------------------//
u8 men_MENU_CONTROL(void)
{
u8 key_event = 0;
 if (men_CHECK_EV(men_EVENT_REF))
  {
   //обновить значения параметров в мульти режиме
   //if (men_STATUS==men_MULTI_ITEM) men_SHOW_ITEMS(); 
   //обновить ззначение параметра в одиночном режиме
   /*if (men_STATUS==men_ONE_ITEM)  {
    							   men_READ_VALUE_PARAM(men_POINTER);
								   men_SHOW_ONEITEM(men_POINTER);
								   //mem_SHOW_ITEM(mem_POINTER,3);
								  }*/
   //обновить главную форму
   if (men_STATUS==men_MAIN)       
   									men_SHOW_REFRESH();//mem_SHOW_MAINFORMS(mem_ID_FORM);

  }

 /*if (men_CHECK_EV(men_EVENT_TIME_MESAGE))	//истекло время меню
  {
   men_STATUS = men_SAVE_STATUS;
   //обновить меню
   //men_UPDATE(); 
  }*/
 
 //mem_CONTROL_CONFIG();

 //обработка событий клавиатуры
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
return  key_event;  
}

//----------------------------------------------------------------//
// Функция обработки нажатия клавиши Вверх
//----------------------------------------------------------------//
void men_UP_MENU(void)
{
 unsigned int temp_reg;
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL; 

 switch (men_STATUS)
   {
    case men_MAIN:       temp_reg = REG(NUMFILE_CURENT);
						 if (temp_reg<REG_MAX(NUMFILE_CURENT)) {
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
						 break;
    case men_MULTI_ITEM: 
					     men_DEC_POINT();
					     break;
						 
	case men_EDIT_ITEM:  
	 					 typ_INC_VALUR(&typVALUE_PARAM,Items[men_POINTER].Typedata);
						 men_SHOW_ITEM(men_POINTER,3);
						 men_SHOW_CURSOR_EDIT();
						 //lcd_SET_POS(70+5-mem_RETURN_CURSOR_POS(Items[mem_POINTER].Typedata));
						 //cd_SET_BLINC_CURSOR;
						 vga_UPDATE();
						 break;
						
    /*case memVIEWARH:    
						{
						 if (recn<maxrec) 
  							if (INC_RECORD()) 
    						 {
     						  recn++;
     						  mem_SHOW_ARHIVE();
    						 } 
                         return;
						}
	//case select_value:{inc_val();  return;}
	//case input_parol: {inc_parol();return;}
	//case arh:         {inc_rec();  return;}	*/
	default:return;
   }
   
}

//----------------------------------------------------------------//
// Функция обработки нажатия клавиши Вниз
//----------------------------------------------------------------//
void men_DW_MENU(void)
{
 unsigned int temp_reg;
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL;

 switch (men_STATUS)
   {
    case men_MAIN:       temp_reg = REG(NUMFILE_CURENT);
						 if (temp_reg>REG_MIN(NUMFILE_CURENT)) {
						 										temp_reg--;
																REGW(NUMFILE_CURENT,temp_reg);
																rod_GET_NameElement(&NEl,temp_reg);
							 	//Alex
							 if (NEl.Number == 0) 
								 {
									 temp_reg++;
									 REGW(NUMFILE_CURENT,temp_reg);
									 rod_GET_NameElement(&NEl,temp_reg);
								 }
								//*Alex
																men_SHOW_REFRESH();
																}; 
						 break;
    case men_MULTI_ITEM: 
   	 				     men_INC_POINT();
					     break;

    case men_EDIT_ITEM:  
	 					 typ_DEC_VALUR(&typVALUE_PARAM,Items[men_POINTER].Typedata);
						 men_SHOW_ITEM(men_POINTER,3);
						 men_SHOW_CURSOR_EDIT();
						 //lcd_SET_POS(70+5-mem_RETURN_CURSOR_POS(Items[mem_POINTER].Typedata));
						 //cd_SET_BLINC_CURSOR;
						 vga_UPDATE();
						 break;
						 
    /*case memVIEWARH:    
						{
						 if (recn>1) 
  							if (DEC_RECOCR()) 
   							 {
    						  recn--;
    						  mem_SHOW_ARHIVE();
   							 }
						}
   //case select_value:{dec_val();  return;}
   //case input_parol: {dec_parol();return;}
   //case arh:         {dec_rec();  return;}   */
    default:return;
   }   
  
}


//----------------------------------------------------------------//
// Функция обработки нажатия клавиши Меню/Отм
//----------------------------------------------------------------//
void men_ES_MENU(void)
{
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL;

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
					      men_SHOW_MAINFORMS(form_MESUARE); //главная форма
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
 //if (mem_TIME_PAROL!=NIL) mem_TIME_PAROL = mem_TIME_RES_PAROL;
FILE  		*pRFile = 0;
 switch (men_STATUS)
  {
   case men_MAIN:       
   						if (REG(CHANEL_MESUARE)==0x08) //выборка
						 if (REG(SAMPLING_STATUS)!=0xFF)
						  {
						   temp_reg = REG(NUMFILE_CURENT);
						   sprintf(FileName,"Signal %d.dat",NEl.Number);
						   //strcpy(men_Message,"Сохранить в файл:   ");
							//wav_WriteMonoFile(FileName,SIM,25000,25000);//50000*/
   						//	if (men_SHOW_MESSAGE("Сохранить?",NEl.StringName_1,15000)) //если нажали кнопку сохранить
							// {
							  men_SHOW_MESSAGE("Сохранение...","",0);

							  SET_CLOCK_SPEED(CLK_72MHz); 
							  REGW(NUMFILE,0); //ставим лок-байт
							  //sprintf(FileName,"M:\\DIR\\Signal %d.wav",temp_reg);
							  //wav_WriteMonoFile(FileName,ext_adc_SIM,25000,25000);//создаем WAV файл
							  sprintf(FileName,"M:\\DIR%d\\Signal %d.dat",Road_Number,NEl.Number);
							  dat_CreateFile(FileName,ext_adc_SIM,25000,&k_reg);		  //создаем DAT файл
							  REGW(NUMFILE,temp_reg); //снимаем лок-байт
							  SET_CLOCK_SPEED(CLK_8MHz);
							   
							  REGWDEF(SAMPLING_STATUS);
								//Alex
							  temp_reg++;
								REGW(NUMFILE_CURENT,temp_reg);
								temp_reg = REG(15);
								temp_reg++;
								REGW(15,temp_reg);
								//Num_of_Signals++;
								REGW(NUMFILE,temp_reg); //снимаем лок-байт
								
							// }
						  }
   						break;
   /*case men_MESSAGE:
					   if (men_TIME_MES!=NIL) men_TIME_MES = 0;
					   break;  */
					  
   case men_MULTI_ITEM:
		 //Alex
		 if (Items[men_POINTER].Data_reg == 0xFF)
		 {
			 //men_SHOW_MESSAGE("Форматирование...","",0);
			 //while(1);
			 //men_POINTER = 0;
			 pRFile = fopen ("M:\\Road.log","w");
			 fprintf(pRFile,"Road.%03u",road_pos);
			 fclose(pRFile);
			 REGW(NUMFILE_CURENT,0x00);
			 		//	sprintf(temp,"Road.%03u",ip);
			//pRFile = fopen (temp,"r");
			 //fwrite(&NEl,1,SizeElement,pFile);
			 NVIC_SystemReset();
			 return;
		 }
		 //*Alex
   					   if (Items[men_POINTER].Typedata==10)//если команда
					    {
						 if (Items[men_POINTER].Data_reg==1) //форматирование
						  if (men_SHOW_MESSAGE("Форматировать память?","",15000)) //если нажали кнопку сохранить
						   {
						     men_SHOW_MESSAGE("Форматирование...","",0);
							 //REGW(NUMFILE,0); //ставим лок-байт
							 //сохринить калибровочный параметр
	 						 //BKP_WriteBackupRegister(BKP_DR2, REG(K_VIBRO));
							 FORMAT();
							 NVIC_SystemReset();
						   }
						  else 
						  	men_SHOW_MENU();
						  break;
						}

                       //вход в режим редактирования
					   if (Items[men_POINTER].Typedata<0x80)//если параметр
					    {  
						 men_STATUS = men_ONE_ITEM;
						 men_READ_VALUE_PARAM(men_POINTER);
						 if (((Items[men_POINTER].Options&O_RW)>0)&&(Items[men_POINTER].LevelAcces<=men_LEVEL_ACCES))
						  {
						   men_STATUS = men_EDIT_ITEM;//режим редактирования
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
					   //изменение значения регистра непосредственно из меню,
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
					   //вызов подменю
					   if (Items[men_POINTER].Typedata==0xFE)
					    if (Items[men_POINTER].LevelAcces<=men_LEVEL_ACCES) 
						 {
						  men_CALL_CUB_ITEM();
						  break;
						 }

					  /* if ((Items[mem_POINTER].Typedata==0xFF)&&(Items[mem_POINTER].Data_reg==0xFF))//просмотр архивов
					    {
						 mem_STATUS = memVIEWARH;
 						 recn = 1;
 						 ch =   1;
 						 RESET_REC();
 						 INC_RECORD();
 						 //READ_ARC(recn,ch);
 						 lcd_CLEAR;;
						 mem_SHOW_ARHIVE();
						 return;
						}	*/
					    
					    //if (Items[mem_POINTER].LevelAcces<=mem_LEVEL_ACCES) ;//команда*/
						 
					   break;
					 
   /*case memONE_ITEM: {
   					   lcd_CLEAR;
					   mem_SHOW_MENU();
					   return;}	  */

   case men_EDIT_ITEM:{
   					   //lcd_CLEAR;
					   //проверяем если ввод пароля
					   if (Items[men_POINTER].Typedata==7)
					     if (typVALUE_PARAM.val==mes_PAROL2) 
						 	{
						  	 men_LEVEL_ACCES = 2;
						     men_STATUS = men_MULTI_ITEM;
						     men_TIME_PAROL = men_TIME_RES_PAROL;
						     //men_SHOW_MESSAGE("Уровень доступа - 2",4000);
						    }
 						 else
						 if (typVALUE_PARAM.val==mes_PAROL1)
						    {
						     men_LEVEL_ACCES = 1;
						     men_STATUS = men_MULTI_ITEM;
						     men_TIME_PAROL = men_TIME_RES_PAROL;
						     //mem_SHOW_MESSAGE("Уровень доступа - 1",4000);
						    }
 						 else ;//men_SHOW_MESSAGE("НеправильныЙ пароль!",4000);
					   else
					   if (Items[men_POINTER].Typedata==10)
					    {
						 //команда
						 //mem_MAKE_COMAND(Items[mem_POINTER].Data_reg);
						 men_STATUS = men_MULTI_ITEM;
						 men_SHOW_MENU();
						}
					   else
						 if (typ_INC_DIGIT(Items[men_POINTER].Typedata)==typ_TRUE)
						  {
					   	   //сохранение параметра
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
					   
					   return;}
   //case select_value:{dec_val();  return;}
   //case input_parol: {dec_parol();return;}
   //case arh:         {dec_rec();  return;}  */
   default:return;
  }  

}
