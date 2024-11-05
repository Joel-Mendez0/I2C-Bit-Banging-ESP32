
# ESP32 I2C Bit-Banging Driver

This project demonstrates a custom I2C bit-banging implementation to enable communication between a ESP32 microcontroller and a I2C-enabled device. The bit-banged I2C protocol provides control over communication without relying on the built-in hardware I2C peripherals.

## Features

- Custom I2C bit-banging implementation.
- Handles 1000-byte data transfers in approximately 450 milliseconds.
- Optimized for performance with efficient use of DRAM and IRAM.
- Fully implemented in C using the ESP-IDF framework.

## System Specifications

### ESP32
- **DRAM Usage**: 11,132 bytes (6.2% used)
  - `.data size`: 8,788 bytes
  - `.bss size`: 2,344 bytes
- **IRAM Usage**: 51,394 bytes (39.2% used)
  - `.text size`: 50,367 bytes
  - `.vectors size`: 1,027 bytes
- **Flash Usage**: 130,263 bytes
  - `.text size`: 87,931 bytes
  - `.rodata size`: 42,076 bytes
- **Total Image Size**: 190,445 bytes

## Performance

- **Transfer Speed**: Approximately 450,473 microseconds (450 ms) for 1000 bytes of data transfer.

**Test Code**
```
	start_time = esp_timer_get_time();
	MasterI2CTransmit(data, length);
	end_time = esp_timer_get_time();
	printf("Master Transmit Time: %lld microseconds\n", (end_time - start_time));
```

## Pulse View

![image](https://github.com/user-attachments/assets/beb5ba5f-e013-4291-903e-85f1863a3554)

## Setup Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Joel-Mendez0/I2C-Bit-Banging-ESP32.git
   cd I2C-Bit-Banging-ESP32
   ```

2. **Configure the ESP-IDF environment**:
   Follow the official [ESP-IDF installation guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).

3. **Build and flash the project**:
   ```bash
   idf.py set-target esp32
   idf.py build
   idf.py flash
   ```

## License

This project is licensed under the MIT License
