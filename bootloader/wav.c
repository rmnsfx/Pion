#include <File_Config.h>
#include <stdio.h>
#include "wav.h"



S8 dat_CreateFile(const char *FileName, void *Frame, U32 LengthFrame, float *k_norm)
{
  FILE 		   *pFile;

  fdelete(FileName);

  pFile = fopen (FileName,"w");

  if (pFile==0) return -1;
  else
   if (fwrite(k_norm,1,4,pFile)==4)
    if (fwrite(Frame,1,LengthFrame*2,pFile)==LengthFrame);
 	 {
	  fclose(pFile);
	  return 0; 
	 }

 fclose(pFile);
 return -2;

}


S8 wav_CreateFile(const char *FileName, void *Frame, U32 LengthFrame, U8 Channels, U32 SamplesPerSec)
{
 IDRiff RIFF;
 IDWave WAV;
 IDSampleWave  Sample;
 FILE 		   *pFile;
 //int st;

 RIFF.Riff 		= 0x46464952;
 RIFF.Len  		= sizeof(IDWave)+sizeof(IDSampleWave)+(Channels*2*LengthFrame);
 WAV.Wav 		= 0x45564157;
 WAV.Fmt 		= 0x20746D66;
 WAV.Len 		= 0x0010;
 WAV.Type 		= 0x0001;
 WAV.Channels 	= Channels;	
 WAV.SamplesPerSec = SamplesPerSec;
 WAV.AvgBytesPerSec = SamplesPerSec*WAV.Channels*2;
 WAV.Align		= (16*Channels)/8; 
 WAV.Bits		= 0x0010;
 
 Sample.Data	= 0x61746164;
 Sample.Len		= Channels*2*LengthFrame;

 fdelete(FileName);
 pFile = fopen (FileName,"w");

 if (pFile==0) return -1;
 else
  if (fwrite (&RIFF,1,sizeof(RIFF),pFile)==sizeof(RIFF))
   if (fwrite (&WAV,1,sizeof(WAV),pFile)==sizeof(WAV));
 	if (fwrite (&Sample,1,sizeof(Sample),pFile)==sizeof(Sample));
 	 if (fwrite (Frame,1,Sample.Len,pFile)==Sample.Len);
 		{
		 fclose(pFile);
		 return 0; 
		}

 fclose(pFile);
 return -2;
}  

