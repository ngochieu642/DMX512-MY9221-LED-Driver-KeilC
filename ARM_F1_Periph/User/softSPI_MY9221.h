#include "stm32f10x.h"

/*LED function*/
void trigger_latch(void);
void write16(uint16_t data);
void beginWrite(void);
void endWrite(void);
void ClearLED(void);
void TestLED(int myCase);
void TestLED_ALL(int myCode);
/*UART LED*/
void uartLED(int myCase);
void uartAllLED(int myCode);