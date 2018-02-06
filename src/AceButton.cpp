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

#include "AceButton.h"

using namespace ace_button;

// Check that the Arduino constants HIGH and LOW are defined to be 1 and 0,
// respectively. Otherwise, this library won't work.
#if HIGH != 1
  #error HIGH must be defined to be 1
#endif
#if LOW != 0
  #error LOW must be defined to be 0
#endif

AceButton::AceButton(uint8_t pin, uint8_t defaultReleasedState, uint8_t id):
    mButtonConfig(ButtonConfig::getSystemButtonConfig()) {
  init(pin, defaultReleasedState, id);
}

void AceButton::init(uint8_t pin, uint8_t defaultReleasedState, uint8_t id) {
  mPin = pin;
  mId = id;
  mFlags = 0;
  mLastButtonState = kButtonStateUnknown;
  mLastDebounceTime = 0;
  mLastClickTime = 0;
  setDefaultReleasedState(defaultReleasedState);
}

void AceButton::setDefaultReleasedState(uint8_t state) {
  if (state == HIGH) {
    mFlags |= kFlagDefaultReleasedState;
  } else {
    mFlags &= ~kFlagDefaultReleasedState;
  }
}

uint8_t AceButton::getDefaultReleasedState() {
  return (mFlags & kFlagDefaultReleasedState) ? HIGH : LOW;
}

// NOTE: It would be interesting to rewrite the check() method using a Finite
// State Machine.
void AceButton::check() {
  // Retrieve the current time just once and use that in the various checkXxx()
  // functions below. This provides some robustness of the various timing
  // algorithms even if any of the event handlers takes more time than the
  // threshold time limits such as 'debounceDelay' or longPressDelay'.
  uint16_t now = mButtonConfig->getClock();

  uint8_t buttonState = mButtonConfig->readButton(mPin);

  // debounce the button and return if not debounced
  if (!checkDebounced(now, buttonState)) return;

  // check if the button was not initialized (i.e. UNKNOWN state)
  if (!checkInitialized(now, buttonState)) return;

  checkOrphanedClick(now, buttonState);
  if (mButtonConfig->isFeature(ButtonConfig::kFeatureLongPress)) {
    checkLongPress(now, buttonState);
  }
  if (mButtonConfig->isFeature(ButtonConfig::kFeatureRepeatPress)) {
    checkRepeatPress(now, buttonState);
  }
  checkChanged(now, buttonState);
}

bool AceButton::checkDebounced(uint16_t now, uint8_t buttonState) {
  if (isDebouncing()) {
    // currently in debouncing phase
    bool isDebouncingTimeOver =
        (now - mLastDebounceTime >= mButtonConfig->getDebounceDelay());
    if (isDebouncingTimeOver) {
      clearDebouncing();
      return true;
    } else {
      return false;
    }
  } else {
    // Currently not in debouncing phase. Check for a button state change. This
    // will also detect a transition from kButtonStateUnknown to HIGH or LOW.
    if (buttonState == getLastButtonState()) {
      // no change, return immediately
      return true;
    }

    // button has changed so, enter debouncing phase
    setDebouncing();
    mLastDebounceTime = now;
    return false;
  }
}

bool AceButton::checkInitialized(uint16_t now, uint16_t buttonState) {
  if (mLastButtonState != kButtonStateUnknown) {
    return true;
  }

  // If transitioning from the initial "unknown" button state, just set the last
  // valid button state, but don't fire off the event handler. This handles the
  // case where a momentary switch is pressed down, then the board is rebooted.
  // When the board comes up, it should not fire off the event handler. This
  // also handles the case of a 2-position switch set to the "pressed"
  // position, and the board is rebooted.
  mLastButtonState = buttonState;
  return false;
}

void AceButton::checkLongPress(uint16_t now, uint8_t buttonState) {
  if (buttonState == getDefaultReleasedState()) {
    return;
  }

  if (isPressed() && !isLongPressed()) {
    if (now - mLastPressTime >= mButtonConfig->getLongPressDelay()) {
      setLongPressed();
      handleEvent(kEventLongPressed);
    }
  }
}

void AceButton::checkRepeatPress(uint16_t now, uint8_t buttonState) {
  if (buttonState == getDefaultReleasedState()) {
    return;
  }

  if (isPressed()) {
    if (isRepeatPressed()) {
      if (now - mLastRepeatPressTime
          >= mButtonConfig->getRepeatPressInterval()) {
        handleEvent(kEventRepeatPressed);
        mLastRepeatPressTime = now;
      }
    } else {
      if (now - mLastPressTime >= mButtonConfig->getRepeatPressDelay()) {
        setRepeatPressed();
        // Trigger the RepeatPressed immedidately, instead of waiting until the
        // first getRepeatPressInterval() has passed.
        handleEvent(kEventRepeatPressed);
        mLastRepeatPressTime = now;
      }
    }
  }
}

void AceButton::checkChanged(uint16_t now, uint8_t buttonState) {
  if (buttonState == getLastButtonState()) {
    return;
  }

  mLastButtonState = buttonState;
  checkPressed(now, buttonState);
  checkReleased(now, buttonState);
}

void AceButton::checkPressed(uint16_t now, uint8_t buttonState) {
  if (buttonState == getDefaultReleasedState()) {
    return;
  }

  // button was pressed
  mLastPressTime = now;
  setPressed();
  handleEvent(kEventPressed);
}

void AceButton::checkReleased(uint16_t now, uint8_t buttonState) {
  if (buttonState != getDefaultReleasedState()) {
    return;
  }

  // check for click (before sending off the Released event)
  // Make sure that we don't clearPressed() before calling this.
  if (mButtonConfig->isFeature(ButtonConfig::kFeatureClick)
      || mButtonConfig->isFeature(ButtonConfig::kFeatureDoubleClick)) {
    checkClicked(now, buttonState);
  }

  // check if Released events are suppressed
  bool suppress =
      ((isLongPressed() &&
          mButtonConfig->
              isFeature(ButtonConfig::kFeatureSuppressAfterLongPress)) ||
      (isRepeatPressed() &&
          mButtonConfig->
              isFeature(ButtonConfig::kFeatureSuppressAfterRepeatPress)) ||
      (isClicked() &&
          mButtonConfig->isFeature(ButtonConfig::kFeatureSuppressAfterClick)) ||
      (isDoubleClicked() &&
          mButtonConfig->
              isFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick)));

  // button was released
  clearPressed();
  clearDoubleClicked();
  clearLongPressed();
  clearRepeatPressed();

  if (!suppress) {
    handleEvent(kEventReleased);
  }
}

void AceButton::checkClicked(uint16_t now, uint8_t buttonState) {
  if (!isPressed()) {
    // Not a Click unless the previous state was a Pressed state.
    // This can happen if the chip was rebooted with the button Pressed. Upon
    // Release, it shouldn't generated a click, even accidentally due to a
    // spurious value in mLastPressTime.
    clearClicked();
    return;
  }
  if (now - mLastPressTime >= mButtonConfig->getClickDelay()) {
    clearClicked();
    return;
  }

  // check for double click
  if (mButtonConfig->isFeature(ButtonConfig::kFeatureDoubleClick)) {
    checkDoubleClicked(now, buttonState);
  }

  // suppress a second click (both buttonState change and event message) if
  // double-click detected, which has the side-effect of preventing 3 clicks
  // from generating another double-click at the third click.
  if (isDoubleClicked()) {
    clearClicked();
    return;
  }

  // we got a single click
  mLastClickTime = now;
  setClicked();
  handleEvent(kEventClicked);
}

void AceButton::checkDoubleClicked(uint16_t now, uint8_t buttonState) {
  if (!isClicked()) {
    clearDoubleClicked();
    return;
  }

  if (now - mLastClickTime >= mButtonConfig->getDoubleClickDelay()) {
    clearDoubleClicked();
    return;
  }

  setDoubleClicked();
  handleEvent(kEventDoubleClicked);
}

void AceButton::checkOrphanedClick(uint16_t now, uint8_t buttonState) {
  // The amount of time which must pass before a click is determined to be
  // orphaned and reclaimed. If only DoubleClicked is supported, then I think
  // just getDoubleClickDelay() is correct. No other higher level event uses the
  // first Clicked event. If TripleClicked becomes supported, I think
  // orphanedClickDelay will be either (2 * getDoubleClickDelay()) or
  // (getDoubleClickDelay() + getTripleClickDelay()), depending on whether the
  // TripleClick has an independent delay time, or reuses the DoubleClick delay
  // time. But I'm not sure that I've thought through all the details.
  uint16_t orphanedClickDelay = mButtonConfig->getDoubleClickDelay();

  if (isClicked() && (now - mLastClickTime >= orphanedClickDelay)) {
    clearClicked();
  }
}

void AceButton::handleEvent(uint8_t eventType) {
  ButtonConfig::EventHandler eventHandler = mButtonConfig->getEventHandler();
  if (eventHandler) {
    eventHandler(this, eventType, getLastButtonState());
  }
}
