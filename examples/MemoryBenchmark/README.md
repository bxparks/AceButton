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

**Version**: AceButton v1.8.1

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
nano.txt
micro.txt
samd.txt
esp8266.txt
esp32.txt
teensy32.txt
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
* ButtonConfig: 1 `AceButton` using the default system `ButtonConfig`
* Encoded4To2ButtonConfig: 3 `AceButton` using 1 `Encoded4To2ButtonConfig`
* Encoded8To3ButtonConfig: 7 `AceButton` using 1 `Encoded8To3ButtonConfig`
* EncodedButtonConfig: 7 `AceButton` using 1 `EncodedButtonConfig`
* LadderButtonConfig: 7 `AceButton` using 1 `LadderButtonConfig`

## Library Size Changes

Prior to v1.5, the 'Baseline' numbers also included the system `ButtonConfig`
that is automatically created by the library. But in v1.5, I discovered that
defining the `ButtonConfig::ButtonConfig()` constructor with a `= default`
instead of explicitly defining an empty body `{}` allows the compiler/linker
to completely remove the static instance of the system `ButtonConfig` if it is
never referenced. This caused the incremental size of the library as listed in
the tables to increase by 200-1000 bytes, but the total flash size of any
program that uses the AceButton library to actually decreased by 40-80 bytes
because it could use the `= default` implementation instead of an explicit empty
implementation.

In v1.6, adding support for using `IEventHandler` as the event handler increased
the library flash size between 34 to 64 bytes on 8-bit and 32-bit processors.
The RAM size is unchanged because only a single bit-field flag is used which
required no additional fields to be added to the `ButtonConfig`.

In v1.6.1, making the destructor in `ButtonConfig` to be `virtual` was extended
to SAMD and Teensy architectures (joining ESP8266 and ESP32). This seems to
cause flash memory usage to increase by about 200-300 bytes on the SAMD, and
about 3500 bytes on the Teensy 3.2. Even though those platforms have significant
amounts of flash memory (256kB if I recall), those numbers are not ideal. The
mitigating factor is that the increase in flash size is probably due `malloc()`
and `free()` so if any other library brings those in (e.g. the `String` class),
then the incremental cost is not significant.

In v1.8, adding support for `kEventLongReleased` increased the flash memory
consumption by 8 to 56 bytes.

## Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    610/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1946/   51 |  1336/   40 |
| Encoded4To2ButtonConfig         |   2160/   82 |  1550/   71 |
| Encoded8To3ButtonConfig         |   2428/  139 |  1818/  128 |
| EncodedButtonConfig             |   2504/  188 |  1894/  177 |
| LadderButtonConfig              |   2506/  201 |  1896/  190 |
+--------------------------------------------------------------+

```

## Sparkfun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   3558/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4950/  191 |  1392/   40 |
| Encoded4To2ButtonConfig         |   5166/  222 |  1608/   71 |
| Encoded8To3ButtonConfig         |   5434/  279 |  1876/  128 |
| EncodedButtonConfig             |   5524/  326 |  1966/  175 |
| LadderButtonConfig              |   5574/  341 |  2016/  190 |
+--------------------------------------------------------------+

```

## SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* SparkFun SAMD Core 1.8.1

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  10072/    0 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  11360/    0 |  1288/    0 |
| Encoded4To2ButtonConfig         |  11600/    0 |  1528/    0 |
| Encoded8To3ButtonConfig         |  11744/    0 |  1672/    0 |
| EncodedButtonConfig             |  11816/    0 |  1744/    0 |
| LadderButtonConfig              |  12072/    0 |  2000/    0 |
+--------------------------------------------------------------+

```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 256924/26800 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258440/26840 |  1516/   40 |
| Encoded4To2ButtonConfig         | 258732/26916 |  1808/  116 |
| Encoded8To3ButtonConfig         | 258860/26980 |  1936/  180 |
| EncodedButtonConfig             | 258988/27020 |  2064/  220 |
| LadderButtonConfig              | 259032/27024 |  2108/  224 |
+--------------------------------------------------------------+

```

## STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  19144/ 3788 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  21872/ 3848 |  2728/   60 |
| Encoded4To2ButtonConfig         |  22024/ 3884 |  2880/   96 |
| Encoded8To3ButtonConfig         |  22164/ 3948 |  3020/  160 |
| EncodedButtonConfig             |  22280/ 3980 |  3136/  192 |
| LadderButtonConfig              |  25076/ 3992 |  5932/  204 |
+--------------------------------------------------------------+

```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 206625/14564 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 209881/15308 |  3256/  744 |
| Encoded4To2ButtonConfig         | 210085/15348 |  3460/  784 |
| Encoded8To3ButtonConfig         | 210213/15412 |  3588/  848 |
| EncodedButtonConfig             | 210385/15444 |  3760/  880 |
| LadderButtonConfig              | 211773/15452 |  5148/  888 |
+--------------------------------------------------------------+

```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   7664/ 3048 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12652/ 4200 |  4988/ 1152 |
| Encoded4To2ButtonConfig         |  12904/ 4260 |  5240/ 1212 |
| Encoded8To3ButtonConfig         |  13052/ 4324 |  5388/ 1276 |
| EncodedButtonConfig             |  13076/ 4332 |  5412/ 1284 |
| LadderButtonConfig              |  13724/ 4336 |  6060/ 1288 |
+--------------------------------------------------------------+

```

