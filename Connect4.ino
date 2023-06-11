#include <FastLED.h>     // include the FastLED library code

#define DATA_PIN     2
#define NUM_LEDS    50
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS  25   // MAX is 255
#define LED_TYPE    WS2811

CRGB leds[NUM_LEDS+1];
byte rows[8][8] = {{7,48,47,46,45,44,43,42},{7,35,36,37,38,39,40,41},{7,34,33,32,31,30,29,28},{7,21,22,23,24,25,26,27},{7,20,19,18,17,16,15,14},{7,7,8,9,10,11,12,13},{7,6,5,4,3,2,1,0},{1}};
byte boardValues[7][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
const byte numColumnButtons = 7;  // Number of buttons connected
byte columnButtonPins[numColumnButtons] = {3, 4, 5, 6, 7, 8, 9};  // Pins connected to buttons
byte resetButtonPin = 10;
byte NUM_IN_A_ROW_TO_WIN = 4;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < numColumnButtons; i++) {
    pinMode(columnButtonPins[i], INPUT_PULLUP);
  }
  leds_init();
  leds[49] = CRGB::Red;
  FastLED.show();
}

void loop() {
  for (byte i = 0; i < numColumnButtons; i++) {
    byte buttonState = digitalRead(columnButtonPins[i]);

    if (buttonState == HIGH) {
      drop(i);
    }
  }
  if (digitalRead(resetButtonPin) == HIGH) {
    for(byte x = 0; x<7; x++){
      rows[x][0] = 7;
      leds_init();
    }
    FastLED.show();
  }
  delay(100);
}

void leds_init() //turns off all LEDs
{
  for ( byte x = 0; x < NUM_LEDS; x=x+1 ) {
    leds[x] = CRGB::Black;
  }
}


void drop(byte column){
  CRGB color = CRGB::Yellow;
  if (rows[7][0] == 1){
    color = CRGB::Red;
    rows[7][0] = 2;
  }
  else {
    rows[7][0] = 1;
  }
  for (byte x = 1; x < rows[column][0];  x=x+1 ){
    leds[rows[column][x]] = color;
    FastLED.delay(100);
    leds[rows[column][x]] = CRGB::Black;
    FastLED.delay(10);
  }
  leds[rows[column][rows[column][0]]] = color;
  //win_at(column,rows[column][rows[column][0]], boardValues)
  if (rows[7][0] == 2) {
    leds[49] = CRGB::Yellow;
  }
  else {
    leds[49] = CRGB::Red;
  }
  FastLED.delay(100);
  rows[column][0] = rows[column][0] - 1;
}
