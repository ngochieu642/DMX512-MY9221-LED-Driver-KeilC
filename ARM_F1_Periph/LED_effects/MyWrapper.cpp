#include "MyWrapper.h"
#include "Sparkle.h"

extern "C"{
	MySparkle* newMySparkle(){
		return new MySparkle();
	}
	void MySparkle_int_set(MySparkle* v, int color, int freq, bool darkOrNot){
		v->int_set(color,freq,darkOrNot);
	}
	int MySparkle_getColor(MySparkle* v){
		return *v->getColor();
	}
	void deleteMySparkle(MySparkle* v){
		delete v;
	}
}
