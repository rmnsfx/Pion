
bit FlashReadStatus();

void ReadFlash(unsigned long addr,  void *destination, unsigned char count); //
void WriteFlash(unsigned long addr, void *source, unsigned char count);
void SPI_Init (void);
bit  FLASH_INIT();
/*
void SPIWriteBuf(unsigned char byteaddr, unsigned char *source, unsigned char count);
void SPIBufRead(unsigned char byteaddr, unsigned char *destination, unsigned char count);
*/


void SPIWriteBuf(unsigned long addr, unsigned char *source, unsigned char count);
void SPIBufRead(unsigned long addr, unsigned char *destination, unsigned char count);