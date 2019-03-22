#ifdef __cplusplus
extern "C" {
#endif
	//Code here
	typedef struct MySparkle MySparkle;
	MySparkle* newMySparkle();
	void MySparkle_int_set(MySparkle* v,int color, int freq, bool darkOrNot);
	int MySparkle_getColor(MySparkle* v);
	void deleteMySparkle(MySparkle* v);
	#ifdef __cplusplus
}
#endif