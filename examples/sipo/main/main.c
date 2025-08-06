#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_err.h>
#include <shiftregister_gpio.h>

#define TAG "sipo"

#ifndef APP_CPU_NUM
/* single core series, such as esp32c2, does not have APP_CPU_NUM */
#define APP_CPU_NUM PRO_CPU_NUM
#endif

void task(void *pvParameters)
{
    uint8_t data[] = { 0x0 };
    size_t size = sizeof(data);

    /* configure GPIOs */
    shiftregister_config_t config = {
        .data_io_num = GPIO_NUM_6,
        .clk_io_num = GPIO_NUM_7,
        .rclk_io_num = GPIO_NUM_8,
        .srclr_io_num = GPIO_NUM_NC,
        .oe_io_num = GPIO_NUM_NC,
        .mode = SHIFTREGISTER_MODE_WRITE
    };
    ESP_ERROR_CHECK(shiftregister_gpio_init(&config));

    /* all pins == HIGH */
    data[0] = 0xf;
    ESP_ERROR_CHECK(shiftregister_gpio_transfer(&config, data, size));

    /* all pins == LOW */
    data[0] = 0x0;
    ESP_ERROR_CHECK(shiftregister_gpio_transfer(&config, data, size));
    vTaskDelay(1000);

    /* last bit == HIGH */
    data[0] = 0x1;
    ESP_ERROR_CHECK(shiftregister_gpio_transfer(&config, data, size));

    while (1) {
        vTaskDelay(1000);

        /* left-shift */
        data[0] = data[0] << 1;
        if (data[0] == 0) {
            data[0] = 0x1;
        }

        /* transfer */
        ESP_ERROR_CHECK(shiftregister_gpio_transfer(&config, data, size));
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(task, "task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}
