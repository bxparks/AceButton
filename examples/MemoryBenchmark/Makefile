# See https://github.com/bxparks/UnixHostDuino for documentation about using
# UnixHostDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := MemoryBenchmark
ARDUINO_LIBS := AceButton
MORE_CLEAN := more_clean
include ../../../UnixHostDuino/UnixHostDuino.mk

.PHONY: benchmarks

TARGETS := nano.txt micro.txt samd.txt esp8266.txt esp32.txt teensy32.txt

README.md: $(TARGETS) generate_readme.py
	./generate_readme.py > $@

benchmarks: $(TARGETS)

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
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
