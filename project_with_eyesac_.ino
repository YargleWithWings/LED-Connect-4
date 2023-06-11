#include <FastLED.h>     // include the FastLED library code

#define DATA_PIN     2
#define NUM_LEDS    50
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS  255   // MAX is 255
#define LED_TYPE    WS2811

CRGB leds[NUM_LEDS+1];
byte rows[8][8] = {{7,48,47,46,45,44,43,42},{7,35,36,37,38,39,40,41},{7,34,33,32,31,30,29,28},{7,21,22,23,24,25,26,27},{7,20,19,18,17,16,15,14},{7,7,8,9,10,11,12,13},{7,6,5,4,3,2,1,0},{1}};
byte boardValues[7][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  byte NUM_IN_A_ROW_TO_WIN = 4;
  leds_init();
  leds[49] = CRGB::Red;
  FastLED.show();
}

void loop() {
  Serial.println(digitalRead(3));
  Serial.print(digitalRead(4));
  Serial.print(digitalRead(5));
  Serial.print(digitalRead(6));
  byte column0 = digitalRead(3);
  byte column1 = digitalRead(4);
  byte column2 = digitalRead(5);
  byte column3 = digitalRead(6);
  byte column4 = digitalRead(7);
  byte column5 = digitalRead(8);
  byte column6 = digitalRead(9);
  byte reset = digitalRead(10);
  if (column0 == 1) {
    drop(0);
  }
  else if (column1 == 1) {
    drop(1);
  }
  else if (column2 == 1) {
    drop(2);
  }
  else if (column3 == 1) {
    drop(3);
  }
  else if (column4 == 1) {
    drop(4);
  }
  else if (column5 == 1) {
    drop(5);
  }
  else if (column6 == 1) {
    drop(6);
  }
  else if (reset == 1) {
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
/*
void enough_in_a_row_direction(byte[7][7] board, byte start_x, byte start_y, byte x_offset_param, byte y_offset_param){
  try {
    byte current_color = board[start_x][start_y];
    if (current_color == 0){
      return false;
    } 
  }
  catch () {
    return false;
  }

  byte num_in_row = 1;
  for (byte x = 0; x<2, x++){ //FINISH FOR LOOP
    if (x == 1){
      byte x_offset = x_offset_param;
      byte y_offset = y_offset_param;
    }
    else {
      byte x_offset = -x_offset_param;
      byte y_offset = -y_offset_param;
    }
    try {
      byte x = start_x;
      byte y = start_y;
      while true{
        x = x + x_offset;
        y = y + y_offset;
        if (x<0 or y<0){
          break;
        }
        if (board[x][y] == current_color){
          num_in_a_row = num_in_a_row + 1;
          if (num_in_a_row == NUM_IN_A_ROW_TO_WIN){
            return True;
          }
        }
        else{
          break;
        }
      }
    }
    catch(){
      continue;
    }
    return false;
  }
}

void win_at(byte x, byte y,byte board){
  try{
    current_color = board[x][y];
  }
  catch(){
    return 0
  }
  if (enough_in_a_row_direction(board, x, y, 1, 0) or enough_in_a_row_direction(board, x, y, 0, 1) or enough_in_a_row_direction(board, x, y, 1, 1)){
    return current_color
  }
  else{
    return 0
  }
}
*/

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
