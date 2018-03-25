/*
 * A demo that distinguishes a "Clicked" from a "DoubleClicked" by using a
 * Released event intead. "Clicked" (actually Released) turns on the LED. A
 * DoubleClick turns off the LED.
 *
 * Normally, AceButton cannot actually separate a Clicked from a DoubleClicked
 * because the Clicked event will always trigger if a DoubleClicked occurs. We
 * cannot suppress the first Clicked because it's already happened in the past
 * by the time the DoubleClicked occurs, and the first Clicked cannot predict
 * the future.
 *
 * The only way to suppress the Clicked "after" a DoubleClicked is to delay the
 * sending of the Clicked event until we wait getDoubleClickDelay() time after
 * the Clicked, at which time we can tell if a DoubleClicked as occurred or not.
 * But this algorithm means that every Clicked event would be delayed by
 * (kClickDelay + kDoubleClickDelay + 2 * kDebounceDelay) which is 700 ms using
 * the default values.
 *
 * This workaround uses a Released event instead of a Clicked to trigger the
 * first action, then use the DoubleClick for the second action, while
 * suppressing a Released after a DoubleClicked. The drawback to this workaround
 * is that it is no longer possible to distinguish between a normal
 * Pressed/Released input from a Clicked input. But often this is actually the
 * what we want, because users are not always physically able to perform the two
 * types of inputs accurately. In other words, it's too easy for a user to
 * mistakenly trigger Clicked when they meant to just trigger a
 * Pressed/Released, or the reverse. By simply ignoring the Clicked, we
 * sometimes get a more reliable user interface.
 *
 * See Also:
 *    "examples/ClickPostponedAfterDoubleClick/" program which uses
 *    kFeatureSuppressClickBeforeDoubleClick flag.
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

  // Configure the ButtonConfig with the event handler to send only
  // kEventReleased and kEventDoubleClicked. Note that kEventDoubleClicked
  // automatically implies kEventClicked.
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
}

void loop() {
  // Should be called every 20ms or faster for the default debouncing time
  // of ~50ms.
  button.check();
}

// The event handler for the button. A kEventClicked is always triggered but
// just ignore it in the event handler.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventReleased:
      digitalWrite(LED_BUILTIN, LED_ON);
      break;
    case AceButton::kEventDoubleClicked:
      digitalWrite(LED_BUILTIN, LED_OFF);
      break;
  }
}
