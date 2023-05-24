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

**Version**: AceButton v1.10.0

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
* Add `kEventHeartBeat`
    * Increases flash size of `ButtonConfig` by ~150 bytes on AVR, ~50 bytes on
      32-bit processors.

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
| ButtonConfig                    |   1576/   56 |  1310/   45 |
| ButtonConfigFast1               |   1472/   56 |  1206/   45 |
| ButtonConfigFast2               |   1368/   73 |  1102/   62 |
| ButtonConfigFast3               |   1408/   90 |  1142/   79 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   1754/   93 |  1488/   82 |
| Encoded8To3ButtonConfig         |   1962/  162 |  1696/  151 |
| EncodedButtonConfig             |   2010/  185 |  1744/  174 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   1934/  198 |  1668/  187 |
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
| ButtonConfig                    |   1970/   56 |  1508/   45 |
| ButtonConfigFast1               |   1686/   56 |  1224/   45 |
| ButtonConfigFast2               |   1586/   73 |  1124/   62 |
| ButtonConfigFast3               |   1628/   90 |  1166/   79 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   2098/   93 |  1636/   82 |
| Encoded8To3ButtonConfig         |   2318/  162 |  1856/  151 |
| EncodedButtonConfig             |   2362/  185 |  1900/  174 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   2360/  198 |  1898/  187 |
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
| ButtonConfig                    |   5042/  196 |  1564/   45 |
| ButtonConfigFast1               |   4640/  196 |  1162/   45 |
| ButtonConfigFast2               |   4536/  213 |  1058/   62 |
| ButtonConfigFast3               |   4578/  230 |  1100/   79 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |   5172/  233 |  1694/   82 |
| Encoded8To3ButtonConfig         |   5392/  302 |  1914/  151 |
| EncodedButtonConfig             |   5450/  323 |  1972/  172 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |   5496/  338 |  2018/  187 |
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
| ButtonConfig                    |  35180/    0 |  1112/    0 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  35420/    0 |  1352/    0 |
| Encoded8To3ButtonConfig         |  35548/    0 |  1480/    0 |
| EncodedButtonConfig             |  35636/    0 |  1568/    0 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  35876/    0 |  1808/    0 |
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
| Baseline                        |  21500/ 3840 |     0/    0 |
| Baseline+pinMode+digitalRead    |  23440/ 3860 |  1940/   20 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  24412/ 3904 |  2912/   64 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  24564/ 3948 |  3064/  108 |
| Encoded8To3ButtonConfig         |  24700/ 4028 |  3200/  188 |
| EncodedButtonConfig             |  24772/ 4036 |  3272/  196 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  27708/ 4048 |  6208/  208 |
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
| ButtonConfig                    |  11684/    0 |  1100/    0 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         |  11924/    0 |  1340/    0 |
| Encoded8To3ButtonConfig         |  12064/    0 |  1480/    0 |
| EncodedButtonConfig             |  12132/    0 |  1548/    0 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              |  12472/    0 |  1888/    0 |
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
| ButtonConfig                    | 261589/27944 |  1484/   52 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         | 261741/27984 |  1636/   92 |
| Encoded8To3ButtonConfig         | 261885/28064 |  1780/  172 |
| EncodedButtonConfig             | 262013/28104 |  1908/  212 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              | 262057/28116 |  1952/  224 |
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
| ButtonConfig                    | 238385/22096 | 10036/  120 |
|---------------------------------+--------------+-------------|
| Encoded4To2ButtonConfig         | 238601/22144 | 10252/  168 |
| Encoded8To3ButtonConfig         | 238721/22224 | 10372/  248 |
| EncodedButtonConfig             | 238765/22232 | 10416/  256 |
|---------------------------------+--------------+-------------|
| LadderButtonConfig              | 243437/22408 | 15088/  432 |
+--------------------------------------------------------------+

```

