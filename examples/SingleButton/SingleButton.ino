/*
 * A demo of a simple AceButton used to handle the events from one button.
 * Similar to HelloButton with some additions;
 * - more comments
 * - prints out the button events to the Serial monitor
 * - enables the all button events, including LongPress and RepeatPress
 * - suppresses lower-level events when higher-level events are detected
 *   (e.g. Clicked suppressed Released, DoubleClicked suppresses the
 *   second Clicked, LongPressed suppressed the Released, etc.)
 */

#include <AceButton.h>
using namespace ace_button;

#define ENABLE_SERIAL 1

// The pin number attached to the button.
const int BUTTON_PIN = 2;

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// One button wired to the pin at BUTTON_PIN. Automatically uses the default
// ButtonConfig. The alternative is to call the AceButton::init() method in
// setup() below.
AceButton button(BUTTON_PIN);

void setup() {
#if ENABLE_SERIAL == 1
  Serial.begin(9600);
#endif

  // initialize built-in LED as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
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
    case AceButton::kEventPressed:
      digitalWrite(LED_BUILTIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_BUILTIN, LED_OFF);
      break;
  }
}
