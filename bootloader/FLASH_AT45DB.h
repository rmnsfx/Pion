#ifndef _FLASH_AT_45DB
#define _FLASH_AT_45DB

unsigned char   AT45DB_INIT(void);
void 			AT45DB_CHIP_ERASE(void);
void 			AT45DB_PAGE_ERASE(unsigned int adr_page);
void 			AT45DB_PAGE_SET(unsigned int adr_page, unsigned char set_data);
void 			AT45DB_CHIP_SET(unsigned char set_data);			

void 			AT45DB_READ_PAGE(unsigned int adr_page, unsigned char *data);
void 			AT45DB_WRITE_PAGE(unsigned int adr_page, const unsigned char  * data);
void 			AT45DB_READ_DATA(unsigned int adr_page, unsigned short offset, unsigned char *buf, unsigned short size);//читаем данные впределах одной страницы
void 			AT45DB_WRITE_DATA(unsigned int adr_page, unsigned short offset, unsigned char *buf, unsigned short size);//меняем данные впределах одной страницы

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <o>Выбор DataFlash  <1=> AT45DB161D  <2=> AT45DB321D
#define DataFlashType   2
//   <h>asdf 
//	 </h>
#define HREN   2

 
#define AT45DB321D		
//#define AT45DB161D 

#ifdef  AT45DB321D				    //начало AT45DB321D 


	#define AT45DB_FAMILY_CODE				0x27												//индентификатор флэш AT45DB321D
	#define AT45DB_PAGE_SIZE				512													//размер страницы в байтах
	#define AT45DB_PAGE_COUNT				8192												//кол-во страниц во флэщ
	#define AT45DB_FLASH_SIZE				((unsigned long)AT45DB_PAGE_COUNT*AT45DB_PAGE_SIZE) //размер флэш в байтах 4Мб

	#define AT45DB_CHIP_MASK				(AT45DB_PAGE_COUNT-1)
    #define AT45DB_PAGE_MASK				(AT45DB_PAGE_SIZE-1)

	#define _DEVICE_ID_READ 				0x9F
	#define _MEMORY_PAGE_READ				0xD2
	#define _CONTINUOUS_ARRAY_READ_LF		0x03
	#define _CONTINUOUS_ARRAY_READ_HF		0x0B
	#define _BUF1_READ_LF					0xD1
	#define _BUF2_READ_LF					0xD3
	#define _BUF1_READ						0xD4
	#define _BUF2_READ						0xD6

	#define _BUF1_WRITE						0x84
	#define _BUF2_WRITE						0x87
	#define _BUF1_TO_MEM_PROG_ERASE			0x83
	#define _BUF2_TO_MEM_PROG_ERASE			0x86
	#define _BUF1_TO_MEM_PROG_NOT_ERASE		0x88
	#define _BUF2_TO_MEM_PROG_NOT_ERASE		0x89
	#define _PAGE_ERASE						0x81
	#define _BLOCK_ERASE					0x50
	#define _SECTOR_ERASE					0x7C
	#define _CHIP_ERASE1					0xC7
	#define _CHIP_ERASE2					0x94
	#define _CHIP_ERASE3					0x80
	#define _CHIP_ERASE4					0x9A
						
	#define _MEM_PROGRAM_THROUGH_BUF1		0x82
	#define _MEM_PROGRAM_THROUGH_BUF2		0x85

	#define _MEM_TO_BUF1					0x53
	#define _MEM_TO_BUF2					0x55
	#define _MEM_COMPARE_BUF1				0x60
	#define _MEM_COMPARE_BUF2				0x61

	#define _STATUS_READ					0xD7




#endif								//конец AT45DB321D 


#endif
