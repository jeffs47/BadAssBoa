// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET//
#include "FastLED.h" 
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 101 // Number of LEDs in strip
struct CRGB leds[NUMPIXELS]; 
byte temp[NUMPIXELS];
byte wave;
int wave_scale = 20;
unsigned long currentMillis,previousMillis = 0;
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void setup() {
//Serial.begin(9600);
  FastLED.addLeds<APA102,11,13,GBR>(leds, NUMPIXELS);
  setup_hue_array();
  FastLED.clear();
  FastLED.setBrightness(3);
  FastLED.show();
}

int     pixel = 0, dir = 0, tail = -10; // Index of the pixel and direction (0 = right, 1 = left)
int     func = 0; 
uint32_t color = 0xFF0000;      // 'On' color (starts red)
int     intSpeed = 10;
int     r = 255, g = 0, b = 0;
char    colorfam = 'red';
uint8_t bright = 3;  //12 default
uint8_t thishue;                                              // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.
int     shuffle = 0;


void loop(){
  //IF in Shuffle Mode, Shuffle.
  if (shuffle == 1){
    ChangeMe();
  }
  
  //**When the "A" button is pressed, change function
  if (digitalRead (4) == 1) {
    changeFunction();
    do{}while (digitalRead (4) == 1);
  }
  //**When the "C" button is pressed, change the speed of the animation.
  if (digitalRead (6) == 1){
   changeSpeed();
    do{}while (digitalRead (6) == 1);
   }
  //**When the "B" button is pressed, change the brightness
  if (digitalRead (5) == 1){
    setBright();
  do{}while (digitalRead (5) == 1);
  }
  //**When the "D" button is pressed, change the color
  if (digitalRead (8) == 1){
    primaryswitcher();
    do{}while (digitalRead (8) == 1);
  }
// See what the function is set to and then run the right method.
  switch (func){
    case 0: solidcolor(); break;
    case 1: colorleft(); break;
    case 2: colorleftright(); break;
    case 3: tenright(); break;
    case 4: tenbothpersist(); break;
    case 5: tenboth(); break;
    case 6: rainbow(); break;
    case 7: pov(); break;
    case 8: twinkleRand(); break;
    case 9: wave_fx(); break;
    case 10: rainbow_march(); break;
    case 11: Fire2012(); break;
  }
}

void primaryswitcher(){
  switch (color)
        {
        case 0xFF0000: 
          color = 0xFF00FF; break;
        case 0xFF00FF: 
          color = 0x0000FF; break;
        case 0x0000FF: 
          color = 0x00FFFF; break;
        case 0x00FFFF: 
          color = 0x00FF00; break;
        case 0x00FF00: 
          color = 0xFFFF00; break;
        case 0xFFFF00: 
          color = 0x62FFFF; break;
        default:
          color = 0xFF0000;break;
        }
}
void changeFunction(){ // Change the function of the strip.
  pixel = 0;
  tail = -10;
  dir = 0;
  FastLED.clear();
  FastLED.show();
  //LEDS.clear();
    if (shuffle == 1){
      func = 0;
      shuffle = 0;
    }
    else if (func == 11){
      func = 2; 
      shuffle = 1;
     }
    else {
       ++func;
    }
//    else if (func == 11){
//      func = 0;
//       ++func;
//       currentMillis,previousMillis = 0;
//       wave_scale = 20;
//       setup_hue_array();
//    }

}
void changeSpeed(){ // Change the speed of the strip. The value below is the delay in MS for the Delay() command.
  if (intSpeed < 50){
      intSpeed = intSpeed +10;
     }
    else{
       intSpeed = 0;
    }
}
void twinkleRand() {
	// set background
	 stripSet(0,10);
	 // for each num
	 //for (int i=0; i<num; i++) {
         leds[random(NUMPIXELS)] = color;	   
	 //}
	FastLED.show();
	delay(intSpeed*3);
}

// quickly set the entire strip a color
void stripSet(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUMPIXELS; i++) {
    leds[i] = c; 
  }
  // move the show outside the loop
  FastLED.show();
  delay(wait);
}
void HSVtoRGB(int hue, int sat, int val,int *colors) {
  	int r, g, b, base;

	// hue: 0-359, sat: 0-255, val (lightness): 0-255


	if (sat == 0) { // Achromatic color (gray).
		colors[0]=val;
		colors[1]=val;
		colors[2]=val;
	} else  {
		base = ((255 - sat) * val)>>8;
		switch(hue/60) {
			case 0:
				colors[0] = val;
				colors[1] = (((val-base)*hue)/60)+base;
				colors[2] = base;
				break;
			case 1:
				colors[0] = (((val-base)*(60-(hue%60)))/60)+base;
				colors[1] = val;
				colors[2] = base;
				break;
			case 2:
				colors[0] = base;
				colors[1] = val;
				colors[2] = (((val-base)*(hue%60))/60)+base;
				break;
			case 3:
				colors[0] = base;
				colors[1] = (((val-base)*(60-(hue%60)))/60)+base;
				colors[2] = val;
				break;
			case 4:
				colors[0] = (((val-base)*(hue%60))/60)+base;
				colors[1] = base;
				colors[2] = val;
				break;
			case 5:
				colors[0] = val;
				colors[1] = base;
				colors[2] = (((val-base)*(60-(hue%60)))/60)+base;
				break;
		}
		
	}
}
void setBright(){
  switch (bright)
    {
      case 0:
        bright = 3; break;
      case 3:
        bright = 6; break; 
      case 6:
        bright = 12; break; 
      case 12:
        bright = 25; break;      
      case 25:
        bright = 50; break;
      case 50:
        bright = 100; break;
      case 100:
        bright = 150; break;
      case 150:
        bright = 200; break;
      case 200:
        bright = 255; break;
      case 255:
        bright = 0; break;
    }
      FastLED.setBrightness(bright);
      FastLED.show();
}
void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
  thishue++;                                                  // Increment the starting hue.
  fill_rainbow(leds, NUMPIXELS, thishue, deltahue);  // Use FastLED's fill_rainbow routine.
  FastLED.show();
  delay(intSpeed/7);
} 
void rainbow(){
  leds[pixel] = CRGB (r,g,b);
  //strip.setPixelColor(pixel, r,g,b);
      
      //delay(intSpeed); 
      switch (colorfam)
      {
        case 'red':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(++b < 255){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                colorfam = 'blue';
              }
          }
          break;
          case 'blue':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(--r > 0){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                colorfam = 'bluegreen';
              }
          }
          break;
          case 'bluegreen':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(++g < 255){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                colorfam = 'snot';
              }
          }
          break;
          case 'snot':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(--b > 0){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                colorfam = 'yellow';
              }
          }
          break;
          case 'yellow':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(++r < 255){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                colorfam = 'z';
              }
          }
          break;
             case 'z':
          if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
              if(--g > 0){
                pixel = 0;
                FastLED.show();
                delay(intSpeed/10);
              }
              else {
                r = 255;
                g = 0;
                b = 0;
                colorfam = 'red';
              }
          }
          break;
       }
}
void solidcolor(){
  leds[pixel] = color;
        if(++pixel >= NUMPIXELS) {
           pixel = 0; 
          FastLED.show();
        }
}
void tenright(){
          //***** 10 LEDs moving to the right. 
        if (tail >= 0){
          leds[tail] = 0; // 'Off' pixel at tail
        }
        else{
          leds[0] = 0; // 'Off' pixel at tail
        }
        if (pixel <= 101){
          leds[pixel] = color; // 'On' pixel at head
        }
        else{
          leds[101] = color; // 'On' pixel at head
        }
        
        FastLED.show();                     // Refresh strip
        delay(intSpeed);                        // Pause 20 milliseconds (~50 FPS)      
        if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
          pixel = 0;                       //  Yes, reset head index to start
          primaryswitcher();
          }
          if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}
void pov(){
   leds[pixel] = color;
        if(++pixel >= NUMPIXELS) {
          switch (color)
            {
            case 0x640000: 
              color = 0x0000FF; break;
            case 0x0000FF: 
              color = 0x00FF00; break;
            default:
              color = 0x640000;break;
            }
          pixel = 0; 
          FastLED.show();
          delay(intSpeed/3);
        }
}
void tenbothpersist(){ //***** 10 LEDs moving to the right then the left without a color change until the loop completes     
     if (tail >= 0){
          leds[tail] = 0; // 'Off' pixel at tail
        }
        else{
          leds[0] = 0; // 'Off' pixel at tail
        }
        if (pixel >= 0){
          leds[pixel] = color; // 'On' pixel at head
        }
        else{
          leds[0] = color; // 'On' pixel at head
        }
        FastLED.show();                     // Refresh strip
      delay(intSpeed);                        // Pause 20 milliseconds (~50 FPS)      
      if (dir == 1){
        if(--pixel == -12) {
          dir = 0;
          tail = -10;
          pixel = 0;
          primaryswitcher();
        }
        if(--tail <= -10) tail = 111; // Increment, reset tail index
        }
      else{
        if(++pixel >= (NUMPIXELS+10)) {         // Increment head index.  Off end of strip?
          dir = 1;
          tail = 111;
          pixel = 101;
        }
        if(++tail >= NUMPIXELS) tail = -10; // Increment, reset tail index
      }
}
void tenboth(){    //***** 10 LEDs moving to the right then the left   
      if (tail >= 0){
          leds[tail] = 0; // 'Off' pixel at tail
        }
        else{
          leds[0] = 0; // 'Off' pixel at tail
        }
        if (pixel >= 0){
          leds[pixel] = color; // 'On' pixel at head
        }
        else{
          leds[0] = color; // 'On' pixel at head
        }
     FastLED.show();                     // Refresh strip
      delay(intSpeed);                        // Pause 20 milliseconds (~50 FPS)      
      if (dir == 1){
        if(--pixel == -12) {
          dir = 0;
          tail = -10;
          pixel = 0;
          primaryswitcher();
        }
        if(--tail <= -10) tail = 111; // Increment, reset tail index
        }
      else{
        if(++pixel >= (NUMPIXELS+10)) {         // Increment head index.  Off end of strip?
          dir = 1;
          tail = 111;
          pixel = 101;
        primaryswitcher();
        }
        if(++tail >= NUMPIXELS) tail = -10; // Increment, reset tail index
      }
}
void colorleftright(){   //**** Color Wheel from left then right.
   leds[pixel] = color;
    FastLED.show();                     // Refresh strip
    delay(intSpeed);                        // Pause 20 milliseconds (~50 FPS)
    if (dir == 1){
      if(--pixel == 0) {
        dir = 0;
        primaryswitcher();
      }
    }
    else{
      if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
        dir = 1;
        primaryswitcher();
      }
    }
}
void colorleft(){  //***** Color Wheel from the left.
   leds[pixel] = color;
    FastLED.show();                     // Refresh strip
    delay(intSpeed);                        // Pause 20 milliseconds (~50 FPS)
    if(++pixel >= NUMPIXELS) {         // Increment head index.  Off end of strip?
      pixel = 0;                       //  Yes, reset head index to start
      primaryswitcher();
      }
}
void setup_hue_array(){
  static byte x, hue = 0;
  
  //only assign data for half the led strip
  for(byte i = 0; i < ((NUMPIXELS/2)+ 1); i++){
    
    //for every 5 leds, increase the hue by 20
    if(x++ == 5){
      hue += 20;
      x = 0;   
    }
    
    //assign the hue for later use in the temp[] array
    temp[i] = hue;
  }
}
void wave_fx(){
  //define the middle of the strip
  static byte middle = NUMPIXELS/2;
  
  //check the millis() timer
  currentMillis = millis();
  
  //if enough time has passed, run the program
  if(currentMillis - previousMillis >= 10) {
    
    for (byte i = 0; i < middle; i++){
      //this is the function that creates the wave effect
      //it may take time to fully understand what is happening here
      wave = sin8((millis() / wave_scale) * i);
      
      //populate the leds[x] with the temp[x] array hue's
      leds[i] = CHSV(temp[i], 255, wave);
    }
    //now mirror the data from leds #0 thru #middle
    //to leds #middle thru #end of string
    for(byte i = middle; i < NUMPIXELS; i++){
      leds[i] = leds[NUMPIXELS - i];
    }
    //timestamp the animation and show the leds[] data
    previousMillis = currentMillis;
    LEDS.show();
  }
}
void Fire2012()
{
// Array of temperature readings at each simulation cell
  random16_add_entropy( random());
  static byte heat[NUMPIXELS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUMPIXELS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUMPIXELS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUMPIXELS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUMPIXELS; j++) {
        leds[j] = HeatColor( heat[j]);
    }
    FastLED.show(); // display this frame
    FastLED.delay(1000 / 60);
}

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 999999;                          // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    if (secondHand ==  0)  {func = 2;}
    if (secondHand ==  10)  {func = 6;}
    if (secondHand == 20)  {func = 10;}
    if (secondHand == 30)  {func = 10;}
    if (secondHand == 40)  {func = 8;}
    if (secondHand == 50)  {func = 9;}
//    if (secondHand == )  {func = 2;}
//    if (secondHand == 35)  {func = 6;}
//    if (secondHand == 40)  {func = 10;}
//    if (secondHand == 45)  {func = 11;}
//    if (secondHand == 50)  {func = 8;}
//    if (secondHand == 55)  {func = 9;}
  }
} // ChangeMe()
      
