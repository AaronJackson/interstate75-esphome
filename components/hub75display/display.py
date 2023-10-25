import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA
)

# DEPENDENCIES = []

hub75display_ns = cg.esphome_ns.namespace("hub75display");
print(hub75display_ns)
HUB75Display = hub75display_ns.class_(
    "HUB75Display", cg.PollingComponent, display.DisplayBuffer
)

CONFIG_SCHEMA = cv.All(
    display.BASIC_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(HUB75Display)
        }
    )
    .extend(cv.polling_component_schema("1s")),
    cv.has_at_most_one_key(CONF_LAMBDA),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await display.register_display(var, config)

    lambda_ = await cg.process_lambda(
        config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
    )
    cg.add(var.set_writer(lambda_))
