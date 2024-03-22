#include <Arduino.h>

#define DCSBIOS_DEFAULT_SERIAL //Use DCSBIOS_DEFAULT_SERIAL is IRQ does not work.
#define USE_MATRIX_SWITCHES
#include <DcsBios.h>

#define BACKLIGHT_LED_PIN  5
#define MASTER_CAUTION_LED_PIN 6


//Create empty char array to save matrix values in. Initialise all of those (aka set them to a value), because otherwise using pointers will be dangerous.
volatile unsigned char in_mat[3][13] = {0};
byte rowPins[3] = {2, 3, 4};
byte colPins[13] = {7, 8, 9, 10, 16, 14, 15, 18, 19, 20, 21, 1, 0};
int numRows = sizeof(rowPins) / sizeof(rowPins[0]);
int numCols = sizeof(colPins) / sizeof(colPins[0]); 

/*Now Create your buttons. You are fee to add in any other Dcs_Bios switches and button with the matrix buttons. But this example will only detail the Matrix buttons.
As you can see, the array is kept in the main, but the call to the buttons contains a pointer to a specific cell in the array. 
This will be the cell the Button.h checks everytime DcsBios::loop() is called.*/

//Example Absolute momentary (button only on as long as it is held)
DcsBios::MatActionButtonSet ufc1("UFC_1", &in_mat[0][1], LOW);
DcsBios::MatActionButtonSet ufc2("UFC_2", &in_mat[0][2], LOW);
DcsBios::MatActionButtonSet ufc3("UFC_3", &in_mat[0][3], LOW);
DcsBios::MatActionButtonSet ufcHack("UFC_HACK", &in_mat[0][4], LOW);
DcsBios::MatActionButtonSet ufcFunc("UFC_FUNC", &in_mat[0][5], LOW);
DcsBios::MatActionButtonSet ufcEnt("UFC_ENT", &in_mat[0][6], LOW);
DcsBios::MatActionButtonSet ufcMasterCaution("UFC_MASTER_CAUTION", &in_mat[0][8], LOW);

DcsBios::Matrix3Pos ufcSteer("UFC_STEER", 1, 0, 2, 0);

DcsBios::Matrix3Pos ufcData("UFC_DATA", 0, 10, 1, 10);
DcsBios::Matrix3Pos ufcSel("UFC_SEL", 0, 11, 1, 11);
DcsBios::Matrix3Pos ufcDepr("UFC_DEPR", 1, 12, 2, 12);
DcsBios::Matrix3Pos ufcInten("UFC_INTEN", 2, 10, 2, 11);


DcsBios::MatActionButtonSet ufc4("UFC_4", &in_mat[1][1], LOW);
DcsBios::MatActionButtonSet ufc5("UFC_5", &in_mat[1][2], LOW);
DcsBios::MatActionButtonSet ufc6("UFC_6", &in_mat[1][3], LOW);
DcsBios::MatActionButtonSet ufc10("UFC_10", &in_mat[1][4], LOW);


DcsBios::MatActionButtonSet ufc7("UFC_7", &in_mat[2][1], LOW);
DcsBios::MatActionButtonSet ufc8("UFC_8", &in_mat[2][2], LOW);
DcsBios::MatActionButtonSet ufc9("UFC_9", &in_mat[2][3], LOW);

DcsBios::MatActionButtonSet ufcClr("UFC_CLR", &in_mat[2][5], LOW);

DcsBios::MatActionButtonSet ufcAltAlrt("UFC_ALT_ALRT", &in_mat[2][6], LOW);
DcsBios::MatActionButtonSet ufcCom1("UFC_COM1", &in_mat[1][7], LOW);
DcsBios::MatActionButtonSet ufcCom2("UFC_COM2", &in_mat[2][7], LOW);
DcsBios::MatActionButtonSet ufcComSec("UFC_COM_SEC", &in_mat[1][8], LOW);
DcsBios::MatActionButtonSet ufcEccm("UFC_ECCM", &in_mat[2][8], LOW);

DcsBios::MatActionButtonSet ufcIdm("UFC_IDM", &in_mat[2][9], LOW);
DcsBios::MatActionButtonSet ufcIff("UFC_IFF", &in_mat[1][9], LOW);
DcsBios::MatActionButtonSet ufcLtr("UFC_LTR", &in_mat[1][5], LOW);

DcsBios::MatActionButtonSet ufcMk("UFC_MK", &in_mat[1][6], LOW);
DcsBios::MatActionButtonSet ufcSpc("UFC_SPC", &in_mat[2][4], LOW);


/* Make the LED connected into a Master Caution Light */
DcsBios::LED masterCaution(0x1012, 0x0800, MASTER_CAUTION_LED_PIN);

void onIntConsoleLightBrightChange(unsigned int newValue) {
	int intesity = map(newValue, 0, 65535, 0 , 180);
    analogWrite(BACKLIGHT_LED_PIN, intesity);
}

DcsBios::IntegerBuffer intConsoleLightBuffer(0x1376, 0xffff, 0, onIntConsoleLightBrightChange);

void setup() {
    DcsBios::setup();
    Serial.begin(9600); // Initialize serial for debugging

    pinMode(BACKLIGHT_LED_PIN, OUTPUT);
    analogWrite(BACKLIGHT_LED_PIN, 16);
  
    for (int i = 0; i < numRows; i++) {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH); // Set rows high to start (inactive)
    }
    for (int i = 0; i < numCols; i++) {
        pinMode(colPins[i], INPUT_PULLUP); // Columns are input with pullup
    }
}

void loop() {
    for (int row = 0; row < numRows; row++) {
        digitalWrite(rowPins[row], LOW); // Activate row

        for (int col = 0; col < numCols; col++) {
            if (digitalRead(colPins[col]) == LOW) { // If button press is detected
                delay(5); // Debounce delay
                if (digitalRead(colPins[col]) == LOW) { // Check again to confirm button press
                    in_mat[row][col] = LOW; // Mark as pressed
                } else {
                    in_mat[row][col] = HIGH; // Mark as not pressed
                }
            } else {
                in_mat[row][col] = HIGH; // Mark as not pressed if initially detected as HIGH
            }
        }
        digitalWrite(rowPins[row], HIGH); // Deactivate row

        delay(10); // Short delay to stabilize before moving to the next row
    }

    DcsBios::loop();
}