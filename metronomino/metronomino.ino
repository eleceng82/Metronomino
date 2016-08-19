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

unsigned int play_tone=0,setted=0,led_time=80,turn_led=0,beats=0,beats_count=0,bpm=0,startup=1,strong_led=0,changed=0,first=1,volume=0;
unsigned int show_circle=0;
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
  
  Serial.begin(9600);
  u8g.setFont(u8g_font_6x10);
  u8g.firstPage();  
  do {
    start_screen();  //show start screen
  } while( u8g.nextPage() );
  
  delay(WAIT_START);
  
  //bpm = START_BPM;
  bpm = 120;
  //beats = START_BEATS;
  beats =4;
  //beats=0;
  volume=START_VOLUME;
  show_circle=1;
 }

void loop() {
  //now=millis();
 
  show();  //view tempo and beats on display
  //if(show_circle == 1) show_circle++;
  //else show_circle=1;
  check_switches();  //poll switches status
  if(play_tone)  //check if it's time to play tone
  {
      if(beats_count < beats) //check beats_count value
      {
        if(beats_count==0)
        {
          strong_led=1; //if beats count = 0 enable strong led and play high tone
          show_circle=1;
          toneAC(1000,volume,TONE_DURATION,true);
        }  
       else 
       {
          strong_led=0; //otherwise disable strong led and play low tone
          if(show_circle == 1) show_circle++;
          else show_circle=1;
          toneAC(500,volume,TONE_DURATION,true);
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
        //show_circle=CIRCLE_STRONG;  //aggiunto
      }  
      else if(strong_led == 0)
      {
        digitalWrite(WEAK_LED_PIN,HIGH);
        //show_circle=CIRCLE_WEAK;  //aggiunto
      }  
  } 
  else
  {
    digitalWrite(WEAK_LED_PIN,LOW);
    digitalWrite(STRONG_LED_PIN,LOW);
  }  
  
  check_time(bpm);  //check time (by millis function) according to the parameter passed
  //Serial.println(millis()-now);
}


void check_time(unsigned int bpm)
{
  float msec=0;
  msec=(1.0 / (bpm / 60.0) * 1000.0)-125.0;  //converts bpm to milliseconds
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
     if((millis() - curr_time) >= (unsigned int)msec)
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
  int i=0;
   u8g.firstPage();  
  do {
      for(i=0; i < 64; i++)
      {
        u8g.drawPixel(35,i);
      }  
      
      for(i=0; i < 35; i++)
      {
        u8g.drawPixel(i,20);
      }  
      
      for(i=0; i < 35; i++)
      {
        u8g.drawPixel(i,42);
      } 
     
      for(i=63; i > 53; i--)
      {
        u8g.drawHLine(76, i,10);
      } 
      
      u8g.drawHLine(36, 12,92);
     
      u8g.setFont(u8g_font_5x8);     
      u8g.drawStr(4, 7, "TEMPO");  
      u8g.drawStr(4, 28, "BEATS");
      u8g.drawStr(2, 50, "VOLUME");
      if(bpm <= 99) u8g.setPrintPos(12,16);
      else u8g.setPrintPos(8,16);
      u8g.print(bpm);
      
      u8g.setPrintPos(13,38);
      u8g.print(beats);
      
      if(volume <= 9) u8g.setPrintPos(12,60);
      else u8g.setPrintPos(10,60);
      u8g.print(volume);
      
      if(show_circle==CIRCLE_STRONG) u8g.drawDisc(44,5,4);
      else if(show_circle==CIRCLE_WEAK) u8g.drawDisc(120,5,4); 
       
  } while( u8g.nextPage() );   
      
    
} 
 
 

void check_switches(void)  //function responsible to poll switches status
{
  
  
  
}  
