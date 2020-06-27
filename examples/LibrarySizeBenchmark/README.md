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
| Baseline                        |    972/   39 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1970/   53 |   998/   14 |
| Encoded4To2ButtonConfig         |   2164/  112 |  1192/   73 |
| Encoded8To3ButtonConfig         |   2432/  169 |  1460/  130 |
| EncodedButtonConfig             |   2504/  192 |  1532/  153 |
| LadderButtonConfig              |   2502/  205 |  1530/  166 |
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
| Baseline                        |   3968/  179 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4974/  193 |  1006/   14 |
| Encoded4To2ButtonConfig         |   5170/  252 |  1202/   73 |
| Encoded8To3ButtonConfig         |   5438/  309 |  1470/  130 |
| EncodedButtonConfig             |   5524/  330 |  1556/  151 |
| LadderButtonConfig              |   5570/  345 |  1602/  166 |
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
| Baseline                        |  11240/ 2404 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12100/ 2424 |   860/   20 |
| Encoded4To2ButtonConfig         |  12288/ 2484 |  1048/   80 |
| Encoded8To3ButtonConfig         |  12432/ 2548 |  1192/  144 |
| EncodedButtonConfig             |  12488/ 2556 |  1248/  152 |
| LadderButtonConfig              |  12740/ 2556 |  1500/  152 |
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
| Baseline                        | 257000/26820 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258268/26840 |  1268/   20 |
| Encoded4To2ButtonConfig         | 258508/26904 |  1508/   84 |
| Encoded8To3ButtonConfig         | 258636/26968 |  1636/  148 |
| EncodedButtonConfig             | 258764/27008 |  1764/  188 |
| LadderButtonConfig              | 258808/27020 |  1808/  200 |
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
| Baseline                        | 208761/15292 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 209845/15308 |  1084/   16 |
| Encoded4To2ButtonConfig         | 210113/15372 |  1352/   80 |
| Encoded8To3ButtonConfig         | 210241/15436 |  1480/  144 |
| EncodedButtonConfig             | 210277/15444 |  1516/  152 |
| LadderButtonConfig              | 211665/15452 |  2904/  160 |
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
| Baseline                        |  11248/ 4180 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12340/ 4200 |  1092/   20 |
| Encoded4To2ButtonConfig         |  12548/ 4260 |  1300/   80 |
| Encoded8To3ButtonConfig         |  12696/ 4324 |  1448/  144 |
| EncodedButtonConfig             |  12720/ 4332 |  1472/  152 |
| LadderButtonConfig              |  13368/ 4336 |  2120/  156 |
+--------------------------------------------------------------+
```
