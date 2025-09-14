#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"


void GPIO_Config(void);
void EXTI_Config(void);
void TIM_Config(void);
void Delay_ms(uint32_t ms);


volatile uint8_t led1_state = 0;

int main(void) {

    SystemInit();


    GPIO_Config();
    EXTI_Config();
    TIM_Config();

 
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);


    while (1) {
        GPIO_SetBits(GPIOB, GPIO_Pin_1); 
        Delay_ms(1000);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1); 
        Delay_ms(1000);
    }
}


void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void EXTI_Config(void) {
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);


    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM_Config(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler = SystemCoreClock / 1000 - 1; 
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = 0xFFFF;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void Delay_ms(uint32_t ms) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < ms);
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    
        Delay_ms(10); 
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) { 
           
            led1_state ^= 1;
            GPIO_WriteBit(GPIOB, GPIO_Pin_0, (led1_state) ? Bit_SET : Bit_RESET);
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
