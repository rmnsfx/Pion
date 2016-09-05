/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SPI_STM32F103.c
 *      Purpose: Serial Peripheral Interface Driver for ST STM32F103
 *      Rev.:    V4.12
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <File_Config.h>
//#include <stm32f10x_lib.h>                  /* STM32F10x Library Definitions */
#include "stm32f10x.h"						  //исправлено 15.10.12

/* SPI_SR - bit definitions. */
#define RXNE    0x01
#define TXE     0x02
#define BSY     0x80

/*----------------------------------------------------------------------------
 *      SPI Driver Functions
 *----------------------------------------------------------------------------
 *  Required functions for SPI driver module:
 *   - void spi_init     ()
 *   - void spi_ss       (U32 ss)
 *   - U8   spi_send     (U8 outb)
 *   - void spi_hi_speed (BOOL on)
 *---------------------------------------------------------------------------*/

/*--------------------------- spi_init --------------------------------------*/

void spi_init_1 (void) {
  /* Initialize and enable the SSP Interface module. */

  /* Enable clock for SPI, GPIOB and AFIO. */
  RCC->APB2ENR |= 0x00000001; //Alternate function clock enable
  RCC->APB2ENR |= 0x00000008; //PORT B clock enable 
  RCC->APB1ENR |= 0x00004000; //SPI clock enable

  /* Reset SPI remap (use PA4..PA7). */
  //AFIO->MAPR   &= 0xFFFFFFFE;

  /* SPI1_NSS is GPIO, output set to high. */
  GPIOB->BSRR = 0x00001000;

  /* SPI1_SCK, SPI1_MISO, SPI1_MOSI are SPI pins. */
  GPIOB->CRH &= ~0xFFFF0000;
  GPIOB->CRH |=  0xB8B30000;

  /* Card Sensor PA.8 input */
  /* 1 = NO Card, 0 = Card plugged. */
  GPIOC->CRL &= ~0xF0000000;
  GPIOC->CRL |=  0x40000000;

  /* Enable SPI in Master Mode, CPOL=0, CPHA=0. */
  /* Clock speed = fPCLK1 / 256 = 280 kHz at 72 MHz PCLK1 clk. */
  SPI2->CR1  = 0x037C;
  SPI2->CR2  = 0x0000;
}


/*--------------------------- spi_hi_speed ----------------------------------*/

void spi_hi_speed (BOOL on) {
  /* Set a SPI clock to low/high speed for SD/MMC. */

  if (on == __TRUE) {
    /* Max. 25 MBit used for Data Transfer. */
    /* Actual setting 18 MBits. */
    SPI2->CR1 &= ~0x0030;
    SPI2->CR1 |=  0x0008;
  }
  else {
    /* Max. 400 kBit used in Card Initialization. */
    /* Actual setting 280 kBits. */
    SPI2->CR1 |= 0x0038;
  }
}


/*--------------------------- spi_ss ----------------------------------------*/

void spi_ss (U32 ss) {
  /* Enable/Disable SPI Chip Select (drive it high or low). */

  GPIOB->BSRR = ss ? 0x00001000 : 0x10000000;
}


/*--------------------------- spi_send --------------------------------------*/

U8 spi_send (U8 outb) {
  /* Write and Read a byte on SPI interface. */

  /* Wait if TXE cleared, Tx FIFO is full. */
  while (!(SPI2->SR & TXE));
  SPI2->DR = outb;

  /* Wait if RNE cleared, Rx FIFO is empty. */
  while (!(SPI2->SR & RXNE));
  return (SPI2->DR);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
