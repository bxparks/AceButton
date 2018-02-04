/*
 * A demo of a simplest AceButton that has a visible effect. One button is
 * connected to the digital pin BUTTON_PIN. It uses the internal pull-up
 * resister (INPUT_PULLUP). Pressing the button turns on the built-in LED.
 * Releasing the button turns off the LED.
 */

#include <AceButton.h>
using namespace ace_button;

const int BUTTON_PIN = 2; // change this to the button pin

AceButton button(BUTTON_PIN);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
}

void loop() {
  button.check();
}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWrite(LED_BUILTIN, HIGH); // turn the led on
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_BUILTIN, LOW); // turn the led off
      break;
  }
}
