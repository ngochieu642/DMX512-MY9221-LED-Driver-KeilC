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
bool SPI_send=false;
														
int main(void){
	/*Config function*/
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();
	
	UART_Configuration();
	NVIC_Configuration();
	TIM_Configuration();
	
	ClearLED();
	TestLED_ALL(0);
	
	while(1){
		
		/*Code for UART Board*/
//		USART_SendData(USART1,(char)uart_data[14]);
//		msDelay(100);
		
		/*Code for LED Bar Board*/
		ClearLED();
		//TestLED_ALL(1);
		uartAllLED(1);
		msDelay(0);
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
	GPIO_InitStructure.GPIO_Pin = DI | DCKI;
	GPIO_Init(PORT_LED,&GPIO_InitStructure);
	
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
void uartLED(int myCase){
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	/*Block 1*/
	uint8_t buff1[12] ={	uart_data[10],uart_data[9],uart_data[8], 		//3
												uart_data[7] ,uart_data[6],uart_data[5],			//2
												uart_data[4] ,uart_data[3],uart_data[2], 			//1
												0x00,0x00,0x00};
	/*Block 2*/
	uint8_t buff2[12] ={	uart_data[19],uart_data[18],uart_data[17], 	//6
												uart_data[16],uart_data[15],uart_data[14],		//5
												uart_data[13],uart_data[12],uart_data[11], 	//4
												0x00,0x00,0x00};
	/*Block 3*/
	uint8_t buff3[12] ={	uart_data[28],uart_data[27],uart_data[26], 	//9
												uart_data[25],uart_data[24],uart_data[23],		//8
												uart_data[22],uart_data[21],uart_data[20], 	//7
												0x00,0x00,0x00};
	/*Block 4*/
	uint8_t buff4[12] ={	uart_data[37],uart_data[36],uart_data[35], 	//12	
												uart_data[34],uart_data[33],uart_data[32],		//11
												uart_data[31],uart_data[30],uart_data[29], 	//10
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
void uartAllLED(int myCode){
	switch(myCode){
		case 0:
			for(int i=0;i<4;i++)
				uartLED(0);
				break;
		default:
				uartLED(1);
				uartLED(2);
				uartLED(3);
				uartLED(4);
				break;
	}
}

void TestLED(int myCase){
	
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	/*Block 1	R G B*/
												/*B		G			R*/
	uint8_t buff1[12] ={	0x01,0x00,0x00, 	//b	3							 3
												0x00,0x01,0x00,		//g 2             2                                           
												0x00,0x00,0x01, 	//r 1            1                                                                                                                    
												0x00,0x00,0x00};
	/*Block 2 RG GB RB*/
	uint8_t buff2[12] ={	0x01,0x00,0x01,						//						6
												0x01,0x01,0x00,						//					 5		
												0x00,0x01,0x01, 					//					4	
												0x00,0x00,0x00};
	/*Block 3 RB GB RG*/
	uint8_t buff3[12] ={	0x00,0x01,0x01,		//									9
												0x01,0x01,0x00,		//								 8
												0x01,0x00,0x01, 	//								7
												0x00,0x00,0x00};
	/*Block 4 B G R*/
	uint8_t buff4[12] ={	0x00,0x00,0x01,		//									12
												0x00,0x01,0x00,		//								11
												0x01,0x00,0x00, 	//							10
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