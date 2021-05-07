/*
 * A simple loop to print out the value of the analog pin when buttons are
 * pressed. The buttons are connected to a single pin using a resistor ladder.
 * See examples/LadderButtons to see how to configure the LadderButtonConfig to
 * detect button events.
 */

#include <stdint.h>
#include <Arduino.h>

// Define the actual ADC pin.
static const uint8_t BUTTON_PIN = A0;

// Call analogRead() every 5 ms (which is how fast
// LadderButtonConfig::checkButtons() should be called), and just print the
// value. Click on the various buttons to see the actual value is returned by
// each button.
void calibrateAnalogRead() {
  static uint16_t prevMillis;

  uint16_t nowMillis = millis();
  if (nowMillis - prevMillis >= 5) {
    prevMillis = nowMillis;

    uint16_t val = analogRead(BUTTON_PIN);
    Serial.println(val);
  }
}

//-----------------------------------------------------------------------------

void setup() {
  delay(1000); // some microcontrollers reboot twice

  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // Don't use internal pull-up resistor because it will change the effective
  // resistance of the resistor ladder.
  pinMode(BUTTON_PIN, INPUT);

  Serial.println(F("setup(): ready"));
}

void loop() {
  calibrateAnalogRead();
}
