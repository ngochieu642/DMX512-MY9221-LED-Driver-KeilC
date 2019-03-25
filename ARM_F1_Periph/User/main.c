#include "main.h"
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	USART_SendData(USART1, (uint8_t) ch); 
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	
	return ch;
}
SysTick_typedef systick={.Systick_Counter=0};

extern void SysTick_DelayUs(uint32_t nTime);
extern void SysTick_DelayMs(uint32_t nTime);
extern uint8_t dmx_receive[512];
														
int main(void){
	/*Config function*/
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();
	
	UART_Configuration();
	NVIC_Configuration();
	TIM_Configuration();
	
	
	while(1){
		msDelay(1000);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_13,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13));
		//GPIO_WriteBit(GPIOB,GPIO_Pin_13,1);
		
	}
}

/*Systick*/
uint32_t System_GetTick(void){
	return systick.Systick_Counter;
}

/*Config*/
void RCC_Configuration(void){
	/*Use PB13, PA9 PA10 UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*Use Timer 2 as tim base*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	/*UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	/*UART2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
}
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/*PB13*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*PA5 and PA7*/
//	GPIO_InitStructure.GPIO_Pin = DI | DCKI;
//	GPIO_Init(PORT_LED,&GPIO_InitStructure);
	
	/*PA9-Rx PA10-Tx*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*PA3 as Rx*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void SysTick_Configuration(void){
  if (SysTick_Config(SystemCoreClock/1000) ) //1000000:us 1000:ms
  { 
		while(1);
  }
}
void TIM_Configuration(void){
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	/*
	TIM2 freq is 1Hz
	Freq = 1Hz = 1s = 1000ms => choose TIM_Periodd = 1000 -1;
	*/
	TIM_DeInit(TIM2);
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_InitStructure.TIM_Prescaler	=63999;
	TIM_InitStructure.TIM_Period =999;
	TIM_InitStructure.TIM_ClockDivision = 0;
	TIM_InitStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE); /*Tim IT Enable*/
	
	/*TIM3 freq = 1000 Hz*/
	TIM_DeInit(TIM3);
	TIM_InitStructure.TIM_Prescaler = 63;
	TIM_InitStructure.TIM_Period = 1000;
	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);
	
}
void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	/*Enable TIM2 interrupt*/
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;
	
	/*UART1*/
	USART_InitStructure.USART_BaudRate = 250000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	/*UART2*/
	USART_InitStructure.USART_BaudRate = 250000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	/*Clear Receive Flag*/
	USART_ClearFlag(USART1,USART_IT_RXNE);
	USART_ClearFlag(USART2,USART_IT_RXNE);
	
	/*Enable interrupt when receive*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	/*Enable UART*/
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
}
/*Delay*/
void usDelay(uint32_t nTime){
	uint16_t counter = nTime & 0xffff;
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCounter(TIM3,counter);
	while (counter > 1)
	{
		counter = TIM_GetCounter(TIM3);
	}
	TIM_Cmd(TIM3, DISABLE);
}
void msDelay(uint32_t nTime){ /*function to delay nTime ms with TIM2 set as ms*/
	uint16_t counter = nTime & 0xffff;
	TIM_Cmd(TIM2,ENABLE);
	TIM_SetCounter(TIM2, counter);
	while (counter > 1)
	{
		counter = TIM_GetCounter(TIM2);
	}
	TIM_Cmd(TIM2,DISABLE);
}