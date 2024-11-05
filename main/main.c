#include <stdio.h>
#include <unistd.h>
#include "I2C.h"
#include "esp_timer.h"

uint64_t start_time;
uint64_t end_time;

void app_main(void)
{
	I2Ccfg cfg = {
		.SDAPin = 22,
		.SCLPin = 23,
		.address = 0b11110000
	};
	
	InitI2C(&cfg);
	
	uint32_t length = 1000;
	uint8_t data[length];
	
	for(uint32_t i = 0; i < length; i++){
		data[i] = i % 256;
	}
	
	sleep(1);
	
	start_time = esp_timer_get_time();
	MasterI2CTransmit(data, length);
	end_time = esp_timer_get_time();
	printf("Master Transmit Time: %lld microseconds\n", (end_time - start_time));
	
	
}
