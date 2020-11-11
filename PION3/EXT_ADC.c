#include "Pin.h"
#include "IIR.h"
#include "EXT_ADC.h"
#include "reg.h"

#define ext_adc_CLK_STROB()	{pin_ADC_CLK_RES;pin_ADC_CLK_SET;}

u8 i;
u8 f;

s16 *ext_adc_DATA;
//u16 ext_adc_PERIOD;
u16 ext_adc_COUNT;
u16 ext_adc_XCOUNT;
s16	ext_adc_SIM[ext_SAMPLING_SIZE];
int64_t ext_adc_VALUE;
u16 ext_mode_reg;
uint64_t ext_adc_DELAY;
int64_t  ext_adc_VAL;

u8	ext_adc_OVER = 0; //индикатор перегруза канала виброускорени€

float k_reg_mul = 1;
s16 dec_koef = 1;

//переменные дециматора
int64_t SAMPLE[4];
u8  INDEX;
u8  DECIMATOR;

const s32 SINUS[50]={0,13,25,37,48,59,68,77,84,90,95,98,100,100,98,95,91,84,77,69,59,48,37,25,13,0,-13,-25,-37,-48,-59,-68,-77,-84,-90,-95,-98,-100,-100,-98,-95,-91,-85,-77,-69,-60,-48,-37,-25,-13};


//фильтр „ебышева 1 рода низких частот 3 пор€дка на 300 √ц 0.2 dB нелинейность ј„’, 12.5к√ц /2 секции
d_COEFF_FILTER FILTER_1_ONE_SECTION=	{0.0075419232028630864, 0.0075419232028630864*2,0.0075419232028630864,
									 	 1,						1.8547644173976141,		-0.88493211020906637};
d_COEFF_FILTER FILTER_1_TWO_SECTION=	{0.057971158924053044,  0.057971158924053044, 	 0,
									 	 1,						0.88405768215189395,	 0};
/*
//фильтр „ебышева 1 рода низких частот 3 пор€дка на 1000 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_2_ONE_SECTION	=	{0.020045145495827765,  0.020045145495827765*2,  0.020045145495827765,
									 	 1,						1.7369411598270685,		-0.81712174181037944};
d_COEFF_FILTER FILTER_2_TWO_SECTION	=	{0.09330953224480508,   0.09330953224480508, 	 0,
									 	 1,						0.81338093551038981,	 0};
*/

//фильтр „ебышева 1 рода низких частот 3 пор€дка на 1000 √ц 0.4 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_2_ONE_SECTION	=	{0.017347571111938184,  0.017347571111938184*2,  0.017347571111938184,
									 	 1,						1.777084239324682,		-0.84647452377243482};
d_COEFF_FILTER FILTER_2_TWO_SECTION	=	{0.078117893832150689,   0.078117893832150689, 	 0,
									 	 1,						0.84376421233569854,	 0};


/*
//фильтр „ебышева 1 рода низких частот 3 пор€дка на 2000 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_3_ONE_SECTION	=	{0.071556426424794956,  0.071556426424794956*2,  0.071556426424794956,
									 	 1,						1.3925675496953345,		-0.67879325539451441};
d_COEFF_FILTER FILTER_3_TWO_SECTION	=	{0.17298129565838802,   0.17298129565838802, 	 0,
									 	 1,						0.65403740868322402,	 0};

*/
//фильтр „ебышева 1 рода низких частот 3 пор€дка на 2000 √ц 0.4 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_3_ONE_SECTION	=	{0.063215884374645495,  0.063215884374645495*2,  0.063215884374645495,
									 	 1,						1.4718713679915838,		-0.72473490549016562};
d_COEFF_FILTER FILTER_3_TWO_SECTION	=	{0.14692024016978588,   0.14692024016978588, 	 0,
									 	 1,						0.70615951966042867,	 0};



//фильтр „ебышева 1 рода низких частот 3 пор€дка на 5000 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_4_ONE_SECTION	=	{0.31915372734296987, 	0.31915372734296987*2,  0.31915372734296987,
									 	 1,						0.21709887495801744,	-0.49371378432989704};
d_COEFF_FILTER FILTER_4_TWO_SECTION	=	{0.37180655604847523,   0.37180655604847523, 	 0,
									 	 1,						0.25638688790304948,	 0};



////фильтр „ебышева 1 рода низких частот 3 пор€дка на 5000 √ц 0.4 dB нелинейность ј„’, 25к√ц /2 секции
//d_COEFF_FILTER FILTER_4_ONE_SECTION	=	{0.29866872495474456, 	0.29866872495474456*2,  0.29866872495474456,
//									 	 1,						0.34573057845827115,	-0.54040547827724927};
//d_COEFF_FILTER FILTER_4_TWO_SECTION	=	{0.32765865296189323,   0.32765865296189323, 	 0,
//									 	 1,						0.34468269407621366,	 0};




//фильтр „ебышева 1 рода низких частот 3 пор€дка на 10000 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_5_ONE_SECTION =	{0.79244066840254967, 	0.79244066840254967*2,   0.79244066840254967,
									 	 1,					   -1.4665190212485764,	    -0.70324365236162223};
d_COEFF_FILTER FILTER_5_TWO_SECTION =	{0.71487112950893494,   0.71487112950893494, 	 0,
									 	 1,					   -0.42974225901786994,	 0};

/*
//фильтр „ебышева 1 рода высоких частот 3 пор€дка на 2 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_6_ONE_SECTION =	{0.9998551434467251,   -0.9998551434467251*2,    0.9998551434467251,
									 	 1,					    1.9997101975399763,	    -0.99971037624692438};
d_COEFF_FILTER FILTER_6_TWO_SECTION =	{0.99969157945534226,  -0.99969157945534226, 	 0,
									 	 1,					    0.99938315891068441,	 0};
*/

//фильтр „ебышева 1 рода высоких частот 3 пор€дка на 5.0 √ц 0.5 dB нелинейность ј„’, 12.5к√ц /2 секции
d_COEFF_FILTER FILTER_6_ONE_SECTION =	{0.9993100220600506,   -0.9993100220600506*2,    0.9993100220600506,
									 	 1,					    1.998617281545052,	    -0.99862280669515058};
d_COEFF_FILTER FILTER_6_TWO_SECTION =	{0.99799807001840368,  -0.99799807001840368, 	 0,
									 	 1,					    0.99599614003680736,	 0};


/*
//фильтр „ебышева 1 рода высоких частот 3 пор€дка на 10 √ц 0.2 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_7_ONE_SECTION =	{0.9992752439224013,   -0.9992752439224013*2,    0.9992752439224013,
									 	 1,					    1.9985482553012819,	    -0.9985527203883231};
d_COEFF_FILTER FILTER_7_TWO_SECTION =	{0.99845979662011097,  -0.99845979662011097, 	 0,
									 	 1,					    0.99691959324022184,	 0};
*/

//фильтр „ебышева 1 рода высоких частот 3 пор€дка на 10 √ц 0.4 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_7_ONE_SECTION =	{0.99929671143865162,   -0.99929671143865162*2,    0.99929671143865162,
									 	 1,					    1.9985907926698825,	    -0.99859605308472399};
d_COEFF_FILTER FILTER_7_TWO_SECTION =	{0.99813006526388015,  -0.99813006526388015, 	 0,
									 	 1,					    0.9962601305277603,	 0};


//фильтр батерворта высоких частот 1 пор€дка на 10 √ц 3 dB нелинейность ј„’, 25к√ц /2 секции
/*d_COEFF_FILTER FILTER_8_ONE_SECTION =	{0.99874493943354881,   -0.99874493943354881,    0,
									 	 1,					    0.9974898788670975,	     0};
  */
//фильтр батерворта высоких частот 1 пор€дка на 8.8 √ц 3 dB нелинейность ј„’, 25к√ц /2 секции
d_COEFF_FILTER FILTER_8_ONE_SECTION =	{0.99887030381932251,   -0.99887030381932251,    0,
									 	 1,					    0.99774060763864492,	 0};

//фильтр „ебышева 1 рода низких частот 3 пор€дка на 1000 √ц 0.4 dB нелинейность ј„’, 12.5к√ц /2 секции
d_COEFF_FILTER FILTER_9_ONE_SECTION	=	{0.063215884374645495,  0.063215884374645495*2,  0.063215884374645495,
									 	 1,						1.4718713679915838,		-0.72473490549016562};
d_COEFF_FILTER FILTER_10_TWO_SECTION=	{0.14692024016978558,   0.14692024016978558, 	 0,
									 	 1,						0.70615951966042867,	 0};
/*
//фильтр батерворта высоких частот 1 пор€дка на 5 √ц 3 dB нелинейность ј„’, 12.5к√ц /2 секции	//фильтр корекции интегратора дл€ виброперемещени€
d_COEFF_FILTER FILTER_11_ONE_SECTION =	{0.99874493943354881,   -0.99874493943354881,    0,
									 	 1,					    0.9974898788670975,	     0};
*/
//фильтр батерворта высоких частот 1 пор€дка на 6.5 √ц 3 dB нелинейность ј„’, 12.5к√ц /2 секции	//фильтр корекции интегратора дл€ виброперемещени€
d_COEFF_FILTER FILTER_11_ONE_SECTION =	{0.99849430491967095,   -0.99849430491967095,    0,
									 	 1,					    0.99698860983934179,	     0};

//фильтр батерворта высоких частот 1 пор€дка на 2,5 √ц 3 dB нелинейность ј„’, 12.5к√ц /2 секции	//фильтр корекции интегратора дл€ виброперемещени€
d_COEFF_FILTER FILTER_12_ONE_SECTION =	{0.99937207592298405,   -0.99937207592298405,    0,
									 	 1,					    0.99874415184596799,	     0};

/*
//банд пас фильтр Ѕатерворта Ќ„ 2 пор€дка на 1.3-2.7 √ц - 25 к√ц / 
d_COEFF_FILTER INTEGRATOR_2Hz = 		{0.00017589824497515666,0,-0.00017589824497515666,
										 1,1.9996479818382571,-0.99964820351004979};
*/
//банд пас фильтр Ѕатерворта Ќ„ 2 пор€дка на 0.8-2.2 √ц - 25 к√ц / 
d_COEFF_FILTER INTEGRATOR_2Hz_25 = 		{0.00017589824486810408,0,-0.00017589824486810408,
										 1,1.9996481940371102,-0.99964820351026384};
	 
										 
//банд пас фильтр Ѕатерворта Ќ„ 2 пор€дка на 1.3-2.7 √ц - 12.5 к√ц / 
d_COEFF_FILTER INTEGRATOR_2Hz = 		{0.00035173463093632329,0,-0.00035173463093632329,
										 1,1.9992956442069167,-0.9992965307381273};										  

//банд пас фильтр Ѕатерворта Ќ„ 2 пор€дка на 4.3-5.7 √ц - 12.5 к√ц / 
d_COEFF_FILTER INTEGRATOR_5Hz = 		{0.0003517346309099877,0,-0.0003517346309099877,
										 1,1.9992903401768221,-0.99929653073818003};										  



//банд пас фильтр Ѕатерворта Ќ„ 2 пор€дка на 6.2-13.8 √ц - 25 к√ц / 
d_COEFF_FILTER INTEGRATOR_10Hz = 		{0.00095413321742376185,0,-0.00095413321742376185,
										 1,1.9980863342850397,-0.99809173356515246};

//банд пас фильтр Ѕатерворта Ќ„ 4 пор€дка на 2-13 √ц - 12.5 к√ц /   интегратор 2-го пор€дка
/*d_COEFF_FILTER INTEGRATOR_10Hz_ONE_SECTION = 	{0.0027592094112730585,0,-0.0027592094112730585,
										 		1,1.9987389536427216,-0.99874021545882663};

d_COEFF_FILTER INTEGRATOR_10Hz_TWO_SECTION = 	{0.0027592094112730585,0,-0.0027592094112730585,
										 		1,1.9934284992364288,-0.99346256736991889};
*/
//банд пас фильтр Ѕатерворта Ќ„ 4 пор€дка на 2-10 √ц - 12.5 к√ц /   интегратор 2-го пор€дка
d_COEFF_FILTER INTEGRATOR_10Hz_ONE_SECTION = 	{0.0020077654921354005,0,-0.0020077654921354005,
										 		1,1.995463030759915,-0.99548271998742521};

d_COEFF_FILTER INTEGRATOR_10Hz_TWO_SECTION = 	{0.0020077654921354005,0,-0.0020077654921354005,
										 		1,1.9988400022247705,-0.9988412954089475};


//фильтр „ебышева 1 рода высоких частот 3 пор€дка на 13,5 √ц 0.1 dB нелинейность ј„’, 12,5к√ц /2 секции
d_COEFF_FILTER FILTER_70_ONE_SECTION =	{0.998825961571007,   -0.998825961571007*2,    0.998825961571007,
									 	 1,					    1.9976439431515745,	    -0.99765990313245334};
d_COEFF_FILTER FILTER_70_TWO_SECTION =	{0.9966014781789202,  -0.9966014781789202, 	 0,
									 	 1,					    0.99320295635784028,	 0};

//Chebyshev Type 1, Fpass=1 Hz, Fs=12500 Hz, Order=3, Passband Ripple=0.2 dB 										 
d_COEFF_FILTER FILTER_SAMPLE_ONE_SECTION =	{0.9998551434467251,  -0.9998551434467251*2,  0.9998551434467251,  1,  1.9997101975399763, -0.99971037624692438};
d_COEFF_FILTER FILTER_SAMPLE_TWO_SECTION =	{0.99969157945534226,  -0.99969157945534226,  0,  		  1,  0.99938315891068441,  0};	

d_FILTER *pHPF_1section,
		 *pHPF_2section,
		 *pLPF_1section,
		 *pLPF_2section, 
		 *pINTEGRAL_1section,
		 *pINTEGRAL_2section;

d_DETECTOR DETECTOR;

//u16 temp;

void ext_adc_SETUP(u16 period_sample)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
 NVIC_InitTypeDef 		  NVIC_InitStructure;
 SPI_InitTypeDef  		  SPI_InitStructure;

 //настраиваем таймер 1
 //TIM1CLK = 72 MHz, Prescaler = 36, TIM2 counter clock = 1 MHz
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

 /* Time base configuration */
  if (period_sample>1) TIM_TimeBaseStructure.TIM_Period = period_sample-1;
  else				   TIM_TimeBaseStructure.TIM_Period = 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  //TIM_PrescalerConfig(TIM1, 71, TIM_PSCReloadMode_Immediate);

   /* TIM IT enable */
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  //TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

 
  /* Enable the TIM1 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  //----------------------------настройка SPI------------------------------------//
  
  
  /* Enable SPI2 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  //pin_ADC_CS_SET;
  //pin_SPI_CS(HIGTH);

  /* SPI2 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

 

  /* TIM2 enable counter */
 /* TIM_Cmd(TIM1, ENABLE);

  while (1);*/
  //это надо убрать от сюда
  
  //pDF1= iir_CreateFilterDec(&A_L[0],&B_L[0],2);
  //pDF2= iir_CreateFilterDec(&A_L[3],&B_L[3],2);
   //pDF3= iir_CreateFilterDec(&A_H[0],&B_H[0],2);
  //pDF4= iir_CreateFilterDec(&A_H[3],&B_H[3],2);

  //инициализаци€ фильтра низких частот 2 секции
  pHPF_1section = iir_DEC_FILTER_CREATE(FILTER_2_ONE_SECTION,2);
  pHPF_2section = iir_DEC_FILTER_CREATE(FILTER_2_TWO_SECTION,2);
  //инициализаци€ фильтра высоких частот 2 секции
  pLPF_1section = iir_DEC_FILTER_CREATE(FILTER_6_ONE_SECTION,2);
  pLPF_2section = iir_DEC_FILTER_CREATE(FILTER_6_TWO_SECTION,2);
  //инициализаци€ интегратора 2 секции
  pINTEGRAL_1section = iir_DEC_FILTER_CREATE(INTEGRATOR_2Hz,2);
  pINTEGRAL_2section = iir_DEC_FILTER_CREATE(INTEGRATOR_2Hz,2);


  //iir_DETECTOR_RESET(&DETECTOR,25000);//50000

  
//	dec_koef = REG(K_VIBRO);
//	k_reg_mul = (float)dec_koef/1000;


}

/*
void TIM1_UP_IRQHandler(void)
{
 
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  //ext_adc_CLK_STROB();
  temp++;
  //ext_adc_READ()-0x8000;
  if (ext_adc_COUNT>0) 
   {
    ext_adc_COUNT--;
	*ext_adc_DATA++ = ext_adc_READ()-0x8000;
	
   }
  if (ext_adc_COUNT==0)	 
	  ext_adc_STOP();	  

  //TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}



void ext_adc_START(s16 *data_in, u16 count_sample)
{
 //подн€ть пины CS,CLK
 pin_ADC_CS(HIGTH);
 pin_ADC_CLK(HIGTH);
 
 ext_adc_DATA   = data_in;
 ext_adc_COUNT  = count_sample;

 if (ext_adc_COUNT>0) {
 						TIM_Cmd(TIM1, ENABLE);		//пуск таймера
						TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
					  }

 temp = 0;

}  */

void TIM1_UP_IRQHandler(void)
{
	
		dec_koef = REG(K_VIBRO);
			
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	
		//pin_KEY(HIGTH);
		//k_reg_mul = (float)REG(K_VIBRO)/1000;
		//k_reg_mul = (float)REG(K_VIBRO)/300000;
	
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

		ext_adc_VALUE = ext_adc_READ()-0x8000;

	
		SAMPLE[INDEX&DECIMATOR] = ext_adc_VALUE;

		INDEX++;

		if ((INDEX&DECIMATOR)==0)
		{
				if (DECIMATOR==0x01) ext_adc_VALUE = (SAMPLE[0]+SAMPLE[1])>>1;
				else				 ext_adc_VALUE = (SAMPLE[0]+SAMPLE[1]+SAMPLE[2]+SAMPLE[3])>>2;
							
				//if ((ext_adc_VALUE * dec_koef > 32500000) || (ext_adc_VALUE * k_reg_mul < -32500000))  		
				if ((ext_adc_VALUE > 32500) || (ext_adc_VALUE < -32500))
				{					
					
					if (!ext_adc_DELAY) ext_adc_OVER = 1;   //выставл€ем признак перегруза канала
					{
						if (ext_adc_VALUE >= 32500) ext_adc_VALUE = 32500;
						if (ext_adc_VALUE <= -32500) ext_adc_VALUE = -32500;
					}
				}
				
				//”множаем на настроечный коэф.	 
//				ext_adc_VALUE = ((ext_adc_VALUE * dec_koef) / 1000);
				
				//обработка сигнала в зависимости от выбраного канала и фильтра

				//домножаем
				ext_adc_VALUE = ext_adc_VALUE<<15;	  //не мен€ть!!!

				switch (ext_mode_reg)
				{
					
					case 0x0101: //канал ускорени€, 2-1000√ц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);  //фильтр Ќ„
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
										 
								 break; 
					case 0x0102: //канал ускорени€, 10-1000√ц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);  //фильтр Ќ„
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
										 
								 break; 
					case 0x0104: //канал ускорени€, 10-2000√ц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);  //фильтр Ќ„
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
										 
								 break; 
					case 0x0108: //канал ускорени€, 10-5000√ц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);  //фильтр Ќ„
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
										 
								 break; 
					case 0x0110: //канал ускорени€, 10-10000√ц
								 //ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);  //фильтр Ќ„
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
										 
								 break; 
					case 0x0201: //канал скорости, 2-1000√ц
								 //фильтр на 1000гц	
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);	
					
												 ext_adc_VALUE = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,ext_adc_VALUE);

								 //фильтр высоких частот батерворта	 10 гц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
					

								 break;
					case 0x0202: //канал скорости, 10-1000√ц
								 //фильтр на 1000гц	
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);	
					
									 //интегратор 1-го пор€дка + фильтр 
									 ext_adc_VALUE = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,ext_adc_VALUE);

								 //фильтр высоких частот батерворта	 10 гц
								 ext_adc_VALUE = iir_DEC_FILTER_1ORD(pLPF_1section,ext_adc_VALUE);

								 break;
					case 0x0204: //канал скорости, 10-2000√ц
								 //фильтр на 2000гц	
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);	
					
									 //интегратор 1-го пор€дка + фильтр 
									 ext_adc_VALUE = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,ext_adc_VALUE);

								 //фильтр высоких частот батерворта	 10 гц
								 ext_adc_VALUE = iir_DEC_FILTER_1ORD(pLPF_1section,ext_adc_VALUE);

								 break;
					case 0x0208: //канал скорости, 10-5000√ц
								 //фильтр на 5000гц	
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);	
					
									 //интегратор 1-го пор€дка + фильтр 
									 ext_adc_VALUE = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,ext_adc_VALUE);

								 //фильтр высоких частот батерворта	 10 гц
								 ext_adc_VALUE = iir_DEC_FILTER_1ORD(pLPF_1section,ext_adc_VALUE);

								 break;
					case 0x0401: //канал перемещени€, 10-1000√ц
								 //фильтр на 300гц	
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,ext_adc_VALUE);	
								 //ext_adc_VALUE = iir_DEC_FILTER_1ORD(pLPF_1section,ext_adc_VALUE);
									ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);  //фильтр ¬„
									 //интегратор 2-го пор€дка + фильтр 
								 ext_adc_VALUE = iir_DEC_FILTER_4ORD(pINTEGRAL_1section,pINTEGRAL_2section,ext_adc_VALUE);

								 break;
					case 0x0800: //канал выборки, 2-10000√ц
								 ext_adc_VALUE  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,ext_adc_VALUE);
								 
								 

								 break;
					default: 	 break;		
					}
			

					//задержка 
					if (ext_adc_DELAY>0) {ext_adc_DELAY--;return;}  
					
					
					//вызов детектора
					iir_DETECTOR(&DETECTOR,ext_adc_VALUE);	

					if (ext_adc_COUNT>0) 
					{
							ext_adc_COUNT--;		
						
							ext_adc_VALUE = ((ext_adc_VALUE * dec_koef) / 1000);
									
							*ext_adc_DATA++ = ((ext_adc_VALUE + 16384)>>15);			
					}

		}

   
		//pin_KEY(LOW);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);		
	
}

void ext_adc_SAMPLING(s16 *data_in, u16 count_sample)
{
 ext_adc_COUNT  = 0;
 ext_adc_DATA   = data_in;
 ext_adc_XCOUNT = count_sample;

 while (count_sample--)
  data_in[count_sample]	= 0;

 ext_adc_COUNT = ext_adc_XCOUNT;
}



 

void ext_adc_START(void)
{
	
	
	
 ext_adc_COUNT = 0;

 pin_ADC_CS(HIGTH);
 //pin_ADC_CLK(HIGTH);
 
 //обнул€ем переменные дециматора
 SAMPLE[0] = 0;
 SAMPLE[1] = 0;
 SAMPLE[2] = 0;
 SAMPLE[3] = 0;

 INDEX = 0;

 ext_adc_DELAY = 25000;//25000; //1 секунда	дл€ 25 к√ц

	

 ext_mode_reg = REG(MODE_REG);
	
			iir_DEC_FILTER_RESET(pHPF_1section);
			iir_DEC_FILTER_RESET(pHPF_2section);
			iir_DEC_FILTER_RESET(pLPF_1section);
			iir_DEC_FILTER_RESET(pLPF_2section);
			iir_DEC_FILTER_RESET(pINTEGRAL_1section);
			iir_DEC_FILTER_RESET(pINTEGRAL_2section);
 
	


 switch (ext_mode_reg)
		   {
		    case 0x0101: //канал ускорени€, 5-1000√ц
						 iir_DEC_FILTER_SET(FILTER_9_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_10_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_6_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_6_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x03;
						 //ext_adc_DELAY = 25000; //1 секунда
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 		 
						 break; 
			case 0x0102: //канал ускорени€, 10-1000√ц
						 iir_DEC_FILTER_SET(FILTER_2_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_2_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_7_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_7_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 		 
						 break; 
			case 0x0104: //канал ускорени€, 10-2000√ц
						 iir_DEC_FILTER_SET(FILTER_3_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_3_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_7_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_7_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 		 
						 break;
			case 0x0108: //канал ускорени€, 10-5000√ц
						 iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_7_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_7_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 		 
						 break; 
			case 0x0110: //канал ускорени€, 10-10000√ц
						 iir_DEC_FILTER_SET(FILTER_5_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_5_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_7_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_7_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 		 
						 break; 			 			  
			case 0x0201: //канал скорости, 5-1000√ц
						 iir_DEC_FILTER_SET(FILTER_9_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_10_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_6_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_6_TWO_SECTION, pLPF_2section);

						 iir_DEC_FILTER_SET(INTEGRATOR_2Hz, pINTEGRAL_1section);

						 DECIMATOR = 0x03;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 
						 break;
			case 0x0202: //канал скорости, 10-1000√ц
						 iir_DEC_FILTER_SET(FILTER_2_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_2_TWO_SECTION, pHPF_2section);	
						 
						 iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 

						 iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 
						 break;
			case 0x0204: //канал скорости, 10-2000√ц
						 iir_DEC_FILTER_SET(FILTER_3_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_3_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 

						 iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 
						 break;
			case 0x0208: //канал скорости, 10-5000√ц
						 iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);

						 iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 

						 iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);

						 DECIMATOR = 0x01;
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 
						 break;
			case 0x0401: //канал перемещени€, 10-300√ц
		
			
						 iir_DEC_FILTER_SET(FILTER_1_ONE_SECTION, pHPF_1section);
						 iir_DEC_FILTER_SET(FILTER_1_TWO_SECTION, pHPF_2section); 
			
						 //iir_DEC_FILTER_SET(FILTER_11_ONE_SECTION, pLPF_1section);// 
						 iir_DEC_FILTER_SET(FILTER_70_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_70_TWO_SECTION, pLPF_2section);

						 iir_DEC_FILTER_SET(INTEGRATOR_10Hz_ONE_SECTION, pINTEGRAL_1section);
						 iir_DEC_FILTER_SET(INTEGRATOR_10Hz_TWO_SECTION, pINTEGRAL_2section);

						 DECIMATOR = 0x03;//деление частоты на 4
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000
						 
						 break;
			case 0x0800: //канал выборки
						 iir_DEC_FILTER_SET(FILTER_SAMPLE_ONE_SECTION, pLPF_1section);
						 iir_DEC_FILTER_SET(FILTER_SAMPLE_TWO_SECTION, pLPF_2section);

						 DECIMATOR = 0x01;
						 ext_adc_DELAY = 25000;//62500;//50000;//40000; //2.5 секунда						 						
						 iir_DETECTOR_RESET(&DETECTOR,25000);//25000//50000

			default: 	 break;		
   		  }



		


 /* Enable SPI1  */
 SPI_Cmd(SPI2, ENABLE);

 TIM_Cmd(TIM1, ENABLE);		//пуск таймера
 TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

			

}



void ext_adc_STOP(void)
{
	
 //pin_ADC_CLK(LOW);
 //ext_adc_COUNT = 0;
 ext_adc_OVER = 0;

 TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
 TIM_Cmd(TIM1, DISABLE);
 TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

 /* Enable SPI1  */
 SPI_Cmd(SPI2, DISABLE);
 pin_ADC_CS(LOW);
}


u8 ext_adc_GET_STATUS(void)
{
 if (ext_adc_COUNT>0) return ext_adc_STATUS_RUN;
 else				 return ext_adc_STATUS_STOP;
}

u8 ext_adc_GET_PROCESS(void)
{
 if (ext_adc_XCOUNT>0)
  return ((u32)(ext_adc_XCOUNT-ext_adc_COUNT)*100)/ext_adc_XCOUNT;
 else
  return 0;
}


void ext_adc_WAIT(void)
{
 while (ext_adc_GET_STATUS()==ext_adc_STATUS_RUN) ;
}


uint64_t ext_adc_READ()
{
 
 uint64_t value = 0;
 
 //pin_ADC_CS(LOW);
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;	

 //pin_ADC_CLK_SET;
 //pin_ADC_CLK_SET;
 SPI_I2S_SendData(SPI2, 0xFF);
 while ((SPI2->SR & SPI_I2S_FLAG_TXE)==0);
 SPI_I2S_SendData(SPI2, 0xFF);
 while ((SPI2->SR & SPI_I2S_FLAG_RXNE)==0);
 value = SPI_I2S_ReceiveData(SPI2);								 	//читаем 1 байт

 while ((SPI2->SR & SPI_I2S_FLAG_TXE)==0);
 SPI_I2S_SendData(SPI2, 0xFF);
 while ((SPI2->SR & SPI_I2S_FLAG_RXNE)==0);
 value = (value<<8)|SPI_I2S_ReceiveData(SPI2);					    //читаем 2 байт

 while ((SPI2->SR & SPI_I2S_FLAG_RXNE)==0);
 value = (value<<8)|SPI_I2S_ReceiveData(SPI2);					   //читаем 3 байт


 pin_ADC_CS_SET;
 
 value=value>>2; 

 return value;
}	

//Alex
uint16_t* calc_from_dat ( const char *FileName, uint16_t* A, uint16_t* V )
{
	s32 t_element;
	s16 tem[1];
	FILE * FTemp;
	static float res[3];
	uint16_t rr[2];
	SET_CLOCK_SPEED(CLK_72MHz); 
	//float* result = &res[0];
	iir_DETECTOR_RESET(&DETECTOR,24000);
	FTemp = fopen(FileName,"r");
	if (FTemp != NULL)
	{
	fseek(FTemp,4,SEEK_SET);
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		iir_DETECTOR(&DETECTOR,t_element/300);
	}
	res[0] = iir_RETURN_MAX_PIK(&DETECTOR);
	*A = (uint16_t)(res[0]*100);
	rr[0] = (uint16_t)(res[0]*100);
	 //????????
	fseek(FTemp,4,SEEK_SET);
	iir_DETECTOR_RESET(&DETECTOR,24000);
	iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
	iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);
	iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 
	iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);	
  	t_element = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,t_element);
		t_element = iir_DEC_FILTER_1ORD(pLPF_1section,t_element);
		iir_DETECTOR(&DETECTOR,t_element*20.916/300);
	}
	res[1] = iir_RETURN_RMS(&DETECTOR);
	*V = (uint16_t)(res[1]*100);
	rr[1] = (uint16_t)(res[1]*100);

	fclose(FTemp);
}
else 
{
	*A = 0;
	*V = 0;
}
	*A = 500;
	*V = 5620;
	//SET_CLOCK_SPEED(CLK_8MHz); 
	//return &res[0];
	return rr;
}

uint16_t calc_from_dat_A ( const char *FileName)
{
		s32 t_element;
	s16 tem[1];
	FILE * FTemp;
	static float res[3];
	uint16_t rr[2];
	SET_CLOCK_SPEED(CLK_72MHz);	
	//float* result = &res[0];
	iir_DETECTOR_RESET(&DETECTOR,25000);
	FTemp = fopen(FileName,"r");
	if (FTemp != NULL)
	{
	fseek(FTemp,4,SEEK_SET);
	iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
	iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);
	iir_DEC_FILTER_SET(FILTER_7_ONE_SECTION, pLPF_1section);
	iir_DEC_FILTER_SET(FILTER_7_TWO_SECTION, pLPF_2section);
		
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);  //фильтр Ќ„
		t_element  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,t_element);
		iir_DETECTOR(&DETECTOR,t_element/300);
	}
	/*
	res[0] = iir_RETURN_MAX_PIK(&DETECTOR);
	res[1] = iir_RETURN_MIN_PIK(&DETECTOR)*(-1);
	if (res[1]<res[0]) res[0] = res[1];
	if (res[0]>4) res[0]-=4; else res[0]=0;	
	*/
	res[0] = iir_RETURN_RMS(&DETECTOR);
	fclose(FTemp);
	//SET_CLOCK_SPEED(CLK_8MHz);
//return 5699;
return ((uint16_t)(res[0]*100));	
}
return 0;
//return 789;
	
}

uint16_t calc_from_dat_V ( const char *FileName)
{
			s32 t_element;
	s16 tem[1];
	FILE * FTemp;
	static float res[3];
	SET_CLOCK_SPEED(CLK_72MHz);
	iir_DETECTOR_RESET(&DETECTOR,24000);
	FTemp = fopen(FileName,"r");
	if (FTemp != NULL)
	{	
		fseek(FTemp,4,SEEK_SET);
	iir_DETECTOR_RESET(&DETECTOR,25000);
	iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
	iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);
	iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 
	iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);	
  	t_element = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,t_element);
		t_element = iir_DEC_FILTER_1ORD(pLPF_1section,t_element);
		iir_DETECTOR(&DETECTOR,t_element*20.916/300);
	}
	res[1] = iir_RETURN_RMS(&DETECTOR);
	fclose(FTemp);
	//SET_CLOCK_SPEED(CLK_8MHz);
	//
	//rr[1] = (uint16_t)(res[1]*100);
//return 7899;
return( (uint16_t)(res[1]*100));	
}	
return 0;
}

uint16_t calc_from_dat_S ( const char *FileName)
{
	s32 t_element;
	s16 tem[4];
	FILE * FTemp;
	static float res[3];
	SET_CLOCK_SPEED(CLK_72MHz);
	iir_DETECTOR_RESET(&DETECTOR,5000);
	FTemp = fopen(FileName,"r");
	if (FTemp != NULL)
	{	
		fseek(FTemp,4,SEEK_SET);
		iir_DEC_FILTER_SET(FILTER_1_ONE_SECTION, pHPF_1section); 
		iir_DEC_FILTER_SET(FILTER_1_TWO_SECTION, pHPF_2section); 
		iir_DEC_FILTER_SET(FILTER_70_ONE_SECTION, pLPF_1section);
		iir_DEC_FILTER_SET(FILTER_70_TWO_SECTION, pLPF_2section);
		iir_DEC_FILTER_SET(INTEGRATOR_10Hz_ONE_SECTION, pINTEGRAL_1section);
		iir_DEC_FILTER_SET(INTEGRATOR_10Hz_TWO_SECTION, pINTEGRAL_2section);
		
		while (fread(tem,sizeof(s16),2,FTemp)==2)
		{
			tem[0] = (tem[0]+tem[1])/2;
			t_element=(s32)((tem[0]<<15)-16384);
			
			t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);	
			t_element  = iir_DEC_FILTER_4ORD(pLPF_1section,pLPF_2section,t_element);  //фильтр ¬„
			t_element = iir_DEC_FILTER_4ORD(pINTEGRAL_1section,pINTEGRAL_2section,t_element);
			
			//t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);	
			//t_element = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,t_element);
			//t_element = iir_DEC_FILTER_1ORD(pLPF_1section,t_element);
			iir_DETECTOR(&DETECTOR,(float)t_element*395.570/300);
		}
		res[1] = iir_RETURN_RMS(&DETECTOR);
		fclose(FTemp);
		//SET_CLOCK_SPEED(CLK_8MHz);
	//
	//rr[1] = (uint16_t)(res[1]*100);
	//return 7899;
	return( (uint16_t)(res[1]*100));	
	}	
return 0;
}

/*void calc_from_frame ( void *Frame, U32 LengthFrame, uint16_t* A, uint16_t* V )
{
	s32 t_element;
	s16 tem[1];
	FILE * FTemp;
	static float res[3];
	SET_CLOCK_SPEED(CLK_72MHz); 
	//float* result = &res[0];
	iir_DETECTOR_RESET(&DETECTOR,24000);
	//FTemp = fopen(FileName,"rb");
	//if (FTemp != NULL)
	//{
	//fseek(FTemp,4,SEEK_SET);
	
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		iir_DETECTOR(&DETECTOR,t_element/300);
	}
	res[0] = iir_RETURN_MAX_PIK(&DETECTOR);
	*A = (uint16_t)(res[0]*100);
	 //????????
	fseek(FTemp,4,SEEK_SET);
	iir_DETECTOR_RESET(&DETECTOR,24000);
	iir_DEC_FILTER_SET(FILTER_4_ONE_SECTION, pHPF_1section);
	iir_DEC_FILTER_SET(FILTER_4_TWO_SECTION, pHPF_2section);
	iir_DEC_FILTER_SET(FILTER_8_ONE_SECTION, pLPF_1section); 
	iir_DEC_FILTER_SET(INTEGRATOR_10Hz, pINTEGRAL_1section);
	while (fread(tem,sizeof(s16),1,FTemp)==1)
	{
		t_element=(s32)((tem[0]<<15)-16384);
		t_element  = iir_DEC_FILTER_4ORD(pHPF_1section,pHPF_2section,t_element);	
  	t_element = iir_DEC_FILTER_2ORD(pINTEGRAL_1section,t_element);
		t_element = iir_DEC_FILTER_1ORD(pLPF_1section,t_element);
		iir_DETECTOR(&DETECTOR,t_element*20.916/300);
	}
	res[1] = iir_RETURN_RMS(&DETECTOR);
	*V = (uint16_t)(res[1]*100);

	//fclose(FTemp);
//}
//else 
//{
//	*A = 0;
//	*V = 0;
//}
	SET_CLOCK_SPEED(CLK_8MHz); 
	//return &res[0];
	return;
}*/
//*Alex

/*
u16 ext_adc_READ()
{
 u8 clk;
 u8 data;
 u16 value;
 
 //pin_ADC_CS(LOW);
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;
 pin_ADC_CS_RES;

 ext_adc_CLK_STROB();	
 ext_adc_CLK_STROB();
 ext_adc_CLK_STROB();
 ext_adc_CLK_STROB();
 ext_adc_CLK_STROB();
 ext_adc_CLK_STROB();


 //pin_ADC_CLK_SET;
 //pin_ADC_CLK_SET;
 clk  = 16;
 data = 1;
 value = 0;

 while (clk--)
  {
   ext_adc_CLK_STROB();
   //pin_ADC_CLK_SET;
   value = (value<<1);

   data = pin_ADC_DATA;

  // value = (value<<1);
   value|=(data&0x01);

  }
 pin_ADC_CS_SET;
 
  

 return value;
}	  */
