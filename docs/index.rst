.. Foo documentation master file, created by
   sphinx-quickstart on Mon Jul 21 22:05:54 2025.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

=========================
esp-idf-lib/shiftregister
=========================

.. doxygenfile:: shiftregister.h

Resources
=========

- `Shift register <https://en.wikipedia.org/wiki/Shift_register>`_ on
  Wikipedia
- `Shift Registers <https://learn.sparkfun.com/tutorials/shift-registers/all>`_
  on sparkfun.
- `SNx4HC595 8-Bit Shift Registers With 3-State Output Registers <https://www.ti.com/lit/ds/symlink/sn74hc595.pdf>`_ (data sheet)
- `Application Note Designing with Shift Registers <https://www.ti.com/lit/an/scea117/scea117.pdf>`_

GPIO
====

Bit-banging with GPIOs.

C header file:

.. code-block:: c

   #include <shiftregister_gpio.h>

.. doxygenfile:: shiftregister_gpio.h
