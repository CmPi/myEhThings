#include "max30205.h"

namespace esphome {
namespace max30205 {

#define MAX30205_CONFIGURATION 0x01
#define MAX30205_TEMPERATURE_REGISTER 0x00

void MAX30205Component::setup() {
  begin();
}

void MAX30205Component::begin() {
  I2CwriteByte(MAX30205_CONFIGURATION, 0x00);
}

float MAX30205Component::getTemperature() {
  uint8_t buffer[2];
  I2CreadBytes(MAX30205_TEMPERATURE_REGISTER, buffer, 2);
  int16_t raw = (buffer[0] << 8) | buffer[1];
  return raw * 0.00390625;  // 0.00390625 °C/LSB
}

void MAX30205Component::update() {
  float fCurrentTemperature = getTemperature();
  if (!isnan(fCurrentTemperature) && fCurrentTemperature != fLastTemperature) {
    publish_state(fCurrentTemperature);
    fLastTemperature = fCurrentTemperature;
  }
}

void MAX30205Component::I2CwriteByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(this->address_);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void MAX30205Component::I2CreadBytes(uint8_t reg, uint8_t *buffer, uint8_t length) {
  Wire.beginTransmission(this->address_);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(this->address_, length);
  for (uint8_t i = 0; i < length; i++) {
    if (Wire.available()) buffer[i] = Wire.read();
  }
}

}  // namespace max30205
}  // namespace esphome
