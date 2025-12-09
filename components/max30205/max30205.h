/*
 * @file max30205.h
 * @author CmPi <cmpi@webe.fr>
 *
 */ 

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace max30205 {

class MAX30205Component : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  float last_temperature_ = NAN;

  bool read_temperature_(float &temperature);
  bool write_register_(uint8_t reg, uint8_t data);
  bool read_register_(uint8_t reg, uint8_t *buffer, uint8_t length);
};

}  // namespace max30205
}  // namespace esphome