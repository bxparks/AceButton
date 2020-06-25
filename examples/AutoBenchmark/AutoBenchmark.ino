/*
 * A program that prints out the time (min/avg/max) taken by the
 * AceButton::check() method.
 */

#include <AceButton.h>
#include <ace_button/testing/TimingStats.h>
#include "ProfilingButtonConfig.h"
using namespace ace_button;
using namespace ace_button::testing;

#if defined(ESP32) && !defined(SERIAL_PORT_MONITOR)
#define SERIAL_PORT_MONITOR Serial
#endif

// The pin number attached to the button.
const int BUTTON_PIN = 2;

// Create one button wired using the ProfilingButtonConfig, which is the same as
// ButtonConfig except readButton() is overridden for profiling purposes.
ProfilingButtonConfig buttonConfig;
AceButton simpleButton(&buttonConfig);

// Create an array of 7 buttons wired to use the EncodedButtonConfig using
// 4 digital pins.
static const uint8_t NUM_PINS = 3;
static const uint8_t PINS[] = {2, 3, 4};
static const uint8_t NUM_BUTTONS = 7;
static AceButton b01(1);
static AceButton b02(2);
static AceButton b03(3);
static AceButton b04(4);
static AceButton b05(5);
static AceButton b06(6);
static AceButton b07(7);
static AceButton* const BUTTONS[NUM_BUTTONS] = {
    &b01, &b02, &b03, &b04, &b05, &b06, &b07,
};
static EncodedButtonConfig encodedButtonConfig(
  NUM_PINS, PINS, NUM_BUTTONS, BUTTONS
);

// Create a LadderButtonConfig with 8 levels for 7 buttons.
static const int ANALOG_BUTTON_PIN = A0;
static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
static const uint16_t LEVELS[NUM_LEVELS] = {
  0   /* 0%, short to ground */,
  133 /* 13%, 1.5 kohm */,
  327 /* 32%, 4.7 kohm */,
  512 /* 50%, 10 kohm */,
  614 /* 60%, 15 kohm */,
  788 /* 77%, 33 kohm */,
  930 /* 91%, 100 kohm */,
  1023 /* 100%, open circuit */,
};
static LadderButtonConfig ladderButtonConfig(
  ANALOG_BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
);

const unsigned long STATS_PRINT_INTERVAL = 2000;
TimingStats stats;

const uint8_t LOOP_MODE_START = 0;
const uint8_t LOOP_MODE_IDLE = 1;
const uint8_t LOOP_MODE_PRESS_RELEASE = 2;
const uint8_t LOOP_MODE_CLICK = 3;
const uint8_t LOOP_MODE_DOUBLE_CLICK = 4;
const uint8_t LOOP_MODE_LONG_PRESS = 5;
const uint8_t LOOP_MODE_ENCODED_BUTTON_CONFIG = 6;
const uint8_t LOOP_MODE_LADDER_BUTTON_CONFIG = 7;
const uint8_t LOOP_MODE_END = 8;
uint8_t loopMode;
uint8_t loopEventType;

// print integer within 3 characters, padded on left with spaces
void printInt(uint16_t i) {
  if (i < 100) SERIAL_PORT_MONITOR.print(' ');
  if (i < 10) SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(i);
}

void printStats() {
  printInt(stats.getMin());
  SERIAL_PORT_MONITOR.print('/');
  printInt(stats.getAvg());
  SERIAL_PORT_MONITOR.print('/');
  printInt(stats.getMax());
  SERIAL_PORT_MONITOR.print(F(" | "));
  printInt(stats.getCount());
}

void nextMode() {
  stats.reset();
  buttonConfig.setButtonState(HIGH);
  loopMode++;
}

// An empty event handler.
void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  loopEventType = eventType;
}

// Measure how long the AceButton.check() takes
void checkSimpleButton() {
  uint16_t startMicros = micros();
  simpleButton.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkEncodedButtons() {
  uint16_t startMicros = micros();
  encodedButtonConfig.checkButtons();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkLadderButtons() {
  uint16_t startMicros = micros();
  ladderButtonConfig.checkButtons();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void loopStart() {
  static unsigned long start = millis();

  // Wait one iteration for things to cool down.
  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.println(
        F("------------------------+-------------+---------+"));
    SERIAL_PORT_MONITOR.println(
        F("button event            | min/avg/max | samples |"));
    SERIAL_PORT_MONITOR.println(
        F("------------------------+-------------+---------+"));
    nextMode();
  }
}

void loopEnd() {
  SERIAL_PORT_MONITOR.println(
      F("------------------------+-------------+---------+"));
  nextMode();
}

void loopIdle() {
  static unsigned long start = millis();

  checkSimpleButton();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.print(F("idle                    | "));
    printStats();
    SERIAL_PORT_MONITOR.println(F("    |"));
    nextMode();
  }
}

void loopPressRelease() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 1000) buttonConfig.setButtonState(LOW);
  if (1000 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType == AceButton::kEventReleased) {
      SERIAL_PORT_MONITOR.print(F("press/release           | "));
      printStats();
      SERIAL_PORT_MONITOR.println(F("    |"));
    }
    nextMode();
  }
}

void loopClick() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 200) buttonConfig.setButtonState(LOW);
  if (200 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType == AceButton::kEventClicked) {
      SERIAL_PORT_MONITOR.print(F("click                   | "));
      printStats();
      SERIAL_PORT_MONITOR.println(F("    |"));
    }
    nextMode();
  }
}

void loopDoubleClick() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 200) buttonConfig.setButtonState(LOW);
  if (200 <= elapsed && elapsed < 300) buttonConfig.setButtonState(HIGH);
  if (300 <= elapsed && elapsed < 400) buttonConfig.setButtonState(LOW);
  if (400 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType == AceButton::kEventDoubleClicked) {
      SERIAL_PORT_MONITOR.print(F("double click            | "));
      printStats();
      SERIAL_PORT_MONITOR.println(F("    |"));
    }
    nextMode();
  }
}

void loopLongPress() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed) buttonConfig.setButtonState(LOW);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType == AceButton::kEventRepeatPressed) {
      SERIAL_PORT_MONITOR.print(F("long press/repeat press | "));
      printStats();
      SERIAL_PORT_MONITOR.println(F("    |"));
    }
    nextMode();
  }
}

void loopEncodedButtonConfig() {
  static unsigned long start = millis();

  checkEncodedButtons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.print(F("EncodeButtonConfig      | "));
    printStats();
    SERIAL_PORT_MONITOR.println(F("    |"));
    nextMode();
  }
}

void loopLadderButtonConfig() {
  static unsigned long start = millis();

  checkLadderButtons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.print(F("LadderButtonConfig      | "));
    printStats();
    SERIAL_PORT_MONITOR.println(F("    |"));
    nextMode();
  }
}

void setup() {
  delay(1000); // some microcontrollers reboot twice
  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // wait until ready - Leonardo/Micro
  SERIAL_PORT_MONITOR.println(F("setup(): begin"));

  // Print sizeof various classes
  SERIAL_PORT_MONITOR.print(F("sizeof(AceButton): "));
  SERIAL_PORT_MONITOR.println(sizeof(AceButton));

  SERIAL_PORT_MONITOR.print(F("sizeof(ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(ButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(Encoded4To2ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(Encoded4To2ButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(Encoded8To3ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(Encoded8To3ButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(EncodedButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(EncodedButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(LadderButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(LadderButtonConfig));

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  simpleButton.init(BUTTON_PIN);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  buttonConfig.setEventHandler(handleEvent);
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

  loopMode = LOOP_MODE_START;
  loopEventType = AceButton::kEventPressed;

  SERIAL_PORT_MONITOR.println(F("setup(): end"));
}

void loop() {
  delay(1); // Decrease sampling frequency to about 1000 Hz

  switch (loopMode) {
    case LOOP_MODE_START:
      loopStart();
      break;
    case LOOP_MODE_IDLE:
      loopIdle();
      break;
    case LOOP_MODE_PRESS_RELEASE:
      loopPressRelease();
      break;
    case LOOP_MODE_CLICK:
      loopClick();
      break;
    case LOOP_MODE_DOUBLE_CLICK:
      loopDoubleClick();
      break;
    case LOOP_MODE_LONG_PRESS:
      loopLongPress();
      break;
    case LOOP_MODE_ENCODED_BUTTON_CONFIG:
      loopEncodedButtonConfig();
      break;
    case LOOP_MODE_LADDER_BUTTON_CONFIG:
      loopLadderButtonConfig();
      break;
    case LOOP_MODE_END:
      loopEnd();
      break;
    default:
      #ifdef UNIX_HOST_DUINO
        exit(1);
      #endif
      break;
  }
}

