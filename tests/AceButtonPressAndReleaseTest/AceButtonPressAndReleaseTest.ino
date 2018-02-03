#line 2 "AceButtonPressAndReleaseTest.ino"
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
TestHelper helper(testableConfig, button, eventTracker);

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

// We assume this will be the common case because of the Aruino boards provide
// internal pullup resisters on the digital input pins.
test(press_and_release_pullup) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  // initial button state
  helper.releaseButton(0);
  assertEqual(0, eventTracker.getNumEvents());

  // must wait until the initial debouncing
  helper.releaseButton(50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(100);
  assertEqual(0, eventTracker.getNumEvents());

  // still in debouncing period, so no event yet
  helper.releaseButton(110);
  assertEqual(0, eventTracker.getNumEvents());

  // after more than 50 ms, we should get an event
  helper.pressButton(190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button
  helper.releaseButton(1000);
  assertEqual(0, eventTracker.getNumEvents());

  // wait more than 50 ms
  helper.releaseButton(1060);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
}

// Do the same test as press_and_release_pullup, but using
// the logic levels of an external pulldown resister.
test(press_and_release_pulldown) {
  const uint8_t DEFAULT_RELEASED_STATE = LOW;
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  // initial button state
  helper.releaseButton(0);
  assertEqual(0, eventTracker.getNumEvents());

  // must wait until the initial debouncing
  helper.releaseButton(50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(100);
  assertEqual(0, eventTracker.getNumEvents());

  // still in debouncing period, so no event yet
  helper.pressButton(110);
  assertEqual(0, eventTracker.getNumEvents());

  // after more than 50 ms, we should get an event
  helper.pressButton(190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());

  // release the button
  helper.releaseButton(1000);
  assertEqual(0, eventTracker.getNumEvents());

  // wait more than 50 ms
  helper.releaseButton(1060);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());
}

// The AceButton class uses 16-bit timer variables for memory efficiency.
// Verify that we can rollover those variables without affecting the logic.
test(clock_rollover) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  const unsigned long BASE_TIME = 65500; // rolls over in 36 milliseconds
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  // initial button state
  helper.releaseButton(BASE_TIME + 0);
  assertEqual(0, eventTracker.getNumEvents());

  // initialization phase, so no event yet
  helper.releaseButton(BASE_TIME + 60);
  assertEqual(0, eventTracker.getNumEvents());

  // press after the initialization phase, no event, must wait for debouncing
  helper.pressButton(BASE_TIME + 100);
  assertEqual(0, eventTracker.getNumEvents());

  // after more than 50 ms, we should get an event
  helper.pressButton(BASE_TIME + 190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button
  helper.releaseButton(BASE_TIME + 1000);
  assertEqual(0, eventTracker.getNumEvents());

  // wait more than 50 ms
  helper.releaseButton(BASE_TIME + 1060);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
}
