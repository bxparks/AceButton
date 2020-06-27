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

## Arduino Nano

* 16MHz ATmega328P
* AceButton 1.5
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    610/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1898/   51 |  1288/   40 |
| Encoded4To2ButtonConfig         |   2100/   82 |  1490/   71 |
| Encoded8To3ButtonConfig         |   2368/  139 |  1758/  128 |
| EncodedButtonConfig             |   2426/  188 |  1816/  177 |
| LadderButtonConfig              |   2428/  201 |  1818/  190 |
+--------------------------------------------------------------+
```

## Sparkfun Pro Micro

* AceButton 1.5
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   3558/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4902/  191 |  1344/   40 |
| Encoded4To2ButtonConfig         |   5106/  222 |  1548/   71 |
| Encoded8To3ButtonConfig         |   5374/  279 |  1816/  128 |
| EncodedButtonConfig             |   5446/  326 |  1888/  175 |
| LadderButtonConfig              |   5496/  341 |  1938/  190 |
+--------------------------------------------------------------+
```

## SAMD21 M0 Mini

* AceButton 1.5
* Arduino IDE 1.8.13
* Arduino SAMD Core 1.8.6

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  11084/ 2368 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12056/ 2416 |   972/   48 |
| Encoded4To2ButtonConfig         |  12200/ 2460 |  1116/   92 |
| Encoded8To3ButtonConfig         |  12340/ 2524 |  1256/  156 |
| EncodedButtonConfig             |  12440/ 2548 |  1356/  180 |
| LadderButtonConfig              |  12708/ 2564 |  1624/  196 |
+--------------------------------------------------------------+
```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* AceButton 1.5
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.1

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 257040/26820 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258292/26840 |  1252/   20 |
| Encoded4To2ButtonConfig         | 258568/26916 |  1528/   96 |
| Encoded8To3ButtonConfig         | 258712/26980 |  1672/  160 |
| EncodedButtonConfig             | 258824/27020 |  1784/  200 |
| LadderButtonConfig              | 258868/27024 |  1828/  204 |
+--------------------------------------------------------------+
```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* AceButton 1.5
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 207657/14628 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 209833/15308 |  2176/  680 |
| Encoded4To2ButtonConfig         | 210037/15348 |  2380/  720 |
| Encoded8To3ButtonConfig         | 210165/15412 |  2508/  784 |
| EncodedButtonConfig             | 210337/15444 |  2680/  816 |
| LadderButtonConfig              | 211725/15452 |  4068/  824 |
+--------------------------------------------------------------+
```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* AceButton 1.5
* Arduino IDE 1.8.13
* Teensyduino 1.53.beta
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  10844/ 4160 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12296/ 4200 |  1452/   40 |
| Encoded4To2ButtonConfig         |  12460/ 4236 |  1616/   76 |
| Encoded8To3ButtonConfig         |  12608/ 4300 |  1764/  140 |
| EncodedButtonConfig             |  12672/ 4332 |  1828/  172 |
| LadderButtonConfig              |  13320/ 4336 |  2476/  176 |
+--------------------------------------------------------------+
```
