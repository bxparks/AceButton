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

#ifndef ADJUSTABLE_BUTTON_CONFIG_H
#define ADJUSTABLE_BUTTON_CONFIG_H

#include "ButtonConfig.h"

namespace ace_button {

/**
 * A subclass of ButtonConfig which allows the user to override the varous
 * timing parameters of ButtonConfig at runtime. Each timing parameter
 * is stored in a member variable, so an instance of AdjustableButtonConfig
 * consumes far more static RAM than ButtonConfig. In return, this class allows
 * these parameters to be changed at runtime by the user.
 */
class AdjustableButtonConfig: public ButtonConfig {
  public:
    AdjustableButtonConfig() {
      initInternal();
    }

    virtual void init() override {
      ButtonConfig::init();
      initInternal();
    }

    virtual uint16_t getDebounceDelay() override {
      return mDebounceDelay;
    }

    virtual uint16_t getClickDelay() override {
      return mClickDelay;
    }

    virtual uint16_t getDoubleClickDelay() override {
      return mDoubleClickDelay;
    }

    virtual uint16_t getLongPressDelay() override {
      return mLongPressDelay;
    }

    virtual uint16_t getRepeatPressDelay() override {
      return mRepeatPressDelay;
    }

    virtual uint16_t getRepeatPressInterval() override {
      return mRepeatPressInterval;
    }

    /** Set the debounceDelay. */
    void setDebounceDelay(uint16_t debounceDelay) {
      mDebounceDelay = debounceDelay;
    }

    /** Set the clickDelay. */
    void setClickDelay(uint16_t clickDelay) {
      mClickDelay = clickDelay;
    }

    /** Set the doubleClickDelay. */
    void setDoubleClickDelay(uint16_t doubleClickDelay) {
      mDoubleClickDelay = doubleClickDelay;
    }

    /** Set the longPressDelay. */
    void setLongPressDelay(uint16_t longPressDelay) {
      mLongPressDelay = longPressDelay;
    }

    /** Set the repeatPressDelay. */
    void setRepeatPressDelay(uint16_t repeatPressDelay) {
      mRepeatPressDelay = repeatPressDelay;
    }

    /** Set the repeatPressInterval. */
    void setRepeatPressInterval(uint16_t repeatPressInterval) {
      mRepeatPressInterval = repeatPressInterval;
    }

  private:
    // This method must remain non-virtual so that it can be called safely from
    // the constructor.
    void initInternal() {
      mDebounceDelay = kDebounceDelay;
      mClickDelay = kClickDelay;
      mDoubleClickDelay = kDoubleClickDelay;
      mLongPressDelay = kLongPressDelay;
      mRepeatPressDelay = kRepeatPressDelay;
      mRepeatPressInterval = kRepeatPressInterval;
    }

    uint16_t mDebounceDelay;
    uint16_t mClickDelay;
    uint16_t mDoubleClickDelay;
    uint16_t mLongPressDelay;
    uint16_t mRepeatPressDelay;
    uint16_t mRepeatPressInterval;
};

}
#endif
