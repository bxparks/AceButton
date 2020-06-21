/*
MIT License

Copyright (c) 2020 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "LadderButtonConfig.h"
#include "AceButton.h"

namespace ace_button {

LadderButtonConfig::LadderButtonConfig(
    uint8_t pin,
    uint8_t numLevels,
    const uint16_t levels[],
    uint8_t numButtons,
    AceButton* const buttons[],
    uint8_t defaultReleasedState
):
    mPin(pin),
    mNumLevels(numLevels),
    mNumButtons(numButtons),
    mPressedState(defaultReleasedState ^ 0x1),
    mLevels(levels),
    mButtons(buttons)
{
  for (uint8_t i = 0; i < mNumButtons; i++) {
    AceButton* button = mButtons[i];
    button->setButtonConfig(this);
  }

  // TODO: Verify that the levels[] are monotonically increasing.
}

int LadderButtonConfig::readButton(uint8_t pin) {
  uint8_t virtualPin = getVirtualPin();
  return (virtualPin == pin) ? mPressedState : (mPressedState ^ 0x1);
}

void LadderButtonConfig::checkButtons() const {
  uint8_t virtualPin = getVirtualPin();

  for (uint8_t i = 0; i < mNumButtons; i++) {
    AceButton* button = mButtons[i];
    if (button == nullptr) continue;

    // For each button, call checkState() to allow it to figure out which
    // state it should move to.
    uint8_t buttonPin = button->getPin();
    uint8_t buttonState = (buttonPin == virtualPin)
        ? mPressedState : (mPressedState ^ 0x1);
    button->checkState(buttonState);
  }
}

uint8_t LadderButtonConfig::getVirtualPin() const {
  uint16_t level = analogRead(mPin);

  uint8_t i;
  for (i = 0; i < mNumLevels - 1; i++) {
    // NOTE(brian): This will overflow a 16-bit ADC. If we need to support that,
    // a possible formula that avoids uint32_t instructions might be something
    // like:
    //
    //    threshold = mLevels[i]/2 + mLevels[i+1]/2
    //        + (((mLevels[i] & 0x1) + (mLevels[i+1] & 0x1)) / 2)
    uint16_t threshold = (mLevels[i] + mLevels[i+1]) / 2;
    if (level < threshold) return i;
  }

  // Return the index of the last mLevel, to indicate "nothing found".
  return i;
}

}
