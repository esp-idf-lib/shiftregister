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

#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include <stdint.h> // system headers first
#include <esp_err.h> // then, esp-idf headers
#include <hal/gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @file shiftregister.h
 *
 *  @brief Shift register driver.
 *
 *  @section overiew Overview
 *
 *  A shift register is an IC to save input and/or output pins. To control 8
 *  LEDs with GPIO, 8 GPIO pins are used. With an 8-bit shift register, the
 *  minimum required number of GPIO pins is 3 (SI, SCK, and RCK, described
 *  later).
 *
 *  @section types_of_shift_registers Types of shift registers
 *
 *  There are two types of shift registers: Serial-In Parallel-Out (SIPO) and
 *  Parallel-In Serial-Out (PISO). SIPO is used to control output ports and
 *  PISO is to read inputs from input ports.
 *
 *  @subsection sipo Serial-In Parallel-Out (SIPO)
 *
 *  A Serial-In Parallel-Out shift register has output pins, often eight
 *  pins. These pins are connected to devices or other components to control,
 *  such as LEDs. Notable ICs are:
 *
 *  * 74HC164
 *  * 74HC595
 *
 *  @subsection piso Parallel-In Serial-Out (PISO)
 *
 *  A Parallel-In Serial-Out shift register  has input pins, often eight pins.
 *  These pins are connected to input devices, such as buttons. Notable ICs
 *  are:
 *
 *  * 74HC165
 *
 *  @section pins Input and output pins
 *
 *  Different manufacturers use different names for pin names.  Here, common
 *  names by major manufacturers are used. Please refer to the data sheet for
 *  the shift register.
 *
 *  The output pins are often referred as `QA`, `QB` ,`QC`, and `Q${X}`. If
 *  the shift register is an 8-bit shift register, the last one is `QH`.
 *
 *  The input pins are often referred as `A`, `B`, `C`, and so on.
 *
 *  Often, SIPO and PISO shift register has another output or input pin,
 *  `QH"`. This pin is used to daisy chain shift registers. With two shift
 *  registers, 16 input or output pins are available.
 *
 *  Pins to control the SIPO shift register are:
 *
 *  - `SI`: Indicates the bit value to store or read. Commonly referred as data
 *    pin.
 *  - `SCK`/`SRCLK`: Stores a bit value of SI into the first stage when raising.
 *    Commonly referred as clock pin.
 *  - `RCK`: All the bits are stored into storage register when raising.
 *    Commonly referred as latch pin.
 *  - `G`/`OE`: Enables the outputs when HIGH. Disables the outputs when LOW.
 *    Tied to HIGH when the outputs are always enabled.
 *  - `SCLR`: Clears data in the shift register when LOW. Tie the pin to
 *    HIGH when the pin is not used.
 *
 *  Pins to control the PISO shift register are:
 *
 *  - `QH`: Outputs serial data. Commonly referred as data pin. Often, another
 *    pin, complementary, or inverted, serial output pin is available.
 *  - `SH/LD`/`S/L`: Reads input values when falling.
 *  - `SER`: Reads serial data. Used to daisy chain multiple shift registers.
 *  - `CLK`: Clock inputs.
 *  - `CLK INH`: Starts sending the data when failing.
 *
 *  @section lsbfirst_and_msbfirst LSBFIRST and MSBFIRST
 *
 *  When `LSBFIRST` is used, the data is sent Least Significant Bit first.
 *  That is, when the data is `0b00000001`, or `0x1`, `Q7` is becomes HIGH.
 *
 *  When `MSBFIRST` is used, the data is sent Most Significant Bit first. That
 *  is, when the data is `0b00000001`, Q0 becomes HIGH.
 *
 *  The default is `LSBFIRST`.
 *
 */

/**
 * @brief Operational mode of shift register.
 *
 * Parallel Out mode, or Serial-In Parallel-Out mode, is when the
 * shift register acts as an output device, such as controlling LEDs.
 *
 * Parallel In mode, or Parallel-In Serial-Out mode, is when the shift
 * register acts as an input device, such as reading multiple button
 * inputs.
 */
typedef enum {
    SHIFTREGISTER_MODE_WRITE = GPIO_MODE_OUTPUT, /*!< Parallel Out */
    SHIFTREGISTER_MODE_READ = GPIO_MODE_INPUT,   /*!< Parallel In */
} shiftregister_mode_t;

/**
 * @brief Configuration for GPIO
 */
typedef struct {
    gpio_num_t data_io_num;    /*!< DATA pin number. */
    gpio_num_t clk_io_num;     /*!< CLK pin number. */
    gpio_num_t rclk_io_num;    /*!< Register CLK pin number */
    gpio_num_t srclr_io_num;   /*!< Shift register clear pin number. Use GPIO_NUM_NC to signal the pin is not used. */
    gpio_num_t oe_io_num;      /*!< Output Enable pin number, Use GPIO_NUM_NC to signal the pin is not used. */
    shiftregister_mode_t mode; /*!< Operational mode of the shift register. */
} shiftregister_config_t;

#ifdef __cplusplus
}
#endif

#endif // __SHIFTREGISTER_H__
