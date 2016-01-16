# Metronomino
An arduino based metronome

Metronomino is an arduino sketch that show you how to use millis function, toneAC library an other accessory function to obtain a 
semi-professional metronome. This project reproduces tone on a 0.5 W speaker and thanks to toneAC library you can hear tone
more loud respect to internal arduino tone library.
Additionally you can visualize beats by two led, the red led blinks on strong beat, while the green led blinks on weak beats; tone plays 
according to beats in fact you hear a 1000 Hz tone during strong beat and 500 Hz tone during weak beat.
On the TFT display you can view current tempo and beats values.
To change tempo and beats value you can use three switches, two switches are used to increase or decrease tempo value and an other switch is used 
to change beats value.
Before upload code on the board you have to copy toneAC library in the libraries folder.

This project can be modified to function with others arduino boards or custom board as you prefer.

Files structure:

metronomino: arduino sketch for this project
toneAC: this folder contains library files for tone generation, install this library on arduino IDE before opening the sketch
Prototype_photos: this folder contains some photos of the prototype. 
