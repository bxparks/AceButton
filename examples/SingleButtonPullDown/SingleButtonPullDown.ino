/*
 * A demo of a simple AceButton used to handle the events from one button.
 * Pretty much the same as SingleButton.ino example but the button is wired with
 * an external pull-down resistor, instead of using the built-in pull-up
 * resistor.
 */

#include <AceButton.h>
using namespace ace_button;

#define ENABLE_SERIAL 1

// The pin number attached to the button.
const int BUTTON_PIN = 7;

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Automatically uses the default ButtonConfig. We will configure this later
// using AceButton::init() method in setup() below.
AceButton button;

void setup() {
#if ENABLE_SERIAL == 1
  Serial.begin(9600);
#endif

  // initialize built-in LED as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Button uses an external 10k resistor.
  pinMode(BUTTON_PIN, INPUT);

  // We use the AceButton::init() method here instead of using the constructor
  // to show an alternative. Using init() allows the configuration of the
  // hardware pin and the button to be placed closer to each other.
  button.init(BUTTON_PIN, LOW);

  // Configure the ButtonConfig with the event handler, and enable the LongPress
  // and RepeatPress events which are turned off by default.
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

#if ENABLE_SERIAL == 1
  while (! Serial); // Wait until Serial is ready - Leonardo
  Serial.println(F("setup(): ready"));
#endif
}

void loop() {
  // Should be called every 20ms or faster for the default debouncing time
  // of ~50ms.
  button.check();
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

#if ENABLE_SERIAL == 1
  // Print out a message for all events.
  Serial.print(F("handleEvent(): eventType: "));
  Serial.print(eventType);
  Serial.print(F("; button: "));
  Serial.println(buttonState);
#endif

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventReleased:
      digitalWrite(LED_BUILTIN, LED_ON);
      break;
    case AceButton::kEventPressed:
      digitalWrite(LED_BUILTIN, LED_OFF);
      break;
  }
}
