# Resistor Ladder Buttons

The `LadderButtons.ino` is a sample code for `LadderButtonConfig` which
supports multiple buttons on a single analog pin. The buttons are distinguished
by using a resistor ladder that sets the voltage to different values. The
voltage can be read using the `analogRead()` function.

This program demonstrates 4 buttons using 4 resistors:

* 1 x SparkFun Pro Micro (clone)
* 4 x push buttons
* 1 x 4.7 kohm resistor
* 2 x 10 kohm resistor
* 1 x 47k kohm resistor

## Sample Output

The sample output shows where each button was Pressed then Released. Blank lines
were added for readability:

```
handleEvent(): virtualPin: 0; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 0; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 1; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 1; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 2; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 2; eventType: 1; buttonState: 1

handleEvent(): virtualPin: 3; eventType: 0; buttonState: 0
handleEvent(): virtualPin: 3; eventType: 1; buttonState: 1
```
