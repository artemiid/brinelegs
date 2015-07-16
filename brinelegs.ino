                                                                                                                                                                              

// Artimiid art boat legs

#include <FastLED.h>

// pin definitions
#define LEDS_PIN1 9
uint8_t dip_switch_pins[] = {6,5,4,3,2};

#define NUM_LEDS 15
#define BRIGHTNESS      96

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// ----------------------------------------------------------------------------
// globals

// TODO: 2 separate arrays
CRGB leds[NUM_LEDS];

uint8_t g_address;

uint8_t g_hue = 0;




// ----------------------------------------------------------------------------
// serial debug
unsigned long lastTick=0;
unsigned int sec=1;
unsigned int minute=0;
unsigned int hour=0;


// ----------------------------------------------------------------------------
// setup & loop

void setup() {

    // address dip switch
    for (int i = 0; i < ARRAY_SIZE(dip_switch_pins); i++) {
        pinMode(dip_switch_pins[i], INPUT_PULLUP);
    }

    // 2 led strips
    FastLED.addLeds<NEOPIXEL, LEDS_PIN1>(leds, NUM_LEDS);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    g_address = get_address();

    // setup serial port
    Serial.begin(9600);
    Serial.println("----------------------");
    Serial.print("Brinelegs address: ");
    Serial.println(g_address);
}

typedef void (*SimplePatternList[])();
SimplePatternList g_patterns = { 
    rainbow, 
    rainbowWithGlitter, 
    confetti, 
    sinelon, 
    juggle, 
    bpm 
};
uint8_t g_current_pattern = 0;

void loop() {
    
    FastLED.show();
    
    g_patterns[g_current_pattern]();

    if (millis() - lastTick >= 1000) {          
          sec++;
          if (sec==60)
            {minute++;
              sec=0;
            }
          if(minute==60)
            { hour++;
              minute=0;
            }
          lastTick = millis();
        //Serial.println(millis());    
    }
    if (millis() - lastTick >= 10000) {          
          Serial.print(hour,DEC);
          Serial.print(" : ");
          Serial.print(minute,DEC);
          Serial.print(" : ");
          Serial.print(sec,DEC);
          Serial.println();
    }
    
 

    // do some periodic updates
    //EVERY_N_MILLISECONDS( 20 ) { g_hue++; } // slowly cycle the "base color" through the rainbow
    //EVERY_N_SECONDS( 10 ) { next_pattern(); } // change patterns periodically
}

// ----------------------------------------------------------------------------
// patterns


void rainbow() 
{
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, g_hue, 7);
}

void rainbowWithGlitter() 
{
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    add_glitter(80);
}


void confetti() 
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( g_hue + random8(64), 200, 255);
}

void sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16(13,0,NUM_LEDS);
    leds[pos] += CHSV( g_hue, 255, 192);
}

void bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < NUM_LEDS; i++) { //9948
       leds[i] = ColorFromPalette(palette, g_hue+(i*2), beat-g_hue+(i*10));
    }
}

void juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
        leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

// ----------------------------------------------------------------------------
// helpers

void add_glitter( fract8 chanceOfGlitter) 
{
    if( random8() < chanceOfGlitter) {
        leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
}


uint8_t get_address() {
    uint8_t result = 0;
    for (int i = 0; i < ARRAY_SIZE(dip_switch_pins); i++) {
        result = result | !digitalRead(dip_switch_pins[i]) << i;
    }
    return result;
}

void next_pattern() {
    g_current_pattern = (g_current_pattern + 1) % ARRAY_SIZE(g_patterns);
}
