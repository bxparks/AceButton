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

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |  16/ 17/ 24 | 1927    |
press/release           |   8/ 18/ 28 | 1916    |
click                   |   8/ 17/ 28 | 1921    |
double click            |   8/ 17/ 32 | 1918    |
long press/repeat press |   8/ 20/ 28 | 1917    |
------------------------+-------------+---------+
```

### Arduino Pro Micro

* 16MHz ATmega32U4

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |  16/ 17/ 28 | 1928    |
press/release           |   8/ 18/ 28 | 1919    |
click                   |   8/ 17/ 28 | 1923    |
double click            |   8/ 17/ 36 | 1920    |
long press/repeat press |   8/ 20/ 32 | 1918    |
------------------------+-------------+---------+
```

### Teensy 3.2

* 72 MHz ARM Cortex-M4

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   4/  4/  6 | 1983    |
press/release           |   1/  4/  7 | 1981    |
click                   |   1/  4/  7 | 1982    |
double click            |   1/  4/  8 | 1982    |
long press/repeat press |   1/  4/  7 | 1980    |
------------------------+-------------+---------+
```

### NodeMCU 1.0 clone

* 80MHz ESP8266

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   7/  7/ 12 | 1922    |
press/release           |   5/  7/ 37 | 1921    |
click                   |   5/  7/ 18 | 1922    |
double click            |   5/  7/ 50 | 1917    |
long press/repeat press |   5/  8/ 55 | 1910    |
------------------------+-------------+---------+
```

The large **max** times for "double click" and "long press" seem to be
reproducible. I have not researched this but my speculation is that the system
WiFi code interrupts the `AceButton::check()` method right when the "double
click" and "long press" samples are taken, causing the extra latency.

### ESP32-01 Dev Board

* 240 MHz Tensilica LX6

```
------------------------+-------------+---------+
button event            | min/avg/max | samples |
------------------------+-------------+---------+
idle                    |   4/  4/  4 | 2002    |
press/release           |   3/  3/ 10 | 2002    |
click                   |   3/  3/  9 | 2002    |
double click            |   3/  3/  7 | 2002    |
long press/repeat press |   3/  3/  4 | 2002    |
------------------------+-------------+---------+
```
