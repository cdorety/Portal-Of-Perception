

#include <Wire.h>
#include "sequences.h"
#include <Adafruit_NeoPixel.h>
#include <AVR/pgmspace.h>
//#include <WPROGRAM.H>
#include <Arduino.h>

#define STRAND 2

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 2

#define NUM_strip 270

#define BRIGHTNESS 255

int colorr, colorg, colorb, colorw;

int pos, dir;

uint32_t color[4] = {random(255), random(255), random(255), random(20)};   //no longer used


int larsenPause = 75;  delay used in the larsen function

// ********************************* variable for plasma pattern
float redStates[NUM_strip];
float blueStates[NUM_strip];
float greenStates[NUM_strip];
float whiteStates[NUM_strip];
float fadeRate = .4;



//OctoWS2811 Defn. Stuff
#define COLS_strip 60  // all of the following params need to be adjusted for screen size  **** not really used *****
#define ROWS_strip 16  // LED_LAYOUT assumed 0 if ROWS_strip > 8     **** not really used *****
//#define strip_PER_STRIP (COLS_strip * (ROWS_strip / 6))

//DMAMEM int displayMemory[strip_PER_STRIP*6];
//int drawingMemory[strip_PER_STRIP*6];
//const int config = WS2811_GRB | WS2811_800kHz;
//OctoWS2811 strip(strip_PER_STRIP, displayMemory, drawingMemory, config);

//Byte val 2PI Cosine Wave, offset by 1 PI 
//supports fast trig calcs and smooth LED fading/pulsing.
uint8_t const cos_wave[256] PROGMEM =  
{0,0,0,0,1,1,1,2,2,3,4,5,6,6,8,9,10,11,12,14,15,17,18,20,22,23,25,27,29,31,33,35,38,40,42,
45,47,49,52,54,57,60,62,65,68,71,73,76,79,82,85,88,91,94,97,100,103,106,109,113,116,119,
122,125,128,131,135,138,141,144,147,150,153,156,159,162,165,168,171,174,177,180,183,186,
189,191,194,197,199,202,204,207,209,212,214,216,218,221,223,225,227,229,231,232,234,236,
238,239,241,242,243,245,246,247,248,249,250,251,252,252,253,253,254,254,255,255,255,255,
255,255,255,255,254,254,253,253,252,252,251,250,249,248,247,246,245,243,242,241,239,238,
236,234,232,231,229,227,225,223,221,218,216,214,212,209,207,204,202,199,197,194,191,189,
186,183,180,177,174,171,168,165,162,159,156,153,150,147,144,141,138,135,131,128,125,122,
119,116,113,109,106,103,100,97,94,91,88,85,82,79,76,73,71,68,65,62,60,57,54,52,49,47,45,
42,40,38,35,33,31,29,27,25,23,22,20,18,17,15,14,12,11,10,9,8,6,6,5,4,3,2,2,1,1,1,0,0,0,0
};


//Gamma Correction Curve
uint8_t const exp_gamma[256] PROGMEM =
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,
4,4,4,4,4,5,5,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,11,11,12,12,12,13,13,14,14,14,15,15,
16,16,17,17,18,18,19,19,20,20,21,21,22,23,23,24,24,25,26,26,27,28,28,29,30,30,31,32,32,33,
34,35,35,36,37,38,39,39,40,41,42,43,44,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,
61,62,63,64,65,66,67,68,70,71,72,73,74,75,77,78,79,80,82,83,84,85,87,89,91,92,93,95,96,98,
99,100,101,102,105,106,108,109,111,112,114,115,117,118,120,121,123,125,126,128,130,131,133,
135,136,138,140,142,143,145,147,149,151,152,154,156,158,160,162,164,165,167,169,171,173,175,
177,179,181,183,185,187,190,192,194,196,198,200,202,204,207,209,211,213,216,218,220,222,225,
227,229,232,234,236,239,241,244,246,249,251,253,254,255
};


// ******************************************************begin thunder cloud definitions

#define LENGTH  270    // defines the number of LEDs in the system
#define SHORTLENGTH 15   //disregard this

int volume = 10; // out of 100   // this is the initializer value for the thunder pattern


int Pattern = 1;
int MaxPatterns = 2;
int MaxFrames = 256;
int MaxLoops = 3;
int currentFrame = 0;
int currentLoop = 0;
//int FrameStart = 0;
int temp = 0;
uint16_t hotspotS = 1;
uint16_t hotspotL = 1;
int hotspotCntr = 0;
uint32_t BlackColor = 0;
#define MAXANIMATIONS 1

#define MIC_PIN    A0  // Microphone is attached A0
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     0  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       (30 +1) // Allow dot to go slightly off scale
#define TRIGGER   1000 // volume for triggering a lightning strike

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
  
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 200;

int backgroundBrightness = 0;
int larsenColumnPause = 1;
int lightningOn = 0;
int lightningFrame = 0;
int lightningFrameMax = 5;
int lightningThreshold = 600;


// ******************************************************end thunder cloud definitions




// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel sstrip1 = Adafruit_NeoPixel(LENGTH, STRAND, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_strip, PIN, NEO_GRBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  pinMode(13, OUTPUT);
  sstrip1.begin();
  sstrip1.show(); // Initialize all pixels to 'off'
  strip.begin();
  strip.show();

}


unsigned long frameCount= 10000;


void loop() {
  
  
  
 for(int p = 0; p < 20000; p++){  // about 5 minutes
  processSound();

//  if (lvl > TRIGGER) {
//    if (lightningOn == 0) {
//      lightningOn = 1;
//      lightningFrame = 1;
//    }
//  }
  
  playPattern();
 }
  
  
  for (long p = 0 ; p < 25; p++) {            /// 15 minutes
    larsenColumn();
  }
  
  
  for (long p = 0 ; p < 270; p++) {     /// 45 minutes
     larsen();
  }
    
  
  for (long p = 0 ; p < 1350; p++) {     /// 45 minutes
     vibrate();
  }
  
  for(int w = 0; w < NUM_strip ; w++){ 
    strip.setPixelColor(w, 0, 0, 0, 0);
  }
  
  
    
  for (long p = 0 ; p < 330; p++) {       /// 45 minutes
     diagonalChase();
  }
  
  
 for( long p = 0; p < 270; p++){    //runs for about 45 minutes
  pulseWhite(larsenPause/8);
 }
  
  
  for (long p = 0 ; p < 12722; p++) {            // runs for about 45 minutes
    plasmaCraig();
}

  
  
  for (long p = 0 ; p < 120000; p++) {        /// 45 minutes
    craigSparkle();
  }
  
  for (long p = 0 ; p < 120000; p++) {            /// 45 minutes
    craigSparkleColor();
  }
  
  
//  for (int AnimCounter = 0; AnimCounter < MAXANIMATIONS; AnimCounter++) {
//    animationinfo(AnimCounter,&MaxFrames,&Loops,&FrameStart);
//    
//    for (int8_t i=0; i<Loops; i++) {
//      for (int8_t j=FrameStart; j<FrameStart+MaxFrames-1; j++) {
//        displayImage(&rgbBuf[j][0]);
//      }
//    }
//  }
  
  // setup the animation counter for the next time around
  //if (AnimCounter >= MAXANIMATIONS-1){AnimCounter=0;}else{AnimCounter++;}
  
  
  

//  for (int i = 0; i < 256*5; i++){
//    
//    newRainbowCycle(i, &lstrip1);
//    
//    temp = i + 4;
//    newRainbowCycle(temp, &lstrip2);
//    
//    temp = i + 17;
//    newRainbowCycle(temp, &sstrip1);
//    
//    temp = i + 12;
//    newRainbowCycle(temp, &sstrip2);
//    
//    lstrip1.show();
//    lstrip2.show();
//    sstrip1.show();
//    sstrip2.show();
//    
   //delay(20);
//  }

}

//*************************************************************************************************************************
void larsenColumn(){
  
 for (int m = 0; m < 90; m++){
  strip.setPixelColor(m, 0, 0, 0, 255);
  strip.setPixelColor(m+90, 0, 0, 0, 255);
  strip.setPixelColor(m+180, 0, 0, 0, 255);
  strip.show();
  delay(larsenColumnPause);
  strip.setPixelColor(m, 0, 0, 0, 0);
  strip.setPixelColor(m+90, 0, 0, 0, 0);
  strip.setPixelColor(m+180, 0, 0, 0, 0);
  strip.show();
 } 
 
  
  
}


//***********************************************************************************************************************
void larsen(){
  
 for(int i = 0; i < 15; i++){
   
   strip.setPixelColor(i, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   strip.setPixelColor(i+15, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i+15, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180+15, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   strip.setPixelColor(i+30, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i+30, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180+30, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   strip.setPixelColor(i+45, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i+45, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180+45, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   strip.setPixelColor(i+60, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i+60, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180+60, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   strip.setPixelColor(i+75, map(i, 0, 15, 0, 255), 20, 255, 0);   //(i, map(0, 255, 0, 15);
   strip.setPixelColor(104-i+75, map(i, 0, 15, 255, 0), 60, 100, 0);
   strip.setPixelColor(i+180+75, map(i, 0, 15, 255, 0), (i, 0, 15, 255, 0), (i, 0, 15, 0, 255), 0);
   
   
   strip.show();
   delay(map(cos_wave[i*17],  0, 255, larsenPause, 0));
   
 }
 
  for(int i = 14; i >= 0; i--){
   
   strip.setPixelColor(i+15, 0, 0, 0);
   strip.setPixelColor(104-i+15,0 , 0, 0);
   strip.setPixelColor(i+180+15, 0, 0, 0, 0);
   
   strip.setPixelColor(i+30, 0, 0, 0);
   strip.setPixelColor(104-i+30,0 , 0, 0);
   strip.setPixelColor(i+180+30, 0, 0, 0, 0);
   
   strip.setPixelColor(i+45, 0, 0, 0);
   strip.setPixelColor(104-i+45,0 , 0, 0);
   strip.setPixelColor(i+180+45, 0, 0, 0, 0);
   
   strip.setPixelColor(i+60, 0, 0, 0);
   strip.setPixelColor(104-i+60,0 , 0, 0);
   strip.setPixelColor(i+180+60, 0, 0, 0, 0);
   
   strip.setPixelColor(i+75, 0, 0, 0);
   strip.setPixelColor(104-i+75,0 , 0, 0);
   strip.setPixelColor(i+180+75, 0, 0, 0, 0);
   
   strip.setPixelColor(i, 0, 0, 0);
   strip.setPixelColor(104-i,0 , 0, 0);
   strip.setPixelColor(i+180, 0, 0, 0, 0);
   
   strip.show();
   delay(map(cos_wave[i*17], 0, 255, larsenPause, 0));
   
 } 
 
    
}
    
  
  
  //************************************************************************************************************************  
    
    
 void generateColor(){
   
   
      colorr = random(255);
      colorg = random(255);
      colorb = random(255);
      colorw = random(255);
   
 }
  
//***************************************************************************PULSE WHITE***************************************************************88
void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, j ) );
        }
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, j ) );
        }
        delay(wait);
        strip.show();
      }
}




//*************************************************************************************************************************



void vibrate(){
  
  generateColor();
 
  for(int w = 0; w < 90 ; w++){ 
    
    
    strip.setPixelColor(w, colorr, colorg, colorb, colorw);
   strip.setPixelColor(w+90, 0);
   strip.setPixelColor(w+180, 0);
   
  }
  
   strip.show();
    delay(100);
    
    for(int w = 0; w < 90 ; w++){ 
    
    
    strip.setPixelColor(w,0 , 0, 0, 0);
    strip.setPixelColor(w+90,colorr, colorg, colorb, colorw);
   strip.setPixelColor(w+180,0 , 0, 0, 0);
   
  }
  
   strip.show();
    delay(100);
    
     for(int w = 0; w < 90 ; w++){ 
    
    
    strip.setPixelColor(w,0 , 0, 0, 0);
    strip.setPixelColor(w+90,0 , 0, 0, 0);
    strip.setPixelColor(w+180,colorr, colorg, colorb, colorw);
   
   
  }
  
   strip.show();
    delay(100);
    
   
    
    for(int w = 0; w < 90 ; w++){ 
    
    
    strip.setPixelColor(w,0 , 0, 0, 0);
    strip.setPixelColor(w+90,colorr, colorg, colorb, colorw);
   strip.setPixelColor(w+180,0 , 0, 0, 0);
   
  }
    strip.show();
    delay(100);
}

//******************************************************DIAGONAL CHASE**************************************************************

void diagonalChase(){
  
 
  for(int w = 0; w < 90 ; w++){ 
    
    if (w*3 >254){
    strip.setPixelColor(w,cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
    strip.setPixelColor(w +1,cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
    strip.setPixelColor((w + 92),cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
    strip.setPixelColor((w + 93),cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
    
      strip.setPixelColor((w + 184),cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
      strip.setPixelColor((w + 185),cos_wave[255] , 255-cos_wave[255],cos_wave[255], 0);
    }
    else{
    strip.setPixelColor(w,cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    strip.setPixelColor(w +1,cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    strip.setPixelColor((w + 92),cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    strip.setPixelColor((w + 93),cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    strip.setPixelColor((w + 184),cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    strip.setPixelColor((w + 185),cos_wave[w*3] , 255-cos_wave[w*3],cos_wave[w*3], 0);
    }
    strip.show();
    delay(50);
    strip.setPixelColor(w,0 , 0, 0, 0);
    strip.setPixelColor((w + 92),0 , 0, 0, 0);
    strip.setPixelColor((w + 184),0, 0, 0, 0);
    strip.setPixelColor(w +1,0 , 0, 0, 0);
    strip.setPixelColor((w + 93),0 , 0, 0, 0);
    strip.setPixelColor((w + 185),0 , 0, 0, 0);
    strip.show();
    //delay(10);
    
  }
  
}



//********************************************************************************************************************


void craigSparkle(){
  for(int w = 0; w < NUM_strip ; w++){ 
    strip.setPixelColor(w,0 , 0, 0, 0);
  }
  
  
  
  
  
  int loc1 = random(270);
  int loc2 = random(270);
  int loc3 = random(270); 
  int loc4 = random(270);
  int loc5 = random(270);
  int loc6 = random(270);
  int loc7 = random(270); 
  int loc8 = random(270);
   int loc9 = random(270);
  int loc10 = random(270);
  int loc11 = random(270); 
  int loc12 = random(270);
  int loc13 = random(270);
  int loc14 = random(270);
  int loc15 = random(270); 
  int loc16 = random(270);
  
 
 
  
    strip.setPixelColor(loc1, 0, 0, 0, random(255));// random(255)/5);
    strip.setPixelColor(loc2, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc3, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc4, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc5, 0, 0, 0, random(255));// random(255)/5);
    strip.setPixelColor(loc6, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc7, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc8, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc9, 0, 0, 0, random(255));// random(255)/5);
    strip.setPixelColor(loc10, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc11, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc12, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc13, 0, 0, 0, random(255));// random(255)/5);
    strip.setPixelColor(loc14, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc15, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc16, 0, 0, 0, random(255));//random(255)/5);
    strip.show();
    delay(30);
    
  }

//*************************************************************************************************************************


void craigSparkleColor(){
  for(int w = 0; w < NUM_strip ; w++){ 
    strip.setPixelColor(w, 0, 0, 0, 0);
  }
  int loc1 = random(270);
  int loc2 = random(270);
  int loc3 = random(270); 
  int loc4 = random(270);
  int loc5 = random(270);
  int loc6 = random(270);
  int loc7 = random(270); 
  int loc8 = random(270);
   int loc9 = random(270);
  int loc10 = random(270);
  int loc11 = random(270); 
  int loc12 = random(270);
  int loc13 = random(270);
  int loc14 = random(270);
  int loc15 = random(270); 
  int loc16 = random(270);
  
 
 
  
    strip.setPixelColor(loc1, random(255), 0, 0, 0);// random(255)/5);
    strip.setPixelColor(loc2, 0,random(255), random(255), 0);//random(255)/5);
    strip.setPixelColor(loc3, random(255), 0, random(255), 0);//random(255)/5);
    strip.setPixelColor(loc4, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc5, random(255), 0, 0, 0);// random(255)/5);
    strip.setPixelColor(loc6, 0,random(255), random(255), 0);//random(255)/5);
    strip.setPixelColor(loc7, random(255), 0, random(255), 0);//random(255)/5);
    strip.setPixelColor(loc8, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc9, random(255), 0, 0, 0);// random(255)/5);
    strip.setPixelColor(loc10, 0,random(255), random(255), 0);//random(255)/5);
    strip.setPixelColor(loc11, random(255), 0, random(255), 0);//random(255)/5);
    strip.setPixelColor(loc12, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc13, random(255), 0, 0, 0);// random(255)/5);
    strip.setPixelColor(loc14, 0,random(255), random(255), 0);//random(255)/5);
    strip.setPixelColor(loc15, random(255), 0, random(255), 0);//random(255)/5);
    strip.setPixelColor(loc16, 0, 0, 0, random(255));//random(255)/5);
   
    strip.show();
    delay(30);
    
  }

//*************************************************************************************************************************

void craigSparkleFade(){
  for(int w = 0; w < NUM_strip ; w++){ 
    strip.setPixelColor(w, 0, 0, 0, 0);
  }
  int loc1 = random(270);
  int loc2 = random(270);
  int loc3 = random(270); 
  int loc4 = random(270);
  
 
 
  
    strip.setPixelColor(loc1, 0, 0, 0, random(255));// random(255)/5);
    strip.setPixelColor(loc2, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc3, 0, 0, 0, random(255));//random(255)/5);
    strip.setPixelColor(loc4, 0, 0, 0, random(255));//random(255)/5);
    strip.show();
    //delay(10);
    
  }
  
  
  


//}


//*************************************************************************************************************************

void plasmaCraig(){

frameCount++ ; 
    uint16_t t = fastCosineCalc((45 * frameCount)/70);                                                      //fastCosineCalc((42 * frameCount)/100);  //time displacement - fiddle with these til it looks good...
    uint16_t t2 =  fastCosineCalc((30 * frameCount)/70);                                                       //fastCosineCalc((35 * frameCount)/100); 
    uint16_t t3 =   fastCosineCalc((35 * frameCount)/70);                                                    //fastCosineCalc((38 * frameCount)/100);

    for (uint8_t y = 0; y < ROWS_strip; y++) {
      int left2Right, pixelIndex;
      if (((y % (ROWS_strip/8)) & 1) == 0) {
        left2Right = 1;
        pixelIndex = y * COLS_strip;
      } else {
        left2Right = -1;
        pixelIndex = (y + 1) * COLS_strip - 1;
      }
      for (uint8_t x = 0; x < COLS_strip ; x++) {
        //Calculate 3 seperate plasma waves, one for each color channel
        uint8_t r = fastCosineCalc(((x << 3) + (t >> 1) + fastCosineCalc((t2 + (y << 3)))));
        uint8_t g = fastCosineCalc(((y << 3) + t + fastCosineCalc(((t3 >> 2) + (x << 3)))));
        uint8_t b = fastCosineCalc(((y << 3) + t2 + fastCosineCalc((t + x + (g >> 2)))));
        uint8_t w = fastCosineCalc(((x << 3) + t2 + fastCosineCalc((t + x + (g >> 2)))));
        //uncomment the following to enable gamma correction
          r=pgm_read_byte_near(exp_gamma+r)/2;  
          g=pgm_read_byte_near(exp_gamma+g)/2;
          b=pgm_read_byte_near(exp_gamma+b)/2;
          w=pgm_read_byte_near(exp_gamma+w)/2;
        strip.setPixelColor(pixelIndex, ((r << 24) | (g << 16) | (b << 8) | (w / 5)));
	pixelIndex += left2Right;
      }
    }
    //digitalWrite(13, HIGH);
    strip.show();  // not sure if this function is needed  to update each frame
    delayMicroseconds(7000);
    //digitalWrite(13, LOW);
    
}
   
   
   
   
   
   
   //************************************************************************************************************************* 
inline uint8_t fastCosineCalc( uint16_t preWrapVal)
{
  uint8_t wrapVal = (preWrapVal % 255);
  if (wrapVal<0) wrapVal=255+wrapVal;
  return (pgm_read_byte_near(cos_wave+wrapVal)); 
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//*************************************************************************************************************************

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}



//*************************************************************************************************************************


void playPattern()
{
    int i;
    int backgroundTemp;
    i = currentFrame;

    if (lvl >= lightningThreshold) {
      lightningOn = 1;
      lightningFrame = 0;
    } else if (lightningOn == 1) {
      lightningFrame = lightningFrame + 1;
      if (lightningFrame >= lightningFrameMax) {
        lightningOn = 0;
        lightningFrame = 0;
      }
    }

    if (lightningOn == 1) {
      backgroundTemp = 1000;
    } else {
      backgroundTemp = backgroundBrightness;
    }

    
    if (Pattern == 1){
      // order is pin 9 (L3,forwards), 6 (S3,forward), 5 (S2,forward), 8 (L2,forward), 4 (S1,backward), 7 (L1,backward)
      
      newRainbow_Volume(i, &sstrip1, 0, 2, backgroundTemp, 2, (hotspotS+5)%270);
      sstrip1.show();

  } 
  else if (Pattern == 2) {
    // order is pin 9 (L3,forwards), 6 (S3,forward), 5 (S2,forward), 8 (L2,forward), 4 (S1,backward), 7 (L1,backward)
    
    newRainbow_Volume(i, &sstrip1, 0, 1, backgroundTemp, 2, (hotspotS+5)%270);
  
    sstrip1.show();
  } 
//  else if (Pattern == 3) {
//    // order is pin 9 (L3,forwards), 5 (S2,forward), 7 (L1,backward), 6 (S3,forward), 8 (L2,forward), 4 (S1,backward)
//    
//    newRainbow_Volume(i, &lstrip3, 0, 1);
//    
//    temp = i + 57;
//    newRainbow_Volume(temp, &sstrip2, 0, 1);
//
//    temp = i + 85;
//    newRainbow_Volume(temp, &lstrip1, 1, 1);
//    
//    temp = i + 142;
//    newRainbow_Volume(temp, &sstrip3, 0, 1);
//    
//    temp = i + 170;
//    newRainbow_Volume(temp, &lstrip2, 0, 1);
//    
//    temp = i + 227;
//    newRainbow_Volume(temp, &sstrip1, 1, 1);
//    
//    
//    lstrip1.show();
//    lstrip2.show();
//    lstrip3.show();
//    sstrip1.show();
//    sstrip2.show();
//    sstrip3.show();
//  } 
//  else if (Pattern == 4) {
//    // order is pin 9 (L3,forwards), 5 (S2,forward), 7 (L1,backward), 6 (S3,forward), 8 (L2,forward), 4 (S1,backward)
//    
//    newRainbow_Volume(i, &lstrip3, 0, 1);
//    
//    temp = i + 30;
//    newRainbow_Volume(temp, &sstrip2, 0, 1);
//
//    temp = i + 45;
//    newRainbow_Volume(temp, &lstrip1, 1, 1);
//    
//    temp = i + 75;
//    newRainbow_Volume(temp, &sstrip3, 0, 1);
//    
//    temp = i + 90;
//    newRainbow_Volume(temp, &lstrip2, 0, 1);
//    
//    temp = i + 120;
//    newRainbow_Volume(temp, &sstrip1, 1, 1);
//    
//    
//    lstrip1.show();
//    lstrip2.show();
//    lstrip3.show();
//    sstrip1.show();
//    sstrip2.show();
//    sstrip3.show();
//  }

//  delay(20);

  currentFrame = currentFrame + 1;
  if (currentFrame >= MaxFrames - 1) {
    currentFrame = 0;
    currentLoop = currentLoop + 1;
    if (currentLoop >= MaxLoops) {
      currentLoop = 0;
      Pattern = Pattern + 1;
      if (Pattern >= MaxPatterns + 1) {
        Pattern = 1;
      }
    }
  }
  hotspotCntr = hotspotCntr + 1;
  if (hotspotCntr >= 3){
    hotspotCntr = 0;
    hotspotS = hotspotS + 1;
    if (hotspotS >= 270) {
      hotspotS = 1;
    }
  }
  
}

void processSound()
{
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  n   = random(0,1024);                 // Raw reading from mic 
  //n   = abs(n - 512 - DC_OFFSET);            // Center on zero
  //n   = (n <= NOISE) ? 0 : (n - NOISE);      // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)


  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter
 
//  Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  /* minLvl and maxLvl indicate the volume range over prior frames, used
   for vertically scaling the output graph (so it looks interesting
   regardless of volume level).  If they're too close together though
   (e.g. at very low volume levels) the graph becomes super coarse
   and 'jumpy'...so keep some minimum distance between them (this
   also lets the graph go to zero when no sound is playing):*/
  
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
  if (lvl > maxLvlAvg) lvl = maxLvlAvg;
  if (lvl < minLvlAvg) lvl = minLvlAvg;
  if (maxLvlAvg == minLvlAvg) {
    minLvlAvg = 1;
    maxLvlAvg = 2;
    lvl = 2;
  }
}

void displayImage(const uint8_t *img)
{
  int pos;
  uint8_t r, g, b, d;
  
  pos = 0;
  
  
  // load up all pixels
  for (int i = 0; i<LENGTH; i++) {
    r = pgm_read_byte_near(img + pos + i*3);
    g = pgm_read_byte_near(img + pos + i*3 + 1);
    b = pgm_read_byte_near(img + pos + i*3 + 2);
    sstrip1.setPixelColor(i, r, g, b);
  }
  pos = pos + LENGTH*3;
  
  
  
  
  // show all strips
  sstrip1.show();
  
  
  // wait for the specified delay time
  d = pgm_read_byte_near(img + pos);
  delay(d*10);
  pos = pos + 1;
  
}

//*************************************************************************************************************************



void animationinfo(int animation,int *maxframes,int *loops,int *framestart)
{
  switch(animation){
    case(0):*maxframes=30; 
        *framestart=0;
        *loops=6;
        break;
  }
}






////////////////////////////////////////////////////////////////////////////////////////////////////

void newColorWipe(uint32_t c, uint8_t index, Adafruit_NeoPixel *strip) {
  for(uint16_t i=0; i<index; i++) {
    if (i < strip->numPixels()) {
      strip->setPixelColor(i, c);
    }
  }
}

void newRainbow(uint16_t index, Adafruit_NeoPixel *strip, uint16_t reverse, uint16_t scale) {
  uint16_t i;

  if (reverse == 0){
    for(i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(((strip->numPixels()*scale)+index-(i*scale)-1) & 255));
    }
  } else {
    for(i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(((i*scale)+index) & 255));
    }
  }
}


//*************************************************************************************************************************


void newRainbow_Volume(uint16_t index, Adafruit_NeoPixel *strip, uint16_t reverse, uint16_t scale, uint16_t background, uint16_t radius, uint16_t location) {
  uint16_t i, i_swap;
  uint16_t ind_val, distance;

  if (reverse == 0){
    for(i=0; i<strip->numPixels(); i++) {
      i_swap = strip->numPixels()-i-1;
      if (i_swap < location) {
        distance = location - i_swap;
      } else {
        distance = i_swap - location;
      }
      if (distance >= radius) {
        strip->setPixelColor(i, Wheel_Volume(((strip->numPixels()*scale)+index-(i*scale)-1) & 255,background));
      } else {
        strip->setPixelColor(i, Wheel_Volume(((strip->numPixels()*scale)+index-(i*scale)-1) & 255,1000-background*distance/radius));
      }
    }
  } else {
    for(i=0; i<strip->numPixels(); i++) {
      if (i < location) {
        distance = location - i;
      } else {
        distance = i - location;
      }
      if (distance >= radius) {
        strip->setPixelColor(i, Wheel_Volume(((i*scale)+index) & 255,background));
      } else {
        strip->setPixelColor(i, Wheel_Volume(((i*scale)+index) & 255,1000-background*distance/radius));
      }
    }
  }
}


//*************************************************************************************************************************



// Slightly different, this makes the rainbow equally distributed throughout
void newRainbowCycle(uint16_t index, Adafruit_NeoPixel *strip) {
  uint16_t i;

  for(i=0; i< strip->numPixels(); i++) {
    strip->setPixelColor(i, Wheel(((i * 256 / strip->numPixels()) + index) & 255));
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

//*************************************************************************************************************************



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel_Volume(byte WheelPos, uint16_t brightness) {
  if(WheelPos < 85) {
   return sstrip1.Color(((WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000, ((255 - WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return sstrip1.Color(((255 - WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000, 0, ((WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000);
  } else {
   WheelPos -= 170;
   return sstrip1.Color(0, ((WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000, ((255 - WheelPos * 3)* (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg))*brightness/1000);
  }
}

