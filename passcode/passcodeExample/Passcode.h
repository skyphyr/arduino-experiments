#ifndef PASSCODE_H
#define PASSCODE_H


#include <Arduino.h>


// An enumeration of all the buttons we have which could be
// pressed, plus the number of buttons. 
//! NOTE:
// If you are adding more buttons, all buttons should be added
// before BUTTON_COUNT. You call them whatever you like, but
// to save memory, you should only add as many buttons as you
// have.
enum Button {
  BUTTON_1,
  BUTTON_2,
  BUTTON_COUNT
};

static const byte MAX_PASSCODE_LENGTH = 8;

class Passcode {
  public:
    // Constructor for the passcode class
    Passcode();
    // Set the passcode to be matched against
    void setPasscode(Button passcode[], byte length);
    // Press a button and see if we have a match
    boolean enterButtonAndCheckMatch(Button button);
    
  private:
    // The lookup table for the passcode processing
    byte mFiniteStateMachine[MAX_PASSCODE_LENGTH][BUTTON_COUNT];
    // The current passcode state
    byte mState;
    // Which state is the match state?
    byte mMatchState;
};



#endif
