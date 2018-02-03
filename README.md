# An Adjustable Compact Event-driven (ACE) Button Library for Arduino

## Summary

This library provides classes which accept inputs from a mechanical button
connected to a digital input pin on the Arduino. The library is able to handle
momentary buttons, maintained buttons, and switches. For sake of simplicity, we
will refer to all of these as "buttons" from now on.

The library is called the ACE Button Library (or AceButton Library) because:

* many configurations of the button is **adjustable**, either at compile-time or
  run-time
* the library has been optimized to create **compact** objects which take up
  a minimal amount of static memory
* the library detects changes in the button state and sends **events** to
  a user-defined `EventHandler` callback function

Most of the features of the library can be accessed through 2 classes and
1 callback function:

* `AceButton` (class)
* `ButtonConfig` (class)
* `EventHandler` (typedef)

The `AceButton` class contains the logic for debouncing and determining if a
particular event has occurred. The `ButtonConfig` class holds various timing
parameters, the event handler, code for reading the button, and code for
getting the internal clock. The `EventHandler` is a user-defined callback
function with a specific signature which is registed with the `ButtonConfig`
object. When the library detects interesting events, the callback function is
called by the library, allowing the client code to handle the event.

The supported events are:

* `AceButton::kEventPressed`
* `AceButton::kEventReleased`
* `AceButton::kEventClicked`
* `AceButton::kEventDoubleClicked`
* `AceButton::kEventLongPressed`
* `AceButton::kEventRepeatPressed`

(TripleClicked is not supported but can be easily added to the library if
requested.)

### Features

Here are the high-level features of the AceButton library:

* debounces the mechnical contact
* handles both pull-up and pull-down wiring
* event-driven through a user-defined `EventHandler` callback funcition
* supports 6 event types:
    * Pressed
    * Released
    * Clicked
    * DoubleClicked
    * LongPressed
    * RepeatPressed
* configurations are adjustable at runtime or compile-time
  * timing parameters
  * `digitalRead()` button read function can be overridden
  * `millis()` clock function can be overridden
* small memory footprint
  * each AceButton consumes 14 bytes
  * each ButtonConfig consumes 5 bytes
  * one System ButtonConfig instance created automatically by the library
* thoroughly unit tested using
  [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
* properly handles reboots while the button is pressed
* properly handles orphaned clicks, to prevent spurious double-clicks
* only 15 microseconds per polling call to `AceButton::check()`

Compared to other Arduino button libraries, I think the unique or exceptional
features of the AceButton library are:

* low memory usage
* thorough unit testing
* proper handling of orphaned clicks
* proper handling of a reboot while button is pressed

### HelloButton

Here is a simple program (see `examples/HelloButton.ino`) which controls
the builtin LED on the Arduino board using a momentary button connected
to PIN 2.

```
#include <AceButton.h>

using namespace ace_button;

const uint8_t BUTTON_PIN = 2;

AceButton button(BUTTON_PIN);

setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
}

loop() {
  button.check();
}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWrite(LED_BUILTIN, HIGH); // turn the LED on
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_BUILTIN, LOW); // turn the LED off
      break;
  }
}

```

### Non-goals

An Arduino UNO or Nano has 16 times more flash memory (32KB) than static memory
(2KB), so the library is optimized to minimize the static memory usage. The
AceButton library is not optimized to create a small program size (i.e. flash
memory), or for small CPU cycles (i.e. high execution spped). I assumed that if
you are seriously optimizing for program size or CPU cycles, you will probably
want to write everything yourself from scratch.

That said, the __Stopwatch.ino__ example sketch shows that the call to
`AceButton::check()` (which should be called at least every 10-20 milliseconds
from `setup()`) takes only 15 microseconds on a 16MHz ATmega328P chip in the
idel case. Hopefully that is fast enough for the vast majority of people.

## Installation

The library can be installed from the Arduino IDE Library Manager.
Search for "AceButton". Click install.

It can also be installed by cloning the
[GitHub repository](https://github.com/bxparks/AceButton), then
manually copying over the contents to your `./libraries` directory.

### Examples

The following example sketches are provided:

* HelloButton.ino
  * minimal program that reads a switch and controls the built-in LED
* SingleButton.ino
  * controls a single button wired with a pull-up resister
* SingleButtonPullDown.ino
  * same as SingleButton.ino but with an external pull-down resister
* TunerButtons.ino
  * 5 radio buttons (tune-up, tune-down, and 3 presets)
* Stopwatch.ino
  * measures the speed of `AceButton:check()` with a start/stop/reset button

## Usage

There are 2 classes and one typedef that a user will normally interact with:

* `AceButton` (class)
* `ButtonConfig` (class)
* `EventHandler` (typedef)

We explain how to use these below.

### Include Header and Use Namespace

To prevent name clashes with other libraries that the calling code may use, all
classes are defined in the `ace_button` namespace. To use the code without
prepending the `ace_button::` prefix, use the `using` directive:

```
#include <AceButton.h>
using namespace ace_button;
```

If you are dependent on just `AceButton`, the following might be sufficient:

```
#include <AceButton.h>
using ace_button::AceButton;
```

### AceButton Class

Each physical button will be handled by an instance of `AceButton`. At a
minimum, the instance needs to be told the pin number of the button. This can
be done through the constructor:

```
const uint8_t BUTTON_PIN = 2;

AceButton button(BUTTON_PIN);
```

Or we can use the `init()` method in the `setup()`:

```
AceButton button;

void setup() {
  button.init(BUTTON_PIN);
  ...
}
```

Both the constructor and the `init()` function take 3 optional parameters:
```
AceButton(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH, uint8_t id = 0);

void init(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH, uint8_t id = 0);
```

* `pin`: the I/O pin number assigned to the button
* `defaultReleasedState`: the logical value of the button when it is in its
  default "released" state (`HIGH` using a pull-up resister,
  `LOW` for a pull-down pull-down resister)
* `id`: an optional,user-defined identifier for the the button,
  for example, an index into an array with additional information

The `pin` must be defined. But the other two may be optional in many cases.

### ButtonConfig Class

The core concept of the AceButton library is the separation of the
button (`AceButton`) from its configuration (`ButtonConfig`).

* The `AceButton` class has the logic for debouncing and detecting the various
  events (Pressed, Released, etc), and the various bookkeeping variables
  needed to implement the logic. These variables are associated with the
  specific instance of that `AceButton`.
* The `ButtonConfig` class has the various timing parameters which control
  how much time is needed to detect certain events. This class also has the
  ability to override the default methods for reading the pin (`digitalRead()`)
  and the clock (`millis()`). This ability allows unit tests to be written.

The `ButtonConfig` can be created simply:

```
ButtonConfig buttonConfig;
```

and assigned to one or more `AceButton` instances:

```
AceButton button1;
AceButton button2;

void setup() {
  button1.setButtonConfig(buttonConfig);
  button2.setButtonConfig(buttonConfig);
  ...
}
```

We explain below (see _Single Button Simplifications_ section below) that in
the simple case, we don't need to explicitly create an instance of
`ButtonConfig` or call `setButtonConfig()` because all instances of `AceButton`
automatically gets assigned an instance of the System ButtonConfig which is
automatically created by the library.

The `ButtonConfig` class provides a number of methods which are mostly
used internally by the `AceButton` class. The one method which is expected
to be used by the calling client code is `setEventHandler()` which
assigns the user-defined `EventHandler` callback fundtion to the `ButtonConfig`
instance. This is explained in more detail below in the
_EventHandler Callback_ section.

The `ButtonConfig` class uses `virtual` functions so that the user can
override various parameters. Normally we try to avoid virtual methods in an
embedded environment. But we wanted the ability to plug in different types of
`ButtonConfig` into the `AceButton` class, and this C++ feature solves this
problem very well. The cost is 2 extra bytes for the virtual table
pointer in each instance of `ButtonConfig`, and an extra CPU overhead during
the call to the virtual functions.

We have assumed that there is a 1-to-many relationship between a `ButtonConfig`
and the `AceButton`. In other words, multiple buttons will normally be
associated with a single configuration. Each `AceButton` has a pointer to an
instance of `ButtonConfig`. So the cost of separating the `ButtonConfig` from
`AceButton` is 2 bytes in each instance of `AceButton`. Note that this is
equivalent to adding virtual methods to `AceButton` (which would add 2 bytes),
so in terms of static RAM size, this is a wash.

The library is designed to handle multiple buttons, and it assumes that the
buttons are normally grouped together into a handful of types. For example,
consider the buttons of a car radio. It has several types of buttons:

* the tuner buttons (2, up and down)
* the preset buttons (6)
* the AM/FM band button (1)

In this example, there are 9 buttons, but only 3 instances of `ButtonConfig`
would be needed.

### EventHandler

The event handler is a callback function that gets called when the `AceButton`
class determines that an interesting event happened on the button. The
advantage of this mechanism is that all the complicated logic of determining
the various events happens inside the `AceButton` class, and the user will
normally not need to worry about the details.

#### EventHandler Signature

The event handler has the following signature:

```
typedef void (*EventHandler)(AceButton* button, uint8_t eventType,
    uint8_t buttonState);
```

The event handler is registered with the `ButtonConfig` object, not with the
`AceButton` object as you might expect.

```
void setEventHandler(EventHandler eventHandler);
```

The motivation for this design is to save static memory. If multiple buttons
are associated with a single `ButtonConfig`, then it is not necessary for every
button of that type to hold the same pointer to the `EventHandler` function. It
is only necessary to save that information once, in the `ButtonConfig` object.

#### EventHandler Parameters

The `EventHandler` function received 3 parameters from the `AceButton`:

* `aceButton`
  * pointer to the `AceButton` instance that generated this event
  * can be used to retrieve the `getPin()` or the `getId()`
* `eventType`
  * the type of this event given by the various `AceButton::kEventXxx` constants
* `buttonState`
  * the `HIGH` or `LOW` button state that generated this event

If you are using only a single button, then you should need to check
only the `eventType`.

#### One EventHandler

Only a single `EventHandler` per `ButtonConfig` is supported. An alternative
would have been to register a separate event handler for each of the 6
`kEventXxx` events. But each callback function requires 2 bytes of memory, and
it was assumed that in most cases, the calling client code would be interested
in only a few of these event types, so it seemed wasteful to allocate 12 bytes
when most of these would be unused. If the client code really wanted separate
event handlers, it can be easily emulated by invoking them through the main
event handler:

```
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton:kEventPressed:
      handleEventPressed(button, eventType, buttonState);
      break;
    case AceButton::kEventReleased:
      handleEventReleased(button, eventType, buttonState);
      break;
    ...
  }
}
```

#### EventHandler Tips

The Arduino runtime environment is single-threaded, so the `EventHandler` is
called in the middle of the `AceButton::check()` method, in the same thread as
the `check()` method. It is therefore important to write the `EventHandler`
code to run somewhat quickly, so that the delay doesn't negatively impact the
logic of the `AceButton::check()` algorithm. Since `AceButton::check()` should
run approximately every 10-20 ms, the user-provided `EventHandler` should run
somewhat faster than 10-20 ms. Given a choice, it is probably better to use the
`EventHandler` to set some flags or variables and return quickly, then do
additional processing from the `loop()` method.

Speaking of threads, the API of the AceButton Library was designed to work in a
multi-threaded environment, if that situation were to occur in the Arduino
world.

### Event Types

The supported events are defined by a list of constants in `AceButton`:

* `AceButton::kEventPressed`
* `AceButton::kEventReleased`
* `AceButton::kEventClicked`
* `AceButton::kEventDoubleClicked`
* `AceButton::kEventLongPressed`
* `AceButton::kEventRepeatPressed`

These values are sent to the `EventHandler` in the `eventType` parameter.

### ButtonConfig Feature Flags

There are 6 flags defined in `ButtonConfig` which can be used to
control the behavior of `AceButton` event handling:

* `ButtonConfig::kFeatureLongPress`
* `ButtonConfig::kFeatureRepeatPress`
* `ButtonConfig::kFeatureSuppressAfterClick`
* `ButtonConfig::kFeatureSuppressAfterDoubleClick`
* `ButtonConfig::kFeatureSuppressAfterLongPress`
* `ButtonConfig::kFeatureSuppressAfterRepeatPress`
* `ButtonConfig::kFeatureSuppressAll`

These constants are used to set or clear the given flag:

```
ButtonConfig* config = button.getButtonConfig();

config->setFeature(ButtonConfig::kFeatureLongPress);
config->setFeature(ButtonConfig::kFeatureRepeatPress);
...
config->clearFeature(ButtonConfig::kFeatureLongPress);
config->clearFeature(ButtonConfig::kFeatureRepeatPress);
...
if (config->isFeature(ButtonConfig::kFeatureLongPress)) {
  ...
}
if (config->clearFeature(ButtonConfig::kFeatureRepeatPress)) {
  ...
}
```

The meaning of these flags are described below.

#### Event Activation

Of the 6 event types, two are not active by default:

* `AceButton::kEventLongPressed`
* `AceButton::kEventRepeatPressed`

To receive these events, call `ButtonConfig::setFeature()` with the following
flags respectively:

* `ButtonConfig::kFeatureLongPress`
* `ButtonConfig::kFeatureRepeatPress`

It seems unlikely that both `LongPress` and `RepeatPress` events would be
useful at the same time, but both event types can be activated if you need it.

#### Event Suppression

Event types can be considered to be built up in layers, starting with the
lowest level primitive events: Pressed and Released. Higher level events are
built on top of the lower level events through various timing delays. When a
higher level event is detected, it is sometime useful to suppress the lower
level event that was used to detect the higher level event.

For example, a Clicked event requires a Pressed event followed by a Released
event within a `ButtonConfig::getClickDelay()` milliseconds (200 ms by
default). The Pressed event is always generated. If a Clicked event is
detected, we could choose to generate both a Released event and a Clicked
event, and this is the default behavior.

However, many times, it is useful to suppress the Released event if the Clicked
event is detected. The `ButtonConfig` can be configured to suppress these lower
level events. Call the `setFeature(feature)` method passing the various
`kFeatureSuppressXxx` constant:

* `ButtonConfig::kFeatureSuppressAfterClick`
  * suppresses the Released event after a Clicked event is detected
* `ButtonConfig::kFeatureSuppressAfterDoubleClick`
  * suppresses the Released event and the second Clicked event if a
    DoubleClicked event is detected (the first Clicked event cannot be
    suppressed, because we don't yet know if another click will be generated by
    the button)
* `ButtonConfig::kFeatureSuppressAfterLongPress`
  * suppresses the Released event if a LongPressed event is detected
* `ButtonConfig::kFeatureSuppressAfterRepeatPress`
  * suppresses the Released event after the last RepeatPressed event
* `ButtonConfig::kFeatureSuppressAll`
  * a convenience parameter that is the equivalent of suppressing all of the
    previous events

By default, no suppression is performed.

As an example, to suppress the `Released` event after a `LongPressed` event
(this is actually often the case), you would do this:

```
ButtonConfig* config = button.getButtonConfig();
config->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
```

A special convenient constant `kFeatureSuppressAll` is available that is
the equivalent of using all suppression constants:

```
ButtonConfig* config = button.getButtonConfig();
config->setFeature(ButtonConfig::kFeatureSuppressAll);
```

### Single Button Simplifications

Although the AceButton library is designed to shine for multiple buttons, you
may just want to handle one button. The library provides some features to make
the simple case easy.

1. The library automatically creates one instance of `ButtonConfig`
   called a "System ButtonConfig". This System ButtonConfig can be retrieved
   using the class static method `ButtonConfig::getSystemButtonConfig()`.
1. Every instance of `AceButton` is assigned an instance of the System
   ButtonConfig by default (which can be overridden manually).
1. A convenience method allows the `EventHandler` for the System
   ButtonConfig to be set easily through `AceButton` itself, instead of having
   to get the System ButtonConfig first, then set the event handler. In other
   words, `button.setEventHandler(handleEvent)` is a synonym for
   `button.getButtonConfig()->setEventHandler(handleEvent)`.

These simplifying features allow a single button to be configured and used like
this:

```
AceButton button(BUTTON_PIN);

setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
  ...
}

loop() {
  button.check();
}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  ...
}
```

To configure the System ButtonConfig, you may need to add something like
this to the `setup()` section:

```
  button.getButtonConfig()->setFeature(ButtonConfig::kFeatureLongPress);
```

### Multiple Buttons

When transitioning from a single button to multiple buttons, it's important to
remember what's happening underneath the convenience methods. The single
`AceButton` button is assigned to the System ButtonConfig that was created
automatically. When an `EventHandler` is assigned to the button, it is actually
assigned to the System ButtonConfig. All subsequent instances of `AceButton`
will also be associated with this event handler, unless another `ButtonConfig`
is explicitly assigned.

See the example sketch `TunerButtons.ino` to see how to use multiple
`ButtonConfig` instances with multiple `AceButton` instances.

### Events After Reboot

A number of edge cases occur when the the microcontroller is rebooted:

* if the button is held down, should the Pressed event be triggered?
* if the button is in its natural Released state, should the Released event
  happen?
* if the button is Pressed down, and `ButtonConfig` is configured to
  support RepeatPress events, should the `kEventRepeatPressed` events
  be triggered initially?

I think most users would expect that in all these cases, the answer is no, the
microcontroller should not trigger an event until the button undergoes a
human-initiated change in state. The AceButton library implements this logic.
(It might be useful to make this configurable using a `ButtonConfig` feature
flag but that is not implemented.)

### Orphaned Clicks

When a Clicked event is generated, the `AceButton` class looks for a
second Clicked event within a certain time delay (default 400 ms) to
determine if the second Clicked event is actually a DoubleClicked event.

All internal timestamps in `AceButton` are stored as `uint16_t`
(i.e. an unsigned integer of 16 bits) in millisecond units. A 16-bit
unsigned counter rolls over after 65536 iterations. Therefore, if the second
Clicked event happens between (65.636 seconds, 66.036 seconds) after the first
Clicked event, a naive-logic would erroneously consider the (long-delayed)
second click as a double-click.

The `AceButton` contains code that prevents this from happening.

Note that even if the `AceButton` class uses an `unsigned long` type (a 32-bit
integer on the Arduino), the overflow problem would still occur after `2^32`
milliseconds (i.e. 49.7 days). To be strictly correct, the `AceButton` class
would still need logic to take care of orphaned Clicked events.

## Resource Consumption

Here are the sizes of the various classes:

* sizeof(AceButton): 14
* sizeof(ButtonConfig): 5
* sizeof(AdjustableButtonConfig): 17
* sizeof(TestableButtonConfig): 11

(An early version of `AceButton`, with only half of the functionality, consumed
40 bytes. It got down to 11 bytes before additional functionality increased it
to 14.)

Program size:

* `HelloButton` sketch: 1972 bytes flash
* `HelloButton` sketch without `AceButton`: 622 bytes flash
* Therefore, the AceButton library: 1350 bytes flash

CPU cycles:

* 15 microseconds for AceButton::check()

## System Requirements

This library was developed using Arduino IDE 1.8.5 on an Arduino Nano clone
running at 16 MHz. The Nano uses an ATmega328P chip, with 32KB of flash, and
2KB of static RAM.

### Background Motivation

There are numerous "button" libraries out there for the Arduino. Why write
another one? I wanted to add a button to an addressable strip LED controller,
which was being refreshed at 120 Hz. I had a number of requirements:

* the button needed to support a LongPress event, in addition to the simple
  Press and Release events
* the button code must not interfere with the LED refresh code which was
  updating the LEDs at 120 Hz
* well-tested, I didn't want to be hunting down random and  obscure bugs

Since the LED refresh code needed to run while the button code was waiting for
a "LongPress" delay, it seemed that the cleanest API for a button library
would use an event handler callback mechanism. This reduced the number of
candidate libraries to a handful. Of these, only a few of them supported a
LongPress event. I did not find the remaining ones flexible enough for my
button needs in the future. Finally, I knew that it was tricky to write correct
code for debouncing and detecting various events (e.g. DoubleClick, LongPress,
RepeatPress). I looked for a library that contained unit tests, and I found
none.

I decided to write my own and use the opportunity to learn how to create and
publish an Arduino library.

## License

Apache License 2.0

## Author

Created by Brian T. Park (brian@xparks.net).
