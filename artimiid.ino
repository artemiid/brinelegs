// Artimiid art boat legs

// pins used for the LEDs
// LED1 13
// LED2 12


// pins used for the switches
// S1 7
// S2 6
// S3 5
// S4 4
// S5 3
// S6 2

// state of each 5 switches (0 or 1)
int dip_state[5];

// counters
int i, j;

void setup() {

  // pins for dip switches are inputs
  // switches 1-5(6 on GND) on pins 6-2
  for (i = 6; i >= 2; i--) {
    pinMode(i, INPUT_PULLUP);
  }

  // setup serial port
  Serial.begin(9600);
  Serial.println("Serial port open");
}


void loop() {
  for (i = 0, j = 6; i < 5, j >= 2; i++, j--) {
    dip_state[i] = digitalRead(j);
    Serial.print(dip_state[i]);
  }
  Serial.println();
}
