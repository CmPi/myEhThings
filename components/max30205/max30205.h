#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace max30205 {

class MAX30205Component : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
 public:
  explicit MAX30205Component(i2c::I2CComponent *parent, uint32_t update_interval = 10000)
      : PollingComponent(update_interval), I2CDevice(parent) {}

  void setup() override;
  void update() override;
  float get_setup_priority() const override { return esphome::setup_priority::BUS; }

 protected:
  float fLastTemperature = NAN;

  void begin();
  float getTemperature();
  void I2CwriteByte(uint8_t reg, uint8_t data);
  void I2CreadBytes(uint8_t reg, uint8_t *buffer, uint8_t length);
};

}  // namespace max30205
}  // namespace esphome
