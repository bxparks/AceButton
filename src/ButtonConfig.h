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

#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

#include <Arduino.h>

// TODO: Verify if this is actually needed. The program size seems to be
// identical with or without it on the Arduino IDE (which uses gcc).
#define ACE_BUTTON_INLINE __attribute__((always_inline))

namespace ace_button {

// forward declare the AceButton
class AceButton;

/**
 * Class that defines the timing parameters and event handler of an AceButton or
 * a group of AceButton instances. It is assumed that in many cases, a group of
 * multiple buttons will need to be assigned the same configuration parameters.
 * For example, various timing delays and the EventHandler. Instead of storing
 * these parameters in each instance of AceButton (which consumes static
 * memory), we save space by collecting them into a separate ButtonConfig class.
 * Each AceButton instance contains a pointer to an instance of ButtonConfig,
 * and an instance of ButtonConfig will be shared among multiple AceButtons.
 *
 * Most of the parameters are actually hardwired into the various virtual
 * methods below. This has the advantage of reducing memory consumption of even
 * this class. If a parameter needs to be changed, there are 2 options:
 *
 * 1. Use the AdjustableButtonConfig subclass which allows all of these
 *    parameters to be changed at runtime, at the cost of additional static
      memory usage to hold those parameters.
 * 2. Subclass this ButtonConfig class, then override only the specific
 *    method to modify the specific timing parameter. This has the advantage
 *    of consuming no additional static RAM, at the expense of creating
 *    another class.
 *
 * A single default "System" ButtonConfig instance is created automatically and
 * can be accessed using the ButtConfig::getSystemButtonConfig() static method.
 * For convenience and ease of use, every instance of AceButton is attached to
 * this "System" ButtonConfig by default. The client code can override this
 * association by attaching another ButtonConfig instance using the
 * AceButton::setButtonConfig() method.
 */
class ButtonConfig {
  public:
    // Various timing constants, in milliseconds.
    //
    // Note that the timing constants are stored as uint16_t (2
    // bytes) instead of unsigned long (4 bytes) which is the type returned by
    // the millis() system method. It turns out that we can store and perform
    // all timing calculations using uint16_t without ill effect, as long as the
    // polling of AceButton::check() happens more frequently than the rollover
    // time of a uint16_t (i.e. 65.536 seconds) and certain precautions (e.g.
    // AceButton::checkOrphanedClick()) are taken before a uint16_t rollover
    // happens. In theory, these additional precautions would be needed even if
    // an 'unsigned long' is used but almost no one does them because they
    // assume that their code won't be running continuously for the rollover
    // time of an 'unsigned long' (i.e. 49.7 days).

    /** Default value returned by getDebounceDelay(). */
    static const uint16_t kDebounceDelay = 50;

    /** Default value returned by getClickDelay(). */
    static const uint16_t kClickDelay = 200;

    /** Default value returned by getDoubleClickDelay(). */
    static const uint16_t kDoubleClickDelay = 400;

    /** Default value returned by getLongPressDelay(). */
    static const uint16_t kLongPressDelay = 1000;

    /** Default value returned by getRepeatPressDelay(). */
    static const uint16_t kRepeatPressDelay = 1000;

    /** Default value returned by getRepeatPressInterval(). */
    static const uint16_t kRepeatPressInterval = 200;

    // Various features controlled by feature flags.

    /** Flag to activate the AceButton::kEventClicked event. */
    static const uint8_t kFeatureClick = 0x01;

    /**
     * Flag to activate the AceButton::kEventDoubleClicked event.
     * Activating this automatically activates kEventClicked since there is
     * no double-click without a click.
     */
    static const uint8_t kFeatureDoubleClick = 0x02;

    /** Flag to activate the AceButton::kEventLongPress event. */
    static const uint8_t kFeatureLongPress = 0x04;

    /** Flag to activate the AceButton::kEventRepeatPressed event. */
    static const uint8_t kFeatureRepeatPress = 0x08;

    /** Flag to suppress kEventReleased after a kEventClicked. */
    static const uint8_t kFeatureSuppressAfterClick = 0x10;

    /**
     * Flag to suppress kEventReleased after a kEventDoubleClicked. A
     * kEventClicked is _always_ suppressed after a kEventDoubleClicked to
     * prevent generating 2 double-clicks if the user performed a triple-click.
     */
    static const uint8_t kFeatureSuppressAfterDoubleClick = 0x20;

    /** Flag to suppress kEventReleased after a kEventLongPressed. */
    static const uint8_t kFeatureSuppressAfterLongPress = 0x40;

    /** Flag to suppress kEventReleased after a kEventRepeatPressed. */
    static const uint8_t kFeatureSuppressAfterRepeatPress = 0x80;

    /**
     * Convenience flag to suppress all suppressions.
     * Calling setFeature(kFeatureSuppressAll) suppresses all lower-level
     * events, and clearFeature(kFeatureSuppressAll) clears all suppressesion.
     * Note however that isFeature(kFeatureSuppressAll) currently means "is ANY
     * feature enabled?" not "are ALL features enabled?".
     */
    static const uint8_t kFeatureSuppressAll =
        (kFeatureSuppressAfterClick |
        kFeatureSuppressAfterDoubleClick |
        kFeatureSuppressAfterLongPress |
        kFeatureSuppressAfterRepeatPress);

    /**
     * The event handler signature.
     *
     * @param button pointer to the AceButton that generated the event
     * @param eventType the event type which trigger the call
     * @param buttonState the state of the button that triggered the event
     */
    typedef void (*EventHandler)(AceButton* button, uint8_t eventType,
        uint8_t buttonState);

    /** Constructor. */
    ButtonConfig();

    // These configuration methods are virtual so that they can be overriddden.
    // Subclasses can override at the class-level by defining a new virtual
    // function in the subclass, or by defining an instance variable and storing
    // the parameter with each instance of this class.

    /** Milliseconds to wait for debouncing. */
    virtual uint16_t getDebounceDelay() { return kDebounceDelay; }

    /** Milliseconds to wait for a possible click. */
    virtual uint16_t getClickDelay() { return kClickDelay; }

    /**
     * Milliseconds between the first and second click to register as a
     * double-click.
     */
    virtual uint16_t getDoubleClickDelay() {
      return kDoubleClickDelay;
    }

    /** Milliseconds for a long press event. */
    virtual uint16_t getLongPressDelay() {
      return kLongPressDelay;
    }

    /**
     * Milliseconds that a button needs to be Pressed down before the start of
     * the sequence of RepeatPressed events. The first event will fire as soon
     * as this delay has passed. Subsequent events will fire after
     * getRepeatPressInterval() time.
     */
    virtual uint16_t getRepeatPressDelay() {
      return kRepeatPressDelay;
    }

    /**
     * Milliseconds between two successive RepeatPressed events.
     */
    virtual uint16_t getRepeatPressInterval() {
      return kRepeatPressInterval;
    }

    // The getClock() and readButton() are external dependencies that normally
    // would be injected using separate classes, but in the interest of saving
    // RAM in an embedded environment, we expose them in this class instead.

    /**
     * Return the milliseconds of the internal clock. Override to use something
     * other than millis(). The return type is 'unsigned long' instead of
     * uint16_t because that's the return type of millis().
     */
    virtual unsigned long getClock() { return millis(); }

    /**
     * Return the HIGH or LOW state of the button. Override to use something
     * other than digitalRead(). The return type is 'int' instead of uint16_t
     * because that's the return type of digitalRead().
     */
    virtual int readButton(uint8_t pin) {
      return digitalRead(pin);
    }

    // These methods return the various feature flags that control the
    // functionality of the AceButton.

    /** Check if the given features are enabled. */
    bool isFeature(uint8_t features) ACE_BUTTON_INLINE {
      return mFeatureFlags & features;
    }

    /** Enable the given features. */
    void setFeature(uint8_t features) ACE_BUTTON_INLINE {
      mFeatureFlags |= features;
    }

    /** Disable the given features. */
    void clearFeature(uint8_t features) ACE_BUTTON_INLINE {
      mFeatureFlags &= ~features;
    }

    /** Return the eventHandler. */
    EventHandler getEventHandler() ACE_BUTTON_INLINE {
      return mEventHandler;
    }

    /**
     * Install the event handler. The event handler must be defined for the
     * AceButton to be useful.
     */
    void setEventHandler(EventHandler eventHandler) ACE_BUTTON_INLINE {
      mEventHandler = eventHandler;
    }

    /**
     * Return a pointer to the singleton instance of the ButtonConfig
     * which is attached to all AceButton instances by default.
     */
    static ButtonConfig* getSystemButtonConfig() ACE_BUTTON_INLINE {
      return &sSystemButtonConfig;
    }

  protected:
    /**
     * Initialize to its pristine state, except for the EventHandler which is
     * unchanged. This is intended mostly for testing purposes.
     */
    virtual void init() { mFeatureFlags = 0; }

  private:
    /** The event handler for all buttons associated with this ButtonConfig. */
    EventHandler mEventHandler;

    /** A bit mask flag that activates certain features. */
    uint8_t mFeatureFlags;

    /**
     * A single static instance of ButtonConfig provided by default to all
     * AceButton instances.
     */
    static ButtonConfig sSystemButtonConfig;
};

}
#endif
