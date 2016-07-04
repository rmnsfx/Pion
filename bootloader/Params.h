#ifndef _PARAMS
#define _PARAMS

typedef struct 
 {
   unsigned short 	Min;
   unsigned short 	Max;
   unsigned short 	Def;
   unsigned char 	CopyFlash;
 } TPARAMS;

extern const TPARAMS PARAMS[];

#endif
