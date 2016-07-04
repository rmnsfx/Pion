#include <Types.h>
//#include <Externalmap.h>
#include <Items.h>

#include <string.h>
#include <stdio.H>

const char show_max[]  = "max";
const char show_min[]  = "min";
const char show_nil[]  = "н/д";
const char show_err[]  = "ошибка";

#define   nil 255

TValue_param typVALUE_PARAM;
char   typ_DIGIT;
char   vlast[20];

const char MATH[12][4]= {"Янв","Фев","Мар","Апр","МаЙ","Июн","Июл","Авг","Сен","Окт","Ноя","Дек"};

//----------------------------------------------------------------//

void typ_STR_Insert(char *s,unsigned char c, unsigned int pos)   //вставить символ в строку *s 
{
 char m[maxlenval];
 strcpy(m,s+pos);
 *(s+pos) = c;
 strcpy(s+pos+1,m);
} 

//----------------------------------------------------------------//

char* typ_FORMAT_STR(char *STR,TBit LEFT,unsigned char size_array)
{
unsigned char n,pos;
 if (size_array<maxlenval) 
   {
    n = strlen(STR);
	if (n>=size_array) return STR;

	if (LEFT) {
	           pos = n;
			   n = size_array-n; 
			   while (n--)
			    {strcpy(STR+pos," ");pos++;}
			  }
    else
			  {
			   n = size_array-n; 
			   while (n--)
			    typ_STR_Insert(STR,' ',0);
			  }
   }
   
  
 return STR;
}

//----------------------------------------------------------------//

char typ_RET_DIGIT(unsigned int val)
{
 if (val>9999) return 5;
 if (val>999)  return 4;
 if (val>99)   return 3;
 if (val>9)    return 2;
 
 return 1;
}

//----------------------------------------------------------------//

char* typ_SHOW_VALUE_0(TValue_param *VP,unsigned char Point, TBit SHOW_ZERO)
{
unsigned int count_digit = 5;
//float f;
//int count = 3;

 //count_digit = typ_RET_DIGIT(VP->max);

 VP->valstr[count_digit] = 0;

 if (VP->val==NIL) 
  					   strcpy(VP->valstr,show_nil);//while (count_digit--) VP->valstr[count_digit] = '-';
 else
  if (VP->val>VP->max) strcpy(VP->valstr,show_max);
  else
  if (VP->val<VP->min) strcpy(VP->valstr,show_min);
  else
   {
    count_digit = typ_RET_DIGIT(VP->max);

    if (Point==0)
    	 if (SHOW_ZERO)
      	  sprintf(VP->valstr,"%0*u",count_digit,VP->val);
     	 else 
	 	  sprintf(VP->valstr,"%u",VP->val);
    else
	    {
	     if (SHOW_ZERO)
		  if (count_digit>(Point+1))
		   sprintf(VP->valstr,"%0*u",count_digit,VP->val);
		  else
		   sprintf(VP->valstr,"%0*u",(unsigned int)Point+1,VP->val);
         else 
		   sprintf(VP->valstr,"%0*u",(unsigned int)Point+1,VP->val);

         count_digit = strlen(VP->valstr)-Point;
         typ_STR_Insert(VP->valstr,'.',count_digit);
        }
   }

 
 return VP->valstr;//yp_FORMAT_STR(vlast,Options&O_LEFT,Size
 //usb_byte = sprintf(usb_buffer,"mkdir CKB_%04d\r",HOLD[SHR]);
}


//----------------------------------------------------------------//

char* typ_SHOW_ZNAK(TValue_param *VP,unsigned char Point, TBit SHOW_ZERO)
{
 char 			znak = 0;
 unsigned short v = 0;
 

 if (VP->val==NIL) strcpy(VP->valstr,"-----");
 else
  if (VP->val>VP->max) strcpy(VP->valstr,show_max);
  else
  if (VP->val<VP->min) strcpy(VP->valstr,show_min);
  else
   {
    if (VP->val>0x8000) {v = VP->val-0x8000;znak='+';}
	if (VP->val<0x8000) {v = 0x8000-VP->val;znak='-';}
	if (VP->val==0x8000){v = 0;znak=' ';}

    if (Point==0)
    	 if (SHOW_ZERO)
      	  sprintf(VP->valstr,"%04u",v);
     	 else 
	 	  sprintf(VP->valstr,"%u",v);
    else
	    {
	     if (SHOW_ZERO)
		  if (4>(Point+1))
		   sprintf(VP->valstr,"%04u",v);
		  else
		   sprintf(VP->valstr,"%0*u",(unsigned int)Point+1,v);
         else 
		   sprintf(VP->valstr,"%0*u",(unsigned int)Point+1,v);

         v = strlen(VP->valstr)-Point;
         typ_STR_Insert(VP->valstr,'.',v);
        }

     typ_STR_Insert(VP->valstr,znak,0);
   }
return VP->valstr;
}



//----------------------------------------------------------------//

char* typ_SHOW_ENUM_VALUE(TValue_param *VP, unsigned char id)
{
 unsigned char i=0;
 //unsigned char g=0;
 while (i<COUNT_ENUM) if (Enum_val[i].Id==id) break; else i++;

 if (VP->val==NIL)   {
 					  strcpy(VP->valstr,show_nil);
					  return VP->valstr;  
					 }
 else
 if (i==COUNT_ENUM)  {
 					  strcpy(VP->valstr,show_err);
 					  return VP->valstr;  
					 }

 else 
 while ((Enum_val[i].Id==id)&&(i<COUNT_ENUM))

   if (Enum_val[i].V==VP->val) 
   					 {
    				  strcpy(VP->valstr,Enum_val[i].S);
					  return VP->valstr; 
					  }
   else i++;
  			               
 			         
 strcpy(VP->valstr,show_err);
 return VP->valstr;  

}  

//----------------------------------------------------------------//

char* typ_SHOW_TIME_VALUE(TValue_param *VP)
{
//char math;

 VP->valstr[0] = 0; 

 if (VP->val == NIL) strcpy(VP->valstr,"--:--");
  else
   {
    if (VP->val>2359) VP->val = 2359;
    
    sprintf(VP->valstr,"%02u:%02u",(VP->val/100),(VP->val%100));
    
   }

 
return VP->valstr;
}
/*
char* typ_SHOW_TIME_VALUE(TValue_param *VP)
{
 unsigned int sec,min,hour;

 VP->valstr[0] = 0; 

 if (VP->val == NIL) strcpy(VP->valstr,"--:--:--");
 else
  {
   sec  = VP->val&0x20;
   min  = (VP->val>>6)&0x20;
   hour = VP->val>>12;
   sprintf(VP->valstr,"%02u:%02u:%02u",hour,min,sec);
  }

return VP->valstr;
}	  */


//----------------------------------------------------------------//

char* typ_SHOW_DATA_VALUE(TValue_param *VP, TBit Format_data)
{
char math;

 VP->valstr[0] = 0; 

 if (Format_data==0) //01-Апр
  if (VP->val == NIL) strcpy(VP->valstr,"-----");
  else
   {
    if (VP->val>1231) VP->val = 1231;
	if (VP->val<101)  VP->val = 101;
    
    sprintf(VP->valstr,"%02u",(VP->val%100));

	math =  VP->val/100;
    //strcpy(&VP->valstr[2],",");
	strcpy(&VP->valstr[2],&MATH[math-1][0]);
   }
 else
  if (VP->val == NIL) strcpy(VP->valstr,"--.--");
  else
   {
    if (VP->val>1231) VP->val = 1231;
	if (VP->val<101)  VP->val = 101;

	sprintf(VP->valstr,"%02u.%02u",(VP->val%100),(VP->val/100));
   }

 
return VP->valstr;
}
/*
char* typ_SHOW_DATA_VALUE(struct TValue_param *VP, bit Format_data)
{
 unsigned int math,dey,year;

 VP->valstr[0] = 0; 

 if (VP->val == NIL) strcpy(VP->valstr,"--.--.--");
 else
  {
   dey  = VP->val&0x10;
   math = (VP->val>>5)&0x08;
   year = VP->val>>9;
   sprintf(VP->valstr,"%02u.%02u.%02u",dey,math,year);

  }
return VP->valstr;
}*/


//----------------------------------------------------------------//

char* typ_SHOW_COUNTTIME_VALUE(TValue_param *VP, char Separator)
{
 unsigned int v;
 
 VP->valstr[0] = 0; 
 if (VP->val == NIL)  strcpy(VP->valstr,"----"); 
 else
  {
   v = VP->val/60; 
   if (v>99) v = 99;

   sprintf(VP->valstr,"%02u%02u",v,(VP->val%60));
   
  }

typ_STR_Insert(VP->valstr,Separator,2);
return VP->valstr;
}



//----------------------------------------------------------------//

char* typ_SHOW_PAROL(TValue_param *VP, unsigned char SHOW)
{
 if (VP->val==0) strcpy(VP->valstr,"----");
 else
  if (SHOW>0) return typ_SHOW_VALUE_0(VP,0,(TBit)1);
  else strcpy(VP->valstr,"****");

return VP->valstr;
}

//----------------------------------------------------------------//

unsigned char typ_Ret_Enum_El(unsigned char id)
{
 unsigned char i=0;
 while (i<COUNT_ENUM) 
    if (Enum_val[i].Id==id) break; else i++;
  if (i==COUNT_ENUM) return nil;  //элемент не найдер

 
return i;
} 

//----------------------------------------------------------------//

unsigned  typ_Ret_Next_V(TValue_param *VP,unsigned char nomer)
{
 unsigned char id=Enum_val[nomer].Id;
 while (nomer<COUNT_ENUM)
    if (nomer!=last_enum)
	  if (Enum_val[nomer].Id!=id) return Enum_val[nomer-1].V;
	  else if (Enum_val[nomer].V==VP->val) 
	          if ((Enum_val[nomer].Id==Enum_val[nomer+1].Id)&&(Enum_val[nomer+1].V<=VP->max))
			    return Enum_val[nomer+1].V;
	          else return VP->max;//minval;//Enum[nomer].V;
           else nomer++;
    else 
	  if (Enum_val[nomer].Id==id) return Enum_val[nomer].V;
	  else return Enum_val[nomer-1].V;
return 0;
} 

//----------------------------------------------------------------//

unsigned  typ_Ret_Pred_V(TValue_param *VP,unsigned char nomer)
{
 unsigned char id=Enum_val[nomer].Id;
 unsigned char n=nomer;
 while (nomer<COUNT_ENUM)
	if (Enum_val[nomer].Id!=id) return Enum_val[n].V;
	else if (Enum_val[nomer].V==VP->val) 
            if (nomer==0) return Enum_val[0].V;
			else
	            if ((Enum_val[nomer].Id==Enum_val[nomer-1].Id)&&(Enum_val[nomer-1].V>=VP->min)) 
			     return Enum_val[nomer-1].V;
	            else return VP->min;//Enum[nomer].V;
          else if (nomer!=last_enum) nomer++;
		       else return Enum_val[n].V;
return 0;  
} 

//----------------------------------------------------------------//

void typ_INC_ENUM(TValue_param *VP,unsigned char type_edit)
{
 unsigned char nomer;
 nomer = typ_Ret_Enum_El(type_edit);
 if (nomer==nil) return;
 
 VP->val = typ_Ret_Next_V(VP,nomer);
 
} 

//----------------------------------------------------------------//

void typ_DEC_ENUM(TValue_param *VP,unsigned char type_edit)
{
 unsigned char nomer;
 nomer = typ_Ret_Enum_El(type_edit);
 if (nomer==nil) return;

 VP->val = typ_Ret_Pred_V(VP,nomer);
} 

//----------------------------------------------------------------//

void typ_INC_DIGITVAL(TValue_param *VP)
{
 unsigned char j;
 unsigned int k=1;
 for (j=1;j<=typ_DIGIT;j++) k = k*10;
 if ((VP->max-VP->val)>=k) VP->val+= k;else VP->val = VP->max;
}	

//----------------------------------------------------------------//

void typ_DEC_DIGITVAL(TValue_param *VP)
{
 unsigned char j;
 unsigned int k=1;
 for (j=1;j<=typ_DIGIT;j++) k = k*10;
 if ((VP->val-VP->min)>=k) VP->val-= k;else VP->val = VP->min;
}

//----------------------------------------------------------------//

void typ_INC_DATAVALUE(TValue_param *VP)
{
unsigned char MM=0,DD=0;  // счетчики месецев и дней
 MM = VP->val/100;
 DD = VP->val-MM*100;
 
 if (typ_DIGIT==0) 
   if (MM<12) MM++;
   else MM=1;// 
 
 if (typ_DIGIT==1)
 
   if (DD<31) DD++;
   else DD=1;
 
VP->val = MM*100+DD;

}

//----------------------------------------------------------------//

void typ_DEC_DATAVALUE(TValue_param *VP)
{
unsigned char MM=0,DD=0;  // счетчики месецев и дней
 MM = VP->val/100;
 DD = VP->val-MM*100;

 if (typ_DIGIT==0) 
   if (MM>1) MM--;
   else MM=12;
      

 if (typ_DIGIT==1)
   if (DD>1) DD--;
   else DD=31;
 
  
 VP->val = MM*100+DD;

}

//----------------------------------------------------------------//

void typ_INC_TIMEVALUE(TValue_param *VP)
{
unsigned char MM=0,HH=0;  // счетчики минут и часов
 HH = VP->val/100;
 MM = VP->val-HH*100;

 if (typ_DIGIT==0) 
   if (MM<59) MM++;
   else MM=0;// минуты

 if (typ_DIGIT==1)
   if (HH<23)  HH++;
   else HH=0;
  

 
VP->val = HH*100+MM;
}

//----------------------------------------------------------------//

void typ_DEC_TIMEVALUE(TValue_param *VP)
{
unsigned char MM=0,HH=0;  // счетчики минут и часов
 HH = VP->val/100;
 MM = VP->val-HH*100;

 if (typ_DIGIT==0) 
   if (MM>0) MM--;
   else MM=59;// минуты

 if (typ_DIGIT==1)
   if (HH>0)  HH--;
   else HH=23;
  

 
VP->val = HH*100+MM;
}


void typ_INC_ZnachCountTime(TValue_param *VP)
{
 unsigned char MM;
 unsigned char HH;  // счетчики минут и часов
 HH=VP->val/60;
 MM=VP->val%60;
 if (typ_DIGIT==0) 
    if (MM<59) MM++;
    else MM=0;// минуты   
 if (typ_DIGIT==1) 
    if (MM<50) MM+=10;
    else MM=MM%10; 
 if (typ_DIGIT==2) HH++;  
 if (typ_DIGIT==3) HH+=10; 
 if (HH>99) HH=99;
 VP->val=HH*60+MM;

 if (VP->val>VP->max) VP->val = VP->max;
 if (VP->val<VP->min) VP->val = VP->min;
}

void typ_DEC_ZnachCountTime(TValue_param *VP)
{
 unsigned char MM;  // счетчики минут и часов
 unsigned char HH;  // счетчики минут и часов
 HH=VP->val/60;
 MM=VP->val%60;
 if (typ_DIGIT==0) 
    if (MM>0) MM--;
   else MM=59;// минуты
 if (typ_DIGIT==1) 
    if (MM>9) MM-=10;
    else MM=MM+50;   
 if (typ_DIGIT==2) 
  if (HH>0) HH--;
 if (typ_DIGIT==3) 
  if (HH>=10) HH-=10; 

 VP->val=HH*60+MM;

 if (VP->val>VP->max) VP->val = VP->max;
 if (VP->val<VP->min) VP->val = VP->min;

}

//----------------------------------------------------------------//уменьшение редактируемого разряда
/*
void typ_DEC_DIGIT(unsigned char type_edit)
{
 if (type_edit<4) 
  if (typ_DIGIT>0) typ_DIGIT--;

 if ((type_edit>=4)&&(type_edit<=7))
  if (typ_DIGIT>0) typ_DIGIT--;

 if ((type_edit==16)||(type_edit==17))
  if (typ_DIGIT>0) typ_DIGIT--;
 
 if ((type_edit==18)||(type_edit==19))
  if (typ_DIGIT>0) typ_DIGIT--;

 
}	*/

//----------------------------------------------------------------//увеличение редактируемого разряда
//вовращает  typ_TRUE если редактируемый разряд последний (старший).
TBit typ_INC_DIGIT(unsigned char type_edit)
{
 unsigned char n;
 n = typ_RET_DIGIT(typVALUE_PARAM.max)-1;

 if (type_edit<4)
  if (typ_DIGIT<n) typ_DIGIT++;
  else			   return typ_TRUE;

 if ((type_edit>=4)&&(type_edit<=7))
  if (typ_DIGIT<3) typ_DIGIT++;
  else			   return typ_TRUE;

 if ((type_edit==16)||(type_edit==17))
  if (typ_DIGIT<3) typ_DIGIT++;
  else			   return typ_TRUE;

 if ((type_edit==19)||(type_edit==18))
  if (typ_DIGIT<1) typ_DIGIT++; 
  else			   return typ_TRUE;

 if ((type_edit>24)&&(type_edit<128)) return typ_TRUE;
 

  return   typ_FALSE;
}

//----------------------------------------------------------------//инкремент значения

void typ_INC_VALUR(TValue_param *VP,unsigned char type_edit)
{
 if (type_edit>24)  typ_INC_ENUM(VP,type_edit);

 if (type_edit<10)   typ_INC_DIGITVAL(VP);

 if ((type_edit==16)||(type_edit==17)) typ_INC_ZnachCountTime(VP);
 
 if (type_edit==19) typ_INC_DATAVALUE(VP);	

 if (type_edit==18) typ_INC_TIMEVALUE(VP);

  
}

//----------------------------------------------------------------//декремент значения
void typ_DEC_VALUR(TValue_param *VP,unsigned char type_edit)
{
 if (type_edit>24)  typ_DEC_ENUM(VP,type_edit);

 if (type_edit<10)   typ_DEC_DIGITVAL(VP);

 if ((type_edit==16)||(type_edit==17)) typ_DEC_ZnachCountTime(VP);

 if (type_edit==19) typ_DEC_DATAVALUE(VP);	

 if (type_edit==18) typ_DEC_TIMEVALUE(VP);


}
/*
unsigned char typ_GET_OFFSET_DIGIT(unsigned char type_edit)
{
 if (type_edit<4) return typ_DIGIT
  
 if ((type_edit>=4)&&(type_edit<=7))
  if (typ_DIGIT<3) typ_DIGIT++;
  else			   return typ_TRUE;

 if ((type_edit==16)||(type_edit==17))
  if (typ_DIGIT<3) typ_DIGIT++;
  else			   return typ_TRUE;

 if ((type_edit==19)||(type_edit==18))
  if (typ_DIGIT<1) typ_DIGIT++; 
  else			   return typ_TRUE;

  return   typ_FALSE;
} */

