#include "inc/random.h"

void Random_Init()
{
  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
            ADC1_CHANNEL_3,
            ADC1_PRESSEL_FCPU_D8,
            ADC1_EXTTRIG_TIM, DISABLE,
            ADC1_ALIGN_RIGHT,
            ADC1_SCHMITTTRIG_ALL, DISABLE);
  ADC1_Cmd(ENABLE);
}

uint16_t Random_Rand()
{
  Random_Init();
  return ADC1_GetConversionValue();
}
