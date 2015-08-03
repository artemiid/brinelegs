                                                                                                                                                                              

// Artimiid art boat legs

#include <FastLED.h>

// pin definitions
#define LEDS_PIN 9
uint8_t dip_switch_pins[] = {6,5,4,3,2};

#define NUM_LEDS        150
#define BRIGHTNESS      200

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define FRAMES_PER_SECOND  120


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
    if (random8() < 45) {
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
    uint32_t time_delta = get_row() * 200;

    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16(11,0,NUM_LEDS);
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
    uint16_t time_delta = get_row_in_center() * 75;

    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
        leds[beatsin16(i+7,0,NUM_LEDS, time_delta)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void waves() {
    uint32_t time_delta = get_row_in_center() * 700;

    uint16_t stop1 = beatsin8(11, 15, 35, time_delta);
    uint16_t stop2 = beatsin8(7, 70, 120, time_delta);
    uint16_t halfgap = 4;
    uint16_t blur = 10;
    uint16_t margin = halfgap + blur;

    CRGB color1 = CRGB::Indigo;
    CRGB color2 = CRGB::Teal;
    CRGB color3 = CRGB::DeepPink;

    fill_solid(leds, stop1-margin, color1);
    fill_gradient_RGB(leds, stop1-margin, color1, stop1-halfgap, CRGB::Black);    
    fill_solid(&leds[stop1-halfgap], halfgap + halfgap, CRGB::Black);
    fill_gradient_RGB(leds, stop1+halfgap, CRGB::Black, stop1+margin, color2);
    fill_solid(&leds[stop1+margin], stop2-stop1-margin-margin, color2);
    fill_gradient_RGB(leds, stop2-margin, color2, stop2-halfgap, CRGB::Black);
    fill_solid(&leds[stop2-halfgap], halfgap + halfgap, CRGB::Black);
    fill_gradient_RGB(leds, stop2+halfgap, CRGB::Black, stop2+margin, color3);
    fill_solid(&leds[stop2+margin], NUM_LEDS-stop2-margin-margin, color3);

    uint8_t brightness = beatsin8(20);
    FastLED.setBrightness(scale8_video(255-brightness, BRIGHTNESS));
}

void rainbow_waves() {
    uint32_t time_delta = get_row_in_center() * 700;

    uint16_t stop1 = beatsin8(11, 15, 35, time_delta);
    uint16_t stop2 = beatsin8(7, 70, 120, time_delta);
    uint16_t halfgap = 5;
    uint16_t blur = 10;
    uint16_t margin = halfgap + blur;

    uint8_t hue = beatsin8(4, 0, 255, get_row_in_center() * 100);

    CHSV color1 = CHSV(hue, 200, 255);
    CHSV color2 = CHSV(hue+64, 200, 255);
    CHSV color3 = CHSV(hue+128, 200, 255);

    fill_solid(leds, stop1-margin, color1);
    fill_gradient_RGB(leds, stop1-margin, color1, stop1-halfgap, CRGB::Black);    
    fill_solid(&leds[stop1-halfgap], halfgap + halfgap, CRGB::Black);
    fill_gradient_RGB(leds, stop1+halfgap, CRGB::Black, stop1+margin, color2);
    fill_solid(&leds[stop1+margin], stop2-stop1-margin-margin, color2);
    fill_gradient_RGB(leds, stop2-margin, color2, stop2-halfgap, CRGB::Black);
    fill_solid(&leds[stop2-halfgap], halfgap + halfgap, CRGB::Black);
    fill_gradient_RGB(leds, stop2+halfgap, CRGB::Black, stop2+margin, color3);
    fill_solid(&leds[stop2+margin], NUM_LEDS-stop2-margin, color3);
}

void stripes() {
    uint16_t time_delta = get_row_in_center() * 150;
    fadeToBlackBy( leds, NUM_LEDS, 20);
    leds[beatsin16(7, 0, NUM_LEDS, time_delta)] |= CHSV( g_hue, 255, 192);
    leds[beatsin16(9, 0, NUM_LEDS, time_delta)] |= CHSV( g_hue+10, 255, 192);
    leds[beatsin16(8, 0, NUM_LEDS, time_delta)] |= CHSV( g_hue+128, 255, 192);
    leds[beatsin16(10, 0, NUM_LEDS, time_delta)] |= CHSV( g_hue+138, 255, 192);
}

// ----------------------------------------------------------------------------
// setup & loop


typedef void (*SimplePatternList[])();
SimplePatternList patterns_center = { 
    big_confetti,
    rainbow_waves,
    stripes,
    // juggle, 
    // rainbow, 
    // rainbowWithGlitter, 
    // confetti, 
    // sinelon, 
};

SimplePatternList patterns_edge = { 
    big_confetti,
    waves,
    sinelon, 
};

uint8_t current_pattern = 0;

SimplePatternList* patterns = (SimplePatternList*)&patterns_center;
uint8_t patterns_count = ARRAY_SIZE(patterns_center);
uint32_t pattern_period = 50000;


void setup() {

    // address dip switch
    for (int i = 0; i < ARRAY_SIZE(dip_switch_pins); i++) {
        pinMode(dip_switch_pins[i], INPUT_PULLUP);
    }

    // led strip
    FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    g_address = get_address();
    random16_set_seed(g_address);

    if (g_address <= 4 || g_address >= 21) {
        patterns = (SimplePatternList*)&patterns_edge;
        patterns_count = ARRAY_SIZE(patterns_edge);
        pattern_period = 70000;
    }
}

uint32_t m = 0;
uint32_t pattern_start = 0;

void loop() {

    (*patterns)[current_pattern]();
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND); 

    // using this instead of EVERY_N_MILLISECONDS
    m = millis();
    if (m - pattern_start >= pattern_period) {
        next_pattern();
        FastLED.setBrightness(BRIGHTNESS);
        pattern_start += pattern_period;
    }

    // do some periodic updates
    EVERY_N_MILLISECONDS( 30 ) { g_hue++; } // slowly cycle the "base color" through the rainbow
}


// ----------------------------------------------------------------------------
// helpers

void add_glitter( fract8 chanceOfGlitter) 
{
    if( random8() < chanceOfGlitter) {
        leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
}

void transition_through_black(CRGB * leds, uint8_t count, CRGB from, CRGB to) {
    uint16_t half = (count / 2);
    uint16_t last = count - 1;
    fill_gradient_RGB( leds,    0, from, half, CRGB::Black);
    fill_gradient_RGB( leds, half, CRGB::Black, last, to);
}

// 1 based
uint8_t get_row() {
    return ((g_address + 1) / 2);
}

// 0 based
uint8_t get_row_in_center() {
    return get_row() - 3;
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
