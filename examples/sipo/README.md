# Example: Serial-In Parallel-Out

## Requirements

* A shift register
* Eight LEDs and registers (optional, but recommended)

## Wiring

| ESP GPIO number | Shift register pin |
|-----------------|--------------------|
| GPIO 6          | DATA, SER          |
| GPIO 7          | CLK, SRCLK         |
| GPIO 8          | RCLK               |

> [!NOTE]
> OE, Output Enable, pin should be connected to GND.

## What the example does

The example does:

* Configure GPIOs by `shiftregister_gpio_init()`.
* Set all the outputs HIGH.
* Delay 1 sec.
* Set all the outputs LOW.
* Delay 1 sec.
* Set the last bit to HIGH.
* In a loop
    * Delay 1 sec.
    * Left-shift the bits.
    * if the bits == 0, set 0x01.
    * Send the data to the shift register.
