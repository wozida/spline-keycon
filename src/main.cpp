#include <Arduino.h>
#include "Keypad.h"

#define ROWS 4
#define COLS 4
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//connect to the row pinouts of the keypad
byte rowPins[ROWS] = {2, 3, 4, 5};
//connect to the column pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// The buffer to store the key presses
String buffer = "";

String bufferToCommand(String buffer) {
  char res[16];
  if (buffer[0] == 'A' || buffer[0] == 'B') {
    // move X/Y axis relative
    char axis = buffer[0] == 'A' ? 'X' : 'Y';
    int dir = 1;
    if (buffer[1] == '*') { // negative direction
      dir = -1;
    }
    int distance = buffer.substring(2).toInt();
    if (distance > 99) {
        distance = 99;
    }
    snprintf(res, sizeof(res), "%cS%c%02d", axis, dir == 1 ? '+' : '-', distance);
  }
  else if (buffer[0] == 'C' && buffer[1] == '1') {
    // store current position
    int slot = buffer[2] - '0';
    snprintf(res, sizeof(res), "S%02d", slot);
  }
  else if (buffer[0] == 'C' && buffer[1] == '2') {
    // return to stored position
    int slot = buffer[2] - '0';
    snprintf(res, sizeof(res), "G%02d", slot);
  }
  else if (buffer[0] == 'C' && buffer[1] == '3') {
    // Deploy the actuator
    snprintf(res, sizeof(res), "D");
  }
  else if (buffer[0] == 'C' && buffer[1] == '4') {
    // Retract the actuator
    snprintf(res, sizeof(res), "U");
  }
  else if (buffer[0] == 'C' && buffer[1] == '5') {
    // Record program
    snprintf(res, sizeof(res), "P");
  }
  else if (buffer[0] == 'C' && buffer[1] == '6') {
    // Stop recording program, save to EEPROM
    int slot = buffer[2] - '0';
    snprintf(res, sizeof(res), "E%02d", slot);
  }
  else if (buffer[0] == 'C' && buffer[1] == '7') {
    // Abort recording
    snprintf(res, sizeof(res), "A");
  }
  else if (buffer[0] == 'C' && buffer[1] == '8') {
    // Run program
    int slot = buffer[2] - '0';
    snprintf(res, sizeof(res), "R%02d", slot);
  }
  else {
    // Invalid command. Make SPLINE beep.
    snprintf(res, sizeof(res), "B");
  }
  // Add header and footer
  char cmdbuf[32];
  cmdbuf[0] = '[';
  cmdbuf[1] = '\0';
  strcat(cmdbuf, res);
  strcat(cmdbuf, "#");
  // calculate checksum
  int checksum = 0;
  for (int i = 0; i < strlen(cmdbuf); i++) {
    checksum += cmdbuf[i];
  }
  checksum = checksum % 0xa5;
  char checksumbuf[4];
  snprintf(checksumbuf, sizeof(checksumbuf), "%02x", checksum);
  strcat(cmdbuf, checksumbuf);
  strcat(cmdbuf, "]");
  // return the command
  return String(cmdbuf);
}

void setup() {
  Serial.begin(115200);
  // Set all the row pins as output
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
  }
  // Set all the column pins as input
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT);
  }
}

void loop() {
  // Read one key press
  char key = keypad.getKey();
  // If a key is pressed
  if (key) {
    // If the key is not the enter key
    if (key != '#') {
      // Add the key to the buffer
      buffer += key;
    }
    // If the key is the enter key
    else {
      // Generate the command from the buffer
      String command = bufferToCommand(buffer);
      // Send the command over UART
      Serial.println(command);
      // Clear the buffer
      buffer = "";
    }
  }
}

