# LibrarySizeBenchmark

A small sketch to determine the size of the AceButton library.
First we compile it with `#define USE_ACE_BUTTON 1` to include the library.
Then we compile it with `#define USE_ACE_BUTTON 0` to exclude the library.
The difference should give us a rough idea of the size of the library.
(The compiler will produce slightly difference results for different programs.)

```
-------------+--------------+---------------+------------+
board        | AceButton    | w/o AceButton | Difference |
-------------+--------------+---------------+------------+
ATmega328P   |   1959/   53 |   1057/   39  |  902/14    |
ATmega32U4   |   4962/  193 |   4060/  179  |  902/14    |
SAMD21       |  12620/ 2996 |  11780/ 2976  |  840/20    |
ESP8266      | 258252/26840 | 257000/26820  | 1252/20    |
ESP32        | 209833/15308 | 208769/15292  | 1064/16    |
-------------+--------------+---------------+------------+
```

The above numbers were observing using Arduino IDE 1.8.13 the following
compiler versions:

* ATmega328P: AVR Core 1.8.3
* ATmega32U4: SparkFun AVR Core 1.1.13
* SAMD21: Arduino SAMD Core 1.8.6
* ESP8266 core 2.7.1
* ESP32 core 1.0.4
