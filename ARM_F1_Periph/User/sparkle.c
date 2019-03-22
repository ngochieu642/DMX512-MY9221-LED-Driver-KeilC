#include "sparkle.h"
#include "main.h"

int ChoseFlashPosition(int max, int min){
	/*chose a random number from 1 to 4*/
	return rand()%(max+1-min)+min;
}
void SparkleWithBackground(uint64_t RGB_Background,int channelSpark){/*Spark with 4 channel*/
	uint8_t RED,GREEN,BLUE;
	RED 	= RGB_Background>>16&0xff;
	GREEN = RGB_Background>>8&0xff;
	BLUE	= RGB_Background&0xff;
	uint8_t buff[12] ={		BLUE,GREEN,RED,		
												BLUE,GREEN,RED,		
												BLUE,GREEN,RED,
												0,0,0};
	uint8_t buffSpark[12] ={0x0f,0x0f,0x0f,		
													0x0f,0x0f,0x0f,		
													0x0f,0x0f,0x0f,		
													0,0,0};
	switch(channelSpark){
		case 0:
			SetColorWithCode(0,RGB_Background);
			break;
		case 1:
			{
			for(int i=0;i<3;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buffSpark[i]);
			}
			endWrite();
	
			break;
			}
		case 2:
		{
			for(int i=0;i<2;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buffSpark[i]);
			}
			endWrite();
			
			for(int i=0;i<1;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			break;
		}
		case 3:
		{
			for(int i=0;i<1;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buffSpark[i]);
			}
			endWrite();
			
			for(int i=0;i<2;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			break;
		}
		case 4:
		{
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buffSpark[i]);
			}
			endWrite();
			
			for(int i=0;i<3;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff[i]);
				}
				endWrite();
			}
			break;
		}
		default:
			break;
	}
}
void Sparkle12Channel(uint64_t RGB_Background, int channelSpark, uint64_t RGBsparkColor){/*Spark with 12 channel*/
	uint8_t RED,GREEN,BLUE,sparkRED,sparkGREEN,sparkBLUE;
	
	RED 	= RGB_Background>>16&0xff;
	GREEN = RGB_Background>>8&0xff;
	BLUE	= RGB_Background&0xff;
	
	sparkRED 		= RGBsparkColor>>16&0xff;
	sparkGREEN 	= RGBsparkColor>>8&0xff;
	sparkBLUE		= RGBsparkColor&0xff;
	uint8_t sparkColor[3] ={sparkBLUE,sparkGREEN,sparkRED};
	
	uint8_t buffGR[12] ={	BLUE,GREEN,RED,		
												BLUE,GREEN,RED,		
												BLUE,GREEN,RED,		
												0,0,0};
	uint8_t buff_10_12[12] ={	BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														0,0,0};	
	uint8_t buff_7_9[12] ={		BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														0,0,0};
	uint8_t buff_4_6[12] ={		BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														0,0,0};
	uint8_t buff_1_3[12] ={		BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														BLUE,GREEN,RED,		
														0,0,0};
	/*1-3*/
	if(channelSpark==1){
		for(int i=6;i<9;i++)
			buff_1_3[i]=sparkColor[i-6];
	}
	if(channelSpark==2){
		for(int i=3;i<6;i++)
			buff_1_3[i]=sparkColor[i-3];
	}
	if(channelSpark==3){
		for(int i=0;i<3;i++)
			buff_1_3[i]=sparkColor[i];
	}
	
	/*4-6*/
	if(channelSpark==4){
		for(int i=6;i<9;i++)
		buff_4_6[i]=sparkColor[i-6];
	}
	if(channelSpark==5){
		for(int i=3;i<6;i++)
		buff_4_6[i]=sparkColor[i-3];
	}
	if(channelSpark==6){
		for(int i=0;i<3;i++)
		buff_4_6[i]=sparkColor[i];
	}
	
	/*7-9*/
	if(channelSpark==7){
		for(int i=6;i<9;i++)
		buff_7_9[i]=sparkColor[i-6];
	}
	if(channelSpark==8){
		for(int i=3;i<6;i++)
		buff_7_9[i]=sparkColor[i-3];
	}
	if(channelSpark==9){
		for(int i=0;i<3;i++)
		buff_7_9[i]=sparkColor[i];
	}
	
		/*10-12*/
	if(channelSpark==10){
		for(int i=6;i<9;i++)
		buff_10_12[i]=sparkColor[i-6];
		}
	if(channelSpark==11){
		for(int i=3;i<6;i++)
		buff_10_12[i]=sparkColor[i-3];
	}
	if(channelSpark==12){
		for(int i=0;i<3;i++)
		buff_10_12[i]=sparkColor[i];
	}
	/*Push LED*/
	ClearLED();
	
	beginWrite();
	for(int i=0;i<12;i++){
		write16(buff_10_12[i]);
	}
	endWrite();
	
	beginWrite();
	for(int i=0;i<12;i++){
		write16(buff_7_9[i]);
	}
	endWrite();
	
	beginWrite();
	for(int i=0;i<12;i++){
		write16(buff_4_6[i]);
	}
	endWrite();
	
	beginWrite();
	for(int i=0;i<12;i++){
		write16(buff_1_3[i]);
	}
	endWrite();
};