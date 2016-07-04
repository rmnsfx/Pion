#include "stm32f10x.h"
#include "Backup.h"

bkp_status_t bkp_SETUP(void)
{ 
 /* Enable PWR and BKP clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  /* Enable write access to Backup domain */
   /* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);

  
  return bkp_RESET;
   

  return bkp_NO_RESET;
}

