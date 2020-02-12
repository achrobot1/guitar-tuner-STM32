#include "delay.h"

// 5. Timer code
void Delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void){
	if(TimingDelay != 0x00)
		TimingDelay--;
}


