
#ifndef _rtc_h
#define _rtc_h

#define rtc_YEAR		1	//год
#define rtc_MON			2	//месяц
#define rtc_DAY			3	//день
#define rtc_HOUR		4	//часы
#define rtc_MIN			5	//минуты
#define rtc_SEC			6	//секуды


TStatus rtc_SETUP(void);

void rtc_COPY_RTC_TO_BUFER(void); //скопировать дату время в буфер из RTC
void rtc_COPY_BUFER_TO_RTC(void); //записать дату и время в RTC из буфера

//------- чтение из буфера --------------------------------------------------------//
unsigned short rtc_READ(unsigned char ID);

																	   
//------ запись в буфер -----------------------------------------------------------//
//записать год в абсолютном выражении (>1900)
//записать месяц (1-12)
//записать день  (1-31)
//записать часы  (0-23)
//записать мин (0-59)
//записать сек (0-59)
void rtc_WRITE(unsigned char ID,unsigned short new_val);

//------ событие 1 сек ------------------------------------------------------------//
//возвращает 1 если событие наступило иначе 0
unsigned char rtc_CHECK_EVENT_1S(void);



#endif
