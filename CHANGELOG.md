# Changelog

* 1.2 (2018-08-14)
    * Add `AceButton(ButtonConfig*)` constructor to support constructor
      dependency injection. Recommended over `setButtonConfig()`.
      Should be 100% backwards compatible.
    * Add examples/CapacitiveSwitch program to demonstrate integration
      with CapacitiveSensor library to support capacitive switches.
    * Add continuous integration using AUniter/Jenkins.
* 1.1.1 (2018-06-21)
    * Fix compiler warnings about unused parameters in eventHandler callback.
    * Move AutoBenchmark results into AutoBenchmark/README.md.
    * Update various links in AceButton/README.md.
    * No functional or API change.
* 1.1.0 (2018-05-03)
    * Change to MIT License.
    * Add instrumentation of AceButton.check() using TimingStats to measure
      performance. (Fixes #9)
    * Add `examples/AutoBenchmark` sketch to auto-generate benchmarks for
      various microcontrollers.
    * Verify that library and examples compile, and the tests pass for ESP32.
    * Add a third method for distinguishing Clicked from DoubleClicked using
      both techniques described in 1.0.6.
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
