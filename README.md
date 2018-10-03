# QdScope
A quick and dirty 'scope, logic analyser, frequency counter etc. for Arduino Nano

You can probably hack this to work on other Arduino boards.

All output is via the serial port - I know it isn't ideal, but I did say it's quick'n'dirty. Serial output takes a chunk of time during which the capture functions aren't running.

Early version has a 4-channel logic capture - basic logic analyser.
Coming soon: frequency counter
Later maybe: analog inputs
All for low-frequency work. If you want high frequency and/or accuracy, go and buy or borrow some real gear. :-)
