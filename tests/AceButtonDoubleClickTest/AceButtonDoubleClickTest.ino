#line 2 "AceButtonDoubleClickTest.ino"
/*
   Copyright 2018 Brian T. Park

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <ArduinoUnit.h>
#include <AdjustableButtonConfig.h>
#include <AceButton.h>
#include <testing/TestableButtonConfig.h>
#include <testing/EventTracker.h>
#include <testing/TestHelper.h>

using namespace ace_button;
using namespace ace_button::testing;

const uint8_t PIN = 13;
const uint8_t BUTTON_ID = 1;

TestableButtonConfig testableConfig;
AceButton button;
EventTracker eventTracker;
TestHelper helper(&testableConfig, &button, &eventTracker);

// The event handler takes the arguments sent with the event and stored them
// into the EventTracker circular buffer.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  eventTracker.addEvent(eventType, buttonState);
}

void setup() {
  Serial.begin(9600);
  testableConfig.setEventHandler(handleEvent);
  button.setButtonConfig(&testableConfig);
  while (!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  Test::run();
}

// Test a double click. Verify also that a triple-click does not generate a
// spurious second double click. It should generate only the following:
//  Pressed, Clicked, Pressed, DoubleClicked, Pressed, Clicked
// because we have suppressed the Released events.
test(double_click_suppressed) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  testableConfig.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
  testableConfig.setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // generate first click

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce.
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());

  // generate generate second click within 400 ms of the CLICK event (which
  // occurred at +350 ms) for a double click

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 500);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 550);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 650);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get a double-click.
  helper.releaseButton(BASE_TIME + 700);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventDoubleClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());

  // generate third click within 400 ms of the DoubleClicked event (which
  // occurred at +700 ms)

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 900);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event,
  helper.pressButton(BASE_TIME + 950);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 1050);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce.
  // Verify that we get only 1 Clicked event not another DoubleClicked.
  helper.releaseButton(BASE_TIME + 1100);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
}

// Test a double click without kFeatureSuppressAfterDoubleClick.
// Three rapid clicks should generate the following:
//    Pressed, Released, Clicked, Pressed, Released, DoubleClicked, Pressed,
//    Released, Clicked.
test(double_click_not_suppressed) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureDoubleClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // generate first click

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce.
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // generate second click within 400 ms of the Clicked event (which
  // occurred at +350 ms) for a double click

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 500);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 550);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 650);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get a DoubleClicked, and a
  // Released because we don't suppress.
  helper.releaseButton(BASE_TIME + 700);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventDoubleClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // generate third click within 400 ms of the DoubleClicked event (which
  // occurred at +700 ms)

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 900);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event,
  helper.pressButton(BASE_TIME + 950);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 1050);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce.
  // Verify that we get only 1 Clicked event not another DoubleClicked, 
  // and an unsuppressed Released.
  helper.releaseButton(BASE_TIME + 1100);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());
}

// Test that no double clicks generated with isFeature() flag off.
test(no_double_click_without_feature_flag) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  uint8_t expected;

  // reset the button
  // make sure isFeatureDoubleClick flag is cleared
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureClick);
  testableConfig.clearFeature(ButtonConfig::kFeatureDoubleClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // generate first click

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce to get Released
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // generate second click within 400 ms of the Clicked event (which
  // occurred at +350 ms) for a double click

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 500);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 550);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 650);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get just another click since
  // double-click is turned off.
  helper.releaseButton(BASE_TIME + 700);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());
}

// Test that an orphaned click is properly removed to prevent spurious
// double-click if the second click happens slightly over 65.536 seconds later.
test(orphaned_click_cleared) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  const unsigned long ROLLOVER_TIME = 65536;
  uint8_t expected;

  // reset the button, and enable double-click
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureClick);
  testableConfig.setFeature(ButtonConfig::kFeatureDoubleClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms to get event
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // Move time forward, so that the orphaned click is cleared.
  // If AceButton.checkOrphanedClick() is disabled, or this statement is removed
  // (thereby preventing a call to checkOrphanedClick()), then the asserts below
  // will fail.
  helper.checkTime(BASE_TIME + 5000);
  assertEqual(0, eventTracker.getNumEvents());

  // Generate another click between (65.535s, 65.535s + 400 ms) of the first
  // CLICK event (i.e. +250 ms). If the first orphaned click was not properly
  // reset, then this will genearte a double click instead of a single click.

  // button pressed, but must wait to debounce
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 400);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 450);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 550);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get a single click, not
  // a double click.
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 600);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());
}

// Test that an orphaned click generates a double click if not cleared.
test(orphaned_click_causes_double_click_if_not_cleared) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  const unsigned long ROLLOVER_TIME = 65536;
  uint8_t expected;

  // reset the button, and enable double-click
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureClick);
  testableConfig.setFeature(ButtonConfig::kFeatureDoubleClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms to get event
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // Simulate an orphaned click not getting cleared by not calling
  // AceButton.check() for 65536 milliseconds.

  // Generate another click between (65.535s, 65.535s + 400 ms) of the first
  // CLICK event (i.e. +250 ms). If the first orphaned click was not properly
  // reset, then this will genearte a double click instead of a single click.

  // button pressed, but must wait to debounce
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 400);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 450);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 550);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get a double click because the
  // orphaned click was not removed before the 16-bit integer overflowed .
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 600);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventDoubleClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());
}

// Test that an orphaned click is removed if Click is enabled.
test(orphaned_click_removed_if_click_enabled) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500;
  const unsigned long ROLLOVER_TIME = 65536;
  uint8_t expected;

  // reset the button, and enable double-click
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  testableConfig.setFeature(ButtonConfig::kFeatureClick);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initilization phase
  helper.releaseButton(BASE_TIME + 50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(BASE_TIME + 140);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms to get event
  helper.releaseButton(BASE_TIME + 350);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());

  // Move time forward, so that the orphaned click is cleared.
  // If AceButton.checkOrphanedClick() is disabled, or this statement is removed
  // (thereby preventing a call to checkOrphanedClick()), then the asserts below
  // will fail.
  helper.checkTime(BASE_TIME + 5000);
  assertEqual(0, eventTracker.getNumEvents());

  // Turn on DoubleClick in the middle of click processing. If we called
  // checkOrphanedClick() only if DoubleClick was enabled (instead of checking
  // it when Click is enabled as well), then this change in ButtonConfig will
  // cause this test to fail.
  testableConfig.setFeature(ButtonConfig::kFeatureDoubleClick);

  // Generate another click between (65.535s, 65.535s + 400 ms) of the first
  // CLICK event (i.e. +250 ms). If the first orphaned click was not properly
  // reset, then this will genearte a double click instead of a single click.

  // button pressed, but must wait to debounce
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 400);
  assertEqual(0, eventTracker.getNumEvents());

  // after 50 ms or more, we should get an event
  helper.pressButton(ROLLOVER_TIME + BASE_TIME + 450);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button within 200 ms for a click, but must wait for debounce
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 550);
  assertEqual(0, eventTracker.getNumEvents());

  // Wait another 50 ms for debounce. Should get a single click, not
  // a double click.
  helper.releaseButton(ROLLOVER_TIME + BASE_TIME + 600);
  assertEqual(2, eventTracker.getNumEvents());
  expected = AceButton::kEventClicked;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(1).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(1).getButtonState());
}
