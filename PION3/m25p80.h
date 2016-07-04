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
 
#ifndef M25P80_H
#define M25P80_H

#include "stm32f10x.h"
#include "type.h"
 
#define M25P80_PAGE_SIZE              256  /* bytes */
#define M25P80_PAGE_NUMBER            4096
#define M25P80_SECTOR_SIZE            256  /* pages */
#define M25P80_SECTOR_NUMBER          16
 
BYTE  m25p80_SETUP(void);
BYTE  m25p80_GET_SIGNATURE(void);
void  m25p80_BLOCK_WIP(void); 
BYTE  m25p80_GET_STATE(void); 
void  m25p80_WAKEUP(void); 
void  m25p80_POWER_DOWN(void);
 
void  m25p80_ERASE_SECTOR(BYTE sector); 
void  m25p80_ERASE_BULK(void);
void  m25p80_WRITE(DWORD addr, BYTE *buffer, WORD size);
void  m25p80_READ(DWORD addr, BYTE *buffer, WORD size); 


#define m25p80_save_page(page, buffer) m25p80_write( ((uint32_t)(page))<<8, (buffer), 256)
 
#define m25p80_load_page(page, buffer) m25p80_read ( ((uint32_t)(page))<<8, (buffer), 256)
 
#endif
