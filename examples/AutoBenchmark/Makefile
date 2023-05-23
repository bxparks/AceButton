# See https://github.com/bxparks/EpoxyDuino for documentation about using
# EpoxyDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := AutoBenchmark
ARDUINO_LIBS := EpoxyMockDigitalWriteFast AceCommon AceButton
MORE_CLEAN := more_clean
include ../../../EpoxyDuino/EpoxyDuino.mk

.PHONY: benchmarks

AUNITER_DIR := ../../../AUniter/tools

TARGETS := nano.txt micro.txt samd21.txt stm32.txt samd51.txt \
	esp8266.txt esp32.txt

README.md: generate_readme.py generate_table.awk $(TARGETS)
	./generate_readme.py > $@

benchmarks: $(TARGETS)

# The USB/ACM ports can change dynamically. Make sure that the microcontroller
# is on the correct port before using these Make targets.
nano.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nano:USB0

micro.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END micro:ACM0

samd21.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END xiao:ACM0

stm32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END stm32:ACM0

samd51.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END 'itsym4:ACM*'

esp8266.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nodemcu:USB0

esp32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END esp32:USB0

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
