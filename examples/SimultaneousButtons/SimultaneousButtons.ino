/*
 * A demo of detecting simultaneous button presses of 2 buttons by using a
 * custom IEventHandler with internal variables to keep track of the button
 * press condition of each button.
 *
 * In this particular implementation, a "Both Buttons Pressed" or a "Both
 * Buttons Released" event should happen only when a transition occurs between
 * those states. If one button is held down, and the other is pressed and
 * released, neither of these events should trigger. Therefore, the program
 * should print something like the following on the Serial monitor when a button
 * is pressed and released while holding the other one:
 *
 * <verbatim>
 * B1 pressed
 * B2 pressed
 * B1 and B2 both pressed
 * B1 released
 * B1 pressed
 * B2 released
 * B2 pressed
 * B1 released
 * B2 released
 * B1 and B2 both released
 * <endverbatim>
 *
 * There may be other implementations which require that when a button is held
 * down, and the other button is pressed and released, it actually triggers the
 * "Both Buttons are Pressed" event. The logic below needs to be modified to
 * handle that.
 *
 * Another requirement may be that the "Both Buttons Pressed" event is triggered
 * only when both buttons are pressed within a small window of time (e.g. 200
 * milliseconds). That would require keeping track of the timestamp of when the
 * Pressed events occurred for each button.
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
    explicit ButtonHandler() {}

    void handleEvent(AceButton* button, uint8_t eventType,
        uint8_t /*buttonState*/) override {
      switch (eventType) {
        case AceButton::kEventPressed: {
          uint8_t pin = button->getPin();
          uint16_t now = millis();
          if (pin == A2) {
            mIsB1Pressed = true;
            handleB1Pressed();
          } else if (pin == A3) {
            mIsB2Pressed = true;
            handleB2Pressed();
          }

          if (checkBothPressed()) {
            handleBothPressed();
          }

          break;
        }

        case AceButton::kEventReleased: {
          uint8_t pin = button->getPin();
          if (pin == A2) {
            mIsB1Pressed = false;
            handleB1Released();
          } else if (pin == A3) {
            mIsB2Pressed = false;
            handleB2Released();
          }

          if (checkBothReleased()) {
            handleBothReleased();
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

    void handleB1Released() {
      Serial.println("B1 released");
    }

    void handleB2Released() {
      Serial.println("B2 released");
    }

    void handleBothPressed() {
      Serial.println("B1 and B2 both pressed");
    }

    void handleBothReleased() {
      Serial.println("B1 and B2 both released");
    }

  private:
    /**
     * Determine if a transition to both buttons are pressed at the same time
     * has happened. Pressing one button up and down, while keeping the other
     * one pressed should NOT cause this event.
     */
    bool checkBothPressed() {
      bool bothPressed = mIsB1Pressed && mIsB2Pressed;
      if (bothPressed && ! mBothPressed) {
        mBothPressed = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * Determine if a transition from both buttons Pressed to both buttons
     * Released has happened. Pressing one button up and down, while keeping the
     * other one pressed should NOT cause this event.
     */
    bool checkBothReleased() {
      bool bothReleased = !mIsB1Pressed && !mIsB2Pressed;
      if (bothReleased && mBothPressed) {
        mBothPressed = false;
        return true;
      } else {
        return false;
      }
    }

  private:
    bool mIsB1Pressed = false;
    bool mIsB2Pressed = false;
    bool mBothPressed = false;
};

ButtonHandler handleEvent;

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
