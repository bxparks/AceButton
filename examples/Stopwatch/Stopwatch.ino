/*
 * A demo app that calculates the ammount of time taken by the
 * AceButton::check() method. It acts like a stopwatch, supporting the following
 * operations:
 *
 * - press: start the stopwatch
 * - press: stop the stopwatch, printing out the result
 * - long press: reset the stopwatch, allowing press to start the process again
 *
 * It appears that AceButton::check() takes about 16 microseconds.
 */

#include <AceButton.h>
#include <AdjustableButtonConfig.h>

using namespace ace_button;

// The pin number attached to the button.
const uint8_t BUTTON_PIN = 2;

// Use an instance of AdjustableButtonConfig for demo purposes. We could have
// just used the default System ButtonConfig since a long press delay of 1000 ms
// is good enough. But this shows how to configure an AceButton to use a
// different ButtonConfig.
AdjustableButtonConfig adjustableButtonConfig;

// One button wired to the pin at BUTTON_PIN. Automatically uses the System
// ButtonConfig. The alternative is to call the AceButton::init() method in
// setup() below.
AceButton button(BUTTON_PIN);

// counters to determine the duration of a single call to AceButton::check()
unsigned long loopCounter = 0;
unsigned long startMillis = 0;
unsigned long stopMillis  = 0;

// states of the stopwatch
const uint8_t STOPWATCH_INIT = 0;
const uint8_t STOPWATCH_STARTED = 1;
const uint8_t STOPWATCH_STOPPED = 2;

// implements a finite state machine (FSM)
uint8_t stopwatchState = STOPWATCH_INIT;

bool allEventsEnabled = false;

void setup() {
  Serial.begin(9600);

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Override the System ButtonConfig with our own AdjustableButtonConfig
  // instance instead.
  button.setButtonConfig(&adjustableButtonConfig);

  // Configure the ButtonConfig with the event handler and enable LongPress.
  // SupressAfterLongPress is optional since we don't do anything if we get it.
  adjustableButtonConfig.setEventHandler(handleEvent);
  adjustableButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  adjustableButtonConfig.setLongPressDelay(2000);

  while (! Serial); // Wait until Serial is ready - Leonardo
  Serial.println(F("setup(): stopwatch ready"));
}

void loop() {
  // Use an explicit infinite loop to avoid the overhead of the loop() processor
  // for timing purposes. Normally, we wouldn't do this.
  while (true) {
    // Should be called every 20ms or faster for the default debouncing time
    // of ~50ms.
    button.check();

    // increment loop counter
    if (stopwatchState == STOPWATCH_STARTED) {
      loopCounter++;
    }
  }
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  unsigned long now = millis();
  switch (eventType) {
    case AceButton::kEventPressed:
      if (stopwatchState == STOPWATCH_INIT) {

        // enable or disable higher level events, to get different performance
        // numbers
        if (allEventsEnabled) {
          enableAllEvents();
        } else {
          disableAllEvents();
        }

        Serial.println(F("handleEvent(): stopwatch started"));
        startMillis = now;
        loopCounter = 0;
        stopwatchState = STOPWATCH_STARTED;
      } else if (stopwatchState == STOPWATCH_STARTED) {
        stopMillis = now;
        stopwatchState = STOPWATCH_STOPPED;
        unsigned long duration = stopMillis - startMillis;
        float microsPerLoop = duration * 1000.0 / loopCounter;

        // reenable all events after stopping
        enableAllEvents();

        Serial.println(F("handleEvent(): stopwatch stopped"));
        Serial.print(F("handleEvent(): duration (ms): "));
        Serial.print(duration);
        Serial.print(F("; loopCount: "));
        Serial.print(loopCounter);
        Serial.print(F("; micros/loop: "));
        Serial.println(microsPerLoop);

      }
      break;
    case AceButton::kEventLongPressed:
      if (stopwatchState == STOPWATCH_STOPPED) {
        stopwatchState = STOPWATCH_INIT;
        Serial.println(F("handleEvent(): stopwatch reset"));
        allEventsEnabled = !allEventsEnabled;
      }
      break;
  }
}

void enableAllEvents() {
  Serial.println(F("enabling high level events"));
  adjustableButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  adjustableButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  adjustableButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  adjustableButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
}

void disableAllEvents() {
  Serial.println(F("disabling high level events"));
  adjustableButtonConfig.clearFeature(ButtonConfig::kFeatureClick);
  adjustableButtonConfig.clearFeature(ButtonConfig::kFeatureDoubleClick);
  adjustableButtonConfig.clearFeature(ButtonConfig::kFeatureLongPress);
  adjustableButtonConfig.clearFeature(ButtonConfig::kFeatureRepeatPress);
}
