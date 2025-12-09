#pragma once

#include "esphome.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

// Adresse I2C par défaut
#define MAX30205_ADDRESS        0x48

// Registres
#define MAX30205_TEMPERATURE    0x00
#define MAX30205_CONFIGURATION  0x01
#define MAX30205_THYST          0x02
#define MAX30205_TOS            0x03

class MAX30205Component : public PollingComponent, public esphome::sensor::Sensor, public i2c::I2CDevice {
public:
    float fLastTemperature = 0;
    uint8_t sensorAddress = MAX30205_ADDRESS;

    // Constructeur avec intervalle de polling par défaut
    MAX30205Component() : PollingComponent(10000) {}

    void set_address(uint8_t address) { 
        sensorAddress = address; 
    }

    void setup() override {
        begin();
    }

    void dump_config() override {
        LOG_SENSOR("", "MAX30205", this);
        LOG_I2C_DEVICE(this);
        LOG_UPDATE_INTERVAL(this);
    }

    void update() override {
        float fCurrentTemperature = getTemperature();
        if (fCurrentTemperature != fLastTemperature) {
            publish_state(fCurrentTemperature);
            fLastTemperature = fCurrentTemperature;
        }
    }

    float get_setup_priority() const override { 
        return esphome::setup_priority::BUS; 
    }

protected:
    float getTemperature() {
        uint8_t readRaw[2] = {0};
        I2CreadBytes(sensorAddress, MAX30205_TEMPERATURE, readRaw, 2);
        int16_t raw = (readRaw[0] << 8) | readRaw[1];
        return raw * 0.00390625;
    }

    void begin() {
        I2CwriteByte(sensorAddress, MAX30205_CONFIGURATION, 0x00);
        I2CwriteByte(sensorAddress, MAX30205_THYST, 0x00);
        I2CwriteByte(sensorAddress, MAX30205_TOS, 0x00);
    }

    void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data){
        Wire.beginTransmission(address);
        Wire.write(subAddress);
        Wire.write(data);
        Wire.endTransmission();
    }

    uint8_t I2CreadByte(uint8_t address, uint8_t subAddress){
        Wire.beginTransmission(address);
        Wire.write(subAddress);
        Wire.endTransmission(false);
        Wire.requestFrom(address, (uint8_t) 1);
        return Wire.read();
    }

    void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t *dest, uint8_t count){
        Wire.beginTransmission(address);
        Wire.write(subAddress);
        Wire.endTransmission(false);
        Wire.requestFrom(address, count);
        for (uint8_t i = 0; i < count; i++)
            dest[i] = Wire.read();
    }
};
