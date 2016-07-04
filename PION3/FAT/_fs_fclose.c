/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    _FS_FCLOSE.C 
 *      Purpose: Low level File Close Function
 *      Rev.:    V4.12
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "File_Config.h"

/*--------------------------- __fclose --------------------------------------*/
 
int __fclose (int handle) {
 
  FALLOC alloc;
  IOB *fcb;

 START_LOCK (int);

  fcb = &_iob[handle];
  if (fcb->drive == DRV_MCARD) {
   
    if (fcb->flags & _IOWRT) {
      if (fat_close_write (fcb) == __FALSE) {
        fcb->fileID = 0;
        fcb->flags  = 0;
        RETURN (-1);
      }
    }
  }
  else if ((fcb->flags & _IOWRT) && (fcb->flags & _IOWALLOC)) {
    
    alloc.end    = fcb->_fbot;
    alloc.fileID = fcb->fileID;
    alloc.index  = fcb->_fidx;
    fs_WriteBlock (((DEVCONF *)fcb->DevCfg)[fcb->_fblock].bStart + fcb->_ftop,
                    &alloc, sizeof (FALLOC), fcb);
  }
  fcb->fileID = 0;
  fcb->flags  = 0;
  RETURN (0);

  END_LOCK;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

