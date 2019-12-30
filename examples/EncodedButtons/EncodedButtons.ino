/*
 * A demo of how 7 buttons can be detected using only 3 pins using binary
 * encoding of the 7 buttons into 3 outputs.
 */

#include <AceButton.h>
using namespace ace_button;

/**
 * A ButtonConfig that handles an 8-to-3 binary encoder which converts 8 inputs
 * into 3 outputs. In practice, this means that 7 buttons can be handled with 3
 * pins, because the 8th button is used to represent "no button pressed". One
 * easy way to perform the 8-to-3 encoding is to use 9 diodes with 7 switches,
 * so that each switch translates into a 3-bit binary number:
 *
 *  * S1 = 001
 *  * S2 = 010
 *  * S3 = 011
 *  * S4 = 100
 *  * S5 = 101
 *  * S6 = 110
 *  * S7 = 111
 *
 * 000 means that no button was pressed.
 */
class Encoded8To3ButtonConfig : public ButtonConfig {
  public:
    Encoded8To3ButtonConfig(uint8_t pin1, uint8_t pin2, uint8_t pin3):
      mPin1(pin1),
      mPin2(pin2),
      mPin3(pin3) {}

    /**
     * Return state of the encoded 'pin' number, corresponding to the pull-down
     * states of the actual pins. LOW means that the corresponding encoded
     * virtual pin was pushed.
     */
    int readButton(uint8_t pin) override {
      int s1 = digitalRead(mPin1);
      int s2 = digitalRead(mPin2);
      int s3 = digitalRead(mPin3);

      // Convert the actual pins states into a binary number which becomes
      // the encoded virtual pin numbers of the buttons.
      uint8_t virtualPin = (s1 == LOW)
        | ((s2 == LOW) << 1)
        | ((s3 == LOW) << 2);
      return (virtualPin == pin) ? LOW : HIGH;
    }

  private:
    const uint8_t mPin1;
    const uint8_t mPin2;
    const uint8_t mPin3;
};

#ifdef ESP32
  // Different ESP32 boards use different pins
  static const int LED_PIN = 2;
#else
  static const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
static const int LED_ON = HIGH;
static const int LED_OFF = LOW;

static const uint8_t BUTTON_PIN1 = 2;
static const uint8_t BUTTON_PIN2 = 3;
static const uint8_t BUTTON_PIN3 = 4;

// Each button is assigned to the virtual pin number (1-7) which comes from the
// binary bit patterns of the 3 actual pins.
Encoded8To3ButtonConfig buttonConfig(BUTTON_PIN1, BUTTON_PIN2, BUTTON_PIN3);
AceButton b1(&buttonConfig, 1);
AceButton b2(&buttonConfig, 2);
AceButton b3(&buttonConfig, 3);
AceButton b4(&buttonConfig, 4);
AceButton b5(&buttonConfig, 5);
AceButton b6(&buttonConfig, 6);
AceButton b7(&buttonConfig, 7);

void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // initialize built-in LED as an output
  pinMode(LED_PIN, OUTPUT);

  // Pins uses the built-in pull up register.
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

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
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  b1.check();
  b2.check();
  b3.check();
  b4.check();
  b5.check();
  b6.check();
  b7.check();
}

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
