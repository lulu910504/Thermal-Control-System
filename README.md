# Thermal-Control-System
This project implements an embedded thermal management system using ESP32, integrating temperature sensing, real-time display, and automatic cooling control.
The system monitors temperature using a DS18B20 sensor and activates a cooling module via relay when a predefined threshold is exceeded. An OLED display provides real-time visualization of system status.

<img width="419" height="729" alt="image" src="https://github.com/user-attachments/assets/332ccf51-de04-42db-8612-0a85cc973db7" />


## Hardware Components

- ESP32 (main controller)
- DS18B20 (digital temperature sensor)
- Relay module (high-power switching)
- Thermoelectric cooler (TEC) + fan
- OLED display (SSD1306, I2C interface)
- DC-DC voltage converter (5V / 3.3V regulation)
- 12V power supply (for TEC module)
<img width="531" height="598" alt="image" src="https://github.com/user-attachments/assets/abd12f4f-1302-4a8b-80ea-f0cd19872efa" />

## Methodology

### 1. Temperature Monitoring
The DS18B20 sensor continuously measures temperature and sends data to ESP32.

### 2. Control Logic
- If temperature > threshold → Relay ON → Cooling starts  
- If temperature < threshold → Relay OFF 

## Custom OLED Graphics 
<img width="1507" height="916" alt="image" src="https://github.com/user-attachments/assets/21bbece9-1b3d-4795-bb30-53c1bfa36dda" />

Tool used:
https://javl.github.io/image2cpp/

This tool converts images into byte arrays compatible with embedded systems.

```cpp
const unsigned char epd_bitmap_39_398460_christmas_tree_icon [] PROGMEM = {
  ...
};
