#include <FastLED.h>     // include the FastLED library code

#define DATA_PIN     2
#define NUM_LEDS    50
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS  35   // MAX is 255
#define LED_TYPE    WS2811
#define ROWS 7
#define COLS 7

CRGB leds[NUM_LEDS];
byte cols[ROWS][COLS+1] = {{7,48,47,46,45,44,43,42},{7,35,36,37,38,39,40,41},{7,34,33,32,31,30,29,28},{7,21,22,23,24,25,26,27},{7,20,19,18,17,16,15,14},{7,7,8,9,10,11,12,13},{7,6,5,4,3,2,1,0}};
bool playerTurn = 1;
byte board[ROWS][COLS] = {{2,2,2,2,2,2,2},{2,2,2,2,2,2,2},{2,2,2,2,2,2,2},{2,2,2,2,2,2,2},{2,2,2,2,2,2,2},{2,2,2,2,2,2,2},{2,2,2,2,2,2,2}};
byte columnButtonPins[COLS] = {3, 4, 5, 6, 7, 8, 9};  // Pins connected to buttons
byte resetButtonPin = 10;
bool gameOver = false;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < COLS; i++) {
    pinMode(columnButtonPins[i], INPUT_PULLUP);
  }
  leds_init();
  leds[49] = CRGB::Red;
  FastLED.show();
}

void loop() {
  Serial.println(digitalRead(columnButtonPins[4]));
  for (byte i = 0; i < COLS; i++) {
    byte buttonState = digitalRead(columnButtonPins[i]);

    if (buttonState == HIGH) {
      drop(i);
    }
  }
  if (digitalRead(resetButtonPin) == HIGH) {
    reset();
  }
  delay(100);
}

void reset() //resets the board
{
  for(byte x = 0; x<COLS; x++){
      cols[x][0] = 7;
      leds_init();
    }
    leds[49] = CRGB::Red;
    playerTurn = 1;
    gameOver = false;
    FastLED.show();
}

void leds_init() //turns off all LEDs
{
  for ( byte x = 0; x < NUM_LEDS; x=x+1 ) {
    leds[x] = CRGB::Black;
  }
}


void drop(byte column){
  if(cols[column][0] > 0 && !gameOver) {
  board[cols[column][0]][column] = playerTurn;
  checkWin(column, cols[column][0]);
  CRGB color = CRGB::Blue;
  if (playerTurn){
    color = CRGB::Red;
    playerTurn = 0;
  }
  else {
    playerTurn = 1;
  }
  if (!playerTurn) {
    leds[49] = CRGB::Blue;
  }
  else {
    leds[49] = CRGB::Red;
  }

  for (byte x = 1; x < cols[column][0];  x=x+1 ){
    leds[cols[column][x]] = color;
    FastLED.delay(100);
    leds[cols[column][x]] = CRGB::Black;
    FastLED.delay(10);
  }
  
  leds[cols[column][cols[column][0]]] = color;

  FastLED.delay(1);
  cols[column][0] -= 1;
  }
}

void checkWin(byte playedCol, byte playedRow) {
  //declare bounds
  byte leftmostCol = max(playedCol - 3, 0);
  byte rightmostCol = min(playedCol + 3, COLS - 1);
  byte bottomRow = max(playedRow - 3, 0);
  byte topRow = min(playedRow + 3, ROWS - 1);

  byte numFoundInARow = 0;
  
  //checking Horizontal
  byte score = 1;
  byte count = 1;
   
   while((x+count >= 0) && (x+count < WIDTH))
   {
      if (board_info[y][x+count] == player)  //Check Left
      {
         score++;
         count++;
      }
      else break;  //If no combo is detected break from the loop
   }

   count = 1;
   while((x-count < WIDTH) && (x-count >=0))
   {
      if (board_info[y][x-count] == player)  //Check Right
      {
         score++;
         count++;
      }   
      else break;  //If no combo is detected break from the loop
   }
   
   if (score == 4) return true;
}

