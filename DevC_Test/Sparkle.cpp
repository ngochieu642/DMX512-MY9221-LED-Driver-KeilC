#include "Sparkle.h"
#include <iostream>
#include <ctime>
#include "main.h"

using namespace std;

void MySparkle::int_set(int color, int freq, bool darkOrNot){
	m_ColorCode = color;
	m_Frequency = freq;
	m_DarkAround = darkOrNot;
}
int * getColor(){
	static int color[12];
	
	//Set the seed
	srand((unsigned)time(NULL));
	
	for(int i=0;i<12;i++){
		color[i]=rand();
	}
	return color;
}
