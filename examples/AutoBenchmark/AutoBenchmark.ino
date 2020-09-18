/*
 * A program that prints out the time (min/avg/max) taken by the
 * AceButton::check() method.
 */

#include <AceButton.h>
#include <ace_button/testing/TimingStats.h>
#include "ProfilingButtonConfig.h"
using namespace ace_button;
using namespace ace_button::testing;

// Set this to 1 to run the benchmarks using the IEventHandler object instead of
// EventHandler function. The result is that AceButton::check() seems slightly
// slower (maybe a few microseconds). Probably not enough to worry about.
#define EVENT_HANDLER_CLASS 1

#if defined(ESP32) && !defined(SERIAL_PORT_MONITOR)
#define SERIAL_PORT_MONITOR Serial
#endif

// The pin number attached to the button.
const int BUTTON_PIN = 2;

// Create one button wired using the ProfilingButtonConfig, which is the same as
// ButtonConfig except readButton() is overridden for profiling purposes.
ProfilingButtonConfig buttonConfig;
AceButton simpleButton(&buttonConfig);

// Create 3 buttons for Encoded4To2ButtonConfig
static const uint8_t BUTTON_PIN0 = 2;
static const uint8_t BUTTON_PIN1 = 3;
static const uint8_t BUTTON_PIN2 = 4;
Encoded4To2ButtonConfig encoded4To2ButtonConfig(BUTTON_PIN0, BUTTON_PIN1);
AceButton four1(&encoded4To2ButtonConfig, 1);
AceButton four2(&encoded4To2ButtonConfig, 2);
AceButton four3(&encoded4To2ButtonConfig, 3);

// Create 7 buttons for Encoded8To3ButtonConfig
Encoded8To3ButtonConfig encoded8To3ButtonConfig(
    BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);
AceButton eight1(&encoded8To3ButtonConfig, 1);
AceButton eight2(&encoded8To3ButtonConfig, 2);
AceButton eight3(&encoded8To3ButtonConfig, 3);
AceButton eight4(&encoded8To3ButtonConfig, 4);
AceButton eight5(&encoded8To3ButtonConfig, 5);
AceButton eight6(&encoded8To3ButtonConfig, 6);
AceButton eight7(&encoded8To3ButtonConfig, 7);

// Create an array of 7 buttons wired to use the EncodedButtonConfig using
// 4 digital pins.
static const uint8_t NUM_PINS = 3;
static const uint8_t PINS[] = {2, 3, 4};
static const uint8_t NUM_BUTTONS = 7;
static AceButton e01(1);
static AceButton e02(2);
static AceButton e03(3);
static AceButton e04(4);
static AceButton e05(5);
static AceButton e06(6);
static AceButton e07(7);
static AceButton* const ENCODED_BUTTONS[NUM_BUTTONS] = {
    &e01, &e02, &e03, &e04, &e05, &e06, &e07,
};
static EncodedButtonConfig encodedButtonConfig(
  NUM_PINS, PINS, NUM_BUTTONS, ENCODED_BUTTONS
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
static AceButton r01(1);
static AceButton r02(2);
static AceButton r03(3);
static AceButton r04(4);
static AceButton r05(5);
static AceButton r06(6);
static AceButton r07(7);
static AceButton* const LADDER_BUTTONS[NUM_BUTTONS] = {
    &r01, &r02, &r03, &r04, &r05, &r06, &r07,
};
static LadderButtonConfig ladderButtonConfig(
  ANALOG_BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, LADDER_BUTTONS
);

const unsigned long STATS_PRINT_INTERVAL = 2000;
TimingStats stats;

const uint8_t LOOP_MODE_START = 0;
const uint8_t LOOP_MODE_IDLE = 1;
const uint8_t LOOP_MODE_PRESS_RELEASE = 2;
const uint8_t LOOP_MODE_CLICK = 3;
const uint8_t LOOP_MODE_DOUBLE_CLICK = 4;
const uint8_t LOOP_MODE_LONG_PRESS = 5;
const uint8_t LOOP_MODE_ENCODED_4TO2_BUTTON_CONFIG = 6;
const uint8_t LOOP_MODE_ENCODED_8TO3_BUTTON_CONFIG = 7;
const uint8_t LOOP_MODE_ENCODED_BUTTON_CONFIG = 8;
const uint8_t LOOP_MODE_LADDER_BUTTON_CONFIG = 9;
const uint8_t LOOP_MODE_END = 10;
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

#if EVENT_HANDLER_CLASS
  class ButtonHandler: public IEventHandler {
    public:
      void handleEvent(AceButton* /* button */, uint8_t eventType,
          uint8_t /* buttonState */) override {
        loopEventType = eventType;
      }
  };

  ButtonHandler handleEvent;
#else
  // An empty event handler.
  void handleEvent(AceButton* /* button */, uint8_t eventType,
      uint8_t /* buttonState */) {
    loopEventType = eventType;
  }
#endif

// Measure how long the AceButton.check() takes
void checkSimpleButton() {
  uint16_t startMicros = micros();
  simpleButton.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkEncoded4To2Buttons() {
  uint16_t startMicros = micros();
  four1.check();
  four2.check();
  four3.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkEncoded8To3Buttons() {
  uint16_t startMicros = micros();
  eight1.check();
  eight2.check();
  eight3.check();
  eight4.check();
  eight5.check();
  eight6.check();
  eight7.check();
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

void loopEncoded4To2ButtonConfig() {
  static unsigned long start = millis();

  checkEncoded4To2Buttons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.print(F("Encode4To2ButtonConfig  | "));
    printStats();
    SERIAL_PORT_MONITOR.println(F("    |"));
    nextMode();
  }
}

void loopEncoded8To3ButtonConfig() {
  static unsigned long start = millis();

  checkEncoded8To3Buttons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    SERIAL_PORT_MONITOR.print(F("Encode8To3ButtonConfig  | "));
    printStats();
    SERIAL_PORT_MONITOR.println(F("    |"));
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
#if EVENT_HANDLER_CLASS
  buttonConfig.setIEventHandler(&handleEvent);
#else
  buttonConfig.setEventHandler(handleEvent);
#endif
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if EVENT_HANDLER_CLASS
  encoded4To2ButtonConfig.setIEventHandler(&handleEvent);
#else
  encoded4To2ButtonConfig.setEventHandler(handleEvent);
#endif
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if EVENT_HANDLER_CLASS
  encoded8To3ButtonConfig.setIEventHandler(&handleEvent);
#else
  encoded8To3ButtonConfig.setEventHandler(handleEvent);
#endif
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if EVENT_HANDLER_CLASS
  encodedButtonConfig.setIEventHandler(&handleEvent);
#else
  encodedButtonConfig.setEventHandler(handleEvent);
#endif
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if EVENT_HANDLER_CLASS
  ladderButtonConfig.setIEventHandler(&handleEvent);
#else
  ladderButtonConfig.setEventHandler(handleEvent);
#endif
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

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
    case LOOP_MODE_ENCODED_4TO2_BUTTON_CONFIG:
      loopEncoded4To2ButtonConfig();
      break;
    case LOOP_MODE_ENCODED_8TO3_BUTTON_CONFIG:
      loopEncoded8To3ButtonConfig();
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

