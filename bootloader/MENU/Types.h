

#define maxlenval 	20+1

#define NIL 		0xFFFF		//пустое значени
#define ERR_VAL 	0xFFFE		//значение ошибки

typedef struct 
 					{
					 char 		  valstr[maxlenval];
					 unsigned short val;
					 unsigned short max;
					 unsigned short min;
					 unsigned short def; 
					} TValue_param;

typedef enum {typ_TRUE=1,typ_FALSE=0} TBit;


extern TValue_param typVALUE_PARAM;
extern char   typ_DIGIT;

char* typ_FORMAT_STR(char *STR,TBit LEFT,unsigned char size_array);
char* typ_SHOW_VALUE_0(TValue_param *VP,unsigned char Point, TBit SHOW_ZERO);
char* typ_SHOW_ENUM_VALUE(TValue_param *VP, unsigned char id);
char* typ_SHOW_DATA_VALUE(TValue_param *VP, TBit Format_data);
char* typ_SHOW_TIME_VALUE(TValue_param *VP);
char* typ_SHOW_PAROL(TValue_param *VP, unsigned char SHOW);
char* typ_SHOW_ZNAK(TValue_param *VP,unsigned char Point, TBit SHOW_ZERO);
char* typ_SHOW_COUNTTIME_VALUE(TValue_param *VP, char Separator);

//void typ_DEC_DIGIT(unsigned char type_edit);
TBit typ_INC_DIGIT(unsigned char type_edit);
void typ_INC_VALUR(TValue_param *VP,unsigned char type_edit);
void typ_DEC_VALUR(TValue_param *VP,unsigned char type_edit);

