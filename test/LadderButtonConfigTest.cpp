#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include <ace_button/testing/TestableLadderButtonConfig.h>
#include <ace_button/testing/EventTracker.h>
#include <ace_button/testing/HelperForLadderButtonConfig.h>

using namespace ace_button;
using namespace ace_button::testing;

// --------------------------------------------------------------------------

// work around https://github.com/Arduino-CI/arduino_ci/issues/242
#define A0 2

// Define the actual ADC pin. Actually value not used for testing.
static const uint8_t BUTTON_PIN = A0;

// Create 4 AceButton objects, with their virtual pin number 0 to 3.
// Note: we could use an array of AceButton BUTTONS[4], and use a loop to
// initialize these arrays, but this is more explicit and easier to understand
// as an example code.
static const uint8_t NUM_BUTTONS = 4;
static AceButton b0((uint8_t) 0);
static AceButton b1(1);
static AceButton b2(2);
static AceButton b3(3);
static AceButton* const BUTTONS[NUM_BUTTONS] = {
  &b0, &b1, &b2, &b3,
};

// Define the ADC voltage levels for each button. For 4 buttons,
// we need 5 levels: 0%, 25%, 50%, 75%, 100%.
static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
static const uint16_t LEVELS[NUM_LEVELS] = {
  0 /* 0%, short to ground */,
  327 /* 32%, 4.7 kohm */,
  512 /* 50%, 10 kohm */,
  844 /* 82%, 47 kohm */,
  1023 /* 100%, open circuit */,
};

static TestableLadderButtonConfig testableConfig(
  BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
);
static EventTracker eventTracker;
static HelperForLadderButtonConfig helper(&testableConfig, &eventTracker);

// Store the arguments passed into the event handler into the EventTracker
// for assertion later.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  eventTracker.addEvent(button->getPin(), eventType, buttonState);
}

unittest_setup() {
  testableConfig.init();
  testableConfig.setEventHandler(handleEvent);
}

// --------------------------------------------------------------------------
// LadderButtonConfig
// --------------------------------------------------------------------------

// not sure how to test this properly
//
// unittest(LadderButtonConfig_extractIndex) {
//   assertEqual(0, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 0));
//   assertEqual(0, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 162));
//
//   assertEqual(1, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 163));
//   assertEqual(1, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 418));
//
//   assertEqual(2, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 419));
//
//   assertEqual(4, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 933));
//   assertEqual(4, LadderButtonConfig_extractIndex::extractIndex(NUM_LEVELS, LEVELS, 1023 + 1));
// }

unittest(LadderButtonConfig_press_and_release_pullup) {
  const unsigned long BASE_TIME = 65500; // rolls over in 36 milliseconds
  helper.init();

  // start the AceButton.check()
  helper.releaseButton(BASE_TIME);

  // initialization phase
  helper.releaseButton(BASE_TIME + 50);

  // Press button 1
  helper.pressButton(BASE_TIME + 100, 1);
  assertEqual(0, eventTracker.getNumEvents());

  // Press button 1 still debouncing
  helper.pressButton(BASE_TIME + 110, 1);
  assertEqual(0, eventTracker.getNumEvents());

  // After more than 20ms, button 1 press registers
  helper.pressButton(BASE_TIME + 130, 1);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventPressed, record.getEventType());
    assertEqual(1, record.getPin());
    assertEqual(LOW, record.getButtonState());
  }

  // release the button, still debouncing
  helper.releaseButton(BASE_TIME + 1000);
  assertEqual(0, eventTracker.getNumEvents());

  // release the button, after more 20ms, register a release
  helper.releaseButton(BASE_TIME + 1030);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventReleased, record.getEventType());
    assertEqual(1, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
}

unittest(LadderButtonConfig_click) {
  const unsigned long BASE_TIME = 65500; // rolls over in 36 milliseconds
  helper.init();
  testableConfig.setFeature(ButtonConfig::kFeatureClick);

  // Start the AceButton.check().
  helper.releaseButton(BASE_TIME);

  // Initialization phase.
  helper.releaseButton(BASE_TIME + 50);

  // Press button 2.
  helper.pressButton(BASE_TIME + 100, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // Press button 2 still debouncing.
  helper.pressButton(BASE_TIME + 110, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // After more than 20ms, button 2 press registers.
  helper.pressButton(BASE_TIME + 130, 2);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventPressed, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(LOW, record.getButtonState());
  }

  // Continue pressing button for 100ms.
  helper.pressButton(BASE_TIME + 230, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // Release before 200 ms (from start of pressed event); waiting for
  // debouncing.
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Debouncing finished after 20 ms.
  helper.releaseButton(BASE_TIME + 320);
  assertEqual(2, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventClicked, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
  {
    const EventRecord& record = eventTracker.getRecord(1);
    assertEqual(AceButton::kEventReleased, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
}

unittest_main();
