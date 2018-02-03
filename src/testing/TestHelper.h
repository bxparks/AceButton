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

#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <AceButton.h>
#include <testing/TestableButtonConfig.h>
#include <testing/EventTracker.h>

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
        TestableButtonConfig& testableConfig,
        AceButton& button,
        EventTracker& eventTracker):
      testableConfig_(testableConfig),
      button_(button),
      eventTracker_(eventTracker) {}

    /** Reinitiliaze to its pristine state. */
    void init(uint8_t pin, uint8_t defaultReleasedState, uint8_t id) {
      pin_ = pin;
      defaultReleasedState_ = defaultReleasedState;
      id_ = id;
      button_.init(pin_, defaultReleasedState_, id_);
      testableConfig_.init();
      testableConfig_.setButtonState(defaultReleasedState);
    }

    /**
     * Simulate a press of the button and run the button.check() processing. The
     * defaultReleasedState is determined by whether the button has a pullup
     * (HIGH) or pulldown (LOW) resister.
     */
    void pressButton(unsigned long time) {
      uint8_t targetState = (HIGH == defaultReleasedState_) ? LOW : HIGH;
      testableConfig_.setClock(time);
      testableConfig_.setButtonState(targetState);
      eventTracker_.clear();
      button_.check();
    }

    /**
     * Simulate a release of the button and run the button.check() processing.
     */
    void releaseButton(unsigned long time) {
      uint8_t targetState = (HIGH == defaultReleasedState_) ? HIGH : LOW;
      testableConfig_.setClock(time);
      testableConfig_.setButtonState(targetState);
      eventTracker_.clear();
      button_.check();
    }

    /**
     * Simply move the time forward and check the button. No changes to button.
     */
    void checkTime(unsigned long time) {
      testableConfig_.setClock(time);
      eventTracker_.clear();
      button_.check();
    }

    
  private:
    TestableButtonConfig& testableConfig_;
    AceButton& button_;
    EventTracker& eventTracker_;

    uint8_t pin_;
    uint8_t defaultReleasedState_;
    uint8_t id_;
};

}
}
#endif
