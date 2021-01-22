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

**Version**: AceButton v1.8.1

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
4. Switch off the old microontroller.
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

Version 1.8.1 adds benchmarks for STM32.

## Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |  12/ 15/ 24 | 1932    |
| press/release             |  12/ 16/ 24 | 1924    |
| click                     |  12/ 15/ 24 | 1925    |
| double_click              |   8/ 15/ 32 | 1921    |
| long_press/repeat_press   |  12/ 16/ 24 | 1926    |
| Encoded4To2ButtonConfig   |  56/ 67/ 92 | 1840    |
| Encoded8To3ButtonConfig   | 164/186/208 | 1660    |
| EncodedButtonConfig       |  80/101/124 | 1786    |
| LadderButtonConfig        | 176/201/276 | 1642    |
+---------------------------+-------------+---------+

```

## Sparkfun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |  12/ 15/ 24 | 1931    |
| press/release             |   8/ 16/ 24 | 1924    |
| click                     |  12/ 15/ 24 | 1925    |
| double_click              |  12/ 15/ 32 | 1921    |
| long_press/repeat_press   |  12/ 16/ 28 | 1925    |
| Encoded4To2ButtonConfig   |  60/ 73/ 84 | 1832    |
| Encoded8To3ButtonConfig   | 172/195/220 | 1648    |
| EncodedButtonConfig       |  80/102/132 | 1785    |
| LadderButtonConfig        | 172/203/268 | 1637    |
+---------------------------+-------------+---------+

```

## SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* Sparkfun SAMD Core 1.8.1

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
| idle                      |   5/  8/  9 | 2002    |
| press/release             |   4/  9/ 14 | 2002    |
| click                     |   5/  8/ 14 | 2002    |
| double_click              |   5/  9/ 16 | 2002    |
| long_press/repeat_press   |   5/  9/ 13 | 2002    |
| Encoded4To2ButtonConfig   |  18/ 30/ 31 | 2002    |
| Encoded8To3ButtonConfig   |  48/ 75/ 76 | 2002    |
| EncodedButtonConfig       |  26/ 53/ 54 | 2002    |
| LadderButtonConfig        | 447/472/478 | 2002    |
+---------------------------+-------------+---------+

```

## STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

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
| idle                      |   3/  5/  6 | 2002    |
| press/release             |   3/  5/ 12 | 2002    |
| click                     |   3/  5/  7 | 2002    |
| double_click              |   3/  5/  8 | 2002    |
| long_press/repeat_press   |   3/  5/  7 | 2002    |
| Encoded4To2ButtonConfig   |  13/ 19/ 20 | 2002    |
| Encoded8To3ButtonConfig   |  37/ 53/ 57 | 2002    |
| EncodedButtonConfig       |  20/ 34/ 35 | 2002    |
| LadderButtonConfig        |  82/ 96/ 97 | 2002    |
+---------------------------+-------------+---------+

```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

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
| idle                      |   6/  8/ 63 | 1920    |
| press/release             |   7/  8/ 53 | 1907    |
| click                     |   6/  8/ 32 | 1910    |
| double_click              |   6/  8/ 32 | 1909    |
| long_press/repeat_press   |   6/  8/107 | 1901    |
| Encoded4To2ButtonConfig   |  22/ 27/142 | 1856    |
| Encoded8To3ButtonConfig   |  54/ 67/189 | 1784    |
| EncodedButtonConfig       |  43/ 54/181 | 1815    |
| LadderButtonConfig        |  81/156/405 | 1678    |
+---------------------------+-------------+---------+

```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

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
| idle                      |   3/  3/ 16 | 2002    |
| press/release             |   3/  3/ 15 | 2002    |
| click                     |   3/  3/  7 | 2002    |
| double_click              |   3/  3/  5 | 2002    |
| long_press/repeat_press   |   3/  3/  4 | 2002    |
| Encoded4To2ButtonConfig   |   7/  8/ 10 | 2002    |
| Encoded8To3ButtonConfig   |  16/ 19/ 20 | 2002    |
| EncodedButtonConfig       |  13/ 16/ 24 | 2002    |
| LadderButtonConfig        |  21/ 24/ 76 | 2002    |
+---------------------------+-------------+---------+

```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
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
| idle                      |   2/  3/  6 | 1988    |
| press/release             |   2/  3/  6 | 1987    |
| click                     |   2/  3/  7 | 1987    |
| double_click              |   2/  3/  7 | 1987    |
| long_press/repeat_press   |   2/  4/  6 | 1986    |
| Encoded4To2ButtonConfig   |   6/ 12/ 15 | 1971    |
| Encoded8To3ButtonConfig   |  17/ 30/ 33 | 1936    |
| EncodedButtonConfig       |   8/ 22/ 25 | 1951    |
| LadderButtonConfig        |  16/ 28/ 36 | 1941    |
+---------------------------+-------------+---------+

```

