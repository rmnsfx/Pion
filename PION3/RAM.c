
#include <File_Config.h>
#include <string.h>
#include "type.h"
#include "mscuser.h"

extern  BYTE Memory[];

BOOL mmc_init (void)
{
 


return (__TRUE);
}

BOOL mmc_write_sect (U32 sect, U8 *buf, U32 cnt)
{
 if (sect<MSC_BlockCount)
  {
   memcpy(&Memory[sect*512],buf,cnt*512);
   return (__TRUE);
  }
 else 
  return (__FALSE);
}

BOOL mmc_read_sect (U32 sect, U8 *buf, U32 cnt)
{
 if (sect<MSC_BlockCount)
 memcpy(buf,&Memory[sect*512],cnt*512);
 else
 memset(buf,0,cnt*512);
 


return (__TRUE);
}

BOOL mmc_read_config (MMCFG *cfg)
{ 
return (__TRUE);
}
