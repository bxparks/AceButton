/*
 * A program which compiles in different ButtonConfig instances so that the
 * flash and static memory sizes can be extract from the output of the compiler.
 * Set the FEATURE macro to various integer to compile different parts of the
 * AceButton library.
 */

#include <AceButton.h>
using namespace ace_button;

// Set this to [0..n] to extract the flash and static memory usage.
// 0 - baseline
// 1 - ButtonConfig
// 2 - Encoded4To2ButtonConfig
// 3 - Encoded8To3ButtonConfig
// 4 - EncodedButtonConfig
// 5 - LadderButtonConfig
#define FEATURE 0

// A volatile integer to prevent the compiler from optimizing away the entire
// program.
volatile int disableCompilerOptimization = 0;

#if FEATURE == 1
  static const int BUTTON_PIN = 2;
  AceButton button(BUTTON_PIN);
#elif FEATURE == 2
  static const uint8_t BUTTON_PIN0 = 2;
  static const uint8_t BUTTON_PIN1 = 3;
  Encoded4To2ButtonConfig buttonConfig(BUTTON_PIN0, BUTTON_PIN1);

  AceButton b1(&buttonConfig, 1);
  AceButton b2(&buttonConfig, 2);
  AceButton b3(&buttonConfig, 3);
#elif FEATURE == 3
  static const uint8_t BUTTON_PIN0 = 2;
  static const uint8_t BUTTON_PIN1 = 3;
  static const uint8_t BUTTON_PIN2 = 4;
  Encoded8To3ButtonConfig buttonConfig(BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);

  AceButton b1(&buttonConfig, 1);
  AceButton b2(&buttonConfig, 2);
  AceButton b3(&buttonConfig, 3);
  AceButton b4(&buttonConfig, 4);
  AceButton b5(&buttonConfig, 5);
  AceButton b6(&buttonConfig, 6);
  AceButton b7(&buttonConfig, 7);
#elif FEATURE == 4
  static const uint8_t NUM_PINS = 3;
  static const uint8_t PINS[] = {2, 3, 4};
  static const uint8_t NUM_BUTTONS = 7;
  static AceButton b01(1);
  static AceButton b02(2);
  static AceButton b03(3);
  static AceButton b04(4);
  static AceButton b05(5);
  static AceButton b06(6);
  static AceButton b07(7);
  static AceButton* const BUTTONS[NUM_BUTTONS] = {
      &b01, &b02, &b03, &b04, &b05, &b06, &b07,
  };
  static EncodedButtonConfig buttonConfig(
    NUM_PINS, PINS, NUM_BUTTONS, BUTTONS
  );
#elif FEATURE == 5
  static const uint8_t NUM_PINS = 3;
  static const uint8_t PINS[] = {2, 3, 4};
  static const uint8_t NUM_BUTTONS = 7;
  static AceButton b01(1);
  static AceButton b02(2);
  static AceButton b03(3);
  static AceButton b04(4);
  static AceButton b05(5);
  static AceButton b06(6);
  static AceButton b07(7);
  static AceButton* const BUTTONS[NUM_BUTTONS] = {
      &b01, &b02, &b03, &b04, &b05, &b06, &b07,
  };

  static const int ANALOG_BUTTON_PIN = A0;
  static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
  static const uint16_t LEVELS[NUM_LEVELS] = {
    0   /* 0%, short to ground */,
    133 /* 13%, 1.5 kohm */,
    327 /* 32%, 4.7 kohm */,
    512 /* 50%, 10 kohm */,
    614 /* 60%, 15 kohm */,
    788 /* 77%, 33 kohm */,
    930 /* 91%, 100 kohm */,
    1023 /* 100%, open circuit */,
  };
  static LadderButtonConfig buttonConfig(
    ANALOG_BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
  );
#endif

void handleEvent(AceButton* /* button */, uint8_t /* eventType */,
    uint8_t /* buttonState */) {
  disableCompilerOptimization++;
}

void setup() {
  delay(2000);

#if FEATURE == 1
  pinMode(BUTTON_PIN, INPUT_PULLUP);
#elif FEATURE == 2
  pinMode(BUTTON_PIN0, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
#elif FEATURE == 3
  pinMode(BUTTON_PIN0, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
#elif FEATURE == 4
  pinMode(PINS[0], INPUT_PULLUP);
  pinMode(PINS[1], INPUT_PULLUP);
  pinMode(PINS[2], INPUT_PULLUP);
#elif FEATURE == 5
  pinMode(ANALOG_BUTTON_PIN, INPUT);
#endif

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
#if FEATURE == 1
  ButtonConfig* config = button.getButtonConfig();
#elif FEATURE != 0
  ButtonConfig* config = &buttonConfig;
#endif

#if FEATURE != 0
  config->setEventHandler(handleEvent);
  config->setFeature(ButtonConfig::kFeatureClick);
  config->setFeature(ButtonConfig::kFeatureDoubleClick);
  config->setFeature(ButtonConfig::kFeatureLongPress);
  config->setFeature(ButtonConfig::kFeatureRepeatPress);
  config->setFeature(ButtonConfig::kFeatureSuppressAll);
#endif
}

void loop() {
#if FEATURE == 0
  handleEvent(nullptr, 0, 0);
#elif FEATURE == 1
  button.check();
#elif FEATURE == 2
  b1.check();
  b2.check();
  b3.check();
#elif FEATURE == 3
  b1.check();
  b2.check();
  b3.check();
  b4.check();
  b5.check();
  b6.check();
  b7.check();
#elif FEATURE == 4
  buttonConfig.checkButtons();
#elif FEATURE == 5
  buttonConfig.checkButtons();
#endif
}
