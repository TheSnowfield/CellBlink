#ifndef _TIMER_H_
#define _TIMER_H_

#include <stm8s.h>
#include <stm8s_tim1.h>

void Timer_Init();
void Timer_DelayMilliseconds(uint16_t ms);
void Timer_DelayMicroseconds(uint16_t us);

#endif /* _TIMER_H_ */