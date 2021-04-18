/*
 * A demo of LadderButtonConfig for buttons connected to a single pin
 * using a resistor ladder.
 */

#include <AceButton.h>
using ace_button::AceButton;
using ace_button::ButtonConfig;
using ace_button::LadderButtonConfig;

// Select MODE_CALIBRATE to calibrate the analogRead().
// Select MODE_READ_BUTTONS to read multiple buttons using LadderButtonConfig.
#define MODE_CALIBRATE 1
#define MODE_READ_BUTTONS 2
#define MODE MODE_READ_BUTTONS

#ifdef ESP32
  // Different ESP32 boards use different pins
  static const int LED_PIN = 2;
#else
  static const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
static const int LED_ON = HIGH;
static const int LED_OFF = LOW;

// Define the actual ADC pin.
static const uint8_t BUTTON_PIN = A0;

// Create 4 AceButton objects, with their virtual pin number 0 to 3.
// Note: we could use an array of AceButton BUTTONS[15], and use a loop to
// initialize these arrays, but this is more explicit and easier to understand
// as an example code.
static const uint8_t NUM_BUTTONS = 4;
static AceButton b0((uint8_t) 0);
static AceButton b1(1);
static AceButton b2(2);
static AceButton b3(3);
static AceButton* const BUTTONS[NUM_BUTTONS] = {
    &b0, &b1, &b2, &b3,
};

// Define the ADC voltage levels for each button. For 4 buttons,
// we need 5 levels: 0%, 25%, 50%, 75%, 100%.
static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
static const uint16_t LEVELS[NUM_LEVELS] = {
  0 /* 0%, short to ground */,
  327 /* 32%, 4.7 kohm */,
  512 /* 50%, 10 kohm */,
  844 /* 82%, 47 kohm */,
  1023 /* 100%, open circuit */,
};

// The LadderButtonConfig constructor binds the AceButton to the
// LadderButtonConfig.
static LadderButtonConfig buttonConfig(
  BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
);

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
  Serial.print(F("handleEvent(): "));
  Serial.print(F("virtualPin: "));
  Serial.print(button->getPin());
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWrite(LED_PIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_PIN, LED_OFF);
      break;
  }
}

// Call analogRead() and just print the value. Click on the various buttons to
// see the actual value is returned by each button.
void calibrateAnalogRead() {
  uint16_t val = analogRead(BUTTON_PIN);
  Serial.println(val);
  delay(1);
}

// On most processors, this should be called every 4-5ms or faster, if default
// the debouncing time is ~20ms. On a ESP8266, we must sample *no* faster than
// 4-5 ms to avoid disconnecting the WiFi connection. (See
// https://github.com/esp8266/Arduino/issues/1634 and
// https://github.com/esp8266/Arduino/issues/5083). Let's rate-limit on all
// processors to about 200 samples/second.
void checkButtons() {
  static unsigned long prev = millis();

  // DO NOT USE delay(5) to do this.
  unsigned long now = millis();
  if (now - prev > 5) {
    buttonConfig.checkButtons();
    prev = now;
  }
}

//-----------------------------------------------------------------------------

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // initialize built-in LED as an output
  pinMode(LED_PIN, OUTPUT);

  // Don't use internal pull-up resistor.
  pinMode(BUTTON_PIN, INPUT);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  buttonConfig.setEventHandler(handleEvent);
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);

  Serial.println(F("setup(): ready"));
}

void loop() {
#if MODE == MODE_CALIBRATE
  calibrateAnalogRead();
#else
  checkButtons();
#endif
}

