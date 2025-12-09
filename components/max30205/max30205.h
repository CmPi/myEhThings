#pragma once

#include "esphome.h"

#ifdef ESP32
  #include <Wire.h>
#elif defined(ESP8266)
  #include <Wire.h>
#endif

namespace esphome {
namespace max30205 {

#define MAX30205_I2C_ADDRESS 0x48
#define MAX30205_CONFIGURATION 0x01
#define MAX30205_TEMPERATURE_REGISTER 0x00

class MAX30205Component : public PollingComponent, public sensor::Sensor {
 public:
  MAX30205Component(uint32_t update_interval = 10000)
      : PollingComponent(update_interval), fLastTemperature(NAN) {}

  void setup() override {
    Wire.begin();
    begin();
  }

  void dump_config() override {
    ESP_LOGCONFIG("MAX30205", "MAX30205 sensor:");
    LOG_UPDATE_INTERVAL(this);
  }

  void update() override {
    float fCurrentTemperature = getTemperature();
    if (isnan(fLastTemperature) || fCurrentTemperature != fLastTemperature) {
      publish_state(fCurrentTemperature);
      fLastTemperature = fCurrentTemperature;
    }
  }

  float get_setup_priority() const override { return setup_priority::BUS; }

 protected:
  float fLastTemperature;

  void begin() {
    writeRegister(MAX30205_CONFIGURATION, 0x00);
  }

  void writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MAX30205_I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
  }

  float getTemperature() {
    uint8_t data[2];
    readRegisters(MAX30205_TEMPERATURE_REGISTER, data, 2);
    int16_t raw = (data[0] << 8) | data[1];
    return raw * 0.00390625f; // Conversion selon la datasheet MAX30205
  }

  void readRegisters(uint8_t reg, uint8_t *buffer, uint8_t length) {
    Wire.beginTransmission(MAX30205_I2C_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MAX30205_I2C_ADDRESS, length);
    for (uint8_t i = 0; i < length; i++) {
      buffer[i] = Wire.read();
    }
  }
};

}  // namespace max30205
}  // namespace esphome
