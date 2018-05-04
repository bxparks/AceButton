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

#ifndef ACE_BUTTON_EVENT_TRACKER_H
#define ACE_BUTTON_EVENT_TRACKER_H

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
    // Accept the default copy-constructor and assignment operator.
    //EventRecord(const EventRecord&) = delete;
    //EventRecord& operator=(const EventRecord&) = delete;

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
      
    /** Add event to a buffer of records, stopping when the buffer fills up. */
    void addEvent(uint8_t eventType, uint8_t buttonState) {
      if (mNumEvents < kMaxEvents) {
        mRecords[mNumEvents] = EventRecord(eventType, buttonState);
        mNumEvents++;
      }
    }

    void clear() { mNumEvents = 0; }

    int getNumEvents() { return mNumEvents; }

    EventRecord& getRecord(int i) { return mRecords[i]; }

  private:
    // Disable copy-constructor and assignment operator
    EventTracker(const EventTracker&) = delete;
    EventTracker& operator=(const EventTracker&) = delete;

    // Don't expect more than about 3. Set to 5 just in case.
    static const int kMaxEvents = 5;

    EventRecord mRecords[kMaxEvents];
    int mNumEvents;
};

}
}
#endif
