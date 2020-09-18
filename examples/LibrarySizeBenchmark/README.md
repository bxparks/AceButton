# LibrarySizeBenchmark

The `LibrarySizeBenchmark.ino` compiles example code snippets using the various
`ButtonConfig` classes. The `FEATURE` macro flag controls which feature is
compiled. The `collect.sh` edits this `FEATURE` flag programmatically, then runs
the Arduino IDE compiler on the program, and extracts the flash and static
memory usage into a text file (e.g. `nano.txt`).

The numbers shown below should be considered to be rough estimates. It is often
difficult to separate out the code size of the library from the overhead imposed
by the runtime environment of the processor. For example, it often seems like
the ESP8266 allocates flash memory in blocks of a certain quantity, so the
calculated flash size can jump around in unexpected ways.

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
teensy.txt
```

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
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
instead of explicitly defining an empty body `{}` allows the compiler/linker to
completely remove the static instance of the system `ButtonConfig` if it is
never referenced. This caused the incremental size of the library as listed in
the tables to increase by 200-1000 bytes, but the total flash size of any
program that uses the AceButton library to actually decreased by 40-80 bytes
because it could use the `= default` implementation instead of an explicit empty
implementation.

Support for using an `IEventHandler` object as the event handler in v1.6
increased the library flash size between 34 to 64 bytes on 8-bit and 32-bit
processors. The RAM size is unchanged because only a single bit-field flag is
used which required no additional fields to be added to the `ButtonConfig`.

## Arduino Nano

* 16MHz ATmega328P
* AceButton 1.6
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    610/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1932/   51 |  1322/   40 |
| Encoded4To2ButtonConfig         |   2146/   82 |  1536/   71 |
| Encoded8To3ButtonConfig         |   2414/  139 |  1804/  128 |
| EncodedButtonConfig             |   2490/  188 |  1880/  177 |
| LadderButtonConfig              |   2492/  201 |  1882/  190 |
+--------------------------------------------------------------+
```

## Sparkfun Pro Micro

* 16 MHz ATmega32U4
* AceButton 1.6
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   3558/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4936/  191 |  1378/   40 |
| Encoded4To2ButtonConfig         |   5152/  222 |  1594/   71 |
| Encoded8To3ButtonConfig         |   5420/  279 |  1862/  128 |
| EncodedButtonConfig             |   5510/  326 |  1952/  175 |
| LadderButtonConfig              |   5560/  341 |  2002/  190 |
+--------------------------------------------------------------+
```

## SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* AceButton 1.6
* Arduino IDE 1.8.13
* Arduino SAMD Core 1.8.6

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  11108/ 2368 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12108/ 2416 |  1000/   48 |
| Encoded4To2ButtonConfig         |  12252/ 2460 |  1144/   92 |
| Encoded8To3ButtonConfig         |  12392/ 2524 |  1284/  156 |
| EncodedButtonConfig             |  12496/ 2548 |  1388/  180 |
| LadderButtonConfig              |  12768/ 2564 |  1660/  196 |
+--------------------------------------------------------------+
```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* AceButton 1.6
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.1

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 257140/26820 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258424/26840 |  1284/   20 |
| Encoded4To2ButtonConfig         | 258716/26916 |  1576/   96 |
| Encoded8To3ButtonConfig         | 258860/26980 |  1720/  160 |
| EncodedButtonConfig             | 258972/27020 |  1832/  200 |
| LadderButtonConfig              | 259016/27024 |  1876/  204 |
+--------------------------------------------------------------+
```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* AceButton 1.6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 207657/14628 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 209869/15308 |  2212/  680 |
| Encoded4To2ButtonConfig         | 210073/15348 |  2416/  720 |
| Encoded8To3ButtonConfig         | 210201/15412 |  2544/  784 |
| EncodedButtonConfig             | 210373/15444 |  2716/  816 |
| LadderButtonConfig              | 211761/15452 |  4104/  824 |
+--------------------------------------------------------------+
```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* AceButton 1.6
* Arduino IDE 1.8.13
* Teensyduino 1.53.beta
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  10844/ 4160 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12496/ 4200 |  1652/   40 |
| Encoded4To2ButtonConfig         |  12660/ 4236 |  1816/   76 |
| Encoded8To3ButtonConfig         |  12808/ 4300 |  1964/  140 |
| EncodedButtonConfig             |  12872/ 4332 |  2028/  172 |
| LadderButtonConfig              |  13520/ 4336 |  2676/  176 |
+--------------------------------------------------------------+
```
