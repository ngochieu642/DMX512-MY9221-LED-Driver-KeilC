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

#define PORT_LED 	GPIOA
#define DI		GPIO_Pin_7
#define DCKI	GPIO_Pin_5

extern void SysTick_DelayUs(uint32_t nTime);
extern void SysTick_DelayMs(uint32_t nTime);

SysTick_typedef systick={.Systick_Counter=0};

/*Test variable*/
uint8_t buffer[12] ={	0x01,0x00,0x01,		//Channel 3
											0x00,0x01,0x00,		//Channel 2
											0x00,0x00,0x01, 	//Channel 1
											0x00,0x00,0x00}; /*from left to right is RGB, the order we are using is BGR*/

int main(void){
	/*Config function*/
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();
	NVIC_Configuration(); /*NVIC MUST BE ABOVE TIM*/
	TIM_Configuration();
	
	ClearLED();
	TestLED_ALL(0);
	//SetColorWithCode(0,0x0f0f0f);
	
	while(1){
		/*Delay*/
		msDelay(500);
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13));		
		//int channelNumber = ChoseFlashPosition(12,1);
		//ColorRun(0x010101,0x00000f,1);
		
		uint8_t buffer[12]={3,2,1,6,5,4,9,8,7,12,11,10}; /*From right to Left*/
//		uint64_t RGB_BackGround=0x010101,RGB_RunningColor=0x00000f;
		uint16_t frequency = 7;
//		float timeDelay = 1000/frequency;
//		for (int i=0;i<12;i++)
//		{
//			int channelNumber = buffer[i];
//			msDelay((int)timeDelay);
//			Sparkle12Channel(RGB_BackGround,channelNumber,RGB_RunningColor);
//		}
	
//		for(int i=0;i<12;i++){
		Sparkle12Channel(0x010101,12,0x010000);
		msDelay(200);
		Sparkle12Channel(0x010101,1,0x010000);
		msDelay(200);
		
//		}

	}
}

/*Systick*/
uint32_t System_GetTick(void){
	return systick.Systick_Counter;
}

/*Config*/
void RCC_Configuration(void){
	/*Use PB13, PA5, PA7 for output*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*Use Timer 2 as tim base*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/*PB3*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*PA5 and PA7*/
	GPIO_InitStructure.GPIO_Pin = DI | DCKI;
	GPIO_Init(PORT_LED,&GPIO_InitStructure);
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
	