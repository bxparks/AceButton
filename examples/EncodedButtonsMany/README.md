# Encoded Buttons (Many)

The `EncodedButtonsMany.ino` is a sample code for `EncodedButtonConfig` which is
a generalization of `Encoded4To2ButtonConfig` and `Encoded8To3ButtonConfig`.
These classes implement the
[BinaryEncoding](../../docs/binary_encoding/README.md) wiring. This program is a
demonstration of 16-to-4 encoding to support 15 buttons on a
breadboard using 2 x [74LS148](https://www.ti.com/product/SN74LS148) chips.

## Encoding Using 74LS148

The schematic using 2 x 74LS148 chip:

![Schematic 74LS148](../../docs/binary_encoding/encoded_16to4_74ls148.png)

Here is the breadboard that implements this circuit:

![Breadboard 74LS148](breadboard_16to4_74ls148_small.jpg)
