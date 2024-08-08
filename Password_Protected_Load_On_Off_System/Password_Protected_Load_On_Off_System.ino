#include <Keypad.h> //Handles keypad input.
#include <Wire.h> //Manages I2C communication, used by the LCD
#include <LiquidCrystal_I2C.h> //Controls the LCD using I2C protocol.
LiquidCrystal_I2C lcd(0x3F, 16, 2); //Initializes the LCD with I2C address 0x3F, 16 columns, and 2 rows.

#define passwordsize 5 //Defines the size of the password as 5 (4 digits + 1 null terminator).

char Masterpass[passwordsize] = "1234"; //set any password
char Data[passwordsize]; //Array to store the user-entered password.
byte data_count = 0; //Tracks the number of entered characters.
char customKey; //Stores the current key pressed.

#define relay_pin 10 //Defines the pin connected to the relay.
bool state = HIGH; //Tracks the current state of the relay (HIGH means off, LOW means on).

const byte ROWS = 4;  //Defines a 4x4 keypad with numbers and some special characters.
const byte COLS = 4;  //four columns
//Maps the rows and columns of the keypad to specific Arduino pins.
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad

//Creates an instance of the Keypad class to manage input.
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  /*
lcd.init(): Initializes the LCD.
lcd.backlight(): Turns on the LCD backlight.
pinMode(relay_pin, OUTPUT): Sets the relay pin as an output.
digitalWrite(relay_pin, HIGH): Initializes the relay to the off state (HIGH).
*/
  lcd.init();
  lcd.backlight();
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:"); //Displays "Enter Password:" on the LCD.
  customKey = customKeypad.getKey(); //Waits for a key press and stores the pressed key in customKey.

//If a key is pressed, it is displayed on the LCD, stored in the Data array, and data_count is incremented.
  if (customKey) {
    lcd.setCursor(data_count, 1);
    lcd.print(customKey);
    Data[data_count] = customKey;
    data_count++;
  }
  /*
When 4 characters are entered (data_count == passwordsize - 1):
Clears the LCD.
Compares the entered password (Data) with the correct password (Masterpass) using strcmp.
If the password matches:
Toggles the relay state (turns the load on/off).
Displays "Password Matched" and "Load ON" or "Load OFF" depending on the state.
If the password is incorrect, displays "Wrong Password."
After displaying the message for 3 seconds (delay(3000)), the LCD is cleared.
Resets data_count to 0 for the next input.
*/
  if (data_count == passwordsize - 1) {
    lcd.clear();
    if (!strcmp(Data, Masterpass)) {
      state = !state;  //alter the state
      digitalWrite(relay_pin, state);
      lcd.setCursor(0, 0);
      lcd.print("Password Matched");
      if (state == LOW) {
        lcd.setCursor(4, 1);
        lcd.print("Load ON");
      } else {
        lcd.setCursor(4, 1);
        lcd.print("Load OFF");
      }
      delay(3000);
      lcd.clear();
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password");
      delay(3000);
      lcd.clear();
    }
    //delay(1000);
    data_count = 0;
  }
}
