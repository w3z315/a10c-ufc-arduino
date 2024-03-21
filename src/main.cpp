#include <Arduino.h>

#define BACKLIGHT_LED_PIN  5
#define MASTER_CAUTION_LED_PIN 6
#define PLACEHOLDER_KEY_LABEL "_PLACEHOLDER_"

const byte ROWS = 3;
const byte COLS = 13;

String hexaKeys[ROWS][COLS] = {
  {PLACEHOLDER_KEY_LABEL, "1_ABC", "2_DEF", "3_GHI", "HACK", "FUNC", "ENT", PLACEHOLDER_KEY_LABEL, "MASTER_CAUTION", PLACEHOLDER_KEY_LABEL, "DATA_UP", "SEL_UP", PLACEHOLDER_KEY_LABEL},
  {"STEER_UP", "4_JKL", "5_MNO", "6_PQR", "0", "LTR", "MK", "COM_1", "COM_SEC", "IFF", "DATA_DOWN", "SEL_DOWN", "DEPR_UP"},
  {"STEER_DOWN", "7_STU", "8_VWX", "9_YZ", "SPC", "CLR", "ALT_ALRT", "COM_2", "ECCM", "IDM", "INTEN_LEFT", "INTEN_RIGHT", "DEPR_DOWN"}
};

byte rowPins[ROWS] = {2, 3, 4};
byte colPins[COLS] = {7, 8, 9, 10, 16, 14, 15, 18, 19, 20, 21, 1, 0};

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to be ready
  Serial.println("A10C UFC v1.1 --- READY");

  pinMode(BACKLIGHT_LED_PIN, OUTPUT);
  pinMode(MASTER_CAUTION_LED_PIN, OUTPUT);

  analogWrite(BACKLIGHT_LED_PIN, 16);
  
  // Initialize row pins
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW); // Ensure columns are inactive at start
  }
  // Initialize column pins
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int row = 0; row < ROWS; row++) {
      // Set the current column to LOW to activate it
      digitalWrite(rowPins[row], LOW);
    
    for (int col = 0; col < COLS; col++) {
      // Check if the button is pressed
      String key = hexaKeys[row][col];
      if(digitalRead(colPins[col]) == LOW && key != PLACEHOLDER_KEY_LABEL) {
        // Debounce delay
        delay(50);
        // Ensure the button is still pressed
        if(digitalRead(colPins[col]) == LOW && key != PLACEHOLDER_KEY_LABEL) {
          Serial.print("Button Pressed: ");
          Serial.println(key);
          Serial.println("");
        }
        // Wait for button release
        while(digitalRead(colPins[col]) == LOW);
      }
      
    }
    // Reset the current column to HIGH to deactivate it
    digitalWrite(rowPins[row], HIGH);
    
    delay(10); // Short delay to stabilize before moving to the next column
  }
}