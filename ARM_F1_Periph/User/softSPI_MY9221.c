#include "softSPI_MY9221.h"
#include "main.h"

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