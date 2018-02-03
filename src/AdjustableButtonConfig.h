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
      return debounceDelay_;
    }

    virtual uint16_t getClickDelay() override {
      return clickDelay_;
    }

    virtual uint16_t getDoubleClickDelay() override {
      return doubleClickDelay_;
    }

    virtual uint16_t getLongPressDelay() override {
      return longPressDelay_;
    }

    virtual uint16_t getRepeatPressDelay() override {
      return repeatPressDelay_;
    }

    virtual uint16_t getRepeatPressInterval() override {
      return repeatPressInterval_;
    }

    /** Set the debounceDelay. */
    void setDebounceDelay(uint16_t debounceDelay) {
      debounceDelay_ = debounceDelay;
    }

    /** Set the clickDelay. */
    void setClickDelay(uint16_t clickDelay) {
      clickDelay_ = clickDelay;
    }

    /** Set the doubleClickDelay. */
    void setDoubleClickDelay(uint16_t doubleClickDelay) {
      doubleClickDelay_ = doubleClickDelay;
    }

    /** Set the longPressDelay. */
    void setLongPressDelay(uint16_t longPressDelay) {
      longPressDelay_ = longPressDelay;
    }

    /** Set the repeatPressDelay. */
    void setRepeatPressDelay(uint16_t repeatPressDelay) {
      repeatPressDelay_ = repeatPressDelay;
    }

    /** Set the repeatPressInterval. */
    void setRepeatPressInterval(uint16_t repeatPressInterval) {
      repeatPressInterval_ = repeatPressInterval;
    }

  private:
    // This method must remain non-virtual so that it can be called safely from
    // the constructor.
    void initInternal() {
      debounceDelay_ = kDebounceDelay;
      clickDelay_ = kClickDelay;
      doubleClickDelay_ = kDoubleClickDelay;
      longPressDelay_ = kLongPressDelay;
      repeatPressDelay_ = kRepeatPressDelay;
      repeatPressInterval_ = kRepeatPressInterval;
    }

    uint16_t debounceDelay_;
    uint16_t clickDelay_;
    uint16_t doubleClickDelay_;
    uint16_t longPressDelay_;
    uint16_t repeatPressDelay_;
    uint16_t repeatPressInterval_;
};

}
#endif
