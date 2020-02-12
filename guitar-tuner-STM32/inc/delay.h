#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"

static __IO uint32_t TimingDelay;
void Delay(uint32_t nTime);

#endif /* !DELAY_H */
