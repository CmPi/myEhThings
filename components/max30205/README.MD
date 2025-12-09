# ESPHome MAX30205 Temperature Sensor Component

[![GitHub Release](https://img.shields.io/github/release/CmPi/myEhThings.svg?style=flat-square)](https://github.com/CmPi/myEhThings/releases)
[![License](https://img.shields.io/github/license/CmPi/myEhThings.svg?style=flat-square)](LICENSE)

ESPHome custom component for the **MAX30205** human body temperature sensor. This high-accuracy I²C temperature sensor is designed for medical and fitness applications.

## Component Features

- 🔌 Easy integration with ESPHome
- 🛡️ Robust error handling and status reporting
- 📝 Detailed logging for troubleshooting
- 🔧 Configurable I²C address (0x48 to 0x4F)
- ⚙️ Configurable update interval
- ✅ Compatible with ESPHome 2024.x and 2025.x
- 🔄 Automatic sensor registration in Home Assistant

## MAX30205 Sensor Features

- 🌡️ High accuracy: ±0.1°C (37°C to 39°C)
- 📊 16-bit temperature resolution (0.00390625°C per LSB)
- ⚡ Low power consumption
- 🔌 I²C interface (up to 400kHz)
- 📍 8 selectable I²C addresses

## Hardware

### MAX30205 Specifications

- **Supply Voltage:** 2.7V to 3.3V
- **Temperature Range:** 0°C to 50°C
- **Accuracy:** ±0.1°C (37°C to 39°C), ±0.3°C (0°C to 50°C)
- **Interface:** I²C (up to 400kHz)
- **Default Address:** 0x48

**Datasheet:** [MAX30205 Human Body Temperature Sensor (Maxim Integrated)](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX30205.pdf)

### Wiring

```
MAX30205    ESP32/ESP8266
--------    -------------
VDD    -->  3.3V
GND    -->  GND
SDA    -->  GPIO21 (or your I²C SDA pin)
SCL    -->  GPIO22 (or your I²C SCL pin)
```

**Important:** Add 4.7kΩ pull-up resistors on SDA and SCL lines if not already present on your module.

### I²C Address Configuration

The MAX30205 supports 8 different I²C addresses depending on the A0, A1, A2 pins:

| A2  | A1  | A0  | Address |
|-----|-----|-----|---------|
| GND | GND | GND | 0x48    |
| GND | GND | VDD | 0x49    |
| GND | VDD | GND | 0x4A    |
| GND | VDD | VDD | 0x4B    |
| VDD | GND | GND | 0x4C    |
| VDD | GND | VDD | 0x4D    |
| VDD | VDD | GND | 0x4E    |
| VDD | VDD | VDD | 0x4F    |

Use the I²C scan feature to detect your device's address:

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true
```

## Installation

Add this custom component to your ESPHome configuration:

```yaml
external_components:
  - source: github://CmPi/myEhThings
    components: [ max30205 ]
```

Or use a specific version:

```yaml
external_components:
  - source: github://CmPi/myEhThings@v1.0.0
    components: [ max30205 ]
```

## Configuration

### Minimal Configuration

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

sensor:
  - platform: max30205
    name: "Body Temperature"
```

### Full Configuration

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true
  frequency: 100kHz  # Optional: slower speed for long wires

sensor:
  - platform: max30205
    name: "Body Temperature"
    address: 0x48  # Optional: default is 0x48
    update_interval: 10s  # Optional: default is 10s
    accuracy_decimals: 2  # Optional: default is 2
    filters:  # Optional: add filters
      - sliding_window_moving_average:
          window_size: 5
          send_every: 5
```

## Configuration Variables

- **name** (*Required*, string): The name of the sensor.
- **address** (*Optional*, int): The I²C address of the sensor. Defaults to `0x48`.
- **update_interval** (*Optional*, Time): The interval to check the sensor. Defaults to `10s`.
- **accuracy_decimals** (*Optional*, int): Number of decimal places. Defaults to `2`.
- All other options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).

## Example Use Cases

### Medical Monitoring

```yaml
sensor:
  - platform: max30205
    name: "Patient Temperature"
    update_interval: 30s
    on_value:
      then:
        - if:
            condition:
              lambda: 'return x > 38.0;'
            then:
              - homeassistant.service:
                  service: notify.mobile_app
                  data:
                    message: "High temperature detected: {{ x }}°C"
```

### Home Assistant Integration

```yaml
sensor:
  - platform: max30205
    name: "Body Temperature"
    id: body_temp
    on_value:
      then:
        - homeassistant.event:
            event: esphome.temperature_reading
            data:
              temperature: !lambda 'return x;'
              device: "bedroom_sensor"
```

## Troubleshooting

### Device Not Found

If you see `Communication with MAX30205 failed!`:

1. **Check I²C address**: Use `scan: true` in your I²C config to detect the actual address
2. **Check wiring**: Verify SDA, SCL, VDD, and GND connections
3. **Check pull-up resistors**: Ensure 4.7kΩ resistors are present on SDA and SCL
4. **Check power supply**: MAX30205 requires 3.3V (not 5V)
5. **Try lower frequency**: Add `frequency: 100kHz` to I²C config

### Unstable Readings

- Add a moving average filter in your configuration
- Check for loose connections
- Ensure sensor has good thermal contact with measurement surface
- Allow warm-up time after power-on

## Compatibility

- ✅ ESPHome 2024.x
- ✅ ESPHome 2025.x
- ✅ ESP32 (all variants)
- ✅ ESP8266

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Credits

Developed by [@CmPi](https://github.com/CmPi)

## References

- [MAX30205 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX30205.pdf)
- [ESPHome Documentation](https://esphome.io/)
- [I²C Component Documentation](https://esphome.io/components/i2c.html)