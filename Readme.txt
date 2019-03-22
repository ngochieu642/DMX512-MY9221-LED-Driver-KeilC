Use ST-LINK V2 to program
	+ Remove 2 jumpers on CN3
	+ Conect CN2 pin to Pin on STM32F1
		CN PIN Order (nhìn từ trên xuống dưới)
		1. VDD_Target (nhưng không nên nối với VDD của f1 vì st link đã có nguồn từ ARM f4)
		2. SWDCLK
		3. GND
		4. SWDIO
	+ Cấp nguồn stm32f4
	+ Cấp nguồn stm32f1
	+ Nạp bắng KeilC