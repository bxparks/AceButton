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

**Version**: AceButton v1.9.2

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
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* Arduino AVR Boards 1.8.4
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(ButtonConfigFast1<>): 18
sizeof(ButtonConfigFast2<>): 18
sizeof(ButtonConfigFast3<>): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |  12/ 15/ 24 |    1931 |
| press/release             |  12/ 15/ 24 |    1927 |
| click                     |  12/ 15/ 24 |    1928 |
| double_click              |  12/ 15/ 32 |    1924 |
| long_press/repeat_press   |  12/ 16/ 24 |    1926 |
|---------------------------+-------------+---------|
| ButtonConfigFast1         |  12/ 14/ 24 |    1934 |
| ButtonConfigFast2         |  20/ 27/ 36 |    1910 |
| ButtonConfigFast3         |  28/ 40/ 48 |    1886 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  60/ 69/ 76 |    1836 |
| Encoded8To3ButtonConfig   | 164/187/196 |    1656 |
| EncodedButtonConfig       |  80/102/112 |    1782 |
| LadderButtonConfig        | 176/201/272 |    1637 |
+---------------------------+-------------+---------+

```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(ButtonConfigFast1<>): 18
sizeof(ButtonConfigFast2<>): 18
sizeof(ButtonConfigFast3<>): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |  12/ 15/ 24 |    1931 |
| press/release             |  12/ 15/ 32 |    1928 |
| click                     |   8/ 15/ 24 |    1929 |
| double_click              |   8/ 15/ 32 |    1924 |
| long_press/repeat_press   |  12/ 16/ 28 |    1925 |
|---------------------------+-------------+---------|
| ButtonConfigFast1         |   8/ 15/ 24 |    1932 |
| ButtonConfigFast2         |  20/ 27/ 36 |    1909 |
| ButtonConfigFast3         |  28/ 41/ 52 |    1886 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  60/ 72/ 84 |    1830 |
| Encoded8To3ButtonConfig   | 172/197/208 |    1643 |
| EncodedButtonConfig       |  80/104/116 |    1780 |
| LadderButtonConfig        | 176/200/268 |    1639 |
+---------------------------+-------------+---------+

```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* STM32duino 2.2.0

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   3/  5/  6 |    2002 |
| press/release             |   3/  5/  8 |    2002 |
| click                     |   3/  5/  8 |    2002 |
| double_click              |   3/  5/ 13 |    2002 |
| long_press/repeat_press   |   3/  5/  7 |    2002 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  15/ 20/ 21 |    2002 |
| Encoded8To3ButtonConfig   |  43/ 56/ 57 |    2002 |
| EncodedButtonConfig       |  21/ 34/ 35 |    2002 |
| LadderButtonConfig        |  84/ 99/103 |    2002 |
+---------------------------+-------------+---------+

```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* ESP8266 Boards 3.0.2

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   6/  7/ 53 |    1921 |
| press/release             |   6/  7/ 37 |    1922 |
| click                     |   6/  7/ 18 |    1921 |
| double_click              |   6/  7/ 10 |    1921 |
| long_press/repeat_press   |   6/  7/ 10 |    1921 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  21/ 26/ 45 |    1887 |
| Encoded8To3ButtonConfig   |  53/ 65/ 69 |    1809 |
| EncodedButtonConfig       |  40/ 52/ 60 |    1842 |
| LadderButtonConfig        |  79/ 91/193 |    1762 |
+---------------------------+-------------+---------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* ESP32 Boards 2.0.2

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   2/  3/ 16 |    2003 |
| press/release             |   2/  2/ 13 |    2002 |
| click                     |   2/  2/ 10 |    2003 |
| double_click              |   2/  2/ 11 |    2003 |
| long_press/repeat_press   |   2/  2/ 11 |    2003 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |   6/  6/ 15 |    2002 |
| Encoded8To3ButtonConfig   |  13/ 16/ 17 |    2001 |
| EncodedButtonConfig       |  11/ 14/ 22 |    2001 |
| LadderButtonConfig        |  87/ 91/231 |    2001 |
+---------------------------+-------------+---------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.20.2
* Teensyduino 1.56
* Compiler options: "Faster"

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   2/  3/  6 |    1988 |
| press/release             |   2/  3/  6 |    1988 |
| click                     |   2/  3/  6 |    1987 |
| double_click              |   2/  3/  8 |    1987 |
| long_press/repeat_press   |   2/  4/  6 |    1987 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |   6/ 12/ 15 |    1970 |
| Encoded8To3ButtonConfig   |  16/ 30/ 33 |    1937 |
| EncodedButtonConfig       |   8/ 22/ 25 |    1950 |
| LadderButtonConfig        |  16/ 26/ 36 |    1943 |
+---------------------------+-------------+---------+

```

