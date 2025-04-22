#include <hal.h>
#include <hal_sim.h>

#include <bbb_port_ids.h>
#include <pins.h>


#define HAL_RETURN_IF_ERROR(...) \
    do {                         \
        hal_error_t _status = __VA_ARGS__;  \
        if (HAL_IS_ERROR(_status)) {        \
            return _status;                  \
        }\
    } while(0)


hal_error_t configure_bbb_ports(hal_env_t* env) {
    for (int i = 0; i < get_pin_def_count(); ++i) {
        const pin_t* pin = get_pin_def_for_index(i);

        halsim_port_handle_t handle;
        HAL_RETURN_IF_ERROR(halsim_create_port(env, pin->id, &handle));
        HAL_RETURN_IF_ERROR(halsim_config_port_types(env, handle, pin->supported_types));
        HAL_RETURN_IF_ERROR(halsim_config_port_name(env, handle, pin->name));

        if (pin->supported_types & (HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT)) {
            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_DIO_POLL_EDGE, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE));
            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE));
        }
        if (pin->supported_types & (HAL_TYPE_ANALOG_INPUT | HAL_TYPE_ANALOG_OUTPUT)) {
            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_ANALOG_MAX_VOLTAGE, HAL_CONFIG_FLAG_READABLE));
            HAL_RETURN_IF_ERROR(halsim_port_set_prop(env, handle, HAL_CONFIG_ANALOG_MAX_VOLTAGE, ANALOG_MAX_VOLTAGE_MV));

            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_ANALOG_MAX_VALUE, HAL_CONFIG_FLAG_READABLE));
            HAL_RETURN_IF_ERROR(halsim_port_set_prop(env, handle, HAL_CONFIG_ANALOG_MAX_VALUE, ANALOG_MAX_VALUE));

            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_ANALOG_SAMPLE_RATE, HAL_CONFIG_FLAG_READABLE));
            HAL_RETURN_IF_ERROR(halsim_port_set_prop(env, handle, HAL_CONFIG_ANALOG_SAMPLE_RATE, ANALOG_SAMPLE_RATE_PERIOD_US));
        }
        if (pin->supported_types & HAL_TYPE_PWM_OUTPUT) {
            HAL_RETURN_IF_ERROR(halsim_config_port_prop(env, handle, HAL_CONFIG_PWM_FREQUENCY, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE));
        }
    }

    return HAL_SUCCESS;
}
