/*
 * @file max30205.cpp
 * @author CmPi <cmpi@webe.fr>
 *
 */ 

#include "max30205.h"
#include "esphome/core/log.h"

namespace esphome {
namespace max30205 {

static const char *const TAG = "max30205";

static const uint8_t MAX30205_CONFIGURATION = 0x01;
static const uint8_t MAX30205_TEMPERATURE_REGISTER = 0x00;

void MAX30205Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up MAX30205...");
  
  if (!this->write_register_(MAX30205_CONFIGURATION, 0x00)) {
    ESP_LOGE(TAG, "Failed to configure MAX30205");
    this->mark_failed();
    return;
  }
  
  ESP_LOGCONFIG(TAG, "MAX30205 setup complete");
}

void MAX30205Component::dump_config() {
  ESP_LOGCONFIG(TAG, "MAX30205:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Temperature", this);
  
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with MAX30205 failed!");
  }
}

bool MAX30205Component::read_temperature_(float &temperature) {
  uint8_t buffer[2];
  
  if (!this->read_register_(MAX30205_TEMPERATURE_REGISTER, buffer, 2)) {
    return false;
  }
  
  int16_t raw = (buffer[0] << 8) | buffer[1];
  temperature = raw * 0.00390625f;  // 0.00390625 °C/LSB
  
  return true;
}

void MAX30205Component::update() {
  float current_temperature;
  
  if (!this->read_temperature_(current_temperature)) {
    ESP_LOGW(TAG, "Failed to read temperature");
    this->status_set_warning();
    return;
  }
  
  this->status_clear_warning();
  
  if (!std::isnan(current_temperature)) {
    ESP_LOGD(TAG, "Temperature: %.2f°C", current_temperature);
    this->publish_state(current_temperature);
    this->last_temperature_ = current_temperature;
  }
}

bool MAX30205Component::write_register_(uint8_t reg, uint8_t data) {
  if (!this->write_byte(reg, data)) {
    ESP_LOGW(TAG, "Failed to write register 0x%02X", reg);
    return false;
  }
  return true;
}

bool MAX30205Component::read_register_(uint8_t reg, uint8_t *buffer, uint8_t length) {
  if (!this->read_bytes(reg, buffer, length)) {
    ESP_LOGW(TAG, "Failed to read register 0x%02X", reg);
    return false;
  }
  return true;
}

}  // namespace max30205
}  // namespace esphome