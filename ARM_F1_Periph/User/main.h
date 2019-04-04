#include "stm32f10x.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"

#define PORT_LED 	GPIOA
#define DI		GPIO_Pin_7
#define DCKI	GPIO_Pin_5
/*Systick*/
typedef struct
{
	uint32_t Systick_Counter;
}SysTick_typedef;

extern SysTick_typedef systick;
uint32_t System_GetTick(void);

/*Init*/
void SysTick_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);
void NVIC_Configuration(void);
void UART_Configuration(void);
void DMA_Configuration(void);

/*Delay*/
void usDelay(uint32_t nTime);
void msDelay(uint32_t nTime);

/*LED function*/
void trigger_latch(void);
void write16(uint16_t data);
void beginWrite(void);
void endWrite(void);
void SendSPI(void);
void ClearLED(void);
/*UART LED*/
void uartLED(uint8_t buffer[],int myCase);
void uartAllLED(int myCode,uint8_t buffer[]);