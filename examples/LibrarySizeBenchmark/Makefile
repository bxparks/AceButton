# See https://github.com/bxparks/UnixHostDuino for documentation about using
# UnixHostDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := LibrarySizeBenchmark
ARDUINO_LIBS := AceButton
MORE_CLEAN := more_clean
include ../../../UnixHostDuino/UnixHostDuino.mk

TARGETS := nano.txt micro.txt samd.txt esp8266.txt esp32.txt teensy32.txt

# Type 'make benchmarks' to regenerate the benchmark files.
benchmarks: $(TARGETS)

.PHONY: benchmarks

nano.txt:
	./collect.sh nano $@

micro.txt:
	./collect.sh micro $@

samd.txt:
	./collect.sh mkrzero $@

esp8266.txt:
	./collect.sh esp8266 $@

esp32.txt:
	./collect.sh esp32 $@

teensy32.txt:
	./collect.sh teensy32 $@

more_clean:
	rm -f $(TARGETS)
