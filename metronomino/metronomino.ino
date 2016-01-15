/*
  Metronomino
  An arduino based metronome.

  This sketch show you how to make a simple metronome using Arduino UNO board, toneAC library and built in millis function
  This project can be replicated on other Arduino boards (Leonardo, Mega etc) but keep in mind that connections change consequently
  
  This example code is in the public domain.

  Sketch written by Pasquale Barrega on January 12nd 2016
 */
 
 /*----------------------------Connections for this project on Arduino UNO -----------------------------------------*/
 /* Speaker + to pin 9
 *  Speaker - to pin 10
 *  4.7K resistor between pin 2 ang GND (TEMPO UP input)
 *  4.7K resistor between pin 3 and GND (TEMPO DOWN input)
 *  4.7K resistor between pin 8 and GND (BEATS CHANGE input)
 *  One switch between pin 2 and +5V pin (TEMPO UP switch)
 *  One switch between pin 3 and +5V pin (TEMPO DOWN switch)
 *  One switch between pin 8 and +5V pin (BEATS CHANGE switch)
 *  One red LED between pin 4 by 330 Ohm series resistor and GND
 *  One green LED between pin A0 by 330 Ohm series resistor and GND
 * Pin 1 of Robot display to +5V pin
 * Pin 2 of Robot display to pin 12
 * Pin 3 of Robot display to pin 13
 * Pin 4 of Robot display to pin 11
 * Pin 5 of Robot display to pin 7
 * Pin 6 of Robot display No connection
 * Pin 7 of Robot display to pin 6
 * Pin 8 of Robot display to pin 5
 * Pin 9 of Robot display to +5V pin
 * Pin 10 of Robot display to GND pin
 
 /*------------------------------------ end Connections on Arduino UNO ----------------------------------------------*/
 


#include <toneAC.h>
#include <TFT.h>
#include <SPI.h>
#include <math.h>
#include "define.h"

unsigned int play_tone=0,setted=0,led_time=80,turn_led=0,beats=0,beats_count=0,bpm=0,startup=1,strong_led=0,changed=0,first=1;
unsigned long int curr_time=0,now=0;

void check_time(unsigned int msec);
void start_screen(void);
void show(void);

TFT TFTscreen = TFT(cs, dc, rst);  //display object declaration

void setup() 
{
  
  /*------------- display init ---------------------*/
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255, 255, 255);
  /*-------------- end display init ----------------*/
  start_screen(); //show startup screen
  delay(8000);
  TFTscreen.background(0, 0, 0);  //clear display
  
  /*------------------ I/O pin init -----------------*/
  pinMode(STRONG_LED_PIN,OUTPUT);
  pinMode(WEAK_LED_PIN,OUTPUT);
  pinMode(TEMPO_UP_PIN,INPUT);
  pinMode(TEMPO_DOWN_PIN,INPUT);
  pinMode(BEATS_CHANGE_PIN,INPUT);
  digitalWrite(STRONG_LED_PIN,LOW);
  digitalWrite(WEAK_LED_PIN,LOW);
  /*------------------ end I/O pin init ---------------*/
  
  bpm = START_BPM;
  beats = START_BEATS;
 }

void loop() {
  //now=millis();
  show();  //view tempo and beats on display
  check_switches();  //poll switches status
  if(play_tone)  //check if it's time to play tone
  {
      if(beats_count < beats) //check beats_count value
      {
        if(beats_count==0)
        {
          strong_led=1; //if beats count = 0 enable strong led and play high tone
          toneAC(1000,10,TONE_DURATION,true);
        }  
       else 
       {
          strong_led=0; //otherwise disable strong led and play low tone
          toneAC(500,10,TONE_DURATION,true);
       }  
       beats_count++;
       if(beats_count >= beats) beats_count=0;
     }
    
  } 
  
  if(turn_led) //check if it's time to turn on led
  {
      if(strong_led)
      {
        digitalWrite(STRONG_LED_PIN,HIGH);
      }  
      else if(strong_led == 0)
      {
        digitalWrite(WEAK_LED_PIN,HIGH);
      }  
  } 
  else
  {
    digitalWrite(WEAK_LED_PIN,LOW);
    digitalWrite(STRONG_LED_PIN,LOW);
  }  
  
  check_time(bpm);  //check time (by millis function) according to the parameter passed
}


void check_time(unsigned int bpm)
{
  unsigned int msec=0;
  
  msec=floor((1.0 / (bpm / 60.0)) * 1000.0) - 35;  //converts bpm to milliseconds
  //Serial.println(msec);
  if(setted==0)
  {
    play_tone=0;
    if(startup == 1) turn_led=0;
    else turn_led=1;
    curr_time=millis();
    setted=1;
  }
  else
  {
    if((millis()-curr_time) >= LIGHT_DURATION)
    {
      
      turn_led=0;
    } 
     if((millis() - curr_time) >= msec)
    {
      curr_time=0;
      setted=0;
      play_tone=1;
      startup=0;
    
    }
    else return;
   } 
     
return;
}



void start_screen(void)  //function resonsible to show startup screen
{
  TFTscreen.setTextSize(2);
  TFTscreen.text("Metronomino\n ", 15, 10);
  TFTscreen.text("An Arduino",20,25);
  TFTscreen.text("    based  ",5,40);
  TFTscreen.text("  metronome  ",5,55);
  TFTscreen.text("    by    ",20,70);
  TFTscreen.text("Pasquale",40,85);
  TFTscreen.text("Barrega",45,100);
  //TFTscreen.text("A metronome based on Arduino\n ", 10, 60);

}

void show(void)  //function responsible to show current tempo and beats data
{
  char tempo[4];
  char div[2];
  
  String vel = String(bpm);
  String measures = String(beats);
  vel.toCharArray(tempo, 4);
  measures.toCharArray(div,2);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.setTextSize(3);
  TFTscreen.text("Tempo ",0,0);
  TFTscreen.setTextSize(4);
  if (changed ==1)
  {
    TFTscreen.background(0, 0, 0);
    delay(250);
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.setTextSize(3);
    TFTscreen.text("Tempo ",0,0);
    TFTscreen.setTextSize(4);
    TFTscreen.text(tempo,0,30);
    TFTscreen.setTextSize(2);
    TFTscreen.text("Beats ", 80, 70);
    TFTscreen.setTextSize(3);
    TFTscreen.text(div,100,90);
    changed =0;
  }  
  else if(first ==1)
  {
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.setTextSize(3);
    TFTscreen.text("Tempo ",0,0);
    TFTscreen.setTextSize(4);
    TFTscreen.text(tempo, 0, 30);
    TFTscreen.setTextSize(2);
    TFTscreen.text("Beats ", 80, 70);
    TFTscreen.setTextSize(3);
    TFTscreen.text(div,100,90);
    first=0;
   }
  else; 
} 
 
 

void check_switches(void)  //function responsible to poll switches status
{
  if(digitalRead(TEMPO_UP_PIN))
  {
    changed = 1;
    delay(WAIT_SWITCH);
    if(bpm < 208) bpm=bpm+1;
    else if(bpm == 208) bpm=208;
    else;
  }

  else if(digitalRead(TEMPO_DOWN_PIN))
  {
    changed = 1;
    delay(WAIT_SWITCH);
    if(bpm > 40) bpm=bpm-1;
    else if(bpm == 40) bpm=40;
    else;
  }
  
  else if(digitalRead(BEATS_CHANGE_PIN))
  {
    changed = 1;
    delay(WAIT_SWITCH);
    beats_count = 0;
    strong_led=0;
    if(beats < 7) beats=beats+1;
    else if(beats == 7) beats = 0;
    
  }  
  else
  {
    changed = 0;
    
  }  
}  
