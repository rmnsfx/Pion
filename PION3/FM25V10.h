#ifndef FM25V10_H
#define FM25V10_H

#include "stm32f10x.h"
#include "type.h"
#include "main.h"

#define fram_MEMORY_SIZE	(128*1024)	  //128Í¡
#define fram_MAX_ADRES		0x0001FFFF

#define OPCODE_WREN		0x06 //Set Write Enable Latch
#define OPCODE_WRDI		0x04 //Write Disable
#define OPCODE_RDSR		0x05 //Read Status Register
#define OPCODE_WRSR		0x01 //Write Status Register
#define OPCODE_READ		0x03 //Read Memory Data
#define OPCODE_FSTRD	0x11 //Fast Read Memory Data
#define OPCODE_WRITE	0x02 //Write Memory Data
#define OPCODE_SLEEP	0xB9 //Enter Sleep Mode
#define OPCODE_RDID		0x9F //Read Device ID
#define OPCODE_SNR		0xC3 //Read S/N

TStatus fram_SETUP(void);
BYTE  	fram_GET_BYTE(BYTE opcode);
void  	fram_WRITE(DWORD addr, BYTE *buffer, DWORD size);
void  	fram_READ(DWORD addr, BYTE *buffer, DWORD size);
void  	fram_ERASE_BULK(DWORD addr, DWORD size);
void  	fram_WRITE_ENABLE(void);

#define fram_GET_SIGNATURE() fram_GET_BYTE(OPCODE_RDID)	
#define fram_GET_STATUS()	 fram_GET_BYTE(OPCODE_RDSR)	





#endif

