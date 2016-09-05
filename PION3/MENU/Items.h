

#define MAX_ITEMS     50		//количество элементов меню
#define COUNT_ENUM    6		//количество перечилсяемых элементов
#define last_enum     COUNT_ENUM - 1




//опции
//#define   O_REF	   0x01 //1 - обновлять/0 - не обновлять
#define   O_LEFT   0x02 //1 - выравнивание по левому краю/0 - выравнивание по правому краю
#define   O_SZ	   0x04 //1 - показывать не значащие нули/0 - нет
#define   O_HR	   0x08 //1 - уставка(HOLD)/0 - измеряемый(INPUT)
#define   O_RW	   0x10 //1 - доступно редактирование/0 - только чтение
		
//уровни доступа
#define   L_ACCES0 0    //уровень доступа 0
#define   L_ACCES1 1    //уровень доступа 1
#define   L_ACCES2 2    //уровень доступа 2

//39 байт
typedef struct { 
  unsigned char Id_group;                    //идентификатор группы
  		   char Text_0[21];                  //имя параметра, группы
		   char Text_1[21];                  //имя параметра, группы
  		   char Caption[21];					 //сокращенное имя параметра
  		   char MeasureText[4];  			 //еденица измерения
  //unsigned char Data_reg;                    //номер параметра
  unsigned char Options;                     //опции параметра 
  unsigned char Typedata;                    /*
                                             FFh - заглушка 
                                             FEh - меню
                                             FDh 
											 
											 
                                             0 - xxxxx  
				                             1 - xxxx.x
				                             2 - xxx.xx
											 3 - xx.xxx
											 4 - (+/-)xxxx   //знаковое значение (0x0000 - максималное отрцательное;0xFFFF - максимальное положительное 0x8000- ноль
                                             5 - (+/-)xxx.x
											 6 - xxxx вывод и редактирование пароля
											 7 - xxxx ввод пароля

											 10 - команда

											 16 - xxЧxx   //количество минут переводит в часы/минуты     
											 17 - xxМxx   //количество секунд переводит в минуты/секунды
                                             18 - время   //часы,минуты
											 19 - дата    //месяц,число
											 
				                             25-150 - перечисляемый тип

											*/
                    
  unsigned char LevelAcces;                 /*уровень доступа к параметрам
      					                     00 - доступен для редактирования на всех уровнях
					                         01 - доступен для редактирования на 1 уровне
					                         02 - доступен для редактирвоания на 2 уровне
  											
				                             */
  unsigned char Config;                      //маска конфигурации 
  unsigned char Data_reg;                 //указатель на группу или адрес параметра
  } TItem;

typedef struct  {                         //определяем структуру для перечисляемого типа
  unsigned char Id;                          //
  unsigned char V;
  		   char S[20];
  } TEnum_val;


extern const TItem     Items[MAX_ITEMS];
extern const TEnum_val Enum_val[COUNT_ENUM];



