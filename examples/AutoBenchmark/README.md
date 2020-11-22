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
which will add to the timing values in actual usage.
The [digitalWriteFast library](https://github.com/NicksonYap/digitalWriteFast)
might be an alternative if speed is critical.

**Version**: AceButton v1.8

## Benchmark Results

All times are in microseconds. The "samples" column is the number of
`TimingStats::update()` calls that were made.

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3
* `micros()` has a resolution of 4 microseconds

**Memory**
```
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   8/ 15/ 24 | 1933    |
press/release           |   8/ 16/ 24 | 1925    |
click                   |  12/ 15/ 24 | 1926    |
double click            |   8/ 15/ 28 | 1921    |
long press/repeat press |  12/ 16/ 24 | 1925    |
Encode4To2ButtonConfig  |  56/ 67/ 92 | 1840    |
Encode8To3ButtonConfig  | 164/186/208 | 1660    |
EncodeButtonConfig      |  80/100/124 | 1786    |
LadderButtonConfig      | 180/201/280 | 1642    |
------------------------+-------------+---------+
```

### SAMD21

* SAMD21 M0 Mini, 48MHz SAMD21
* Arduino IDE 1.8.13
* SparkFun SAMD Boards 1.8.1

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   4/  8/ 13 | 2002    |
press/release           |   4/  9/ 14 | 2002    |
click                   |   4/  8/ 14 | 2002    |
double click            |   4/  8/ 16 | 2002    |
long press/repeat press |   4/  9/ 14 | 2002    |
Encode4To2ButtonConfig  |  18/ 30/ 34 | 2002    |
Encode8To3ButtonConfig  |  49/ 76/ 80 | 2002    |
EncodeButtonConfig      |  27/ 54/ 58 | 2002    |
LadderButtonConfig      | 447/474/478 | 2002    |
------------------------+-------------+---------+
```

**Note:** The 400-800 microseconds per iteration (of 7 buttons) for
`LadderButtonConfig` on the SAMD21 may be an anomaly of my cheap clone, or it
may be an pervasive problem with the `analogRead()` function of a SAMD21
microcontroller. I advise validating this result on your specific SAMD21 board.

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36
```

**CPU**

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   6/  8/ 67 | 1922    |
press/release           |   6/  8/ 61 | 1910    |
click                   |   6/  8/ 36 | 1910    |
double click            |   6/  8/111 | 1895    |
long press/repeat press |   6/  8/111 | 1874    |
Encode4To2ButtonConfig  |  22/ 27/146 | 1853    |
Encode8To3ButtonConfig  |  54/ 67/193 | 1785    |
EncodeButtonConfig      |  42/ 54/181 | 1814    |
LadderButtonConfig      |  81/148/420 | 1683    |
------------------------+-------------+---------+
```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   3/  3/ 16 | 2002    |
press/release           |   3/  3/ 13 | 2002    |
click                   |   3/  3/  8 | 2002    |
double click            |   3/  3/  5 | 2002    |
long press/repeat press |   3/  3/  4 | 2002    |
Encode4To2ButtonConfig  |   7/  8/ 12 | 2002    |
Encode8To3ButtonConfig  |  15/ 19/ 20 | 2002    |
EncodeButtonConfig      |  13/ 16/ 25 | 2002    |
LadderButtonConfig      |  21/ 25/ 77 | 2002    |
------------------------+-------------+---------+
```

### Teensy 3.2

**Stale Data**: I am no longer able to program Teensy from my computer. Maybe it
was the upgrade from Ubuntu 18.04 to 20.04. Maybe it was upgrading Arduino IDE
1.8.9 to 1.8.13. Maybe it was upgrading Teensyduino. I don't know. I think the
following data is from AceButton v1.5.

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53.beta
* Compiler options: "Faster"

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   2/  3/  5 | 1988    |
press/release           |   2/  3/  6 | 1988    |
click                   |   2/  3/  6 | 1988    |
double click            |   2/  3/  6 | 1988    |
long press/repeat press |   2/  3/  6 | 1987    |
Encode4To2ButtonConfig  |   6/ 11/ 14 | 1972    |
Encode8To3ButtonConfig  |  16/ 27/ 30 | 1942    |
EncodeButtonConfig      |   8/ 20/ 22 | 1957    |
LadderButtonConfig      |  16/ 25/ 33 | 1945    |
------------------------+-------------+---------+
```
