/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    MSCUSER.H
 *      Purpose: Mass Storage Class Custom User Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __MSCUSER_H__
#define __MSCUSER_H__

#include "type.h"
#include "main.h"

// для форматирования флэш
//#define ClusterPerDisk	3839488//60000// ///3600000 //кластеров на 1 диск	 //полный объем
////#define ClusterPerFat	((ClusterPerDisk)/256+1) //кол-во кластеров на 1 фат, для FAT16, очень грубый расчет...
//#define ClusterPerFat 32768

//#define DISK_OFFSET		1								//смещение в секторах
//#define FAT_OFFSET		(DISK_OFFSET+1)
//#define ROOT_OFFSET		(FAT_OFFSET+ClusterPerFat)
//#define FILEDATA_OFFSET (ROOT_OFFSET+1)				    //область данных


/* Mass Storage Memory Layout */
//#define MSC_MemorySize  ClusterPerDisk*512//8192//8192//1024*512//8192//4096//4096//8192
//#define MSC_BlockSize   512
//#define MSC_BlockCount  (MSC_MemorySize / MSC_BlockSize)
//#define MSC_BlockCount  ClusterPerDisk
//#define MSC_Offset 		1024//смещение внтури Flash

/* Max In/Out Packet Size */
#define MSC_MAX_PACKET  64//64

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x81
#define MSC_EP_OUT      0x02

/* MSC Requests Callback Functions */
extern BOOL MSC_Reset     (void);
extern BOOL MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);

extern BYTE  Memory[];

#endif  /* __MSCUSER_H__ */
