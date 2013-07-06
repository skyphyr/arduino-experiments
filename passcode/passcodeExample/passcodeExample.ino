#include "Passcode.h"

#include <Servo.h>

// Constants for our pin references
//! NOTE:
//! Pins 0 and 1 are used for serial communication, so if you want to
//! use the Serial, then you'll need to shift the LEDs to other pins.
static const int UNLOCKED_LED_PIN = 0;
static const int LOCKED_LED_PIN = 1;
static const int BUTTON_1_PIN = 2;
static const int BUTTON_2_PIN = 3;
static const int SERVO_PIN = 4;

// The delay between reads when taking a stable (debounced) read
static const int STABLE_READ_DELAY = 5;
// The length of time the read must be consistent for a debounced read
static const int STABLE_READ_TIME = 25;

// The angle to turn the servo to when unlocked
static const byte UNLOCKED_ANGLE = 166;
// The angle to turn the servo to when locked
static const byte LOCKED_ANGLE = 0;

// Keep track of the state of the buttons last time we read them
boolean lastButton1State;
boolean lastButton2State;

// Track whether we are locked - false now - setup will toggle it true
boolean lockedState = false;

// The passcode processing object. It stores and checks the passcode.
Passcode passcode;

// The lock servo
Servo lockServo;

boolean wasButtonReleased(int pin, boolean& lastButtonState, boolean buttonPressedState) {
  // Read the current pin
  int currentButtonState;
  // Check it for stability
  int previousButtonState = digitalRead(pin);
  // Count how long
  int stableCount = 0;
  // Debounce the button read (ensure the value is stable for STABLE_READ_TIME ms)
  while (stableCount < STABLE_READ_TIME) {
    // Delay so we're not reading too fast and can count
    // the approximate stable time
    delay(STABLE_READ_DELAY);
    // read the value
    currentButtonState = digitalRead(pin);
    if (currentButtonState == previousButtonState) {
      // It's remainded stable throughout the delay
      // so increment the time passed
      stableCount += STABLE_READ_DELAY;
    } else {
      // It's not remained stable, so reset the time passed
      stableCount = 0;
      // Set the previous state to the new one
      previousButtonState = currentButtonState;
    }
  } // We'll only get out of this once the value has been consistent for STABLE_READ_TIME ms
  
  // Hold the last button state locally, so we don't dupe the
  // code to update the last button state for each return or risk
  // forgetting to update the last button state
  int localLastButtonState = lastButtonState;
  
  // Update the last button state to the current state
  lastButtonState = currentButtonState;
  
  // Check if the button's state has changed
  if (localLastButtonState != currentButtonState) {
    // Check if the button was pressed
    if (localLastButtonState == buttonPressedState) {
      // It was pressed, meaning it's now been released.
      return true;
    }
  }
  
  // The button has either not changed or was just pressed, either
  // way - it's not released, so return false
  return false;
}

void setup() {
  // Set the LED pins to outputs, they need to be to push voltage
  pinMode(UNLOCKED_LED_PIN, OUTPUT);
  pinMode(LOCKED_LED_PIN, OUTPUT);
  
  // Led the button pins to inputs, they need to read
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  
  // Get the current button state before we do anything
  lastButton1State = digitalRead(BUTTON_1_PIN);
  lastButton2State = digitalRead(BUTTON_2_PIN);
  
  // Set the locked state to true on startup
  toggleState();
  
  // Define our button pattern, it can be no more than 8 characters
  Button pattern[] = {BUTTON_1, BUTTON_2, BUTTON_1, BUTTON_1, BUTTON_2, BUTTON_2, BUTTON_2, BUTTON_2};
  // Set the passcode, provide the length for safety
  passcode.setPasscode(pattern, 8);
  
  // Initialize the lock servo
  lockServo.attach(SERVO_PIN);
}

void toggleState() {
  // Switch the locked state.
  lockedState = !lockedState;
  // Toggle the LEDs to reflect the new state.
  digitalWrite(UNLOCKED_LED_PIN, !lockedState);
  digitalWrite(LOCKED_LED_PIN, lockedState);
  
  // Move the servo to it's lock position
  lockServo.write(lockedState ? LOCKED_ANGLE : UNLOCKED_ANGLE);
}

void loop() {
  // check if button 1 was just released
  //! NOTE: because we're checking button 1 first, if both are
  //! released at exactly the same time it will always be the same
  //! as releasing button 1 then releasing button 2

  // The button on my makershield goes to ground when pressed, so
  // it's voltage is low when pressed.
  if (wasButtonReleased(BUTTON_1_PIN, lastButton1State, LOW)) {
    // Enter button 1 into the passcode and check if it completes the passcode
    if (passcode.enterButtonAndCheckMatch(BUTTON_1)) {
      // The passcode has been entered - toggle the lock
      toggleState();
    }
  }
  
  if (wasButtonReleased(BUTTON_2_PIN, lastButton2State, HIGH)) {
    // Enter button 2 into the passcode and check if it completes the passcode
    if (passcode.enterButtonAndCheckMatch(BUTTON_2)) {
      // The passcode has been entered - toggle the lock
      toggleState();
    }
  }
}
