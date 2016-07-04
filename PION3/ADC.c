#include "stm32f10x.h"

#define adc_COUNT 		128
#define adc_Vref  		3300 //��
#define MAX_BAT_VOLTAGE	4150 //���������� ��� 100%
#define MIN_BAT_VOLTAGE	3100 //���������� ��� 0%
#define VREF_INT	1200 //���������� ��� 0%

    //unsigned short adc_MESURE_BUF[adc_COUNT];

void adc_POWER(FunctionalState cmd)
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, cmd);
}


void adc_SETUP(void)
{

   ADC_InitTypeDef   ADC_InitStructure;

  //����������� ����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);

   //����������� ���
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfChannel = 1;
   ADC_Init(ADC1, &ADC_InitStructure);

   /* ADC1 regular channel14 configuration */ 
   ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_239Cycles5);
   
   /* Enable ADC1 */
   ADC_Cmd(ADC1, ENABLE);
   //���������� ���
   // Enable ADC1 reset calibaration register 
   ADC_ResetCalibration(ADC1);
   // Check the end of ADC1 reset calibration register 
   while(ADC_GetResetCalibrationStatus(ADC1));
   // Start ADC1 calibaration */
   ADC_StartCalibration(ADC1);
   // Check the end of ADC1 calibration */
   while(ADC_GetCalibrationStatus(ADC1));
	 
ADC_TempSensorVrefintCmd(ENABLE);
  
   ADC_Cmd(ADC1, DISABLE);
   //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
   //adc_POWER(DISABLE);
  
}

unsigned int adc_BAT_MEASURE(void)
{
  unsigned long SUM = 0;
	unsigned int vref_volt = 0;
	unsigned long vsupl_volt = 0;
  unsigned char i = adc_COUNT;

  adc_POWER(ENABLE);
  ADC_Cmd(ADC1, ENABLE);
	//Alex
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_239Cycles5);
	
	i = adc_COUNT;
	while (i--)
   {
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
		vref_volt += ADC_GetConversionValue(ADC1); 
		} 
	vref_volt = vref_volt/adc_COUNT;
	vsupl_volt = VREF_INT*4096/vref_volt;
	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	//*Alex
	i = adc_COUNT;
  while (i--)
   {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET) ;
	SUM+= ADC_GetConversionValue(ADC1);
   } 

  ADC_Cmd(ADC1, DISABLE);
  adc_POWER(DISABLE);

  SUM = SUM/adc_COUNT;
  SUM = (SUM*vsupl_volt*2+2048)/4095;	//-

  return  SUM;
}

unsigned int adc_BAT_PERCENT(void)
{
 unsigned int temp_reg;
	static uint8_t bat = 250;
static uint32_t bat_per_summ = 0;
	static uint16_t bat_counter = 0;
 //��������� ������ ����
	temp_reg = adc_BAT_MEASURE();
	if (temp_reg>MAX_BAT_VOLTAGE) temp_reg = 100;
	else
	if (temp_reg<MIN_BAT_VOLTAGE) temp_reg = 0;
	else
	 {
	  temp_reg-=MIN_BAT_VOLTAGE;
	  temp_reg = (temp_reg*100)/(MAX_BAT_VOLTAGE-MIN_BAT_VOLTAGE);
	 }
	 
	 if (bat_counter == 100)
	 {
		 bat = bat_per_summ/100;
		 bat_per_summ = 0;
		 bat_counter = 0;
	 }
	 else bat_per_summ +=temp_reg;
	 bat_counter++;
	 
	 if (bat == 250) bat = temp_reg;
	 
 return bat;
}


unsigned int adc_BAT_MEASURE_edit(void)
{
  unsigned long SUM = 0;
	unsigned int vref_volt = 0;
	unsigned long vsupl_volt = 0;
  unsigned char i = adc_COUNT;

  adc_POWER(ENABLE);
  ADC_Cmd(ADC1, ENABLE);
	//Alex
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_239Cycles5);	
	i = adc_COUNT;
	while (i--)
  {
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
			vref_volt += ADC_GetConversionValue(ADC1); 
	} 
	vref_volt = vref_volt/adc_COUNT;
	vsupl_volt = VREF_INT*4500/vref_volt;
	
	 
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	//*Alex
	i = adc_COUNT;
  while (i--)
  {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET) ;
		SUM+= ADC_GetConversionValue(ADC1);
  } 

  ADC_Cmd(ADC1, DISABLE);
  adc_POWER(DISABLE);

  SUM = SUM/adc_COUNT;
  SUM = (SUM*vsupl_volt*2+2250)/4500;	//-

  return  SUM;
}



unsigned int adc_BAT_PERCENT_edit(void)
{
	unsigned int temp_reg;
	static uint8_t bat = 250;
	static uint32_t bat_per_summ = 0;
	static uint16_t bat_counter = 0;
	
	//��������� ������ ����
	temp_reg = adc_BAT_MEASURE_edit();
	
	
	if (temp_reg>3600) temp_reg = 100;
	else
		if (temp_reg<3200) temp_reg = 0;
		else
		{
			temp_reg -= 3200;
			temp_reg = (temp_reg*100)/(3600-3200);
		}
	
 return temp_reg;
}

unsigned int adc_BAT_PERCENT_edit_charge(void)
{
	unsigned int temp_reg;
	static uint8_t bat = 250;
	static uint32_t bat_per_summ = 0;
	static uint16_t bat_counter = 0;
	
	//��������� ������ ����
	temp_reg = adc_BAT_MEASURE_edit();
	
	
	if (temp_reg>4000) temp_reg = 100;
	else
		if (temp_reg<3200) temp_reg = 0;
		else
		{
			temp_reg -= 3200;
			temp_reg = (temp_reg*100)/(4000-3200);
		}
	
 return temp_reg;
}
  
/*
unsigned int adc_BAT_MEASURE(void)
{
  unsigned char i;
  unsigned long SUM;

  adc_POWER(ENABLE);
  DMA1_Channel1->CNDTR = adc_COUNT;
  DMA_ClearITPendingBit(DMA_IT_TC);
 


  ADC_Cmd(ADC1, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while (DMA_GetITStatus(DMA_IT_TC)==RESET);

  ADC_Cmd(ADC1, DISABLE);
  DMA_Cmd(DMA1_Channel1, DISABLE);
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);
  
  adc_POWER(DISABLE);	
  
  i=adc_COUNT;
  SUM = 0;
  while (i--)
   SUM+=adc_MESURE_BUF[i];

  SUM = SUM/adc_COUNT;
  SUM = (SUM*3330)/4095;

  return  SUM;
}

*/


//����� �����., ����., PA3
unsigned int adc_BAT_MEASURE_NEW_AMPER (void)
{
  unsigned long SUM = 0;
	unsigned int vref_volt = 0;
	//double vsupl_volt = 0;
  unsigned char i = adc_COUNT;

  adc_POWER(ENABLE);
  ADC_Cmd(ADC1, ENABLE);
	//Alex
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_239Cycles5);	
	i = adc_COUNT;
	while (i--)
  {
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
			vref_volt += ADC_GetConversionValue(ADC1); 
	} 
	vref_volt = vref_volt/adc_COUNT;
	//vsupl_volt = (double) (vref_volt / 4096) * 3.3;


  return vref_volt;
}

//����� �����., ����., PA1
unsigned int adc_BAT_MEASURE_NEW_VOLT (void)
{
  unsigned long SUM = 0;
	unsigned int vref_volt = 0;
	//double vsupl_volt = 0;
  unsigned char i = adc_COUNT;

  adc_POWER(ENABLE);
  ADC_Cmd(ADC1, ENABLE);
	//Alex
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	
	i = adc_COUNT;
	while (i--)
  {
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
			vref_volt += ADC_GetConversionValue(ADC1); 
	} 
	vref_volt = vref_volt/adc_COUNT;
	//vsupl_volt = (double) (vref_volt / 4096) * 3.3;


  return vref_volt;
}
