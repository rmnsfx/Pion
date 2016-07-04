/*
 * Copyright  2008-2009 INRIA/SensTools
 * 
 * <dev-team@sentools.info>
 * 
 * This software is a set of libraries designed to develop applications
 * for the WSN430 embedded hardware platform.
 * 
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
*/
 
//#include <io.h>
//#include <signal.h>
 
#include "m25p80.h"
#include "SPI.h"
//#include "stm32f10x.h"
//#include "type.h"
#include "Pin.h"
 
//#define DUMMY 0x0
 
/**************************************************/
/**************************************************/
 
//#define M25P80_PORT_WPR  P1OUT /* 1.2 write enable */
//#define M25P80_PORT_H    P4OUT /* 4.7 Hold         */
 
/* on MSP430_PORT 1                 */
//#define M25P80_PIN_PROTECT    2
 
/* on MSP430_PORT 4                 */
//#define M25P80_PIN_CS         4
//#define M25P80_PIN_HOLD       7
 
#define BITMASK(n)      (1 << n)

//#define M25P80_BIT_HOLD    BITMASK(M25P80_PIN_HOLD)
//#define M25P80_BIT_PROTECT BITMASK(M25P80_PIN_PROTECT)
 
/* hold is negated : active low */
/* Bit : 0 = hold               */
/*       1 = open               */
//#define M25P80_HOLD_ENABLE()   do { M25P80_PORT_H &= ~M25P80_BIT_HOLD; } while(0)
//#define M25P80_HOLD_DISABLE()  do { M25P80_PORT_H |=  M25P80_BIT_HOLD; } while(0)
 
/* write protect negated : active low */
/* Bit : 0 = Write protect on         */
/*       1 = Write protect off        */
//#define M25P80_PROTECT_ENABLE()  do { M25P80_PORT_WPR &= ~M25P80_BIT_PROTECT; } while(0)
//#define M25P80_PROTECT_DISABLE() do { M25P80_PORT_WPR |=  M25P80_BIT_PROTECT; } while(0)
 
/**************************************************/
/**************************************************/
 
#define OPCODE_WREN      0x06u /* write enable         */
#define OPCODE_WRDI      0x04u /* */
#define OPCODE_RDSR      0x05u /* read status register */
#define OPCODE_WRSR      0x01u /* */
#define OPCODE_READ      0x03u
#define OPCODE_FAST_READ 0x0Bu
#define OPCODE_PP        0x02u
#define OPCODE_SE        0xd8u
#define OPCODE_BE        0xc7u
#define OPCODE_DP        0xb9u
#define OPCODE_RES       0xabu
 
#define M25P80_ELECTRONIC_SIGNATURE 0x13


#define  spi1_write_single(a) 1;
#define  spi1_select(a)		2; 	 
#define  spi1_deselect(a)	3;

#define  spi1_read_single() 4
#define  spi1_write(p,l)	l
#define  spi1_read(p,l)		l

  
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 

BYTE m25p80_SETUP(void)
{
  SPI_SETUP();
  return m25p80_GET_SIGNATURE();
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
BYTE m25p80_GET_SIGNATURE(void)
{
  BYTE ret = 0;

  pin_SPI_CS(LOW);
   
  SPI_SendByte(OPCODE_RES);
  SPI_ReadByte();
  SPI_ReadByte();
  SPI_ReadByte();
  
  ret = SPI_ReadByte();
  
  pin_SPI_CS(HIGTH);
  return ret;
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
/* M25p80 is programmed using Hold, Write and Clock 
 * other information is data or command 
*/
 
BYTE m25p80_GET_STATE(void)
{ 
  BYTE ret = 0;
  pin_SPI_CS(LOW);
  
  SPI_SendByte(OPCODE_RDSR);
  ret = SPI_ReadByte();
   
  pin_SPI_CS(HIGTH);
  return ret; 
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_WAKEUP(void)
{
  pin_SPI_CS(LOW);
  SPI_SendByte(OPCODE_RES);
  pin_SPI_CS(HIGTH);
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_POWER_DOWN(void)
{ 
  pin_SPI_CS(LOW);
  SPI_SendByte(OPCODE_DP);
  pin_SPI_CS(HIGTH);
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_BLOCK_WIP(void)
{
  #define WIP 0x01
   
  //BYTE state;
   
  pin_SPI_CS(LOW);
   
  SPI_SendByte(OPCODE_RDSR);
  
  //state = SPI_ReadByte();
  while(SPI_ReadByte() & WIP) ;
  
    
  pin_SPI_CS(HIGTH);
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void m25p80_WRITE_ENABLE(void)
{
  pin_SPI_CS(LOW);
  SPI_SendByte(OPCODE_WREN);
  pin_SPI_CS(HIGTH);
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_ERASE_SECTOR(BYTE ix)
{
  m25p80_BLOCK_WIP();
  m25p80_WRITE_ENABLE();
  pin_SPI_CS(LOW);

  SPI_SendByte(OPCODE_SE);
  /* Send SectorAddr high nibble address byte */
  SPI_SendByte(0);
  /* Send SectorAddr medium nibble address byte */
  SPI_SendByte(0);
  /* Send SectorAddr low nibble address byte */
  SPI_SendByte(ix & 0xFF);
 
  /*spi1_write_single(OPCODE_SE);
  spi1_write_single(ix & 0xff);
  spi1_write_single(0  & 0xff);
  spi1_write_single(0  & 0xff);*/

  pin_SPI_CS(HIGTH);
   
  m25p80_BLOCK_WIP();
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_ERASE_BULK(void)
{
  m25p80_BLOCK_WIP(); 
  m25p80_WRITE_ENABLE();
  
  pin_SPI_CS(LOW);
  SPI_SendByte(OPCODE_BE);
  pin_SPI_CS(HIGTH);
  
  m25p80_BLOCK_WIP(); 
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
void m25p80_WRITE(DWORD addr, BYTE *buffer, WORD size)
{
     
    BYTE *p = buffer, *_end;
    DWORD end = addr + size;
    DWORD i, next_page;
    WORD len;
    
    for(i = addr; i < end;) {
        next_page = (i | 0xff) + 1;
        if(next_page > end)
        {
            next_page = end;
        }
         
        _end = p + (next_page-i);
         
        m25p80_BLOCK_WIP();
        m25p80_WRITE_ENABLE();
        pin_SPI_CS(LOW);
        SPI_SendByte(OPCODE_PP);
        SPI_SendByte((i >> 16) & 0xff);
        SPI_SendByte((i >>  8) & 0xff);
        SPI_SendByte((i >>  0) & 0xff);
        
        len = (_end-p);

		while (len--)
		 {
		  SPI_SendByte(*p);
		  p++;
		 }

        //spi1_write(p, len);	//!!!
        
        p = _end;
        pin_SPI_CS(HIGTH);
        
        i = next_page;
    }
     
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
 
 
void m25p80_READ(DWORD addr, BYTE *buffer, WORD size)
{
  m25p80_BLOCK_WIP();
  pin_SPI_CS(LOW);
 
  SPI_SendByte(OPCODE_READ);
  SPI_SendByte((addr >> 16) & 0xff);
  SPI_SendByte((addr >>  8) & 0xff);
  SPI_SendByte((addr >>  0) & 0xff);

  while (size--)
		 {
		  *buffer = SPI_ReadByte();
		  buffer++;
		 }
  //spi1_read(buffer, size);
  
  pin_SPI_CS(HIGTH);
}
 
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
