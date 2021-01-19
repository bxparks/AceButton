# See https://github.com/bxparks/UnixHostDuino for documentation about using
# UnixHostDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := AutoBenchmark
ARDUINO_LIBS := AceCommon AceButton
MORE_CLEAN := more_clean
include ../../../UnixHostDuino/UnixHostDuino.mk

.PHONY: benchmarks

AUNITER_DIR := ../../../AUniter/tools

TARGETS := nano.txt micro.txt samd.txt esp8266.txt esp32.txt \
stm32.txt # teensy32.txt

README.md: generate_readme.py generate_table.awk $(TARGETS)
	./generate_readme.py > $@

benchmarks: $(TARGETS)

# These targets don't actually work because the tty ports (USB0, ACM0) can
# change dynamically. I may need to create a script to run these interactively,
# something like `collect.sh nano:USB0 micro:ACM0 esp8266:USB1 ...`.
nano.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nano:USB0

micro.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END micro:ACM0

samd.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END samd:ACM0

esp8266.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nodemcu:USB0

stm32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END stm32:ACM0

esp32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END esp32:USB0

# Teensy requires manual capture of teensy32.txt. I can't even do that manually
# on some of my laptops because Teensy refuses to connect. So this Makefile
# rule is an aspirational hope for consistency.
teensy32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END teensy32:ACM0

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
