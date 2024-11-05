#ifndef MAIN_I2C_H_
#define MAIN_I2C_H_

#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/gptimer_types.h"
#include "esp_attr.h"

#define LOW 0
#define HIGH 1

extern gptimer_handle_t clk_timer;

typedef struct{
	gpio_num_t SDAPin;
	gpio_num_t SCLPin;
	uint8_t address;
} I2Ccfg;

/*I2C PROTOTYPES*/
void InitI2C(const I2Ccfg *cfg);
void StartConditionI2C(void);
void StopConditionI2C(void);
void Transmit(uint8_t byte);
void MasterI2CTransmit(uint8_t *data, uint32_t length);
/*TIMER FUNCTIONS*/
void start_timer(uint64_t period); // Starts CLK
void end_timer(void); // Ends CLK

#endif