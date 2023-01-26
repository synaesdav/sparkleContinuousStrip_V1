# sparkleContinuousStrip_V1
Millis controlled sparkles on a NeoPixel strip. Scalable to any size strip. Individual timing on sparkles.

A sketch to explore using timing with millis to create a sparkle effect. Each sparkle is broken up into 4 stages: ramp up, hold at full, trail off, and pause between sparkles. Each stage has variable timing. Set the timing ranges for each stage in getRandom(). The variable colorStyle is used when calling setSparkles(colorStyle) and passed through to getRandom(colorStyle) to determine which color profile is used. Colors are gamma corrected.
