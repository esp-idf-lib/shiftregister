#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include <hal/gpio_types.h>
#include <driver/gpio.h>

#include "shiftregister_gpio.h"

#define PULSE_CLOCK_DELAY_US (1)
#define PULSE_REG_CLOCK_DELAY_US (1)
#define TAG "shiftregister_gpio"

static esp_err_t gpio_pulse(const gpio_num_t pin, unsigned int delay_us)
{
    esp_err_t ret = ESP_FAIL;

    ESP_GOTO_ON_ERROR(gpio_set_level(pin, 1), fail, TAG, "gpio_set_level %s", esp_err_to_name(ret));
    esp_rom_delay_us(delay_us);
    ESP_GOTO_ON_ERROR(gpio_set_level(pin, 0), fail, TAG, "gpio_set_level: %s", esp_err_to_name(ret));
    ret = ESP_OK;
fail:
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "%s", esp_err_to_name(ret));
    }
    return ret;
}

static esp_err_t gpio_write(const gpio_num_t pin, const uint32_t bit)
{
    esp_err_t ret = ESP_FAIL;

    ESP_GOTO_ON_ERROR(gpio_set_level(pin, bit), fail, TAG, "gpio_set_level: %s", esp_err_to_name(ret));
    ret = ESP_OK;
fail:
    return ret;
}

static esp_err_t gpio_read(const gpio_num_t pin, uint8_t *data, const uint32_t index)
{
    esp_err_t ret = ESP_FAIL;
    ESP_GOTO_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "data is NULL");

    /* MSB first */
    data[index] = ((data[index] << 1) & gpio_get_level(pin));
    ret = ESP_OK;
fail:
    return ret;
}

static esp_err_t shiftregister_gpio_init_data(const shiftregister_config_t *config)
{
    esp_err_t ret = ESP_FAIL;
    gpio_config_t io_config = {};

    ESP_GOTO_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "config is NULL");

    io_config.pin_bit_mask = (1ULL << config->data_io_num);
    io_config.mode = config->mode;
    ESP_GOTO_ON_ERROR(gpio_config(&io_config), fail, TAG, "gpio_config: %s", esp_err_to_name(ret));
    ret = ESP_OK;
fail:
    return ret;
}

static esp_err_t shiftregister_gpio_init_other(const shiftregister_config_t *config)
{
    esp_err_t ret = ESP_FAIL;
    gpio_config_t io_config = {};

    ESP_GOTO_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "config is NULL");

    io_config.pin_bit_mask = ((1ULL << config->clk_io_num) | (1ULL << config->rclk_io_num));
    io_config.mode = config->mode;
    if (config->srclr_io_num != GPIO_NUM_NC) {
        io_config.pin_bit_mask |= (1ULL << config->srclr_io_num);
    }
    if (config->oe_io_num != GPIO_NUM_NC) {
        io_config.pin_bit_mask |= (1ULL << config->oe_io_num);
    }
    ESP_GOTO_ON_ERROR(gpio_config(&io_config), fail, TAG, "gpio_config %s", esp_err_to_name(ret));

    if (config->srclr_io_num != GPIO_NUM_NC) {
        ESP_GOTO_ON_ERROR(gpio_write(config->srclr_io_num, !config->srclr_active_level), fail, TAG, "gpio_write %s", esp_err_to_name(ret));
    }
    if (config->oe_io_num != GPIO_NUM_NC) {
        ESP_GOTO_ON_ERROR(gpio_write(config->oe_io_num, config->srclr_active_level), fail, TAG, "gpio_write %s", esp_err_to_name(ret));
    }

    ret = ESP_OK;
fail:
    return ret;
}

esp_err_t shiftregister_gpio_init(const shiftregister_config_t *config)
{
    esp_err_t ret = ESP_FAIL;

    ESP_GOTO_ON_ERROR(shiftregister_gpio_init_other(config), fail, TAG, "shiftregister_gpio_init_other: %s", esp_err_to_name(ret));
    ESP_GOTO_ON_ERROR(shiftregister_gpio_init_data(config), fail, TAG, "shiftregister_gpio_init_data: %s", esp_err_to_name(ret));
    ret = ESP_OK;
fail:
    return ret;
}

esp_err_t shiftregister_gpio_transfer(const shiftregister_config_t *config, uint8_t *data, const size_t size)
{
    esp_err_t ret = ESP_FAIL;
    ESP_GOTO_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "data is NULL");

    for (int index = 0; index < size; index++) {
        uint8_t bits = data[index];
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            if (config->mode == SHIFTREGISTER_MODE_WRITE) {
                /* LSB first */
                ESP_GOTO_ON_ERROR(gpio_write(config->data_io_num, bits & 0x01), fail, TAG, "gpio_write: %s", esp_err_to_name(ret));
                bits = bits >> 1;
            } else {
                ESP_GOTO_ON_ERROR(gpio_read(config->data_io_num, data, index), fail, TAG, "gpio_write: %s", esp_err_to_name(ret));
            }
            ESP_GOTO_ON_ERROR(gpio_pulse(config->clk_io_num, PULSE_CLOCK_DELAY_US), fail, TAG, "gpio_pulse_clock: %s", esp_err_to_name(ret));
            ESP_GOTO_ON_ERROR(gpio_write(config->data_io_num, 0), fail, TAG, "gpio_write: %s", esp_err_to_name(ret));
        }
        ESP_GOTO_ON_ERROR(gpio_pulse(config->rclk_io_num, PULSE_REG_CLOCK_DELAY_US), fail, TAG, "gpio_pulse_reg_clock: %s", esp_err_to_name(ret));
    }
    ret = ESP_OK;
fail:
    return ret;
}
