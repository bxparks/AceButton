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

**Memory**
```
sizeof(AceButton): 14
sizeof(ButtonConfig): 20
sizeof(Encoded4To2ButtonConfig): 23
sizeof(Encoded8To3ButtonConfig): 24
sizeof(EncodedButtonConfig): 27
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |  12/ 13/ 20 | 1934    |
press/release           |   8/ 14/ 24 | 1925    |
click                   |   8/ 14/ 24 | 1925    |
double click            |   8/ 14/ 24 | 1924    |
long press/repeat press |   8/ 15/ 24 | 1926    |
------------------------+-------------+---------+
```

### SAMD21

* SAMD21 M0 Mini, 48MHz SAMD21

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 28
sizeof(Encoded4To2ButtonConfig): 32
sizeof(Encoded8To3ButtonConfig): 32
sizeof(EncodedButtonConfig): 40
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   8/  8/  8 | 2002    |
press/release           |   4/  8/ 13 | 2002    |
click                   |   4/  8/ 16 | 2002    |
double click            |   4/  7/ 14 | 2002    |
long press/repeat press |   4/  8/ 11 | 2002    |
------------------------+-------------+---------+
```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 28
sizeof(Encoded4To2ButtonConfig): 32
sizeof(Encoded8To3ButtonConfig): 32
sizeof(EncodedButtonConfig): 40
```

**CPU**

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   7/  8/ 20 | 1922    |
press/release           |   6/  8/ 45 | 1918    |
click                   |   6/  8/ 20 | 1918    |
double click            |   6/  8/ 51 | 1910    |
long press/repeat press |   6/  8/ 48 | 1903    |
------------------------+-------------+---------+
```

The large **max** times for "double click" and "long press" seem to be
reproducible. I have not researched this but my speculation is that the system
WiFi code interrupts the `AceButton::check()` method right when the "double
click" and "long press" samples are taken, causing the extra latency.

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 28
sizeof(Encoded4To2ButtonConfig): 32
sizeof(Encoded8To3ButtonConfig): 32
sizeof(EncodedButtonConfig): 40
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   3/  3/  3 | 2002    |
press/release           |   2/  2/  9 | 2002    |
click                   |   2/  2/  6 | 2002    |
double click            |   2/  2/  4 | 2002    |
long press/repeat press |   2/  2/  3 | 2002    |
------------------------+-------------+---------+
```

### Teensy 3.2

* 96 MHz ARM Cortex-M4

**Memory**
```
sizeof(AceButton): 16
sizeof(ButtonConfig): 28
sizeof(Encoded4To2ButtonConfig): 32
sizeof(Encoded8To3ButtonConfig): 32
sizeof(EncodedButtonConfig): 40
```

**CPU**
```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   3/  3/  5 | 1985    |
press/release           |   1/  3/  6 | 1983    |
click                   |   1/  3/  6 | 1984    |
double click            |   1/  3/  6 | 1984    |
long press/repeat press |   1/  3/  6 | 1983    |
------------------------+-------------+---------+
```
