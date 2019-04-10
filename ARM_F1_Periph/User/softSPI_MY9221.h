#include "stm32f10x.h"

/*LED function*/
void trigger_latch(void);
void write16(uint16_t data);
void writeBuff16(uint8_t buff[12]);
void beginWrite(void);
void endWrite(void);
void ClearLED(void);
void TestLED(int myCase);
void TestLED_ALL(int myCode);
void TestLED2(void);
void ClearLEDFull(void);

/*UART LED*/
void uartAllLED(void);
