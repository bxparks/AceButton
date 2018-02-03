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
        eventType_(0),
        buttonState_(LOW) {}

    EventRecord(uint8_t eventType, uint8_t buttonState):
        eventType_(eventType),
        buttonState_(buttonState) {}

    uint8_t getEventType() {
      return eventType_;
    }

    uint8_t getButtonState() {
      return buttonState_;
    }

  private:
    uint8_t eventType_;
    uint8_t buttonState_;
};

/**
 * Class that can receive and remember multiple calls to the eventHandler from
 * AceButton.
 */
class EventTracker {
  public:

    EventTracker():
        numEvents_(0) {}
      
    /** Add event to a circular buffer of records. */
    void addEvent(uint8_t eventType, uint8_t buttonState) {
      records_[numEvents_] = EventRecord(eventType, buttonState);
      numEvents_++;
      if (numEvents_ >= MAX_EVENTS) {
        numEvents_ = 0;
      }
    }

    void clear() { numEvents_ = 0; }

    int getNumEvents() { return numEvents_; }

    EventRecord& getRecord(int i) { return records_[i]; }

  private:
    static const int MAX_EVENTS = 5;

    EventRecord records_[MAX_EVENTS];
    int numEvents_;
};

}
}
#endif
