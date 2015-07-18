                                                                                                                                                                              

// Artimiid art boat legs

#include <FastLED.h>

// pin definitions
#define LEDS_PIN 9
uint8_t dip_switch_pins[] = {6,5,4,3,2};

#define NUM_LEDS        150
#define BRIGHTNESS      200

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// ----------------------------------------------------------------------------
// globals

// TODO: 2 separate arrays
CRGB leds[NUM_LEDS];

uint8_t g_address;

uint8_t g_hue = 0;


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
    int pos = random8(NUM_LEDS);
    leds[pos] += CHSV( g_hue + random8(64), 200, 255);
}

void big_confetti() {
    fadeToBlackBy( leds, NUM_LEDS, 4);
    if (random8() < 35) {
        int pos = random8(5, NUM_LEDS - 5);
        int hue = g_hue + random8(64);
        leds[pos-2] += CHSV( hue, 200, 150);
        leds[pos-1] += CHSV( hue, 200, 200);
        leds[pos  ] += CHSV( hue, 200, 255);
        leds[pos+1] += CHSV( hue, 200, 200);
        leds[pos+2] += CHSV( hue, 200, 150);
    }
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
// setup & loop


typedef void (*SimplePatternList[])();
SimplePatternList patterns_center = { 
    rainbow, 
    rainbowWithGlitter, 
    confetti, 
    sinelon, 
    juggle, 
    bpm 
};

SimplePatternList patterns_edge = { 
    big_confetti, 
};

uint8_t current_pattern = 0;

SimplePatternList* patterns = (SimplePatternList*)&patterns_center;
uint8_t patterns_count = ARRAY_SIZE(patterns_center);


void setup() {

    // address dip switch
    for (int i = 0; i < ARRAY_SIZE(dip_switch_pins); i++) {
        pinMode(dip_switch_pins[i], INPUT_PULLUP);
    }

    // led strip
    FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    g_address = get_address();

    if (g_address <= 4 || g_address >= 20) {
        patterns = (SimplePatternList*)&patterns_edge;
        patterns_count = ARRAY_SIZE(patterns_edge);
    }
}

uint32_t m = 0;
uint32_t pattern_start = 0;

void loop() {

    FastLED.show();
    (*patterns)[current_pattern]();


    m = millis();
    if (m - pattern_start >= 10000) {
        next_pattern();
        pattern_start += 10000;
    }

    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { g_hue++; } // slowly cycle the "base color" through the rainbow
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
    current_pattern = (current_pattern + 1) % patterns_count;
}
