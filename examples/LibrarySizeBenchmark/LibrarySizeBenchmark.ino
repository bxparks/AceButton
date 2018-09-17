/*
 * A copy of HelloWorld where we can measure the size of the AceButton library.
 * Set USE_ACE_BUTTON to 1 to include AceButton, 0 to exclude AceButton.
 */

#include <AceButton.h>

using namespace ace_button;

// Set this to 0 to disable the AceButton code, so that we can
// figure out how many bytes is consumed by the AceButton library.
#define USE_ACE_BUTTON 1

// Check for Arduino IDE
#ifndef AUNITER
  #define AUNITER_NANO
  #define AUNITER_LEFT_BUTTON 2
  #warning Assuming AUNITER_NANO
#endif

#if defined(AUNITER_NANO)
  const int BUTTON_PIN = AUNITER_LEFT_BUTTON;
  const int LED_PIN = LED_BUILTIN;
  const int LED_ON = HIGH;
  const int LED_OFF = LOW;
#elif defined(AUNITER_MICRO)
  const int BUTTON_PIN = AUNITER_BUTTON;
  const int LED_PIN = LED_BUILTIN_RX;
  const int LED_ON = LOW;
  const int LED_OFF = HIGH;
#elif defined(AUNITER_ESP32)
  // Different ESP32 boards use different pins, so just pick one for now
  const int LED_PIN = 2;
#else
  const int BUTTON_PIN = 2; // change this to the button pin
  const int LED_PIN = LED_BUILTIN;
#endif

#if USE_ACE_BUTTON == 1
AceButton button(BUTTON_PIN);

void handleEvent(AceButton*, uint8_t, uint8_t);
#endif

void setup() {
  delay(2000);

#if defined(ARDUINO_AVR_LEONARDO)
  RXLED0; // LED off
  TXLED0; // LED off
#endif

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

#if USE_ACE_BUTTON == 1
  button.setEventHandler(handleEvent);
#endif
}

void loop() {
#if USE_ACE_BUTTON == 1
  button.check();
#endif
}

#if USE_ACE_BUTTON == 1
void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWrite(LED_PIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_PIN, LED_OFF);
      break;
  }
}
#endif
