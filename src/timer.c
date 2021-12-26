#include "inc/timer.h"

#define TIMER_DELAY_PERIOD 850
#define TIMER_DELAY_PERIOD_US 4
volatile uint16_t counter;

INTERRUPT_HANDLER(Timer_OverflowIRQHandler, 11)
{
  if(counter) --counter;
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

void Timer_Init()
{
  TIM1_DeInit();
}

void Timer_DelayMilliseconds(uint16_t ms)
{
  TIM1_Cmd(DISABLE);
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, TIMER_DELAY_PERIOD, 10);
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  TIM1_ARRPreloadConfig(ENABLE);

  counter = ms / 10;

  TIM1_Cmd(ENABLE);
  while(counter);
}

void Timer_DelayMicroseconds(uint16_t us)
{
  TIM1_Cmd(DISABLE);
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, TIMER_DELAY_PERIOD_US, 0);
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  TIM1_ARRPreloadConfig(ENABLE);

  counter = us >> 1;

  TIM1_Cmd(ENABLE);
  while(counter);
}
