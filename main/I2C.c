#include "I2C.h"

gptimer_handle_t clk_timer;
static uint64_t default_period = 25;

static volatile uint8_t SDA_LEVEL = 1;
static volatile uint8_t SCL_LEVEL = 1; 

static gpio_config_t SDAconfig;
static gpio_config_t SCLconfig;
static gpio_num_t SDA;
static gpio_num_t SCL;
static uint8_t address;

static bool IRAM_ATTR timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *event_data, void *param) {
    if (SCL_LEVEL) 
    {
		gpio_set_level(SCL, LOW); 
    } 
    else
    {
		gpio_set_level(SCL, HIGH);
    }

    SCL_LEVEL = !SCL_LEVEL;
    return false;
}

void InitI2C(const I2Ccfg *cfg){
	SDA = cfg->SDAPin;
	SCL = cfg->SCLPin;
	address = cfg->address;
	
	SDAconfig = (gpio_config_t){
        .pin_bit_mask = (1ULL << SDA),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    SCLconfig = (gpio_config_t){
        .pin_bit_mask = (1ULL << SCL),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    gpio_config(&SDAconfig);
    gpio_config(&SCLconfig);
    
    gpio_set_level(SCL, HIGH);
    gpio_set_level(SDA, HIGH);

	// Hardware Timer Setup
	gptimer_config_t timer_config = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.direction = GPTIMER_COUNT_UP,
		.resolution_hz = 1000000,
	};
	gptimer_new_timer(&timer_config,&clk_timer);
	
	gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_callback,
    };
	
	gptimer_alarm_config_t alarm_config = {
		.reload_count = 0,
		.alarm_count = default_period,
		.flags.auto_reload_on_alarm = true
	};
	
	gptimer_set_alarm_action(clk_timer, &alarm_config);
    gptimer_register_event_callbacks(clk_timer, &cbs, NULL);
	gptimer_enable(clk_timer);
	
	//Call Transmit once during initialization to stabilize the timing
	uint8_t stable[1] = {0};
	MasterI2CTransmit(stable, 1);
	
    
} 
void start_timer(uint64_t period) {
	
    gptimer_set_alarm_action(clk_timer, &(gptimer_alarm_config_t){
        .alarm_count = period,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true
    });
    
    gptimer_start(clk_timer);
}

void end_timer(void) {
    gptimer_stop(clk_timer);
}

void StartConditionI2C(void){
	/*SDA Gets Pulled LOW, after which the SCL gets pulled LOW*/
	gpio_set_level(SDA,LOW);
	SDA_LEVEL = 0;
	
	gpio_set_level(SCL,LOW);
	SCL_LEVEL = 0;
}

void StopConditionI2C(void) {

	while (SCL_LEVEL); // Wait for clock to go low
        
   	 gpio_set_level(SDA, LOW);     
   
   	while (!SCL_LEVEL); // Wait for clock to go high
    
    	gpio_set_level(SDA, HIGH); // Float SDA line
    
    	while(!SCL_LEVEL);

}

void Transmit(uint8_t byte) {
	
    uint8_t bit_count = 0;

    while (bit_count < 8) {
        // Set SDA when Clock is LOW, so we wait while the clock is high
        while (SCL_LEVEL); // Wait for clock to go low
        
        // Set SDA to transmit the current bit (left shift and mask to get the MSB)
        gpio_set_level(SDA, (byte << bit_count) & 0x80);

        bit_count++;

        // Wait for Clock to go high before moving to the next bit
        while (!SCL_LEVEL); // Wait for clock to go high
    }

    // All bits transmitted, now handle ACK

    // Release SDA (set it high) so that the slave can control it during ACK phase
    while (SCL_LEVEL); // Wait for Clock to go low
    gpio_set_level(SDA, HIGH); // Float SDA line
    
    gpio_set_level(SDA, LOW); // simulate ACK
    while(!SCL_LEVEL);
    
    //EndCLK(); // End clock cycle
}

void MasterI2CTransmit(uint8_t *data, uint32_t length){
	StartConditionI2C();
	
	start_timer(default_period); // Starts CLK
	
	Transmit(address);
	
	for(uint32_t i = 0; i < length; i++){
		Transmit(*data);
		data++;		
	}
	StopConditionI2C();
	end_timer(); // Ends CLK
}
