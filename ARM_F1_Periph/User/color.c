#include "main.h"
#include "color.h"

void SetColorWithCode(int channel, uint64_t RGBcolorCode){/*4 channel*/
	/*RGB color channel*/
	uint8_t RED,GREEN,BLUE;
	RED 	= RGBcolorCode>>16&0xff;
	GREEN = RGBcolorCode>>8&0xff;
	BLUE	= RGBcolorCode&0xff;
	
	uint8_t buff0[12] ={	0x00,0x00,0x00,		
												0x00,0x00,0x00,		
												0x00,0x00,0x00, 	
												0x00,0x00,0x00};
	uint8_t buff[12] ={		BLUE,GREEN,RED,		
												BLUE,GREEN,RED,		
												BLUE,GREEN,RED,		
												0,0,0};

	switch(channel){
		case 1:/*Channel l*/
		{
			for(int i=0;i<3;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff[i]);
			}
			endWrite();
	
			break;
		}
		case 2:/*Channel 2*/
		{
			for(int i=0;i<2;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff[i]);
			}
			endWrite();
			
			for(int i=0;i<1;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			
	
			break;
		}
		case 3:/*Channel 3*/
		{
			for(int i=0;i<1;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff[i]);
			}
			endWrite();
			
			for(int i=0;i<2;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			
			break;
		}
		case 4: /*Channel 4*/
		{
			beginWrite();
			for(int i=0;i<12;i++){
			write16(buff[i]);
			}
			endWrite();
			
			for(int i=0;i<3;i++){
				beginWrite();
				for(int i=0;i<12;i++){
				write16(buff0[i]);
				}
				endWrite();
			}
			break;
		}
		case 0: /*Channel All*/
		{
			for(int i=0;i<4;i++){
				beginWrite();
				for(int i=0;i<12;i++){
					write16(buff[i]);
				}
				endWrite();
			}
			break;
		}
		default:/*Channel None*/
		{
			for(int i=0;i<3;i++){
				beginWrite();
				for(int i=0;i<12;i++){
					write16(buff0[i]);
				}
				endWrite();
			}
			break;
		}
	}
};
