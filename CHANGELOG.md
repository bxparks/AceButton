# Changelog

* 1.0.6 (2018-03-25)
    * Add `kFeatureSuppressClickBeforeDoubleClick` flag to suppress
      Clicked event before a DoubleClicked event, at the cost of slower
      response time of the Clicked event. Added 2 more examples to demonstrate 2
      methods to distinguish between a Clicked and DoubleClicked.
    * Publish [doxygen docs](https://bxparks.github.io/AceButton/html/)
      on GitHub Pages.
* 1.0.5 (2018-03-17)
    * Migrate unit tests to [AUnit](https://github.com/bxparks/AUnit).
    * Fix various typos in README.md.
* 1.0.4 (2018-03-07)
    * Support ESP8266.
        * Split `loop()` in `Stopwatch.ino` into inner and outer loops, to
          allow `loop()` to return periodically.
        * Perform manual testing, since ArduinoUnit does not work on ESP8266.
    * Optimize `check()` so that `checkOrphanedClick()` is called only when
      needed.
    * README.md: add  benchmark numbers for ESP8266, fix typos.
    * Fix various compiler warnings about unused variables.
* 1.0.3 (2018-02-13)
    * Make library work on Teensy LC and 3.2.
        * Fix `elapsedTime` expression that breaks on 32-bit processors
          (whose `int` is 4 bytes instead of 2).
* 1.0.2 (2018-02-07)
    * Add documentation and unit tests for `AdjustableButtonConfig`.
    * Reduce `orphanClickDelay` to 1X `getDoubleClickDelay()` instead of 10X.
* 1.0.1 (2018-02-03)
    * Fix typo in 'library.properties'.
* 1.0.0 (2018-02-03)
    * Initial public release.
