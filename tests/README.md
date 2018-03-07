# AceButtonTest

## ArduinoUnit Fork

These unit tests depend on
[my own forked version](https://github.com/bxparks/arduinounit) of
[ArduinoUnit](https://github.com/mmurdoch/arduinounit). As explained
in [issue #70](https://github.com/mmurdoch/arduinounit/issues/70),
the original ArduinoUnit consumes too much flash memory. The `AceButtonTest.ino`
sketch generated 49kB of flash with the orignal version, which no longer fits
inside the 32kB space of an Arduino Nano (ATmega328P). My forked version
decreases the flash size by 50%, so that `AceButtonTest` consumes only 25kB
instead of 49kB. This now fits inside an Arduino Nano.

I tried splitting the tests into 6 smaller sketches, which worked for a while.
But when I started testing the library on multiple platforms (e.g. Arduino,
Teensy, ESP8266), it became too cumbersome to repeatedly run 6 sketches across
these platforms.

The `AceButtonTest` sketch will probably run fine on an Arduino Mega (with 256kB
of flash) using the original ArduinoUnit. However, flashing 50kB is a
time-consuming process and it may still be worthwhile to use my fork.

## Installation

To install my fork of ArduinoUnit:

* `git clone https://github.com/bxparks/arduinounit.git`
* save the old version of ArduinoUnit
    * `cd .../libraries`
    * `mv ArduinoUnit ArduinoUnit.orig`
* copy over the entire `./arduinounit/` directory to the
  `./libraries/ArduinoUnit/` directory of your Arduino IDE
    * `cp -a arduinounit .../libraries/ArduinoUnit`
    * (Notice the capitalization change)
* you may need to restart your Arduino IDE
