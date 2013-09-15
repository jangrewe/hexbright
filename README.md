HexBright Flex Firmware
=========

There are lots of HexBright Flex firmwares out there, and all have some particular features that i like, but none of them has all that i want, so i set out to build my own.

This was very easy because of the awesome [library](https://github.com/dhiltonp/hexbright) built by [Daniel Hilton](https://github.com/dhiltonp), as it takes all the low-level hardware interaction and hides it far away, presenting you a nice and intelligent interface to your HexBright Flex, so you can concentrate on implementing the features you want.

Jackalope
-------------
The current version, codename "Jackalope", contains the same feature as "Platypus" (see a pattern there?) and adds quick access to the highest brightness level and Taps recording/playback.

**So in addition to all Platypus features (below), you can now also:**

<ul>
<li><b>Off -> Doubleclick (&lt;100 ms) = Static Mode, Maximum Brightness </b></li>
<ul>
<li>Also works in static mode, so you don't have to cycle through all levels</li>
</ul>
<li><b>Off -> Long Hold (&gt;2000 ms) = Taps Mode</b></li>
<ul>
<li>Holding the button, the red LED stays on and the green LED starts flashing rapidly when you are in Taps mode</li>
<li>Release the button, the red LED starts blinking and you can start tapping on the case</li>
<li>Click the button, the green LED starts blinking and plays back your taps</li>
<li>Clicking the button in Taps mode will switch between recording and playing mode</li>
<li>Use Long Hold (&gt;2000 ms) to switch off
<li>Please be aware that the LEDs are blocked by the charging/battery status while the Hexbright is connected via USB</li>
</ul>
</ul>

Platypus
-------------
My first version, codename "Platypus", allows you to cycle through 5 brightness levels (including a very eco-friendly 10 lumen mode) and switch to 4 non-static modes (fade, blinking, strobe, dazzle).

<ul>
<li><b>Off -> Click = Static Mode</b></li>
<ul>
<li>Click to cycle through the brightness levels (10, 250, 500, 750, 1000, 750, 500, 250, 10 and up again)</li>
<li>Short Hold (500-2000 ms) to switch to non-static mode</li>
</ul>
<li><b>Off -> Short Hold (500-2000 ms) = Non-Static Mode</b></li>
<ul>
<li>Click to cycle through non-static modes (blinking, strobe, dazzle, fade)</li>
<li>Short Hold (500-2000 ms) to switch to static mode</li>
</ul>
<li><b>Static/Non-Static Mode -> Long Hold (&gt;2000 ms) = Off</b></li>
</ul>

When switching from Static to Non-Static mode, you don't get the Blinking if you hold the button longer than 500 ms unless you let it go, this was done so that it doesn't start Blinking when switching off. You can enable this by reading the comments in the code and changing the appropriate lines.

When switching from Non-Static mode to Off, the light will go into Static mode for 1500 ms (from 500 ms to 2000 ms) on the last brightness level used in Static mode. This can be disabled by reading the comments in the code and changing the appropriate lines.
