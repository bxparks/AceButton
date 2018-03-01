#line 2 "AceButtonBasicTest.ino"
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
#include <AceButton.h>
#include <AdjustableButtonConfig.h>
#include <testing/TestableButtonConfig.h>
#include <testing/EventTracker.h>
#include <testing/TestHelper.h>

using namespace ace_button;
using namespace ace_button::testing;

const uint8_t PIN = 13;
const uint8_t BUTTON_ID = 1;

ButtonConfig buttonConfig;
TestableButtonConfig testableConfig;
AdjustableButtonConfig adjustableConfig;;
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

  Serial.print(F("sizeof(AceButton): "));
  Serial.println(sizeof(AceButton));
  Serial.print(F("sizeof(ButtonConfig): "));
  Serial.println(sizeof(ButtonConfig));
  Serial.print(F("sizeof(AdjustableButtonConfig): "));
  Serial.println(sizeof(AdjustableButtonConfig));
  Serial.print(F("sizeof(TestableButtonConfig): "));
  Serial.println(sizeof(TestableButtonConfig));
}

void loop() {
  Test::run();
}

// Test that the pin is properly set and retrieved.
test(pin) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;

  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  assertEqual(PIN, button.getPin());
}

// Test that the custom id is properly set and retrieved.
test(custom_id) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  assertEqual(BUTTON_ID, button.getId());
}

// Test that the getLastButtonPressed() returns BUTTON_STATE_UKNOWN initially.
test(button_state_unknown) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  uint8_t expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());
}

test(feature_flags_off_by_default) {
  assertFalse(buttonConfig.isFeature(ButtonConfig::kFeatureClick));
  assertFalse(buttonConfig.isFeature(ButtonConfig::kFeatureDoubleClick));
  assertFalse(buttonConfig.isFeature(ButtonConfig::kFeatureLongPress));
  assertFalse(buttonConfig.isFeature(ButtonConfig::kFeatureRepeatPress));

  assertFalse(buttonConfig.isFeature(
      ButtonConfig::kFeatureSuppressAfterClick));
  assertFalse(buttonConfig.isFeature(
      ButtonConfig::kFeatureSuppressAfterDoubleClick));
  assertFalse(buttonConfig.isFeature(
      ButtonConfig::kFeatureSuppressAfterLongPress));
  assertFalse(buttonConfig.isFeature(
      ButtonConfig::kFeatureSuppressAfterRepeatPress));
}

// Test that the button transitions out of the kButtonStateUnknown after
// getDebounceDelay() time.
test(init_while_released) {
  uint8_t expected;
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  // button is released when the board is rebooted, should trigger an immediate
  // debouncing
  helper.releaseButton(0);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // button is bouncing pressed/released, but must wait to debounce
  helper.pressButton(40);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // button is bouncing pressed/released, but must wait to debounce
  helper.releaseButton(45);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // finally button is known to be released, this doesn't not trigger event
  helper.releaseButton(60);
  assertEqual(0, eventTracker.getNumEvents());
  assertEqual(HIGH, button.getLastButtonState());
}

// Test that the button transitions out of the kButtonStateUnknown when
// rebooted with the button pressed.
test(init_while_pressed) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);
  uint8_t expected;

  // button is pressed when the board is rebooted, should trigger an immediate
  // debouncing
  helper.pressButton(0);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // button is bouncing pressed/released, but must wait to debounce
  helper.releaseButton(40);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // button is bouncing pressed/released, but must wait to debounce
  helper.pressButton(45);
  assertEqual(0, eventTracker.getNumEvents());
  expected = AceButton::kButtonStateUnknown;
  assertEqual(expected, button.getLastButtonState());

  // finally button is known to be released, this doesn't not trigger event
  helper.pressButton(60);
  assertEqual(0, eventTracker.getNumEvents());
  assertEqual(LOW, button.getLastButtonState());
}

// Test that the TestableButtonConfig overrides the corresponding
// parameters on AceButton properly.
test(testable_config) {
  testableConfig.setClock(0);
  assertEqual(0UL, button.getButtonConfig()->getClock());

  testableConfig.setClock(40);
  assertEqual(40UL, button.getButtonConfig()->getClock());

  testableConfig.setButtonState(HIGH);
  assertEqual(HIGH, button.getButtonConfig()->readButton(0));

  testableConfig.setButtonState(LOW);
  assertEqual(LOW, button.getButtonConfig()->readButton(0));
}

// Test that the AdjustableButtonConfig overrides properly.
test(adjustable_config) {
  adjustableConfig.setDebounceDelay(1);
  assertEqual(1U, adjustableConfig.getDebounceDelay());

  adjustableConfig.setClickDelay(2);
  assertEqual(2U, adjustableConfig.getClickDelay());

  adjustableConfig.setDoubleClickDelay(3);
  assertEqual(3U, adjustableConfig.getDoubleClickDelay());

  adjustableConfig.setLongPressDelay(4);
  assertEqual(4U, adjustableConfig.getLongPressDelay());

  adjustableConfig.setRepeatPressDelay(5);
  assertEqual(5U, adjustableConfig.getRepeatPressDelay());

  adjustableConfig.setRepeatPressInterval(6);
  assertEqual(6U, adjustableConfig.getRepeatPressInterval());
}
