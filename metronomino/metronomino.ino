/*
  Metronomino V2
  An arduino based metronome.

  This sketch show you how to make a simple metronome using Arduino UNO board, toneAC library and built in millis function
  This project can be replicated on other Arduino boards (Leonardo, Mega etc) but keep in mind that connections change consequently.
  
  In version 2 I use a 128x64 graphic display precisely the display board for the reprap/prusa 3d printer wich integrates also an incremental encoder, a buzzer and a push button.
  For this application I use display and encoder.
  
  This example code is in the public domain.

  Sketch written by Pasquale Barrega on August 17th 2016
 */
 
 


#include <toneAC.h>
#include "U8glib.h"
#include <math.h>
#include "define.h"

unsigned int play_tone=0,setted=0,led_time=80,turn_led=0,beats=0,beats_count=0,bpm=0,startup=1,strong_led=0,changed=0,first=1;
unsigned long int curr_time=0,now=0;

void check_time(unsigned int msec);
void start_screen(void);
void show(void);

U8GLIB_ST7920_128X64_1X u8g(13, 11, 7);	// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 7

void setup() 
{
  /*------------------ I/O pin init -----------------*/
  pinMode(STRONG_LED_PIN,OUTPUT);
  pinMode(WEAK_LED_PIN,OUTPUT);
  digitalWrite(STRONG_LED_PIN,LOW);
  digitalWrite(WEAK_LED_PIN,LOW);
  /*------------------ end I/O pin init ---------------*/
  u8g.setFont(u8g_font_6x10);
  u8g.firstPage();  
  do {
    start_screen();  //show start screen
  } while( u8g.nextPage() );
  
  delay(WAIT_START);
  
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
      u8g.setPrintPos(20, 8);
      u8g.print("Metronomino V2");
      u8g.setPrintPos(12,20);
      u8g.print("An Arduino based");
      u8g.setPrintPos(36,32);
      u8g.print("Metronome");
      u8g.setPrintPos(56,44);
      u8g.print("By");
      u8g.setPrintPos(16,56);
      u8g.print("Pasquale Barrega");

}

void show(void)  //function responsible to show current tempo and beats data
{
   u8g.firstPage();  
  do {
      u8g.setPrintPos(12,20);
      u8g.print("Prova");
  } while( u8g.nextPage() );   
      
    
} 
 
 

void check_switches(void)  //function responsible to poll switches status
{
  
  
  
}  
