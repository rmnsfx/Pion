#include "IIR.h"
#include "stdio.h"

#define ext_adc_STATUS_RUN  1
#define ext_adc_STATUS_STOP	0

#define ext_SAMPLING_SIZE   25000


//extern s16 		  ext_adc_VALUE;
extern d_DETECTOR 	DETECTOR;
extern u8		  	ext_adc_OVER; //индикатор перегруза канала виброускорения
extern s16		  	ext_adc_SIM[];

void ext_adc_SETUP(u16 period_sample);	//period_sample - период между выборками, мкс
void ext_adc_SAMPLING(s16 *data_in, u16 count_sample);
void ext_adc_SAMPLING_RESET(s16 *data_in, u16 count_sample); //сброс
void ext_adc_START(void);
void ext_adc_STOP(void);
u16  ext_adc_READ(void);

void ext_adc_WAIT(void);
u8   ext_adc_GET_STATUS(void);
u8   ext_adc_GET_PROCESS(void); //состояние выборки в %
float* calc_from_dat ( FILE * FTemp );

