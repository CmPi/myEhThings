import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
)

max30205_ns = cg.esphome_ns.namespace("max30205")
MAX30205Component = max30205_ns.class_("MAX30205Component", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema(
    MAX30205Component,
    unit_of_measurement=UNIT_CELSIUS,
    icon=ICON_THERMOMETER,
    accuracy_decimals=2,
).extend(
    {
        cv.GenerateID(): cv.declare_id(MAX30205Component),
        cv.Optional("address", default=0x48): cv.i2c_address,
    }
).extend(cv.polling_component_schema("10s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    cg.add(var.set_address(config["address"]))
