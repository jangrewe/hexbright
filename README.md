HexBright Flex Firmware
=========

There are lots of HexBright Flex firmwares out there, and all have some particular features that i like, but none of them has all that i want, so i set out to build my own.

This was very easy because of the awesome [framework](https://github.com/dhiltonp/hexbright) built by the [Daniel Hilton](https://github.com/dhiltonp), as it takes all the low-level hardware interaction and hides it far away, presenting you with a nice and intelligent interface to your HexBright Flex, so you can concentrate on implementing the features you want.

Platypus
-------------

My first version, codename "Platypus", allows you to cycle through 5 brightness levels (including a very eco-friendly 10 lumen mode) and switch to 4 non-static modes (fade, blinking, strobe, dazzle).

* **Off -> Click = Static**
* Click to cycle through the brightness levels (10, 250, 500, 750, 1000, 750, 500, 250, 10 and up again)
* Short Hold (500-2000 ms) to switch to non-static mode
* **Off -> Short Hold (500-2000 ms) = Non-Static**
* Click to cycle through non-static modes (blinking, strobe, dazzle, fade)
* Short Hold (500-2000 ms) to switch to static mode
* **Long Hold (>2000 ms) = Off**

When switching from Static to Non-Static mode, you don't get the Blinking if you hold the button longer than 500 ms unless you let it go, this was done so that it doesn't start Blinking when switching off. You can enable this by reading the comments in the code and changing the appropriate lines.

When switching from Non-Static mode to Off, the light will go into Static mode for 1500 ms (from 500 ms to 2000 ms) on the last brightness level used in Static mode. This can be disabled by reading the comments in the code and changing the appropriate lines.
