# Nixie Clock
Since I first discovered Nixie tubes in the early 2000s I've been fascinated by them and have always wanted to own a Nixie clock. Well, what better way than to build one yourself? Due to cost and cuteness, I've gone with the smallest Nixie tubes commonly available, IN-17s.

I seriously started doing the design for this around 2017 to distract myself from my thesis, and after making a rudimentary PCB in Eagle and a 2 tube breadboard prototype the project languished until 2023.

## Design
I'm going for a sandwich PCB design which I can do in stages to play around with various control schemes and other options. I'm going for a relatively standard approach of 74141 drivers, 74595 shift registeres, high side anode control for dimming, and an Arduino running it all with a mostly through-hole design. There are better ways with more modern components, but I'm doing this for A E S T H E T I C.

### Board 1 - Nixie Drivers
The first (and so far only) PCB made, this is where the Nixie tubes are soldered to along with the driver chips and anode current limiting resistors. I've had to make a few custom footprints in kicad since Eagle has gone the way of the dodo, but I really enjoyed this process.

Future revisions will probalby include the following improvements:
* Tighter layout of tubes
* Some rail capacitance
* Silkscreen on back?

## Code
It's relatively straightforward.
1. Get time from RTC module
2. Shift out time to 74595s
3. Do some fun dimming control
4. Repeat

I almost did some nasty stuff with pointers, and then decided I don't need that in my life. I might look at increasing PWM frequency for brightness control in future, but currently it works quite well.

## References
I stand on the shoulders of others, here are all the references I used for this project:
* [Dalibor's guidance on preventing cathode poisoning](https://www.manula.com/manuals/daliborfarny-com/nixie-tubes/1/en/topic/cathode-poisoning-prevention-routine)
* [High side driving with optoisolators](https://www.microfarad.de/nixie-clock/#gallery-10)
* [TLP627 datasheet](https://www.soemtron.org/downloads/disposals/tlp627.pdf)
    * [Local supplier](https://www.mantech.co.za/ProductInfo.aspx?Item=35M5983)
* [High side driving with MPSA92 & 42](https://reipooom.files.wordpress.com/2011/08/nixie-clock-from-jameco1.pdf)
    * [More arguments about this](https://electronics.stackexchange.com/questions/331220/anode-driver-for-nixie-clock)
* [Reference info on IN-17](https://www.swissnixie.com/tubes/IN17/)
* [Shift registers on Arduino](https://docs.arduino.cc/tutorials/communication/guide-to-shift-out)
* [Possible source for Soviet era 74141 drivers](https://tubehobby.com/show_det.php?det=35)
    * These use 2.5mm pinspacing instead of 2.54mm (1mili)
    * Modern sources are Integral from Belarus...