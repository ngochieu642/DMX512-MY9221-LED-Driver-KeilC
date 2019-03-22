#include "colorRun.h"
#include "main.h"

void ColorRun(uint64_t RGB_BackGround, uint64_t RGB_RunningColor, uint16_t frequency){
	uint8_t buffer[12]={3,2,1,6,5,4,9,8,7,12,11,10};
	
	frequency=(frequency>1000)?1000:frequency;
	frequency=(frequency<1)?1:frequency;
	
	float timeDelay = 1000/frequency;
	int delayTime=(int)timeDelay;
	
	while(1){
	for (int i=0;i<12;i++)
		{
			int channelNumber = buffer[i];
			msDelay(delayTime);
			Sparkle12Channel(RGB_BackGround,channelNumber,RGB_RunningColor);
		}
	}
}