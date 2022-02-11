/*
 * A demo of detecting simultaneous button presses of 2 buttons by using a
 * custom IEventHandler with internal variables to keep track of the button
 * press condition of each button.
 *
 * The program should print something like the following on the Serial monitor:
 * <verbatim>
 * B1 pressed
 * B2 pressed
 * B1 and B2 BOTH pressed
 * B1 released
 * B2 released
 * B1 pressed
 * B2 pressed
 * B1 and B2 BOTH pressed
 * B1 released
 * B2 released
 * <endverbatim>
 *
 * Note that this program works only when using ButtonConfig. Buttons using
 * EncodedButtonConfig (binary encoding) and LadderButtonConfig (resistor
 * ladder) cannot detect simultaneous button presses.
 */

#include <stdint.h>
#include <Arduino.h>
#include <AceButton.h>
using namespace ace_button;

// Change these to the pin numbers of the buttons.
const uint8_t BUTTON1_PIN = A2;
const uint8_t BUTTON2_PIN = A3;

ButtonConfig buttonConfig;
AceButton b1(&buttonConfig, BUTTON1_PIN);
AceButton b2(&buttonConfig, BUTTON2_PIN);

/**
 * A custom IEventHandler to detect simultaneous button presses of 2 buttons.
 */
class ButtonHandler: public IEventHandler {
  public:
    /**
     * If requireConcurrentTrigger is true in the constructor, then the 2
     * kEventPressed events from the 2 buttons must happen within this many
     * milliseconds of each other to trigger a "Both Buttons Pressed"
     * detection.
     */
    uint16_t const kConcurrentTriggerMillis = 100;

    /**
     * Constructor.
     *
     * @param requireConcurrentTrigger Set to false to print a message
     *    whenever B1 and B2 are pressed together, even if they are pressed many
     *    seconds apart. Set to true to require that B1 and B2 must be pressed
     *    down within a short time of each other (e.g. 100 milliseconds).
     */
    explicit ButtonHandler(bool requireConcurrentTrigger = false)
        : mRequireConcurrentTrigger(requireConcurrentTrigger)
    {}

    void handleEvent(AceButton* button, uint8_t eventType,
        uint8_t /*buttonState*/) override {
      switch (eventType) {
        case AceButton::kEventPressed: {
          uint8_t pin = button->getPin();
          uint16_t now = millis();
          if (pin == A2) {
            mIsB1Pressed = true;
            mB1PressedMillis = now;
            handleB1Pressed();
          } else if (pin == A3) {
            mIsB2Pressed = true;
            mB2PressedMillis = now;
            handleB2Pressed();
          }

          if (areBothPressed()) {
            handleBothPressed();
          }

          break;
        }

        case AceButton::kEventReleased: {
          uint8_t pin = button->getPin();
          if (pin == A2) {
            mIsB1Pressed = false;
            Serial.println("B1 released");
          } else if (pin == A3) {
            mIsB2Pressed = false;
            Serial.println("B2 released");
          }
        }
      }
    }

    void handleB1Pressed() {
      Serial.println("B1 pressed");
    }

    void handleB2Pressed() {
      Serial.println("B2 pressed");
    }

    void handleBothPressed() {
      Serial.println("B1 and B2 BOTH pressed");
    }

  private:
    /**
     * Determine if both buttons are pressed at the same time.
     *
     * If mRequireConcurrentTrigger is true, then also check if the Pressed
     * event of one button occurred within kConcurrentTriggerMillis of the other
     * button. The mB{x}PressedMillis is a 16-bit integer which can rollover
     * after 65536 milliseconds. So if the first button is held down for more
     * than (65536 - 100 = 65435) milliseconds, it is possible for the 2nd
     * button to trigger a concurrent press when pressed 65 seconds after the
     * first. I can think of 2 possible fixes:
     *
     *  1) Enable kEventRepeatPress, and use one of those events to reset
     *     the mB{x}PressedMillis of the other button appropriately.
     *  2) Use an external periodic task to reset the mB{x}PressedMillis
     *     every few seconds.
     */
    bool areBothPressed() {
      bool bothPressed = mIsB1Pressed && mIsB2Pressed;
      if (! bothPressed) return false;
      if (! mRequireConcurrentTrigger) return true;

      bool isTriggerConcurrent =
          (uint16_t) (mB1PressedMillis - mB2PressedMillis)
              < kConcurrentTriggerMillis
          || (uint16_t) (mB2PressedMillis - mB1PressedMillis)
              < kConcurrentTriggerMillis;
      return isTriggerConcurrent;
    }

  private:
    // Member variables are arranged to reduce memory size on 32-bit processors.

    bool const mRequireConcurrentTrigger;
    bool mIsB1Pressed = false;
    bool mIsB2Pressed = false;
    uint16_t mB1PressedMillis;
    uint16_t mB2PressedMillis;
};

ButtonHandler handleEvent(true /* requireConcurrentTrigger*/);

/** Check buttons with a rate limiter of 5 ms. */
void checkButtons() {
  static uint16_t lastCheck;

  // DO NOT USE delay(5) to do this.
  // The (uint16_t) cast is required on 32-bit processors, harmless on 8-bit.
  uint16_t now = millis();
  if ((uint16_t) (now - lastCheck) >= 5) {
    lastCheck = now;
    b1.check();
    b2.check();
  }
}

//-----------------------------------------------------------------------------

void setup() {
  delay(1000);
  Serial.begin(115200);
  while (! Serial); // For Leonardo/Micro

  // Use built-in pull up.
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  buttonConfig.setIEventHandler(&handleEvent);
  Serial.println(F("Ready for buttons"));
}

void loop() {
  checkButtons();
}
