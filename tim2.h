
#include "stm32f10x.h"

void Timer2_Init(void) {

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

 
    TIM_TimeBaseInitTypeDef timerInit;


    timerInit.TIM_CounterMode = TIM_CounterMode_Up;

 
    timerInit.TIM_Period = 0xFFFF;


    timerInit.TIM_Prescaler = 72 - 1;

  
    timerInit.TIM_ClockDivision = TIM_CKD_DIV1; // M?c d?nh không chia

    TIM_TimeBaseInit(TIM2, &timerInit);

 
    TIM_Cmd(TIM2, ENABLE);
};
