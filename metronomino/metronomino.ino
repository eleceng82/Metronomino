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
#include <Encoder.h>


//#define ENCODER_DO_NOT_USE_INTERRUPTS

unsigned int play_tone=0,setted=0,led_time=80,turn_led=0,beats=0,beats_count=0,bpm=0,startup=1,strong_led=0,changed=0,first=1,volume=0;
unsigned int show_circle=0,item=0;
unsigned long int curr_time=0,now=0;



volatile int lastEncoded = 0;
volatile long encoderValue = 0;
 
long lastencoderValue = 0;
 
int lastMSB = 0;
int lastLSB = 0;

void check_time(unsigned int msec);
void start_screen(void);
void show(void);

Encoder myEnc(5, 6);
U8GLIB_ST7920_128X64_1X u8g(13, 11, 7);	// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 7

void setup() 
{
  /*------------------ I/O pin init -----------------*/
  pinMode(STRONG_LED_PIN,OUTPUT);
  pinMode(WEAK_LED_PIN,OUTPUT);
  digitalWrite(STRONG_LED_PIN,LOW);
  digitalWrite(WEAK_LED_PIN,LOW);
  pinMode(CONFIRM_PIN,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  /*------------------ end I/O pin init ---------------*/
  
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
  
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
  long int newpos = myEnc.read();
  if(item==1) upd_val(40, 208, item);
  else if(item==2) upd_val(0, 7, item);
  else if(item==3) upd_val(0, 10, item);
   
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
      
      switch(item)
      {
       case 1:
         u8g.setPrintPos(28,18);
         u8g.print('O');
         break;
       case 2:
         u8g.setPrintPos(28,38);
         u8g.print('O');
         break;
       case 3:
         u8g.setPrintPos(28,60);
         u8g.print('O');
         break;  
       default:
        break;
      }  
      
      if(show_circle==CIRCLE_STRONG) u8g.drawDisc(44,5,4);
      else if(show_circle==CIRCLE_WEAK) u8g.drawDisc(120,5,4); 
       
  } while( u8g.nextPage() );   
      
    
} 
 
 

void check_switches(void)  //function responsible to poll switches status
{
  unsigned int c=0;
  
  if(digitalRead(CONFIRM_PIN)==0)
  {
    delay(WAIT_SWITCH);
    if(item < 3) item++;
    else item=0;
  }  
    
} 

void upd_val(unsigned int min_value, unsigned int max_value, unsigned int parameter)
{
  int enc;
  
  switch(parameter)
  {
    case 1:
     //Serial.println(enc);
     
       
       if (encoderValue <=min_value) bpm=min_value;
       else if(encoderValue > max_value) bpm=max_value;
       else bpm=encoderValue;
       
     break;
    
    case 2:
       enc=map(encoderValue,0,255,min_value,max_value);
       if(enc <=min_value) beats=min_value;
       else if(enc > max_value) beats=max_value;
       else beats=enc;
       
     break;
    case 3:
       enc=map(encoderValue,0,255,min_value,max_value);
       if(encoderValue <=min_value) volume=min_value;
       else if(encoderValue > max_value) volume=max_value;
       else volume=encoderValue;
       
     break;
    default:
     break;
   }  
  
  
} 

void updateEncoder(){
  int MSB = digitalRead(2); //MSB = most significant bit
  int LSB = digitalRead(3); //LSB = least significant bit
 
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if((sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) && (encoderValue < 255)) encoderValue ++;
  if((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && (encoderValue > 0)) encoderValue --;
 
  lastEncoded = encoded; //store this value for next time
}


/*long newPos = myEnc.read();
  if (newPos != position) {
    position = newPos;*/
