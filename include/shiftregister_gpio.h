/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Tomoyuki Sakurai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SHIFTREGISTER_GPIO_H__
#define __SHIFTREGISTER_GPIO_H__

#include <stdint.h> // system headers first
#include <esp_err.h> // then, esp-idf headers
#include <hal/gpio_types.h>
#include "shiftregister.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize device descriptor.
 *
 * @param[in] config Configuration for GPIO.
 * @return
 *  - ESP_OK: Successful
 *  - Other: Failure
 */
esp_err_t shiftregister_gpio_init(const shiftregister_config_t *config);

/**
 * @brief Write data to the register
 *
 * @param[in] config Configuration for GPIO.
 * @param[in] data The data to write.
 * @param[out] data The data to read.
 * @param[in] size Size of data. The size should be less than or equal to the
 *            number of shift register.
 * @return
 *  - ESP_OK: Successful
 *  - Other error: Failure
 */
esp_err_t shiftregister_gpio_transfer(const shiftregister_config_t *config, uint8_t *data, const size_t size);

#ifdef __cplusplus
}
#endif

#endif // __SHIFTREGISTER_GPIO_H__
