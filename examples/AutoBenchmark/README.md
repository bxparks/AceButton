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

## Benchmark Results

In all of the tests, the **min** time for the "idle" case is larger than any of
the other button events. This is because when a button event occurs, the
`AceButton::checkDebounced()` method returns immediately until the debouncing
time is over which brings down the minimum time. No debouncing is done in the
"idle" case so the minimum code path takes more CPU cycles.

All times are in microseconds. The "samples" column is the number of
`TimingStats::update()` calls that were made.

### Arduino Nano

* 16MHz ATmega328P
* AceButton 1.5
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

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
press/release           |   8/ 16/ 24 | 1924    |
click                   |   8/ 15/ 24 | 1925    |
double click            |  12/ 15/ 28 | 1923    |
long press/repeat press |  12/ 16/ 24 | 1926    |
Encode4To2ButtonConfig  |  60/ 64/ 76 | 1847    |
Encode8To3ButtonConfig  | 164/177/188 | 1672    |
EncodeButtonConfig      |  80/ 92/108 | 1800    |
LadderButtonConfig      | 176/190/276 | 1657    |
------------------------+-------------+---------+
```

### SAMD21

* SAMD21 M0 Mini, 48MHz SAMD21

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
idle                    |   4/  8/ 18 | 1970    |
press/release           |   4/  8/ 19 | 1968    |
click                   |   4/  8/ 17 | 1968    |
double click            |   4/  8/ 17 | 1968    |
long press/repeat press |   4/  8/ 19 | 1967    |
Encode4To2ButtonConfig  |  19/ 23/ 33 | 1941    |
Encode8To3ButtonConfig  |  50/ 60/ 72 | 1874    |
EncodeButtonConfig      |  26/ 36/ 46 | 1917    |
LadderButtonConfig      | 857/866/877 | 1068    |
------------------------+-------------+---------+
```

**Note:** The 800 microseconds per iteration (of 7 buttons) for
`LadderButtonConfig` on the SAMD21 may be an anomaly of my cheap clone, or it
may be an pervasive problem with the `analogRead()` function of a SAMD21
microcontroller. I advise validating this result on your specific SAMD21 board.

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* AceButton 1.5
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.1

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
idle                    |   6/  8/ 52 | 1907    |
press/release           |   6/  8/ 42 | 1904    |
click                   |   6/  8/ 32 | 1901    |
double click            |   6/  8/ 28 | 1900    |
long press/repeat press |   6/  8/ 36 | 1899    |
Encode4To2ButtonConfig  |  21/ 23/ 41 | 1879    |
Encode8To3ButtonConfig  |  54/ 58/ 78 | 1822    |
EncodeButtonConfig      |  43/ 46/ 66 | 1839    |
LadderButtonConfig      | 133/140/262 | 1692    |
------------------------+-------------+---------+
```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* AceButton 1.5
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
idle                    |   3/  4/ 17 | 2002    |
press/release           |   2/  2/ 10 | 2002    |
click                   |   2/  2/  7 | 2002    |
double click            |   3/  3/  5 | 2002    |
long press/repeat press |   3/  3/  4 | 2002    |
Encode4To2ButtonConfig  |   7/  7/ 11 | 2002    |
Encode8To3ButtonConfig  |  16/ 16/ 20 | 2002    |
EncodeButtonConfig      |  13/ 13/ 26 | 2002    |
LadderButtonConfig      |  22/ 23/ 78 | 2002    |
------------------------+-------------+---------+
```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* AceButton 1.5
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
