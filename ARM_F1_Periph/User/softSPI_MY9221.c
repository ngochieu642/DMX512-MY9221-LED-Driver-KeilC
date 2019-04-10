#include "softSPI_MY9221.h"
#include "main.h"

/*LED function*/
void trigger_latch(void){
	/*do the trigger work
	*Based on internal-latch control cycle timing Diagram of MY9221*/
	
	GPIO_WriteBit(PORT_LED,DI,0);
	usDelay(220);
	
	for(int i=0;i<4;i++){
		GPIO_SetBits(PORT_LED,DI);
		usDelay(1);
		GPIO_ResetBits(PORT_LED,DI);
		usDelay(1);
	usDelay(1);
	}
}
void write16(uint16_t data){
	/*send 16 bit using Soft SPI*/
	for (int i=15;i>=0;i--){
		data&=0xff;
		GPIO_WriteBit(PORT_LED,DI,(data>>i)&1);
		GPIO_WriteBit(PORT_LED,DCKI,!GPIO_ReadOutputDataBit(PORT_LED,DCKI)); //Create clock
	}
}
void beginWrite(void){/*write 16 command bit*/
	write16(0); /*Command 8 bit mode - bit 207 to 192, details in MY9221 bit cmd descrip*/
}
void endWrite(void){
	trigger_latch();
}
void writeBuff16(uint8_t buff[12]){
	uint8_t buffTemp[12] ={	buff[10],buff[9],buff[8], 			//3
													buff[7] ,buff[6],buff[5],				//2
													buff[4] ,buff[3],buff[2], 			//1
													0x00,0x00,0x00};
	beginWrite();
	for(int i=0;i<12;i++)
		write16(buffTemp[i]);
	trigger_latch();
}
void ClearLED(void){

		uint8_t myBuff[12] ={0x000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00}; 
		beginWrite();
		for(int i=0;i<12;i++){
			write16(myBuff[i]);
		}
		trigger_latch();
}

void TestLED(int myCase){
	/*This is not the right way to use MY9221, only latch when you have had enoough data*/
	
	/*Color case*/
//	uint8_t buff0[12] ={	0x00,0x00,0x00,		
//												0x00,0x00,0x00,		
//												0x00,0x00,0x00, 	
//												0x00,0x00,0x00};
//	/*Block 1	R G B*/
//												/*B		G			R*/
//	uint8_t buff1[12] ={	0xff,0x00,0x00, 	//b	3							 3
//												0x00,0x01,0x00,		//g 2             2                                           
//												0x00,0x00,0xff, 	//r 1            1                                                                                                                    
//												0x00,0x00,0x00};
//	/*Block 2 RG GB RB*/
//	uint8_t buff2[12] ={	0x01,0x00,0x01,						//						6
//												0x01,0x01,0x00,						//					 5		
//												0x00,0x01,0x01, 					//					4	
//												0x00,0x00,0x00};
//	/*Block 3 RB GB RG*/
//	uint8_t buff3[12] ={	0x00,0x01,0x01,		//									9
//												0x01,0x01,0x00,		//								 8
//												0x01,0x00,0x01, 	//								7
//												0x00,0x00,0x00};
//	/*Block 4 B G R*/
//	uint8_t buff4[12] ={	0x00,0x00,0x01,		//									12
//												0x00,0x01,0x00,		//								11
//												0x01,0x00,0x00, 	//							10
//												0x00,0x00,0x00};
//	
	/*Empty case*/
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	/*Block 1	R G B*/
												/*B		G			R*/
	uint8_t buff1[12] ={	0xff,0x00,0x00, 	//b	3							 3
												0x00,0xff,0x00,		//g 2             2                                           
												0x00,0x00,0xff, 	//r 1            1                                                                                                                    
												0x00,0x00,0x00};
	/*Block 2 RG GB RB*/
	uint8_t buff2[12] ={	0x00,0x00,0x00,						//						6
												0x00,0x00,0x00,						//					 5		
												0x00,0x00,0x00, 					//					4	
												0x00,0x00,0x00};
	/*Block 3 RB GB RG*/
	uint8_t buff3[12] ={	0x00,0x00,0x00,		//									9
												0x00,0x00,0x00,		//								 8
												0x00,0x00,0x00, 	//								7
												0x00,0x00,0x00};
	/*Block 4 B G R*/
	uint8_t buff4[12] ={	0x00,0x00,0x00,		//									12
												0x00,0x00,0x00,		//								11
												0x00,0x00,0x00, 	//							10
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



void ClearLEDFull(void){
	uint8_t myBuff[12] ={0x000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
	for(int j=0;j<4;j++){
		beginWrite();
		for(int i=0;i<12;i++){
			write16(myBuff[i]);
		}
	}
	trigger_latch();
}
void TestLED2(void){/*This the right way to use MY9221*/
	/*
	This function used to test LED color in while loop
	*/
	static uint8_t count=0;
	
	//ClearLEDFull();
	
	beginWrite(); 				//16 bits
	/*This channel will be push first, the final block will receive this*/
	write16(0x00); /*C0 - 16 bit- Blue*/ 
	write16(0x00); /*B0 - 16 bit- Green*/							//	3
	write16(0x00); /*A0 - 16 bit- Red*/					
	
	write16(0x00); /*C1 - 16 bit- Blue*/							//	2
	write16(0x00); /*B1 - 16 bit- Green*/
	write16(0x00); /*A1 - 16 bit- Red*/												
	
	write16(0xff); /*C2 - 16 bit- Blue*/
	write16(0x00); /*B2 - 16 bit- Green*/							//	1
	write16(0x00); /*A2 - 16 bit- Red*/				
	/*Always Zero because this channel is not included in design*/
	write16(0x00); /*C3 - 16 bit- Blue*/
	write16(0x00); /*B3 - 16 bit- Green*/
	write16(0x00); /*A3 - 16 bit- Red*/


	beginWrite(); 				//16 bits
	write16(0x00); /*C0 - 16 bit- Blue*/
	write16(0x00); /*B0 - 16 bit- Green*/							// 6
	write16(0x00); /*A0 - 16 bit- Red*/								
	
	write16(0x00); /*C1 - 16 bit- Blue*/
	write16(0x00); /*B1 - 16 bit- Green*/							// 5
	write16(0x00); /*A1 - 16 bit- Red*/			
	
	write16(0x00); /*C2 - 16 bit- Blue*/
	write16(0x00); /*B2 - 16 bit- Green*/							// 4
	write16(0x00); /*A2 - 16 bit- Red*/				
	/*Always Zero because this channel is not included in design*/
	write16(0x00); /*C3 - 16 bit- Blue*/
	write16(0x00); /*B3 - 16 bit- Green*/
	write16(0x00); /*A3 - 16 bit- Red*/
	
	
	beginWrite(); 				//16 bits
	write16(0x00); /*C0 - 16 bit- Blue*/
	write16(0x00); /*B0 - 16 bit- Green*/							// 9
	write16(0x00); /*A0 - 16 bit- Red*/			
	
	write16(0x00); /*C1 - 16 bit- Blue*/
	write16(0x00); /*B1 - 16 bit- Green*/							// 8
	write16(0x00); /*A1 - 16 bit- Red*/				
	
	write16(0x00); /*C2 - 16 bit- Blue*/
	write16(0x00); /*B2 - 16 bit- Green*/							// 7
	write16(0x00); /*A2 - 16 bit- Red*/				
	/*Always Zero because this channel is not included in design*/
	write16(0x00); /*C3 - 16 bit- Blue*/
	write16(0x00); /*B3 - 16 bit- Green*/
	write16(0x00); /*A3 - 16 bit- Red*/
	
	
	beginWrite(); 				//16 bits
	write16(0x00); /*C0 - 16 bit- Blue*/
	write16(0x00); /*B0 - 16 bit- Green*/							// 12
	write16(0xff); /*A0 - 16 bit- Red*/			
	
	write16(0x00); /*C1 - 16 bit- Blue*/
	write16(0x00); /*B1 - 16 bit- Green*/							// 11
	write16(0xff); /*A1 - 16 bit- Red*/				
	
	write16(0x00); /*C2 - 16 bit- Blue*/
	write16(0x00); /*B2 - 16 bit- Green*/							// 10
	write16(0xff); /*A2 - 16 bit- Red*/				
	/*Always Zero because this channel is not included in design*/
	write16(0x00); /*C3 - 16 bit- Blue*/
	write16(0x00); /*B3 - 16 bit- Green*/
	write16(0x00); /*A3 - 16 bit- Red*/
	
	trigger_latch();	
}
void uartAllLED(void){/*Get Data from UART and push to MY9221 using soft SPI. Order of LED can be different depends on your hardware*/
	/*Block 1*/
	uint8_t buff1[12] ={	uart_data[10],uart_data[9],uart_data[8], 			//3
												uart_data[7] ,uart_data[6],uart_data[5],			//2
												uart_data[4] ,uart_data[3],uart_data[2], 			//1
												0x00,0x00,0x00};
	/*Block 2*/
	uint8_t buff2[12] ={	uart_data[19],uart_data[18],uart_data[17], 		//6
												uart_data[16],uart_data[15],uart_data[14],		//5
												uart_data[13],uart_data[12],uart_data[11], 		//4
												0x00,0x00,0x00};
	/*Block 3*/
	uint8_t buff3[12] ={	uart_data[28],uart_data[27],uart_data[26], 		//9
												uart_data[25],uart_data[24],uart_data[23],		//8
												uart_data[22],uart_data[21],uart_data[20], 		//7
												0x00,0x00,0x00};
	/*Block 4*/
	uint8_t buff4[12] ={	uart_data[37],uart_data[36],uart_data[35], 		//12	
												uart_data[34],uart_data[33],uart_data[32],		//11
												uart_data[31],uart_data[30],uart_data[29], 		//10
												0x00,0x00,0x00};	
	beginWrite(); 				//16 bits
	/*This channel will be push first, the final block will receive this*/
	for(int i=0;i<12;i++)
		write16(buff1[i]);

	beginWrite(); 				//16 bits
	for(int i=0;i<12;i++)
		write16(buff2[i]);
	
	beginWrite(); 				//16 bits
	for(int i=0; i<12;i++)
		write16(buff3[i]);
	
	
	beginWrite(); 				//16 bits
	for(int i=0;i<12;i++)
		write16(buff4[i]);
												
	trigger_latch();	
}