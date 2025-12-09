import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import (
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@CmPi"]

max30205_ns = cg.esphome_ns.namespace("max30205")
MAX30205Sensor = max30205_ns.class_("MAX30205Sensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        UNIT_CELSIUS,
        ICON_THERMOMETER,
        2,
        DEVICE_CLASS_TEMPERATURE,
        STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Optional("address", default=0x48): cv.i2c_address,
        }
    )
    .extend(cv.polling_component_schema("10s"))
    .extend(i2c.i2c_device_schema(0x48))
)


async def to_code(config):
    var = cg.new_Pvariable(config["id"])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
