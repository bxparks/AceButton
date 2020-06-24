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
ATmega328P   |   2118/   53 |   1116/   39  | 1002/14    |
ATmega32U4   |   5120/  193 |   4120/  179  | 1000/14    |
SAMD21       |  12636/ 2992 |  11801/ 2976  |  835/16    |
ESP8266      | 258672/26856 | 257064/26820  | 1608/36    |
ESP32        | 209949/15308 | 208865/15292  | 1084/16    |
-------------+--------------+---------------+------------+
```

The above numbers were observing using Arduino IDE 1.8.13 the following
compiler versions:

* ATmega328P: AVR Core 1.8.3
* ATmega32U4: SparkFun AVR Core 1.1.13
* SAMD21: Arduino SAMD Core 1.8.6
* ESP8266 core 2.7.1
* ESP32 core 1.0.4
