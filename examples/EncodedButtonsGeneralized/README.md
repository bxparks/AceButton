# Encoded Buttons Generalized

The `EncodedButtonsGeneralized.ino` is a sample code for `EncodedButtonConfig`
which is a generalization of `Encoded4To2ButtonConfig` and
`Encoded8To3ButtonConfig`. These classes implement the
[BinaryEncoding](../../docs/binary_encoding/README.md) wiring.

The number of buttons that can be supported by `EncodedButtonConfig` is limited
by the amount of memory required for the instances of AceButton, but more
realistically, by the CPU time needed to execute
`EncodedButtonConfig.checkButtons()` which internally calls the
`AceButton.checkState()` for all the buttons. The amount of CPU time for the
`checkButtons()` call must be smaller than about 4-5ms to allow debouncing and
event detection to work.

This program is a demonstration of 16-to-4 encoding to support 15 buttons on a
breadboard using

* 2 x [74LS148](https://www.ti.com/product/SN74LS148) 8-line to 3-line
  priority encoder
* 1 x [74LS08](https://www.ti.com/lit/ds/symlink/sn54ls08.pdf) quad 2-input AND
  gate
* 1 x SparkFun Pro Micro (clone)
* 4 x push buttons

For demonstration purposes, instead of wiring up all 15 buttons, I
enabled only pins 1, 7, 8, and 15.

The schematic is:

![Schematic 74LS148](../../docs/binary_encoding/encoded_16to4_74ls148.png)

Here is the breadboard that implements this circuit:

![Breadboard 74LS148](breadboard_16to4_74ls148_small.jpg)

## Sample Output

The sample output looks like this (blank lines added for reaability):

```
handleEvent(): virtualPin: 1; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 1; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 7; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 7; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 8; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 8; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 15; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 15; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 1; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 1; eventType: 2; buttonState: 1
handleEvent(): virtualPin: 1; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 7; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 7; eventType: 2; buttonState: 1
handleEvent(): virtualPin: 7; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 8; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 8; eventType: 2; buttonState: 1
handleEvent(): virtualPin: 8; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 15; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 15; eventType: 2; buttonState: 1
handleEvent(): virtualPin: 15; eventType: 1; buttonState: 1
```
