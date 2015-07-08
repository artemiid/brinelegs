
// Artimiid art boat legs

#include <FastLED.h>
#define NUM_LEDS 20
#define DATA_PIN1 11
#define DATA_PIN2 12

CRGB leds[NUM_LEDS];


// counters
int i, j;

// state of each 5 switches (0 or 1)
int dip_state[5];

void setup() {

  // pins for dip switches are inputs
  // switches 1-5(6 on GND) on pins 6-2
  for (i = 6; i >= 2; i--) {
    pinMode(i, INPUT_PULLUP);
  }

  // setup serial port
  Serial.begin(9600);
  Serial.println("Serial port open");
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds, NUM_LEDS);
}


void loop() {
  /* DEBUG SERIAL FOR DIPS */
  for (i = 0, j = 6; i < 5, j >= 2; i++, j--) {
    
    Serial.print("\n");
    Serial.print(address(), DEC);
    Serial.print("\n");
  }
  
  Serial.println();
  
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = CRGB::Blue;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(30);
  };
}

//Create Address from DIP Switch (5 positions used)
byte address(){
  
  //Get the switches state
  for(i=2; i<=6; i++){
    j = (j << 1) | digitalRead(i);   // read the input pin
  }
  
  return j; //return address
}
