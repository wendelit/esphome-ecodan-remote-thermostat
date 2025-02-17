import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from esphome.const import (
    ENTITY_CATEGORY_NONE,
    DEVICE_CLASS_RUNNING,
)

from . import ECODAN, CONF_ECODAN_ID


AUTO_LOAD = ["ecodan"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ECODAN_ID): cv.use_id(ECODAN),
    }).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    hp = await cg.get_variable(config[CONF_ECODAN_ID])

    for key, conf in config.items():
        if not isinstance(conf, dict):
            continue
        id = conf.get("id")
        if id and id.type == binary_sensor.BinarySensor:
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(hp.register_binarySensor(sens, key))
