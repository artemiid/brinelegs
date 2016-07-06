// Artimiid art boat legs
#include <FastLED.h>
// pin definitions
#define LEDS_PIN 10
#define NUM_LEDS        100
#define BRIGHTNESS      200
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int ledsX[NUM_LEDS][3]; //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, ETC)
//-PERISTENT VARS
int idex = 0;        //-LED INDEX (0 to NUM_LEDS-1
int idx_offset = 0;  //-OFFSET INDEX (BOTTOM LED TO ZERO WHEN LOOP IS TURNED/DOESN'T REALLY WORK)
int ihue = 0;        //-HUE (0-360)
int ibright = 0;     //-BRIGHTNESS (0-255)
int isat = 0;        //-SATURATION (0-255)
int bouncedirection = 0;  //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;      //-INC VAR FOR SIN LOOPS
int lcount = 0;      //-ANOTHER COUNTING VAR
// ----------------------------------------------------------------------------
// globals

CRGB leds[NUM_LEDS];
uint8_t g_address;
uint8_t g_hue = 90;

void set_color_led(int adex, int cred, int cgrn, int cblu)
{
  int bdex;

  if (idx_offset > 0) {  //-APPLY INDEX OFFSET
  bdex = (adex + idx_offset) % NUM_LEDS;
  }
  else {bdex = adex;}

  leds[bdex].r = cred;
  leds[bdex].g = cgrn;
  leds[bdex].b = cblu;
}
// ----------------------------------------------------------------------------
// patterns
void glitterFish() {
  add_glitter(15);
  fadeToBlackBy( leds, NUM_LEDS, 4);
  
  int pos = random8(NUM_LEDS);
  int hue = g_hue + random8(120);
  int value = 128;
  
  //Hide Green Hues
  if (hue < 130) {
    hue = g_hue + random8(120);
  }
  // Hide some Blue hues
  //if (hue < 180 && hue > 171) {
    //hue = g_hue + random8(120);
  //}
  // Hide Red hues
  if (hue < 255 && hue > 230) {
    hue = g_hue + random8(120);
  }

  //  
  if (hue < 140) {
    value = random8(128);
  } else if (hue > 200) {
    value = random8(233);
  }
  
  
  leds[pos] += CHSV( hue, 255, value);

  delay(10);
}


// ----------------------------------------------------------------------------
// setup & loop


typedef void (*SimplePatternList[])();
SimplePatternList patterns_center = {
glitterFish,
};

SimplePatternList patterns_edge = {
glitterFish,
};

uint8_t current_pattern = 0;

SimplePatternList* patterns = (SimplePatternList*)&patterns_center;
uint8_t patterns_count = ARRAY_SIZE(patterns_center);


void setup() {

  // led strip
  FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  if (g_address <= 4 || g_address >= 20) {
    patterns = (SimplePatternList*)&patterns_edge;
    patterns_count = ARRAY_SIZE(patterns_edge);
  }
  
  // Begin serial dump for debug
  Serial.begin(9600);
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
}


// ----------------------------------------------------------------------------
// helpers


void add_glitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}


void next_pattern() {
current_pattern = (current_pattern + 1) % patterns_count;
}
