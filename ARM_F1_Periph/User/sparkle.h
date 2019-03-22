#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include "color.h"

int ChoseFlashPosition(int max, int min);
void SparkleWithBackground(uint64_t RGB_Background,int channelSpark);
void Sparkle12Channel(uint64_t RGB_Background, int channelSpark, uint64_t RGBsparkColor);