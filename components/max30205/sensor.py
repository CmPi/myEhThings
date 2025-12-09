#
# @file sensor.py

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import (
    CONF_ID,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
)

CONF_ADDRESS = "address"

# Déclare le namespace et la classe C++
max30205_ns = cg.esphome_ns.namespace("max30205")
MAX30205Component = max30205_ns.class_("MAX30205Component", cg.PollingComponent, sensor.Sensor, i2c.I2CDevice)

# Définition du schéma YAML
CONFIG_SCHEMA = sensor.sensor_schema(
    MAX30205Component,
    unit_of_measurement=UNIT_CELSIUS,
    icon=ICON_THERMOMETER,
    accuracy_decimals=2,
).extend(
    {
        cv.GenerateID(): cv.declare_id(MAX30205Component),
    }
).extend(cv.polling_component_schema("10s")).extend(i2c.i2c_device_schema(0x48))

# Fonction de génération du code
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)