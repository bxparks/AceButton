/*
   Copyright 2018 Brian T. Park

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef EVENT_TRACKER_H
#define EVENT_TRACKER_H

namespace ace_button {
namespace testing {

/**
 * A record of an AceButton event, for testing purposes.
 */
class EventRecord {
  public:
    EventRecord():
        mEventType(0),
        mButtonState(LOW) {}

    EventRecord(uint8_t eventType, uint8_t buttonState):
        mEventType(eventType),
        mButtonState(buttonState) {}

    uint8_t getEventType() {
      return mEventType;
    }

    uint8_t getButtonState() {
      return mButtonState;
    }

  private:
    uint8_t mEventType;
    uint8_t mButtonState;
};

/**
 * Class that can receive and remember multiple calls to the eventHandler from
 * AceButton.
 */
class EventTracker {
  public:

    EventTracker():
        mNumEvents(0) {}
      
    /** Add event to a circular buffer of records. */
    void addEvent(uint8_t eventType, uint8_t buttonState) {
      mRecords[mNumEvents] = EventRecord(eventType, buttonState);
      mNumEvents++;
      if (mNumEvents >= kMaxEvents) {
        mNumEvents = 0;
      }
    }

    void clear() { mNumEvents = 0; }

    int getNumEvents() { return mNumEvents; }

    EventRecord& getRecord(int i) { return mRecords[i]; }

  private:
    static const int kMaxEvents = 5;

    EventRecord mRecords[kMaxEvents];
    int mNumEvents;
};

}
}
#endif
