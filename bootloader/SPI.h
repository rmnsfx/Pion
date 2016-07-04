#include "Pin.h"

#define DUMMY 0
#define SPI_ReadByte() 	SPI_SendByte(DUMMY)
//#define SPI_Read(buf,len) {while (len--) buf = SPI_ReadByte()};

#define SPI_ON()				SPI_Cmd(SPI1, ENABLE)	
#define SPI_OFF()				SPI_Cmd(SPI1, DISABLE);

void SPI_SETUP(void);
u8 SPI_SendByte(u8 byte);






