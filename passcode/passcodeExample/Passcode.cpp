#include "Passcode.h"



Passcode::Passcode()
  : mState(0), mMatchState(MAX_PASSCODE_LENGTH)
{
}


//! NOTE: This entire function is somewhat wasteful on arduino devices
//! with such limited memory (unless you plan on using it to enable
//! setting the passcode on a running device - though then you need to
//! deal with persisting it as well). If you are just using it for a
//! hard coded passcode, you would be better off using this function
//! to generate the mFiniteStateMachine table, print it out, and then
//! hardcode the table and remove this function.
void Passcode::setPasscode(Button passcode[], byte length)
{
  // Seeing we're setting the passcode, we should reset the
  // state incase it was part way through on another
  mState = 0;
  
  // Make sure we have some characters
  if (length) {
    // Set the match state to the length of the passcode
    mMatchState = length;
    
    // Set all the possible first characters to stay in state 0
    // This should be redundant as the memory should automatically
    // be initialized to 0
    for (byte i = 0; i < BUTTON_COUNT; i++) {
      mFiniteStateMachine[0][i] = 0;
    }
    // Only the first character to match should move forward
    mFiniteStateMachine[0][passcode[0]] = 1;
    // Track where we'd go back to in the pattern without a match
    byte nonMatchState = 0;
    // Now go through the rest of the pattern
    for (byte i = 1; i < length; i++) {
      // Go through all the possible characters
      for (byte button = 0; button < BUTTON_COUNT;  button++) {
        // The state if this isn't a match should be the same as if the pattern up
        // to this point were entered.
        mFiniteStateMachine[i][button] = mFiniteStateMachine[nonMatchState][button];
      }
      // The character at this position in the passcode sends us to
      // the next position
      mFiniteStateMachine[i][passcode[i]] = i + 1;
      // Update where a non-match on the next character would set it back to
      nonMatchState = mFiniteStateMachine[nonMatchState][passcode[i]];
    }
  }
}


boolean Passcode::enterButtonAndCheckMatch(Button button)
{
  // Make sure it's a valid button
  if (button >= BUTTON_COUNT) {
    return false;
  }
  // Move the state to where this button sends it to.
  mState = mFiniteStateMachine[mState][button];
  
  if (mState == mMatchState) {
    // We've made it to the match state - so reset the state to the start
    // and return true for a match
    mState = 0;
    return true;
  }
  
  // We haven't matched the pattern yet
  return false;
}
