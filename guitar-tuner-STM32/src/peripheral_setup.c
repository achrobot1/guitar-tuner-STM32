#include "peripheral_setup.h"

void GPIO_setup()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Setup on-board LED
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);

    // Setup PC0 as ADC input pin
    // PC0 default: ADC12_IN10
    GPIO_InitStructure.GPIO_Pin = ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);

    /**
     * PB13 -> SPI2_SCK
     * PB15 -> SPI2_MOSI
     */
    GPIO_InitStructure.GPIO_Pin = SSD1306_SCK | SSD1306_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**
     * PA0 -> Data/Command select
     * PA1 -> Chip select
     * PA8 -> Reset
     */
    GPIO_InitStructure.GPIO_Pin = SSD1306_DC | SSD1306_CS | SSD1306_RESET ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


}

void RCC_setup()
{
    // 1. Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			    RCC_APB2Periph_GPIOC |
			    RCC_APB2Periph_GPIOB |
			    RCC_APB2Periph_ADC1 , ENABLE);

    // Enable TIM2 and SPI2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |
			    RCC_APB1Periph_SPI2, ENABLE);
}

void NVIC_setup()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

}

void TIM_setup()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /*
     * Set Prescaler such that TIM2 counter clock counts at 36 kHz
     */
    // u16 prescaler_value = (u16)(SystemCoreClock/ 36000 ) - 1; 
    u16 prescaler_value = (u16)1; 

    TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler_value;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;                                     
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIM_PERIOD;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

void ADC1_setup()
{
    ADC_InitTypeDef ADC_InitStruct;

    /* Reset ADC init structure parameters values */
    /* Initialize the ADC_Mode member */
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &ADC_InitStruct);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);                                                                        
    ADC_Cmd(ADC1, ENABLE);

    // Calibration
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

void SPI_setup()
{
    SPI_InitTypeDef SPI_InitStructure;

    SPI_StructInit(&SPI_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);   
}
