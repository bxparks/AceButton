/*
 * A demo of a single AceButton which uses an IEventHandler to piggy-back on top
 * of the kEventHeartBeat events to generate 2 additional custom events:
 *  * kCustomEventLongPressed and
 *  * kCustomEventLongReleased.
 *
 * They are triggered when the button is Pressed (or Released) for more than
 * kCustomLongPressDelayMs (or kCustomLongReleaseDelayMs) respectively.
 *
 * This implementation is suitable for switches operating at a much longer time
 * scale (several minutes) than AceButton was originally designed for
 * (10-10000 milliseconds). The built-in AceButton::kEventLongPressed does not
 * work for detecting a LongPressed event lasting several minutes because the
 * internal time counters of AceButton do not support that.
 *
 * Also, there is no built-in support in the AceButton class for detecting the
 * symmetric equivalent of LongReleased. (The built-in kEventLongReleased is a
 * different type of event.) This example shows how to generate the
 * kCustomEventLongReleased event which is the mirror image of
 * kCustomEventLongPressed.
 *
 * With the parameters given below, this program should print something like the
 * following when the button is pressed, and held for over 10 seconds, then
 * released for over 10 seconds:
 * 
 * handleEvent(): [14000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [14578]: eventType: Pressed; buttonState: 0
 * handleEvent(): [16000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [18000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [20000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [22000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [24000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [26000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [26000]: eventType: HeartBeat; buttonState: 0
 * handleCustomLongPressed(): eventType: CustomLongPressed; buttonState: 0
 * handleEvent(): [28000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [28517]: eventType: Released; buttonState: 1
 * handleEvent(): [30000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [32000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [34000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [36000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [38000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [40000]: eventType: HeartBeat; buttonState: 1
 * handleCustomLongReleased(): eventType: CustomLongReleased; buttonState: 1
 * handleEvent(): [42000]: eventType: HeartBeat; buttonState: 1
 * handleEvent(): [42407]: eventType: Pressed; buttonState: 0
 * handleEvent(): [44000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [46000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [48000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [50000]: eventType: HeartBeat; buttonState: 0
 * handleEvent(): [50382]: eventType: Released; buttonState: 1
 * handleEvent(): [51113]: eventType: Pressed; buttonState: 0
 */

#include <AceButton.h>
using namespace ace_button;

// The pin number attached to the button.
const int BUTTON_PIN = A2;

#ifdef ESP32
  // Different ESP32 boards use different pins
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// One button wired to the pin at BUTTON_PIN. Automatically uses the default
// ButtonConfig. The alternative is to call the AceButton::init() method in
// setup() below.
AceButton button(BUTTON_PIN);

// An object that handles the event.
class ButtonHandler: public IEventHandler {
  private:
    // These must be different than the built-in event type codes defined by
    // AceButton::kEventXxx. Currently the builtin ones range from 0-7, so
    // starting from 100 should be more than safe.
    static const uint8_t kCustomEventUnknown = 100;
    static const uint8_t kCustomEventLongPressed = 101;
    static const uint8_t kCustomEventLongReleased = 102;

    static const uint32_t kCustomLongPressDelayMs = 10*1000;
    static const uint32_t kCustomLongReleaseDelayMs = 10*1000;

  public:
    void init() {
      uint32_t now = millis();
      mLastEventType = kCustomEventUnknown;
      mLastPressedTime = now;
      mLastReleasedTime = now;
    }

    void handleEvent(AceButton* button, uint8_t eventType,
        uint8_t buttonState) override {

      uint32_t now = millis();

      // Print out a message for all events.
      Serial.print(F("handleEvent(): ["));
      Serial.print((uint16_t) now);
      Serial.print(F("]: eventType: "));
      Serial.print(AceButton::eventName(eventType));
      Serial.print(F("; buttonState: "));
      Serial.println(buttonState);

      // Control the LED only for the Pressed and Released events.
      // Notice that if the MCU is rebooted while the button is pressed down, no
      // event is triggered and the LED remains off.
      switch (eventType) {
        case AceButton::kEventPressed:
          mLastEventType = eventType;
          mLastPressedTime = now;
          break;
        case AceButton::kEventReleased:
          mLastEventType = eventType;
          mLastReleasedTime = now;
          break;
        case AceButton::kEventHeartBeat:
          switch (mLastEventType) {
            case AceButton::kEventPressed: {
              uint32_t elapsed = now - mLastPressedTime;
              if (elapsed >= kCustomLongPressDelayMs) {
                handleCustomLongPressed(button, kCustomEventLongPressed,
                    buttonState);
                mLastEventType = kCustomEventLongPressed;
              }
              break;
            }
            case AceButton::kEventReleased: {
              uint32_t elapsed = now - mLastReleasedTime;
              if (elapsed >= kCustomLongReleaseDelayMs) {
                handleCustomLongReleased(button, kCustomEventLongReleased,
                    buttonState);
                mLastEventType = kCustomEventLongReleased;
              }
              break;
            }
          }
          break;
      }
    }

  private:
    void handleCustomLongPressed(AceButton* /*button*/, uint8_t /*eventType*/,
        uint8_t buttonState) {
      Serial.print(F("handleCustomLongPressed(): eventType: "));
      Serial.print(F("CustomLongPressed"));
      Serial.print(F("; buttonState: "));
      Serial.println(buttonState);
    }

    void handleCustomLongReleased(AceButton* /*button*/, uint8_t /*eventType*/,
        uint8_t buttonState) {
      Serial.print(F("handleCustomLongReleased(): eventType: "));
      Serial.print(F("CustomLongReleased"));
      Serial.print(F("; buttonState: "));
      Serial.println(buttonState);
    }

  private:
    uint8_t mLastEventType;
    uint32_t mLastPressedTime;
    uint32_t mLastReleasedTime;
};

ButtonHandler handleEvent;

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // initialize built-in LED as an output
  pinMode(LED_PIN, OUTPUT);

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure the EventHandler, the ButtonConfig , and enable the HeartBeat
  // event. Lower the HeartBeatInterval from 5000 to 2000 for testing purposes.
  handleEvent.init();
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setIEventHandler(&handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureHeartBeat);
  buttonConfig->setHeartBeatInterval(2000);

  // Check if the button was pressed while booting
  if (button.isPressedRaw()) {
    Serial.println(F("setup(): button was pressed while booting"));
  }

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  button.check();
}

