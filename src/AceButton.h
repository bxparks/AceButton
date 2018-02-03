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

#ifndef DEBOUNCED_BUTTON_H
#define DEBOUNCED_BUTTON_H

#include <Arduino.h>
#include "ButtonConfig.h"

namespace ace_button {

/**
 * An Adjustable Compact Event-driven (ACE) Button library that debounces and
 * dispatches button events to a user-defined event handler. Supported events
 * types are:
 *
 * - kEventPressed
 * - kEventReleased
 * - kEventClicked
 * - kEventDoubleClicked
 * - kEventLongPressed
 * - kEventRepeatPressed
 *
 * The check() method should be called from the loop() at least 2-3 times during
 * the debouncing time period. For 50 ms delay, the check() method should be
 * called at a minimum of every 15-20 ms. The execution time of check() on a 16
 * MHz Arduino ATmega328P MCU seems to about about 15 microseconds.
 */
class AceButton {
  public:
    // The supported event types.

    /** Button was pressed. */
    static const uint8_t kEventPressed = 0;

    /** Button was released. */
    static const uint8_t kEventReleased = 1;

    /**
     * Button was clicked (Pressed and Released within
     * ButtonConfig::getClickDelay()).
     */
    static const uint8_t kEventClicked = 2;

    /**
     * Button was double-clicked. (Two clicks within
     * ButtonConfig::getDoubleClickDelay()).
     */
    static const uint8_t kEventDoubleClicked = 3;

    /**
     * Button was held down for longer than
     * ButtonConfig::getLongPressDelay()).
     */
    static const uint8_t kEventLongPressed = 4;

    /**
     * Button was held down and auto generated multiple presses. The first event
     * is triggered after ButtonConfig::getRepeatPressDelay(), then the event
     * fires repeatedly every ButtonConfig::getRepeatPressInterval() until the
     * button is released.
     */
    static const uint8_t kEventRepeatPressed = 5;

    /**
     * Button state is unknown. This is a third state (different from LOW or
     * HIGH) used when the class is first initialized upon reboot.
     */
    static const uint8_t kButtonStateUnknown = 2;

    /**
     * Constructor defines parameters of the button that changes from button to
     * button. These parameters don't change during the runtime of the program.
     * Another way to initialize the object is to create an instance using
     * an empty constructor, then use the init() method to initialize the
     * object with these parameters.
     *
     * Using the constructor often reads better for simple situations where only
     * a single button is used, and it doesn't need to be configured
     * significantly. Using the init() method can make the code be more readable
     * when multiple buttons are used, and they need to be significantly
     * customized. The init() method allows the button configuration code to
     * appear in close proximity to the pinMode() methods which sets up the
     * hardware pins.
     *
     * @param pin The pin number of the button. Default 0. Normally the pin
     * number should be given at construction time. However, the pin number
     * the pin number can be omitted so that the pin number can be assigned at
     * setup() time using the setPin() method.
     *
     * @param defaultReleasedState The pin state when the button is in the
     * initial released position. Default HIGH. When using a pullup resister
     * (either external or internal) and the button is connected to ground, this
     * should be set to HIGH. When using an external pulldown resister and the
     * button is connected to Vcc (5V or 3.3V), this should be set to LOW. The
     * defaultReleasedState can be assigned using the constructor or the
     * init() method.
     *
     * @param id This is an optional user-defined identifier for the
     * button. For example, this could be an index into an array of data that is
     * associated with the button.
     */
    explicit AceButton(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH,
        uint8_t id = 0);

    /**
     * Reset the button to the initial constructed state. In particular,
     * getLastButtonState() returns kButtonStateUnknown. The parameters are
     * identical as the parameters in the AceButton() constructor.
     */
    void init(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH,
        uint8_t id = 0);

    /** Get the ButtonConfig associated with this Button. */
    ButtonConfig* getButtonConfig() ACE_BUTTON_INLINE {
      return buttonConfig_;
    }

    /** Set the ButtonConfig associated with this Button. */
    void setButtonConfig(ButtonConfig* buttonConfig) ACE_BUTTON_INLINE {
      buttonConfig_ = buttonConfig;
    }

    /**
     * Convenience method to set the event handler. Event handlers are stored in
     * the ButtonConfig object, not in the AceButton object, to save
     * memory. (Multiple buttons are likely to share the same event handler.) So
     * this method is just a pass-through to ButtonConfig::setEventHandler(). If
     * you are using multiple ButtonConfig objects, you should call the
     * ButtonConfig::setEventHandler() method on those objects directly, instead
     * of using this method.
     */
    void setEventHandler(ButtonConfig::EventHandler eventHandler)
        ACE_BUTTON_INLINE {
      buttonConfig_->setEventHandler(eventHandler);
    }

    /** Get the button's pin number. */
    uint8_t getPin() ACE_BUTTON_INLINE { return pin_; }

    /** Get the custom identifier of the button. */
    uint8_t getId() ACE_BUTTON_INLINE { return id_; }

    /** Get the initial released state of the button, HIGH or LOW. */
    uint8_t getDefaultReleasedState();

    /**
     * Return the button state that was last valid. This is a tri-state
     * function. It may return HIGH, LOW or kButtonStateUnknown to indicate that
     * the last state is not known. This method is **not** for public
     * consumption, it is exposed only for testing purposes. Consider it to be a
     * private method. Use the buttonState parameter provided to the
     * EventHandler.
     *
     * In a more general multi-threaded environment (which the Arduino is not,
     * fortunately or unfortunately), the getLastButtonState() may have changed
     * from the value of buttonState provided to the event handler. In other
     * words, there is a race-condition.
     */
    uint8_t getLastButtonState() ACE_BUTTON_INLINE {
      return lastButtonState_;
    }

    /**
     * Check state of button and trigger event processing. This method should be
     * called from the loop() method in Arduino every 2-3 times during the
     * getDebounceDelay() time (default 50 ms), so about every 10-20 ms.
     *
     * The class will still work if called less of than that because the
     * debouncing algorithm may not work correctly.
     */
    void check();

    /**
     * Returns true if the given buttonState represents a 'Released' state for
     * the button. Returns false if the buttonState is 'Pressed' or
     * kButtonStateUnknown.
     *
     * The HIGH or LOW logical value of buttonState represents different a
     * button position depending on whether the button is wired with a pull-up
     * or a pull-down resister. This method translates the logical level to the
     * physical position which allows the client code to be independent of the
     * physical wiring.
     *
     * Normally, the eventType given to the EventHandler should be sufficient
     * because the value of the evenType already encodes this information.
     * This method is provided just in case.
     */
    bool isReleased(uint8_t buttonState) ACE_BUTTON_INLINE {
      return buttonState == getDefaultReleasedState();
    }

  // Some of these protected methods may be useful to the calling client but I
  // don't want to release them to the public because I want to keep the API as
  // small as possible for easier long term maintenance. (Once a method is
  // released to the public, it must be supported forever to ensure backwards
  // compatibility with older client code.)

  private:
    /** Set the pin number of the button. */
    void setPin(uint8_t pin) ACE_BUTTON_INLINE { pin_ = pin; }

    /**
     * Set the initial released state of the button.
     *
     * @param state If a pull up resister is used, this should be HIGH. If a
     * pull down resister is used, this should be LOW. The behavior is undefined
     * for any other values of 'state'.
     */
    void setDefaultReleasedState(uint8_t state);

    /** Set the identifier of the button. */
    void setId(uint8_t id) ACE_BUTTON_INLINE { id_ = id; }

    // Various bit masks to store a boolean flag in the 'flags_' field.
    // We use bit masks to save static RAM. If we had used a 'bool' type, each
    // of these would consume one byte.
    static const uint8_t kFlagDefaultReleasedState = 0x01;
    static const uint8_t kFlagDebouncing = 0x02;
    static const uint8_t kFlagPressed = 0x04;
    static const uint8_t kFlagClicked = 0x08;
    static const uint8_t kFlagDoubleClicked = 0x10;
    static const uint8_t kFlagLongPressed = 0x20;
    static const uint8_t kFlagRepeatPressed = 0x40;

    // Methods for accessing the button's internal states.
    // I don't expect these to be useful to the outside world.

    // If this is set, then lastDebounceTime_ is valid.
    bool isDebouncing() ACE_BUTTON_INLINE {
      return flags_ & kFlagDebouncing;
    }

    void setDebouncing() ACE_BUTTON_INLINE {
      flags_ |= kFlagDebouncing;
    }

    void clearDebouncing() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagDebouncing;
    }

    // If this is set, then lastPressTime_ is valid.
    bool isPressed() ACE_BUTTON_INLINE {
      return flags_ & kFlagPressed;
    }

    void setPressed() ACE_BUTTON_INLINE {
      flags_ |= kFlagPressed;
    }

    void clearPressed() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagPressed;
    }

    // If this is set, then lastClickTime_ is valid.
    bool isClicked() ACE_BUTTON_INLINE {
      return flags_ & kFlagClicked;
    }

    void setClicked() ACE_BUTTON_INLINE {
      flags_ |= kFlagClicked;
    }

    void clearClicked() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagClicked;
    }

    // A double click was detected. No need to store the last double-clicked
    // time because we don't support a triple-click event (yet).
    bool isDoubleClicked() ACE_BUTTON_INLINE {
      return flags_ & kFlagDoubleClicked;
    }

    void setDoubleClicked() ACE_BUTTON_INLINE {
      flags_ |= kFlagDoubleClicked;
    }

    void clearDoubleClicked() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagDoubleClicked;
    }

    // If this is set, then lastPressTime_ can be treated as the start
    // of a long press.
    bool isLongPressed() ACE_BUTTON_INLINE {
      return flags_ & kFlagLongPressed;
    }

    void setLongPressed() ACE_BUTTON_INLINE {
      flags_ |= kFlagLongPressed;
    }

    void clearLongPressed() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagLongPressed;
    }

    // If this is set, then lastRepeatPressTime_ is valid.
    bool isRepeatPressed() ACE_BUTTON_INLINE {
      return flags_ & kFlagRepeatPressed;
    }

    void setRepeatPressed() ACE_BUTTON_INLINE {
      flags_ |= kFlagRepeatPressed;
    }

    void clearRepeatPressed() ACE_BUTTON_INLINE {
      flags_ &= ~kFlagRepeatPressed;
    }

    /**
     * Return true if debouncing succeeded and the buttonState value can be
     * used. Return false if buttonState should be ignored until debouncing
     * phase is complete.
     */
    bool checkDebounced(uint16_t now, uint8_t buttonState);

    /**
     * Return true if the button was already initialzed and determined to be in
     * a HIGH or LOW state. Return false if the button was previously in
     * kButtonStateUnknown state which implies that the event handler should
     * NOT be fired.
     */
    bool checkInitialized(uint16_t now, uint16_t buttonState);

    /** Check for a long press event and dispatch to event handler. */
    void checkLongPress(uint16_t now, uint8_t buttonState);

    /** Check for a repeat press event and dispatch to event handler. */
    void checkRepeatPress(uint16_t now, uint8_t buttonState);

    /** Check for onChange event and check for Press or Release events. */
    void checkChanged(uint16_t now, uint8_t buttonState);

    /**
     * Check for Released and Click events and dispatch to respective
     * handlers.
     */
    void checkReleased(uint16_t now, uint8_t buttonState);

    /** Check for Pressed event and dispatch to handler. */
    void checkPressed(uint16_t now, uint8_t buttonState);

    /** Check for a single click event and dispatch to handler. */
    void checkClicked(uint16_t now, uint8_t buttonState);

    /**
     * Check for a double click event and dispatch to handler. Return true if
     * double click detected.
     */
    void checkDoubleClicked(uint16_t now, uint8_t buttonState);

    /**
     * Check for an orphaned click that did not generate a double click and
     * clean up internal state. If we don't do this, the second click may be
     * generated after the uint16_t rolls over in 65.5 seconds, causing an
     * unwanted double-click. Even if we used the full 'unsigned long' to store
     * the 'lastClickTime', we'd still need this function to prevent a rollover
     * of the 32-bit number in 49.7 days.
     */
    void checkOrphanedClick(uint16_t now, uint8_t buttonState);

    /**
     * Dispatch to the event handler defined in the buttonConfig_.
     *
     * This method will always be called and it's up to the user-provided
     * handler to ignore the events which aren't interesting.
     *
     * An alternative might be to provide a bitmask filter to select only
     * events which should are registered to trigger the event handler. For
     * example, add the following method:
     *
     * @code
     *    setEventSelection(uint8_t eventSelection) {
     *      eventSelection_ = eventSelection;
     *    }
     * @endcode
     *
     * Set the event selector at setup():
     *
     * @code
     *    setEventSelection(kEventSelectPressed | kEventSelectReleased);
     * @endcode
     *
     * where
     *
     * @code
     *  kEventSelectPressed = (0x1 << kEventPressed);
     *  kEventSelectReleased = (0x1 << kEventReleased);
     *  ...
     * @endcode
     *
     * Then change the handleEvent() method to something like:
     *
     * @code
     * void handleEvent(uint8_t eventType) {
     *    if (eventHandler_ && (eventSelections_ & (0x1 << eventType))) {
     *      handleEvent(this, eventType);
     *    }
     * }
     * @endcode
     *
     * But it is possible that the evaluation of the if-condition above takes
     * longer to evaluate than an empty function call, so we should do some
     * profiling before making this change.
     *
     * @param eventType the type of event given by the kEvent* constants
     */
    void handleEvent(uint8_t eventType);

    uint8_t pin_; // button pin number
    uint8_t id_; // identifier, e.g. an index into an array

    // Internal states of the button debouncing and event handling.
    // NOTE: We don't keep track of the lastDoubleClickTime, because we
    // don't support a TripleClicked event. That may change in the future.
    uint16_t lastDebounceTime_; // ms
    uint16_t lastClickTime_; // ms
    uint16_t lastPressTime_; // ms
    uint16_t lastRepeatPressTime_; // ms

    /** Internal flags. Bit masks are defined by the kFlag* constants. */
    uint8_t flags_;

    /**
     * Last button state. This is a tri-state variable: LOW, HIGH or
     * kButtonStateUnknown.
     */
    uint8_t lastButtonState_;

    /** ButtonConfig associated with this button. */
    ButtonConfig* buttonConfig_;
};

}
#endif
