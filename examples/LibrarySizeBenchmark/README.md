# LibrarySizeBenchmark

The `LibrarySizeBenchmark.ino` compiles example code snippets using the various
`ButtonConfig` classes. The `FEATURE` macro flag controls which feature is
compiled. The `collect.sh` edits this `FEATURE` flag programmatically, then runs
the Arduino IDE compiler on the program, and extracts the flash and static
memory usage into a text file (e.g. `nano.txt`).

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
```

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

## Arduino Nano

* 16MHz ATmega328P
* AceButton 1.5
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    962/   37 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1960/   51 |   998/   14 |
| Encoded4To2ButtonConfig         |   2152/  108 |  1190/   71 |
| Encoded8To3ButtonConfig         |   2420/  165 |  1458/  128 |
| EncodedButtonConfig             |   2492/  188 |  1530/  151 |
| LadderButtonConfig              |   2490/  201 |  1528/  164 |
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
| Baseline                        |   3958/  177 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4964/  191 |  1006/   14 |
| Encoded4To2ButtonConfig         |   5158/  248 |  1200/   71 |
| Encoded8To3ButtonConfig         |   5426/  305 |  1468/  128 |
| EncodedButtonConfig             |   5512/  326 |  1554/  149 |
| LadderButtonConfig              |   5558/  341 |  1600/  164 |
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
| Baseline                        |  11228/ 2404 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12088/ 2424 |   860/   20 |
| Encoded4To2ButtonConfig         |  12272/ 2484 |  1044/   80 |
| Encoded8To3ButtonConfig         |  12416/ 2548 |  1188/  144 |
| EncodedButtonConfig             |  12472/ 2556 |  1244/  152 |
| LadderButtonConfig              |  12724/ 2556 |  1496/  152 |
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
| Baseline                        | 257052/26824 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258320/26852 |  1268/   28 |
| Encoded4To2ButtonConfig         | 258596/26920 |  1544/   96 |
| Encoded8To3ButtonConfig         | 258740/26984 |  1688/  160 |
| EncodedButtonConfig             | 258852/27024 |  1800/  200 |
| LadderButtonConfig              | 258896/27036 |  1844/  212 |
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
| Baseline                        | 208773/15292 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 209857/15308 |  1084/   16 |
| Encoded4To2ButtonConfig         | 210149/15372 |  1376/   80 |
| Encoded8To3ButtonConfig         | 210277/15436 |  1504/  144 |
| EncodedButtonConfig             | 210361/15444 |  1588/  152 |
| LadderButtonConfig              | 211749/15452 |  2976/  160 |
+--------------------------------------------------------------+
```

## Teensy 3.2

* AceButton 1.5
* Arduino IDE 1.8.13
* Teensyduino 1.53.beta

```
+--------------------------------------------------------------+
| functionality                   |    flash/ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  11236/ 4180 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12328/ 4200 |  1092/   20 |
| Encoded4To2ButtonConfig         |  12532/ 4260 |  1296/   80 |
| Encoded8To3ButtonConfig         |  12680/ 4324 |  1444/  144 |
| EncodedButtonConfig             |  12704/ 4332 |  1468/  152 |
| LadderButtonConfig              |  13352/ 4336 |  2116/  156 |
+--------------------------------------------------------------+
```
