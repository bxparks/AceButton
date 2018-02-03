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

#ifndef TESTABLE_BUTTON_CONFIG_H
#define TESTABLE_BUTTON_CONFIG_H

#include "ButtonConfig.h"

namespace ace_button {
namespace testing {

/**
 * A subclass of ButtonConfig which overrides getClock() and readButton() so
 * that their values can be controlled manually. This is intended to be used for
 * unit testing.
 */
class TestableButtonConfig: public ButtonConfig {
  public:
    TestableButtonConfig():
        mMillis(0),
        mButtonState(HIGH) {}

    /**
     * Initialize to its pristine state. This method is needed because
     * ArduinoUnit does not create a new instance of the Test class for each
     * test case, so we have to reuse objects between test cases, so we need a
     * way to reinitialize this object to its pristine state just after
     * construction.
     */
    virtual void init() override {
      ButtonConfig::init();
      mMillis = 0;
      mButtonState = HIGH;
    }

    /** Read the time of the fake clock. */
    virtual unsigned long getClock() override { return mMillis; }

    /** Read the fake physical button. */
    virtual int readButton(uint8_t pin) override { return mButtonState; }

    /** Set the time of the fake clock. */
    void setClock(unsigned long millis) { mMillis = millis; }

    /** Set the state of the fake physical button. */
    void setButtonState(int buttonState) { mButtonState = buttonState; }

  private:
    unsigned long mMillis;
    int mButtonState;
};

}
}
#endif
