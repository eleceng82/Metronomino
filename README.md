# Metronomino
An arduino based metronome

Metronomino is an arduino sketch that show you how to use millis function, toneAC library an other accessory function to obtain a 
semi-professional metronome. This project reproduces tone on a 0.5 W speaker and thanks to toneAC library you can hear tone
more loud respect to internal arduino tone library.
Additionally you can visualize beats by two led, the red led blinks on strong beat, while the green led blinks on weak beats; tone plays 
according to beats in fact you hear a 1000 Hz tone during strong beat and 500 Hz tone during weak beat.
Before upload code on the board you have to copy toneAC library in the libraries folder.
This project can be modified to function with others arduino boards or custom board as you prefer.
