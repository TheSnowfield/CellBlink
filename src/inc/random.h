#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "common.h"

// Also stm8 ADC resolution
#define RANDMAX 1024

void Random_Init();
uint16_t Random_Rand();

#endif /* _RANDOM_H_ */
