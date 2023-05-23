# Memory Benchmark

The `MemoryBenchmark.ino` compiles example code snippets using the various
`ButtonConfig` classes. The `FEATURE` macro flag controls which feature is
compiled. The `collect.sh` edits this `FEATURE` flag programmatically, then runs
the Arduino IDE compiler on the program, and extracts the flash and static
memory usage into a text file (e.g. `nano.txt`).

The numbers shown below should be considered to be rough estimates. It is often
difficult to separate out the code size of the library from the overhead imposed
by the runtime environment of the processor. For example, it often seems like
the ESP8266 allocates flash memory in blocks of a certain quantity, so the
calculated flash size can jump around in unexpected ways.

**Version**: AceButton v1.9.2

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules for several microcontrollers:

```
$ make benchmarks
```
produces the following files:

```
attiny.txt
nano.txt
micro.txt
samd21.txt
stm32.txt
samd51.txt
esp8266.txt
esp32.txt
```

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

## Functionality

* Baseline: A program that does (almost) nothing
* Baseline+pinMode+digitalRead: A program that calls `pinMode()` and
  `digitalRead()`
* ButtonConfig: 1 `AceButton` using a `ButtonConfig`
* ButtonConfigFast1: 1 `AceButton` using `ButtonConfigFast1` (AVR only)
* ButtonConfigFast2: 1 `AceButton` using `ButtonConfigFast2` (AVR only)
* ButtonConfigFast3: 1 `AceButton` using `ButtonConfigFast3` (AVR only)
* Encoded4To2ButtonConfig: 3 `AceButton` using a `Encoded4To2ButtonConfig`
* Encoded8To3ButtonConfig: 7 `AceButton` using a `Encoded8To3ButtonConfig`
* EncodedButtonConfig: 7 `AceButton` using a `EncodedButtonConfig`
* LadderButtonConfig: 7 `AceButton` using a `LadderButtonConfig`

## Library Size Changes

**v1.5**
* Prior to v1.5, the 'Baseline' numbers also included the system `ButtonConfig`
  that is automatically created by the library.
* In v1.5, discovered that defining the `ButtonConfig::ButtonConfig()`
  constructor with a `= default` instead of explicitly defining an empty body
  `{}` allows the compiler/linker to completely remove the static instance of
  the system `ButtonConfig` if it is never referenced.
* Incremental size of the library as listed in the tables to increase by
  200-1000 bytes.
* The total flash size of any program that uses the AceButton library to
  actually decreased by 40-80 bytes because it could use the `= default`
  implementation instead of an explicit empty implementation.

**v1.6**
* Support for using `IEventHandler` increases the library flash size between 34
  to 64 bytes on 8-bit and 32-bit processors.
* The RAM size is unchanged because only a single bit-field flag is used which
  required no additional fields to be added to the `ButtonConfig`.

**v1.6.1**
* Enable virtual destructor in `ButtonConfig` to only for 32-bit processors
  (e.g. SAMD21, STM32, ESP8266, ESP32, Teensy 3.2).
* Seems to cause flash memory usage to increase by about 200-300 bytes on the
  SAMD, and about 3500 bytes on the Teensy 3.2. Even though those platforms have
  significant amounts of flash memory (256kB or more if I recall), those numbers
  are not ideal.
* The mitigating factor is that the increase in flash size is probably due
  `malloc()` and `free()` so if any other library brings those in (e.g. the
  `String` class), then the incremental cost is not significant.

**v1.8**
* Support for `kEventLongReleased` increase the flash memory consumption by 8 to
  56 bytes.

**v1.8.3**
* Upgrade ESP32 Arduino Core from v1.0.4 to v1.0.6.
    * Causes the `LadderButtConfig` to increase flash consumption from ~5kB
      to ~19kB.
    * CPU time consumption for `LadderButtonConfig` also goes up by a factor of
      2.5X (24 micros to 67 micros).
    * Looks like they changed the implementation of `analogRead()` in v1.0.5.
      See https://github.com/espressif/arduino-esp32/issues/4973 and
      https://github.com/espressif/arduino-esp32/pull/3377.

**v1.9**
* Add `ButtonConfigFast1`, `ButtonConfigFast2`, and `ButtonConfigFast3`
  to support `digitalWriteFast.h` libraries. Reduces flash consumption on AVR
  processors by 100 to 400 bytes. Added preliminary support for ATtiny85 using
  [SpenceKonde/ATTinyCore](https://github.com/SpenceKonde/ATTinyCore).

**v1.9.1**
* Upgrade STM32duino Core from 1.9.0 to 2.0.0
    * Flash memory increases by 2.6kB across the board.
    * Static memory decreases by 250 bytes across the board.
    * AceButton code size unchanged.
* Upgrade SparkFun SAMD Core from 1.8.1 to 1.8.3.
    * No changes to flash or static memory.

**v1.9.2**
* Remove SAMD21 since I can no longer flash with the latest tool chain.
* Add overhead of virtual functions on Teensy, which pulls in `malloc()`
  and `free()` even if `new` and `delete` are never used.
    * Increases memory consumption of Baseline by ~3kB, which decreases
      the apparent flash size of various AceButton features on Teensy by ~3kB.
* Add a second Baseline called `Baseline+pinMode+digitalRead`.
    * Includes the Arduino framework's overhead when `pinMode()` and
      `digitalRead()` are used.
    * Subtracting this overhead from various AceButton features gives a better
      benchmark about the flash cost of various features.
    * AVR: 300-400 bytes
    * STM32: 2200 bytes
    * ESP8266: 80 bytes
    * ESP32: 2300 bytes
    * Teensy 3.2: 450 bytes
* Upgrade various tool chains:
    * Arduino IDE from 1.8.13 to 1.8.19
    * Arduino CLI to 0.20.2
    * Arduino AVR from 1.8.3 to 1.8.4
    * STM32duino from 2.0.0 to 2.2.0
    * ESP8266 from 2.7.4 to 3.0.2
    * ESP32 from 1.0.6 to 2.0.2
    * Teensyduino from 1.53 to 1.56

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

## ATtiny85

* 8MHz ATtiny85
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SpenceKonde/ATTinyCore 1.5.2

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    266/   11 |     0/    0 |
| Baseline+pinMode+digitalRead    |    276/   11 |    10/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1406/   51 |  1140/   40 |
| ButtonConfigFast1               |   1302/   51 |  1036/   40 |
| ButtonConfigFast2               |   1266/   65 |  1000/   54 |
| ButtonConfigFast3               |   1312/   79 |  1046/   68 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   1670/   82 |  1404/   71 |
| Encoded8To3ButtonConfig         |   1926/  139 |  1660/  128 |
| EncodedButtonConfig             |   1970/  162 |  1704/  151 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   1894/  175 |  1628/  164 |
+--------------------------------------------------------------+

```

## Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Arduino AVR Boards 1.8.6

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    462/   11 |     0/    0 |
| Baseline+pinMode+digitalRead    |    766/   11 |   304/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1798/   51 |  1336/   40 |
| ButtonConfigFast1               |   1514/   51 |  1052/   40 |
| ButtonConfigFast2               |   1482/   65 |  1020/   54 |
| ButtonConfigFast3               |   1530/   79 |  1068/   68 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   2012/   82 |  1550/   71 |
| Encoded8To3ButtonConfig         |   2280/  139 |  1818/  128 |
| EncodedButtonConfig             |   2326/  162 |  1864/  151 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   2324/  175 |  1862/  164 |
+--------------------------------------------------------------+

```

## SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   3478/  151 |     0/    0 |
| Baseline+pinMode+digitalRead    |   3896/  151 |   418/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4870/  191 |  1392/   40 |
| ButtonConfigFast1               |   4468/  191 |   990/   40 |
| ButtonConfigFast2               |   4432/  205 |   954/   54 |
| ButtonConfigFast3               |   4480/  219 |  1002/   68 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   5086/  222 |  1608/   71 |
| Encoded8To3ButtonConfig         |   5354/  279 |  1876/  128 |
| EncodedButtonConfig             |   5414/  300 |  1936/  149 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   5460/  315 |  1982/  164 |
+--------------------------------------------------------------+

```

## SAMD21 (Seeed XIAO M0)

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Seeeduino SAMD 1.8.4

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  34068/    0 |     0/    0 |
| Baseline+pinMode+digitalRead    |  34132/    0 |    64/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  35132/    0 |  1064/    0 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  35372/    0 |  1304/    0 |
| Encoded8To3ButtonConfig         |  35508/    0 |  1440/    0 |
| EncodedButtonConfig             |  35596/    0 |  1528/    0 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  35828/    0 |  1760/    0 |
+--------------------------------------------------------------+

```

## STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* STM32duino 2.5.0

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  21400/ 3556 |     0/    0 |
| Baseline+pinMode+digitalRead    |  23364/ 3576 |  1964/   20 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  24288/ 3616 |  2888/   60 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  24448/ 3652 |  3048/   96 |
| Encoded8To3ButtonConfig         |  24588/ 3716 |  3188/  160 |
| EncodedButtonConfig             |  24656/ 3724 |  3256/  168 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  27628/ 3736 |  6228/  180 |
+--------------------------------------------------------------+

```

## SAMD51 (Adafruit ItsyBitsy M4)

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Adafruit SAMD 1.7.11

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  10584/    0 |     0/    0 |
| Baseline+pinMode+digitalRead    |  10640/    0 |    56/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  11632/    0 |  1048/    0 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  11888/    0 |  1304/    0 |
| Encoded8To3ButtonConfig         |  12028/    0 |  1444/    0 |
| EncodedButtonConfig             |  12092/    0 |  1508/    0 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  12432/    0 |  1848/    0 |
+--------------------------------------------------------------+

```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP8266 Boards 3.0.2

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 260105/27892 |     0/    0 |
| Baseline+pinMode+digitalRead    | 260201/27892 |    96/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 261509/27936 |  1404/   44 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         | 261693/27976 |  1588/   84 |
| Encoded8To3ButtonConfig         | 261837/28040 |  1732/  148 |
| EncodedButtonConfig             | 261965/28080 |  1860/  188 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              | 262009/28092 |  1904/  200 |
+--------------------------------------------------------------+

```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP32 Boards 2.0.9

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 228349/21976 |     0/    0 |
| Baseline+pinMode+digitalRead    | 235653/22056 |  7304/   80 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 238289/22088 |  9940/  112 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         | 238517/22136 | 10168/  160 |
| Encoded8To3ButtonConfig         | 238641/22200 | 10292/  224 |
| EncodedButtonConfig             | 238685/22208 | 10336/  232 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              | 243357/22384 | 15008/  408 |
+--------------------------------------------------------------+

```

