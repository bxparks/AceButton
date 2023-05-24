#!/usr/bin/python3
#
# Python script that regenerates the README.md from the embedded template. Uses
# ./generate_table.awk to regenerate the ASCII tables from the various *.txt
# files.

from subprocess import check_output

nano_results = check_output(
    "./generate_table.awk < nano.txt", shell=True, text=True)
micro_results = check_output(
    "./generate_table.awk < micro.txt", shell=True, text=True)
samd21_results = check_output(
    "./generate_table.awk < samd21.txt", shell=True, text=True)
stm32_results = check_output(
    "./generate_table.awk < stm32.txt", shell=True, text=True)
samd51_results = check_output(
    "./generate_table.awk < samd51.txt", shell=True, text=True)
esp8266_results = check_output(
    "./generate_table.awk < esp8266.txt", shell=True, text=True)
esp32_results = check_output(
    "./generate_table.awk < esp32.txt", shell=True, text=True)

print(f"""\
# AutoBenchmark

This sketch measures the amount of time consumed by the `AceButton::check()`
method when processing various button events. It uses a special
`ProfilingButtonConfig` object that allows the program to inject button events
into the library. The profiling numbers come from activating the `TimingStats`
object that has been instrumented into the `AceButton::check()` method.

Note that `ProfilingButtonConfig` class generates synthetic button events,
bypassing the actual `digitalRead()` function. The `digitalRead()` function on
an Arduino AVR platform (UNO, Nano, etc) is
[known to be slow](https://forum.arduino.cc/index.php?topic=337578)
which will add to the timing values shown below in actual usage.
The [digitalWriteFast library](https://github.com/NicksonYap/digitalWriteFast)
might be an alternative if speed is critical.

**Version**: AceButton v1.10.0

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## Dependencies

This program depends on the following libraries:

* [AceCommon](https://github.com/bxparks/AceCommon)
* [AceButton](https://github.com/bxparks/AceButton)

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules to generate the `*.txt` results file for several
microcontrollers that I usually support, but the `$ make benchmarks` command
does not work very well because the USB port of the microcontroller is a
dynamically changing parameter. I created a semi-automated way of collect the
`*.txt` files:

1. Connect the microcontroller to the serial port. I usually do this through a
USB hub with individually controlled switch.
2. Type `$ auniter ports` to determine its `/dev/ttyXXX` port number (e.g.
`/dev/ttyUSB0` or `/dev/ttyACM0`).
3. If the port is `USB0` or `ACM0`, type `$ make nano.txt`, etc.
4. Switch off the old microcontroller.
5. Go to Step 1 and repeat for each microcontroller.

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

Fortunately, we no longer need to run `generate_table.awk` for each `*.txt`
file. The process has been automated using the `generate_readme.py` script which
will be invoked by the following command:
```
$ make README.md
```

The CPU times below are given in microseconds. The "samples" column is the
number of `TimingStats::update()` calls that were made.

## CPU Time Changes

**v1.8.1**
* Add benchmarks for STM32.

**v1.8.3**
* Upgrade the ESP32 Arduino Core from v1.0.4 to v1.0.6.
    * Most things remain the same, maybe slightly faster, except for
      `LadderButtonConfig`. The flash memory consumption goes up by about 14kB,
      and the CPU time for `LadderButtonConfig::checkButtons()` goes up
      significantly, from 24 micros on average to 67 micros. It looks like they
      changed the implementation of `analogRead()` in v1.0.5. See
      https://github.com/espressif/arduino-esp32/issues/4973 and
      https://github.com/espressif/arduino-esp32/pull/3377.

**v1.9**
* Add benchmarks for `ButtonConfigFast1<>`, `ButtonConfigFast2<>`, and
  `ButtonConfigFast3<>`, using one of the `<digitalWriteFast.h>` libraries for
  AVR processors.
    * The primary motivation of using `<digitalWriteFast.h>` is the 100-500 byte      reduction in flash memory consumption.
    * As a bonus, the `AceButton::check()` call becomes faster. Comparing the
      numbers for `Encoded4To2ButtonConfig` and `ButtonConfigFast3`, both
      perform 3 calls to `AceButton::check()`, but `ButtonConfigFast3` makes
      those calls almost twice as fast as `Encoded4To2ButtonConfig` (38
      microseconds versus 72 microseconds).

**v1.9.1**
* Upgrade STM32duino Core from 1.9.0 to 2.0.0.
* Upgrade SparkFun SAMD Core from 1.8.1 to 1.8.3.
* No significant change in CPU times.

**v1.9.2**
* Remove SAMD21 since I can no longer flash them using latest tool chain.
* Upgrade various tool chains:
    * Arduino IDE from 1.8.13 to 1.8.19
    * Arduino CLI to 0.20.2
    * Arduino AVR from 1.8.3 to 1.8.4
    * STM32duino from 2.0.0 to 2.2.0
    * ESP8266 from 2.7.4 to 3.0.2
    * ESP32 from 1.0.6 to 2.0.2
    * Teensyduino from 1.53 to 1.56
* No significant changes except for LadderButtonConfig:
    * ESP8266 3.0.2: significantly faster (150 micros to 91 micros).
    * ESP32 2.0.2: significantly slower (68 micros to 91 micros)
    * Probably due to implementation changes of the underlying `analogRead()`
      function.

**v1.10.0**
* Boards
    * Add SAMD21, using Seeed XIAO M0.
    * Add SAMD51, using Adafruit ItsyBitsy M4.
    * Remove Teensy 3.2.
* Tool chain
    * Arduino CLI to 0.31.0
    * Arduino AVR 1.8.6
    * Update ESP32 to 2.0.9
    * Update STM32duino 2.5.0
    * Add Seeeduino SAMD 1.8.4
    * Add Adafruit SAMD 1.7.11
* Add `kEventHeartBeat`.
    * Increases CPU time by a few microseconds or less if you squint hard
      enough.
    * Increases `sizeof(AceButton)` by 3 bytes to 17 on AVR processors, by 4
      bytes to 20 on 32-bit processors.
    * Increases `sizeof(ButtonConfig)` by 2 bytes to 18 on AVR processors`,
      no change on 32-bit processors due to padding in struct.

## Results

The following table shows the minimum, average, and maximum (min/avg/max)
time taken for a number of scenarios in units of microseconds:

* idle
    * one `AceButton::check()` call with no button events
* press/release
    * one `AceButton::check()` call with Press and Release events
* click
    * one `AceButton::check()` call with a Click event
* double_click
    * one `AceButton::check()` call with a DoubleClick event
* long_press/repeat_press
    * one `AceButton::check()` call with LongPress and RepeatPress events
* ButtonConfigFast1 (AVR-only)
    * 1 x `AceButton::check()` calls using `ButtonConfigFast1<>` with no events
* ButtonConfigFast2 (AVR-only)
    * 2 x `AceButton::check()` calls using `ButtonConfigFast2<>` with no events
* ButtonConfigFast3 (AVR-only)
    * 3 x `AceButton::check()` calls using `ButtonConfigFast3<>` with no events
* Encoded4To2ButtonConfig
    * 3 x `AceButton::check()` calls using `Encoded4To2ButtonConfig` with no
      events
* Encoded8To3ButtonConfig
    * 7 x `AceButton::check()` calls using `Encoded8To3ButtonConfig` with no
      events
* EncodedButtonConfig
    * one `EncodedButtonConfig::checkButtons()` call to support 7 virtual
      buttons, with no events
* LadderButtonConfig
    * one `LadderButtonConfig::checkButtons()` call to support 7 virtual
      buttons, with no events

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Arduino AVR Boards 1.8.4
* `micros()` has a resolution of 4 microseconds

```
{nano_results}
```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
{micro_results}
```

## SAMD21 (Seeed XIAO M0)

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Seeeduino SAMD 1.8.4

```
{samd21_results}
```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* STM32duino 2.5.0

```
{stm32_results}
```

## SAMD51 (Adafruit ItsyBitsy M4)

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Adafruit SAMD 1.7.11

```
{samd51_results}
```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP8266 Boards 3.0.2

```
{esp8266_results}
```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP32 Boards 2.0.9

```
{esp32_results}
```
""")
