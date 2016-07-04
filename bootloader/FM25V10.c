#include "FM25V10.h"
#include "SPI.h"
//#include "stm32f10x.h"
//#include "type.h"
#include "Pin.h"

 //unsigned char b;

#define fram_ELECTRONIC_SIGNATURE 0x7F	

void fram_WRITE_ENABLE(void)
{
  pin_SPI_CS(LOW);

  SPI_SendByte(OPCODE_WREN);  //разрешить запись

  pin_SPI_CS(HIGTH);
}


TStatus fram_SETUP(void)
{
  SPI_SETUP();

  if (fram_GET_SIGNATURE()==fram_ELECTRONIC_SIGNATURE)
   {
    fram_WRITE_ENABLE();
    return _OK;
   }
  
  return _ERR;
}

BYTE fram_GET_BYTE(BYTE opcode)
{
  BYTE ret = 0;

  pin_SPI_CS(LOW);
   
  SPI_SendByte(opcode);
  ret = SPI_ReadByte();
  
  pin_SPI_CS(HIGTH);
  return ret;
}

void fram_READ(DWORD addr, BYTE *buffer, DWORD size)
{
  
  addr = addr&fram_MAX_ADRES; 

  if ((fram_MEMORY_SIZE-addr)<size) 
   size = fram_MEMORY_SIZE-addr;
  
  pin_SPI_CS(LOW);

  SPI_SendByte(OPCODE_READ);

 /* b =  (addr>>16)&0x01;
  b =  (addr>>8)&0xFF;
  b =  (addr)&0xFF;*/
  SPI_SendByte((addr>>16)&0x01);
  SPI_SendByte((addr>>8)&0xFF);
  SPI_SendByte((addr)&0xFF);

  while (size--)
   {
   	*buffer = SPI_ReadByte();
	buffer++;
   }

  pin_SPI_CS(HIGTH);
}

void fram_WRITE(DWORD addr, BYTE *buffer, DWORD size)
{
  addr = addr&fram_MAX_ADRES; 

  if ((fram_MEMORY_SIZE-addr)<size) 
   size = fram_MEMORY_SIZE-addr;

  fram_WRITE_ENABLE();

  pin_SPI_CS(LOW);

  SPI_SendByte(OPCODE_WRITE);
  SPI_SendByte((addr>>16)&0x01);
  SPI_SendByte((addr>>8)&0xFF);
  SPI_SendByte((addr)&0xFF);

  while (size--)
   {
   	SPI_SendByte(*buffer);
	buffer++;
   }

  pin_SPI_CS(HIGTH);
}

void fram_ERASE_BULK(DWORD addr, DWORD size)
{
  addr = addr&fram_MAX_ADRES; 

  if ((fram_MEMORY_SIZE-addr)<size) 
   size = fram_MEMORY_SIZE-addr;

  fram_WRITE_ENABLE();

  pin_SPI_CS(LOW);

  SPI_SendByte(OPCODE_WRITE);
  SPI_SendByte((addr>>16)&0x01);
  SPI_SendByte((addr>>8)&0xFF);
  SPI_SendByte((addr)&0xFF);

  while (size--)
   {
   	SPI_SendByte(0);
   }

  pin_SPI_CS(HIGTH);
}
 

