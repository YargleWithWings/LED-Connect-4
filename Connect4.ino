#include <FastLED.h> // include the FastLED library code
#include <vector>

// define constants
#define DATA_PIN 2
#define NUM_LEDS 50
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS 35 // MAX is 255
#define LED_TYPE WS2811
#define ROWS 7
#define COLS 7

// the LED strand
CRGB leds[NUM_LEDS];

// the light arrangement
byte cols[ROWS][COLS + 1] = {{7, 48, 47, 46, 45, 44, 43, 42}, {7, 35, 36, 37, 38, 39, 40, 41}, {7, 34, 33, 32, 31, 30, 29, 28}, {7, 21, 22, 23, 24, 25, 26, 27}, {7, 20, 19, 18, 17, 16, 15, 14}, {7, 7, 8, 9, 10, 11, 12, 13}, {7, 6, 5, 4, 3, 2, 1, 0}};

// the current state of the board
byte board[ROWS][COLS];

bool playerTurn = 1;                                 // 1 is red, 0 is blue
byte columnButtonPins[COLS] = {3, 4, 5, 6, 7, 8, 9}; // Pins connected to buttons to drop pieces
byte resetButtonPin = 10;                            // Button to reset the board
bool gameOver = false;

// Runs once at the start of the program
void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  resetBoard();
  for (int i = 0; i < COLS; i++) {
    pinMode(columnButtonPins[i], INPUT_PULLUP);
  }
  leds_init();
  leds[49] = CRGB::Red; // Shows who's turn it is
  FastLED.show();
}

// Continuously runs while arduino is on
void loop() {
  // Check if a button is pressed
  for (byte i = 0; i < COLS; i++) {
    byte buttonState = digitalRead(columnButtonPins[i]);

    // If a button is pressed, drop a piece in that column
    if (buttonState == HIGH) {
      drop(i);
    }
  }

  // If the reset button is pressed, reset the board
  if (digitalRead(resetButtonPin) == HIGH) {
    reset();
  }
  delay(50);
}

void reset() { // resets the board
  for (byte x = 0; x < COLS; x++) {
    cols[x][0] = 7;
  }
  resetBoard();
  leds_init();
  leds[49] = CRGB::Red;
  playerTurn = 1;
  gameOver = false;
  FastLED.show();
}

void resetBoard() {
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j)
    {
      board[i][j] = 2;
    }
  }
}

void leds_init() { // turns off all LEDs
  for (byte x = 0; x < NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
}

void drop(byte column) {
  // WHY IS IT COLUMN ROW NOT ROW COLUMN??
  if (cols[column][0] > 0 && !gameOver) { // If the column is not full, drop
    board[cols[column][0]][column] = playerTurn; // Set the board state

    // Set the color and the turn indicator
    CRGB color = CRGB::Blue;
    if (playerTurn) {
      color = CRGB::Red;
      playerTurn = 0;
      leds[49] = CRGB::Blue;
    }
    else {
      playerTurn = 1;
      leds[49] = CRGB::Red;
    }

    // display the piece dropping
    for (byte x = 1; x < cols[column][0]; x++) {
      leds[cols[column][x]] = color;
      FastLED.delay(100);
      leds[cols[column][x]] = CRGB::Black;
      FastLED.delay(10);
    }

    // display the piece at the bottom
    leds[cols[column][cols[column][0]]] = color;

    FastLED.delay(1);

    //check if the player won
    checkWin(column);

    // update the row we're on
    cols[column][0] -= 1;
  }
}

void displayWin(std::vector<byte[]> winCoords) {
  
}
bool checkWin(byte playedCol) {
  byte playedRow = cols[playedCol][0];
  bool foundWin = false;
  
  std::vector<byte[]> winCoords;
  winCoords.push_back({playedRow, playedCol});

  // declare bounds
  byte leftmostCol = max(playedCol - 3, 0);
  byte rightmostCol = min(playedCol + 3, COLS - 1);

  // check vertical
  byte numFoundInARow = 1;
  if (cols[column][0] > 3) {
    for (byte i = 1; i < 4; i++) {
      if (board[playedRow][playedCol] == board[playedRow][playedCol - i]) {
        winCoords.push_back({playedRow, playedCol - i});
        numFoundInARow++;
      }
      else {
        break;
      }
    }
  }
  if (numFoundInARow == 4) {
    foundWin = true;
    displayWin(winCoords);
    return true;
  }
  else {
    winCoords.clear();
    winCoords.push_back({playedRow, playedCol});
  }

  // check horizontal
  numFoundInARow = 1;
  for(byte i = column; i > leftmostCol; i--) { //check left
    if (board[playedRow][playedCol] == board[playedRow][i-1]) {
      numFoundInARow++;
      winCoords.push_back({playedRow, i-1});
    }
    else {
      break;
    }
  }
  for(byte i = column; i < rightmostCol; i++) { //check right
    if (board[playedRow][playedCol] == board[playedRow][i+1]) {
      numFoundInARow++;
      winCoords.push_back({playedRow, i+1});
    }
    else {
      break;
    }
  }
  if (numFoundInARow >= 4) {
    foundWin = true;
    displayWin(winCoords);
    return true;
  }
  else {
    winCoords.clear();
    winCoords.push_back({playedRow, playedCol});
  }

  // check southwest to northeast diagonal
  numFoundInARow = 1;
  for(byte i = 1; i < 4; i++) { //check up
    if (playedRow - i < 0 || playedCol - i < 0) {
      break;
    }
    if (board[playedRow][playedCol] == board[playedRow - i][playedCol - i]) {
      numFoundInARow++;
      winCoords.push_back({playedRow - i, playedCol - i});
    }
    else {
      break;
    }
  }
  for(byte i = 1; i < 4; i++) { //check down
    if (playedRow + i >= ROWS || playedCol + i >= COLS) {
      break;
    }
    if (board[playedRow][playedCol] == board[playedRow + i][playedCol + i]) {
      numFoundInARow++;
      winCoords.push_back({playedRow + i, playedCol + i});
    }
    else {
      break;
    }
  }
  if (numFoundInARow >= 4) {
    foundWin = true;
    displayWin(winCoords);
    return true;
  }
  else {
    winCoords.clear();
    winCoords.push_back({playedRow, playedCol});
  }

  // check northwest to southeast diagonal
  numFoundInARow = 1;
  for(byte i = 1; i < 4; i++) { //check up
    if (playedRow - i < 0 || playedCol + i >= COLS) {
      break;
    }
    if (board[playedRow][playedCol] == board[playedRow - i][playedCol + i]) {
      numFoundInARow++;
      winCoords.push_back({playedRow - i, playedCol + i});
    }
    else {
      break;
    }
  }
  for(byte i = 1; i < 4; i++) { //check down
    if (playedRow + i >= ROWS || playedCol - i < 0) {
      break;
    }
    if (board[playedRow][playedCol] == board[playedRow + i][playedCol - i]) {
      numFoundInARow++;
      winCoords.push_back({playedRow + i, playedCol - i});
    }
    else {
      break;
    }
  }
  if (numFoundInARow >= 4) {
    foundWin = true;
    displayWin(winCoords);
    return true;
  }
  else {
    winCoords.clear();
    winCoords.push_back({playedRow, playedCol});
  }

  return false;
}
