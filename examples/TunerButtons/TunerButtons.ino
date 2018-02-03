/*
 * Emulate the buttons of a car radio: 3 preset buttons and a tune-up and
 * a tune-down button.
 */

#include <AceButton.h>

using namespace ace_button;

#define ENABLE_SERIAL 1

const int NUM_PRESETS = 3;

const int PRESET_1_PIN = 6;
const int PRESET_2_PIN = 5;
const int PRESET_3_PIN = 4;
const int TUNE_DOWN_PIN = 3;
const int TUNE_UP_PIN = 2;

// Frequency range of the FM radio.
const uint16_t FM_FREQ_MIN = 879; // 87.9 MHz
const uint16_t FM_FREQ_MAX = 1079; // 107.9 MHz
const uint16_t FM_FREQ_DELTA = 2; // 0.2 MHz increments in USA

// Preset buttons.
ButtonConfig presetConfig;
AceButton presetButton1;
AceButton presetButton2;
AceButton presetButton3;

// Tune up or down buttons.
ButtonConfig tuneConfig;
AceButton tuneDownButton;
AceButton tuneUpButton;

// Array to hold the stations associated with the button whose id is an
// index into this array. The value is the 10 * station_frequency (e.g. 88.5 MHz
// is stored as 885).
//
// NOTE: It might be tempting to subclass the AceButton to hold this information
// but it's cleaner to keep this info separated from the AceButton class
// hierarchy.
uint16_t stations[NUM_PRESETS];

// The current station frequency.
uint16_t currentStation = FM_FREQ_MIN;

void setup() {
#if ENABLE_SERIAL == 1
  Serial.begin(9600);
#endif

  // Configs for the preset buttons. Need Released event to change the station,
  // and LongPressed to memorize the current station. Don't need Clicked.
  presetConfig.setEventHandler(handlePresetEvent);
  presetConfig.setFeature(ButtonConfig::kFeatureLongPress);
  presetConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  // Configs for the tune-up and tune-down buttons. Need RepeatPress instead of
  // LongPress.
  tuneConfig.setEventHandler(handleTuneEvent);
  tuneConfig.setFeature(ButtonConfig::kFeatureClick);
  tuneConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  // These suppressions not really necessary but cleaner.
  tuneConfig.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
  tuneConfig.setFeature(ButtonConfig::kFeatureSuppressAfterRepeatPress);

  // Preset Button 1
  pinMode(PRESET_1_PIN, INPUT_PULLUP);
  presetButton1.setButtonConfig(&presetConfig);
  presetButton1.init(PRESET_1_PIN, HIGH, 0 /* id */);

  // Preset Button 2
  pinMode(PRESET_2_PIN, INPUT_PULLUP);
  presetButton2.setButtonConfig(&presetConfig);
  presetButton2.init(PRESET_2_PIN, HIGH, 1 /* id */);

  // Preset Button 3
  pinMode(PRESET_3_PIN, INPUT_PULLUP);
  presetButton3.setButtonConfig(&presetConfig);
  presetButton3.init(PRESET_3_PIN, HIGH, 2 /* id */);

  // Tune Down Button
  pinMode(TUNE_DOWN_PIN, INPUT_PULLUP);
  tuneDownButton.setButtonConfig(&tuneConfig);
  tuneDownButton.init(TUNE_DOWN_PIN);

  // Tune Up Button
  pinMode(TUNE_UP_PIN, INPUT_PULLUP);
  tuneUpButton.setButtonConfig(&tuneConfig);
  tuneUpButton.init(TUNE_UP_PIN);

  // init the stations associated with each button
  initStations();

#if ENABLE_SERIAL == 1
  while (! Serial); // Wait until Serial is ready - Leonardo
  Serial.println(F("stopwatch ready"));
#endif

  printStation(currentStation);
}

void loop() {
  presetButton1.check();
  presetButton2.check();
  presetButton3.check();
  tuneUpButton.check();
  tuneDownButton.check();
}

void initStations() {
  for (int i = 0; i < NUM_PRESETS; i++) {
    stations[i] = FM_FREQ_MIN;
  }
}

void printStation(uint16_t frequency) {
#if ENABLE_SERIAL == 1
  Serial.print(F("station: "));
  Serial.print(frequency / 10);
  Serial.print('.');
  Serial.println(frequency % 10);
#endif
}

void handlePresetEvent(AceButton* button, uint8_t eventType,
    uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventReleased:
      // We trigger on the Released event not the Pressed event to distinguish
      // this event from the LongPressed event.
      retrievePreset(button->getId());
      break;
    case AceButton::kEventLongPressed:
      setPreset(button->getId());
      break;
  }
}

void handleTuneEvent(AceButton* button, uint8_t eventType,
    uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
    case AceButton::kEventRepeatPressed: {
      uint8_t pin = button->getPin();
      switch (pin) {
        case TUNE_UP_PIN:
          tuneUp();
          break;
        case TUNE_DOWN_PIN:
          tuneDown();
          break;
      }
    }
  }
}

void retrievePreset(uint8_t id) {
  if (id >= 0 || id <= NUM_PRESETS) {
    currentStation = stations[id];
#if ENABLE_SERIAL == 1
    Serial.print(F("memory "));
    Serial.print(id + 1);
    Serial.print(F(": "));
    printStation(currentStation);
#endif
  }
}

void setPreset(uint8_t id) {
  if (id >= 0 || id <= NUM_PRESETS) {
    stations[id] = currentStation;

#if ENABLE_SERIAL == 1
    Serial.print(F("memory "));
    Serial.print(id + 1);
    Serial.print(F(" set: "));
    printStation(currentStation);
#endif
  }
}

void tuneUp() {
  currentStation += FM_FREQ_DELTA;
  if (currentStation > FM_FREQ_MAX) {
    currentStation = FM_FREQ_MIN;
  }
  printStation(currentStation);
}

void tuneDown() {
  currentStation -= FM_FREQ_DELTA;
  if (currentStation < FM_FREQ_MIN) {
    currentStation = FM_FREQ_MAX;
  }
  printStation(currentStation);
}
