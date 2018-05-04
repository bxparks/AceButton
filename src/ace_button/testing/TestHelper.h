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

#ifndef ACE_BUTTON_TEST_HELPER_H
#define ACE_BUTTON_TEST_HELPER_H

#include <AceButton.h>
#include "TestableButtonConfig.h"
#include "EventTracker.h"

namespace ace_button {
namespace testing {

/**
 * A wrapper class that sends emulated button presses and released to the the
 * underlying AceButton class, and captures the resulting events in the
 * provided EventTracker.
 */
class TestHelper {
  public:
    TestHelper(
        TestableButtonConfig* testableConfig,
        AceButton* button,
        EventTracker* eventTracker):
      mTestableConfig(testableConfig),
      mButton(button),
      mEventTracker(eventTracker) {}

    /** Reinitilize to its pristine state. */
    void init(uint8_t pin, uint8_t defaultReleasedState, uint8_t id) {
      mPin = pin;
      mDefaultReleasedState = defaultReleasedState;
      mId = id;
      mButton->init(mPin, mDefaultReleasedState, mId);
      mTestableConfig->init();
      mTestableConfig->setButtonState(defaultReleasedState);
    }

    /**
     * Simulate a press of the button and run the button.check() processing. The
     * defaultReleasedState is determined by whether the button has a pullup
     * (HIGH) or pulldown (LOW) resistor.
     */
    void pressButton(unsigned long time) {
      uint8_t targetState = (HIGH == mDefaultReleasedState) ? LOW : HIGH;
      mTestableConfig->setClock(time);
      mTestableConfig->setButtonState(targetState);
      mEventTracker->clear();
      mButton->check();
    }

    /**
     * Simulate a release of the button and run the button.check() processing.
     */
    void releaseButton(unsigned long time) {
      uint8_t targetState = (HIGH == mDefaultReleasedState) ? HIGH : LOW;
      mTestableConfig->setClock(time);
      mTestableConfig->setButtonState(targetState);
      mEventTracker->clear();
      mButton->check();
    }

    /**
     * Simply move the time forward and check the button. No changes to button.
     */
    void checkTime(unsigned long time) {
      mTestableConfig->setClock(time);
      mEventTracker->clear();
      mButton->check();
    }

  private:
    // Disable copy-constructor and assignment operator
    TestHelper(const TestHelper&) = delete;
    TestHelper& operator=(const TestHelper&) = delete;

    TestableButtonConfig* mTestableConfig;
    AceButton* mButton;
    EventTracker* mEventTracker;

    uint8_t mPin;
    uint8_t mDefaultReleasedState;
    uint8_t mId;
};

}
}
#endif
