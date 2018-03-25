/*
 * A demo that uses kFeatureSuppressClickBeforeDoubleClick to distinguish a
 * Clicked event from a DoubleClicked event. Clicked turns on the LED. A
 * DoubleClick turns off the LED.
 *
 * The only way to suppress the Clicked "after" a DoubleClicked is to delay the
 * sending of the Clicked event until we wait getDoubleClickDelay() time after
 * the Clicked, at which time we can tell if a DoubleClicked has occurred or
 * not. But this algorithm means that every Clicked event is delayed by
 * (kClickDelay + kDoubleClickDelay + 2 * kDebounceDelay) which is 700 ms using
 * the default values.
 *
 * Even though this seems like the right way to do things, there are two
 * problems that this example program shows:
 * 1) The delay explained above, which causes a noticeable delay in turning
 * on the LED after the user clicks.
 * 2) Users cannot physically input a Click reliably. Sometimes, they
 * Press/Release too slowly, no Click is generated and the LED does not turn on.
 * They are forced to click again, sometimes a few more times, depending on the
 * type of switch.
 *
 * See Also:
 *    "examples/ClickVersusDoubleClick/" program which uses the Released event
 *    instead of the Clicked
 */

#include <AceButton.h>
using namespace ace_button;

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
  // initialize built-in LED as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler to send
  // kEventDoubleClicked, which automatically implies kEventDoubleClicked,
  // but then we suppress that Clicked event after a DoubleClicked.
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(
      ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
}

void loop() {
  // Should be called every 20ms or faster for the default debouncing time
  // of ~50ms.
  button.check();
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventClicked:
      digitalWrite(LED_BUILTIN, LED_ON);
      break;
    case AceButton::kEventDoubleClicked:
      digitalWrite(LED_BUILTIN, LED_OFF);
      break;
  }
}
