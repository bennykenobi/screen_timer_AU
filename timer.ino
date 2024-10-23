#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

// Set the countdown duration in seconds
unsigned long countdownDuration = 600; // Countdown time in seconds
boolean countdownActive = false;

// Define pins
const int buttonPin = 11; // Arcade button pin, connect to NO on the button
const int buzzerPin = 9; // Piezo buzzer pin
const int externalLedPin = 13; // Connect the arcade button LED + to this pin
//There is also a button connected to the reset pin which resets the timer after the countdown has ended

void setup() {
  alpha4.begin(0x70); // Change 0x70 to the I2C address of the display
  alpha4.clear();
  alpha4.writeDisplay();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(externalLedPin, OUTPUT);

  // Display time to countdown from
  displayCountdown(countdownDuration);
}

void loop() {
  // Check if the arcade button is pressed
  if (digitalRead(buttonPin) == LOW && !countdownActive) {
    countdownActive = true;
  }

  // Check if the countdown has started
  if (countdownActive) {
    // Update the countdown timer
    updateCountdown();
    blinkExternalLed(); // Blink the arcade button LED while counting down
  }

  // Delay for 1 second
  delay(1000);
}

void updateCountdown() {
  // Subtract 1 second from the countdown 
  if (countdownDuration > 0) {
    countdownDuration--;
  } else {
    // Countdown is complete sound the buzzer
    countdownActive = false;
    soundBuzzer();
  }

  // Display the updated countdown value
  displayCountdown(countdownDuration);
}

void displayCountdown(unsigned long seconds) {
  // Calculate minutes and seconds
  uint8_t minutes = seconds / 60;
  uint8_t secs = seconds % 60;

  // Display the minutes and seconds on the display
  alpha4.writeDigitAscii(0, '0' + (minutes / 10));
  alpha4.writeDigitAscii(1, '0' + (minutes % 10));
  alpha4.writeDigitAscii(2, '0' + (secs / 10));
  alpha4.writeDigitAscii(3, '0' + (secs % 10));
  
  // Update the display
  alpha4.writeDisplay();
}

void soundBuzzer() {
  // Make an annoying sound when the countdown is finished
  tone(buzzerPin, 1100); // Piezo frequency
  delay(10000); // Noise duration
  noTone(buzzerPin);
}

void blinkExternalLed() {
  static unsigned long lastBlinkTime = 0;
  static boolean externalLedState = HIGH;

  // Blink the arcade button LED
  if (millis() - lastBlinkTime >= 500) {
    lastBlinkTime = millis();
    externalLedState = !externalLedState;
    digitalWrite(externalLedPin, externalLedState);
  }
}
