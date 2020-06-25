/*
 * A copy of HelloWorld which allows us to measure the size of the AceButton
 * library. Set USE_ACE_BUTTON to 1 to include AceButton, 0 to exclude
 * AceButton.
 */

#include <AceButton.h>
using namespace ace_button;

// Set this to 0 to disable the AceButton code, so that we can
// figure out how many bytes is consumed by the AceButton library.
#define USE_ACE_BUTTON 0

const int BUTTON_PIN = 2;

// A volatile integer to prevent the compiler from optimizing away the entire
// program.
volatile int disableCompilerOptimization = 0;

#if USE_ACE_BUTTON == 1
AceButton button(BUTTON_PIN);
#endif

#if USE_ACE_BUTTON == 1
void handleEvent(AceButton* /* button */, uint8_t /* eventType */,
    uint8_t /* buttonState */) {
  disableCompilerOptimization++;
}
#endif

void setup() {
  delay(2000);

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
