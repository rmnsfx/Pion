#include "stm32f10x.h"
#include "diskio.h"

//BOOL mmc_init (void);
//BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt);
//BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt);
//BOOL mmc_read_config (MMCFG *cfg);
//BOOL mmc_format(void);

void flash_erase_page(uint32_t address) ;
uint32_t flash_read(uint32_t address); 
void flash_write(uint32_t address,uint32_t data); 

void flash_unlock(void);
void flash_lock();
uint8_t flash_ready(void);