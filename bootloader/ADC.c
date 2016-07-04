#include "stm32f10x.h"

#define adc_COUNT 		128
#define adc_Vref  		3300 //мВ
#define MAX_BAT_VOLTAGE	4150 //напряжение при 100%
#define MIN_BAT_VOLTAGE	3100 //напряжение при 0%

    //unsigned short adc_MESURE_BUF[adc_COUNT];

void adc_POWER(FunctionalState cmd)
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, cmd);
}


void adc_SETUP(void)
{

   ADC_InitTypeDef   ADC_InitStructure;

  //настраиваем вход
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
   /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);*/
   


    // Configure PA.01 (ADC Channel1) as analog input -------------------------
   /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);	 */

   
   //настраиваем АЦП
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfChannel = 1;
   ADC_Init(ADC1, &ADC_InitStructure);

   /* ADC1 regular channel14 configuration */ 
   ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
   
   /* Enable ADC1 */
   ADC_Cmd(ADC1, ENABLE);
   //калибровка АЦП
   // Enable ADC1 reset calibaration register 
   ADC_ResetCalibration(ADC1);
   // Check the end of ADC1 reset calibration register 
   while(ADC_GetResetCalibrationStatus(ADC1));
   // Start ADC1 calibaration */
   ADC_StartCalibration(ADC1);
   // Check the end of ADC1 calibration */
   while(ADC_GetCalibrationStatus(ADC1));

  
   ADC_Cmd(ADC1, DISABLE);
   //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
   //adc_POWER(DISABLE);
  
}

unsigned int adc_BAT_MEASURE(void)
{
  unsigned long SUM = 0;
  unsigned char i = adc_COUNT;

  adc_POWER(ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  while (i--)
   {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET) ;
	SUM+= ADC_GetConversionValue(ADC1);
   } 

  ADC_Cmd(ADC1, DISABLE);
  adc_POWER(DISABLE);

  SUM = SUM/adc_COUNT;
  SUM = (SUM*adc_Vref*2+2048)/4095;

  return  SUM;
}

unsigned int adc_BAT_PERCENT(void)
{
 unsigned int temp_reg;

 //измерение заряда бата
	temp_reg = adc_BAT_MEASURE();
	if (temp_reg>MAX_BAT_VOLTAGE) temp_reg = 100;
	else
	if (temp_reg<MIN_BAT_VOLTAGE) temp_reg = 0;
	else
	 {
	  temp_reg-=MIN_BAT_VOLTAGE;
	  temp_reg = (temp_reg*100)/(MAX_BAT_VOLTAGE-MIN_BAT_VOLTAGE);
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


