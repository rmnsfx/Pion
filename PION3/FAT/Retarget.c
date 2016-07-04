/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    RETARGET.C 
 *      Purpose: Retarget low level functions
 *      Rev.:    V4.12
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <rt_misc.h>
#include <rt_sys.h>
#include <File_Config.h>

//#pragma import(__use_no_semihosting_swi)

/* The following macro definitions may be used to translate this file:

  STDIO - use standard Input/Output device
          (default is NOT used)
 */

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

struct __FILE { int handle; /* Add whatever you need here */ };

#ifdef STDIO
 extern int sendchar (int ch);
 extern int getkey (void);
#endif

/*--------------------------- _ttywrch --------------------------------------*/

void _ttywrch (int ch) {
#ifdef STDIO
  sendchar(ch);
#endif
}

/*--------------------------- _sys_open -------------------------------------*/

FILEHANDLE _sys_open (const char *name, int openmode) {
	__disable_irq();
  /* Register standard Input Output devices. */
  if (strcmp(name, "STDIN") == 0) {
		__enable_irq();
    return (STDIN);
  }
  if (strcmp(name, "STDOUT") == 0) {
		__enable_irq();
    return (STDOUT);
  }
  if (strcmp(name, "STDERR") == 0) {
		__enable_irq();
    return (STDERR);
  }
	__enable_irq();
  return (__fopen (name, openmode));
	
}

/*--------------------------- _sys_close ------------------------------------*/

int _sys_close (FILEHANDLE fh) {
	__disable_irq();
  if (fh > 0x8000) {
		__enable_irq();
    return (0);
  }
	__enable_irq();
  return (__fclose (fh));
	
}

/*--------------------------- _sys_write ------------------------------------*/

int _sys_write (FILEHANDLE fh, const U8 *buf, U32 len, int mode) {
	__disable_irq();
#ifdef STDIO
  if (fh == STDOUT) {
    /* Standard Output device. */
    for (  ; len; len--) {
      sendchar (*buf++);
    }
		__enable_irq();
    return (0);
  }
#endif
  if (fh > 0x8000) {
		__enable_irq();
    return (-1);
  }
	__enable_irq();
  return (__write (fh, buf, len));
	__enable_irq();
}

/*--------------------------- _sys_read -------------------------------------*/

int _sys_read (FILEHANDLE fh, U8 *buf, U32 len, int mode) {
	__disable_irq();
#ifdef STDIO
  if (fh == STDIN) {
    /* Standard Input device. */
    for (  ; len; len--) {
      *buf++ = getkey ();
    }
		__enable_irq();
    return (0);
  }
#endif
  if (fh > 0x8000) {
		__enable_irq();
    return (-1);
  }
	__enable_irq();
  return (__read (fh, buf, len));
	
}

/*--------------------------- _sys_istty ------------------------------------*/

int _sys_istty (FILEHANDLE fh) {
	__disable_irq();
  if (fh > 0x8000) {
		__enable_irq();
    return (1);
  }
	__enable_irq();
  return (0);
	
}

/*--------------------------- _sys_seek -------------------------------------*/

int _sys_seek (FILEHANDLE fh, long pos) {
	__disable_irq();
  if (fh > 0x8000) {
		__enable_irq();
    return (-1);
  }
	__enable_irq();
  return (__setfpos (fh, pos));
	
}

/*--------------------------- _sys_ensure -----------------------------------*/

int _sys_ensure (FILEHANDLE fh) {
	__disable_irq();
  if (fh > 0x8000) {
    return (-1);
  }
  return (__flushbuf (fh));
	__enable_irq();
}

/*--------------------------- _sys_flen -------------------------------------*/

long _sys_flen (FILEHANDLE fh) {
	__disable_irq();
  if (fh > 0x8000) {
    return (0);
  }
  return (__get_flen (fh));
	__enable_irq();
}

/*--------------------------- _sys_tmpnam -----------------------------------*/

int _sys_tmpnam (char *name, int sig, unsigned maxlen) {
  return (1);
}

/*--------------------------- _sys_command_string ---------------------------*/

char *_sys_command_string (char *cmd, int len) {
  return (cmd);
}

/*--------------------------- _sys_exit -------------------------------------*/

void _sys_exit (int return_code) {
  /* Endless loop. */
  while (1);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
