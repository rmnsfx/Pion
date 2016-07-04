

#define key_TIME_BOUNCE 30//15							//мсек
//события
#define key_EVENT_PRESSED_UP			0x00000001
#define key_EVENT_PRESSED_DOWN			0x00000002
#define key_EVENT_PRESSED_MESUARE		0x00000004
#define key_EVENT_PRESSED_ESC_MENU		0x00000008
#define key_EVENT_PRESSED_ENTER			0x00000010
#define key_EVENT_RELEASED_UP			0x00000020
#define key_EVENT_RELEASED_DOWN			0x00000040
#define key_EVENT_RELEASED_MESUARE		0x00000080
#define key_EVENT_RELEASED_ESC_MENU		0x00000100
#define key_EVENT_RELEASED_ENTER		0x00000200
#define key_EVENT_PRESSED_UP_REPEAT

//состояние
#define key_NO			0
#define key_UP			1
#define key_DOWN		2
#define key_MESUARE		4
#define key_ESC_MENU	8
#define key_ENTER		16

typedef enum 
 {
  KEY_EVENT,
  KEY_NO_EVENT
 } TKEY_EVENT;

void 		key_1ms(void);
TKEY_EVENT 	key_CHECK_EVENT(unsigned long mask_event);
TKEY_EVENT	key_CHECK_STATUS(unsigned char mask_state);
//void 		key_RES_EVENT(unsigned long mask_event);

#define 	key_CHECK_EV(mask)	(key_CHECK_EVENT(mask)==KEY_EVENT)
#define 	key_CHECK_ST(mask)  (key_CHECK_STATUS(mask)==KEY_EVENT)



