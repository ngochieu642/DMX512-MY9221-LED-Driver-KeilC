#include "stm32f10x.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include <stdbool.h>

#include <sparkle.h>
#include <color.h>
#include <colorRun.h>

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

/*Delay*/
void usDelay(uint32_t nTime);
void msDelay(uint32_t nTime);

/*LED Function*/
void trigger_latch(void);
void write16(uint16_t data);
void beginWrite(void);
void endWrite(void);
int reverse(bool val);
void writeLevel(uint8_t val, uint8_t brightness);
void writeBits(uint16_t Brightness);
void writeBytes(uint8_t param[10]);
void writeSync(void);
void ClearLED(void);
void TestLED(int myCase);
void TestLED_ALL(int Code);
