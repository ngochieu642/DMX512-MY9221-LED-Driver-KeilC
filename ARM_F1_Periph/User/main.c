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


uint16_t uart_count=0; 
uint8_t uart_data[1000];

extern void SysTick_DelayUs(uint32_t nTime);
extern void SysTick_DelayMs(uint32_t nTime);
extern uint8_t dmx_receive[512];
extern uint16_t dmx_counter;
bool SPI_send=false;
														
int main(void){
	/*Config function*/
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();
	
	UART_Configuration();
	NVIC_Configuration();
	TIM_Configuration();
	
	msDelay(1000);
	ClearLED();
	TestLED_ALL(0);
	//uartAllLED(0,uart_data);
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,0);
	
	while(1){
//		for(int i=0;i<10;i++){
//			USART_SendData(USART1,(char)uart_data[i]);
//		}
		USART_SendData(USART1,(char)uart_data[35]);
		msDelay(100);
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13));
		//uartAllLED(1,uart_data);
		TestLED_ALL(0);
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
	/*PB9*/
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;
	
	/*UART1*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
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
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStructure);
	
	/*Clear Receive Flag*/
	USART_ClearFlag(USART2,USART_IT_RXNE);
	
	/*Enable interrupt when receive or Error*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	/*Enable UART*/
	USART_Cmd(USART1,ENABLE); /*printf*/
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
/*LED function*/
void trigger_latch(void){/*do the trigger work*/
	GPIO_WriteBit(PORT_LED,DI,0);
	msDelay(1);
	for(int i=0;i<4;i++){
		GPIO_SetBits(PORT_LED,DI);
		usDelay(250);
		GPIO_ResetBits(PORT_LED,DI);
		usDelay(250);
		
	}
}
void write16(uint16_t data){/*send 16 bit*/
	for (int i=15;i>=0;i--){
		GPIO_WriteBit(PORT_LED,DI,(data>>i)&1);
		GPIO_WriteBit(PORT_LED,DCKI,!GPIO_ReadOutputDataBit(PORT_LED,DCKI)); //Create clock
	}
}
void beginWrite(void){/*write 16 command bit*/
	write16(0); /*Command 8 bit mode - bit 207 to 192*/
}
void endWrite(void){
	trigger_latch();
}
void ClearLED(void){
		uint8_t myBuff[12] ={0x000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00}; 
		beginWrite();
		for(int i=0;i<12;i++){
			write16(myBuff[i]);
		}
		endWrite();
}
void uartLED(uint8_t buffer[], int myCase){
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	/*Block 1*/
	uint8_t buff1[12] ={	buffer[4],buffer[3],buffer[2], 
												buffer[7],buffer[6],buffer[5],		
												buffer[10],buffer[9],buffer[8], 	
												0x00,0x00,0x00};
	/*Block 2*/
	uint8_t buff2[12] ={	buffer[13],buffer[12],buffer[11], 
												buffer[16],buffer[15],buffer[14],		
												buffer[19],buffer[18],buffer[17], 	
												0x00,0x00,0x00};
	/*Block 3*/
	uint8_t buff3[12] ={	buffer[22],buffer[21],buffer[20], 
												buffer[25],buffer[24],buffer[23],		
												buffer[28],buffer[27],buffer[26], 	
												0x00,0x00,0x00};
	/*Block 4*/
	uint8_t buff4[12] ={	buffer[31],buffer[30],buffer[29], 
												buffer[34],buffer[33],buffer[32],		
												buffer[37],buffer[36],buffer[35], 	
												0x00,0x00,0x00};
	
		switch(myCase){
		case 1:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff1[i]);
			}
			endWrite();
			break;
		case 2:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff2[i]);
			}
			endWrite();
			break;
		case 3:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff3[i]);
			}
			endWrite();
			break;
		case 4:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff4[i]);
			}
			endWrite();
			break;
		default:{
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff0[i]);
			}
			endWrite();
			break;}			
	}										
}
void uartAllLED(int myCode,uint8_t buffer[]){
	switch(myCode){
		case 0:
			for(int i=0;i<4;i++)
				uartLED(buffer,0);
				break;
		default:
				uartLED(buffer,1);
				uartLED(buffer,2);
				uartLED(buffer,3);
				uartLED(buffer,4);
				break;
	}
}
void TestLED(int myCase){
	
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	/*Block 1	R G B*/
	uint8_t buff1[12] ={	0x01,0x00,0x00, 	//b	3
												0x00,0x01,0x00,		//g 2
												0x00,0x00,0x01, 	//r 1
												0x00,0x00,0x00};
	/*Block 2 RG GB RB*/
	uint8_t buff2[12] ={	0x01,0x00,0x01,		
												0x01,0x01,0x00,		
												0x00,0x01,0x01, 	
												0x00,0x00,0x00};
	/*Block 3 RB GB RG*/
	uint8_t buff3[12] ={	0x00,0x01,0x01,		
												0x01,0x01,0x00,		
												0x01,0x00,0x01, 	
												0x00,0x00,0x00};
	/*Block 4 B G R*/
	uint8_t buff4[12] ={	0x00,0x00,0x01,		
												0x00,0x01,0x00,		
												0x01,0x00,0x00, 	
												0x00,0x00,0x00};
	switch(myCase){
		case 1:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff1[i]);
			}
			endWrite();
			break;
		case 2:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff2[i]);
			}
			endWrite();
			break;
		case 3:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff3[i]);
			}
			endWrite();
			break;
		case 4:
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff4[i]);
			}
			endWrite();
			break;
		default:{
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff0[i]);
			}
			endWrite();
			break;}			
	}
}
void TestLED_ALL(int myCode){
	switch(myCode){
		case 0:
			for(int i=0;i<4;i++)
			TestLED(0);
			break;
		default:
			TestLED(1);
			TestLED(2);
			TestLED(3);
			TestLED(4);
			break;
	}
}