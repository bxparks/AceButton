/*
MIT License

Copyright (c) 2018 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACE_BUTTON_ADJUSTABLE_BUTTON_CONFIG_H
#define ACE_BUTTON_ADJUSTABLE_BUTTON_CONFIG_H

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
    // Disable copy-constructor and assignment operator
    AdjustableButtonConfig(const AdjustableButtonConfig&) = delete;
    AdjustableButtonConfig& operator=(const AdjustableButtonConfig&) = delete;

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
