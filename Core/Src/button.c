/*
 * button.c
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */
#include <stddef.h>
#include "button.h"

//Polling methods for button readings
void buttonInitStruct(
    Button_t* buttonStructPtr, PinState_t initialStrate, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void), void (*onPushedTimeElapsed)(void)){

  buttonStructPtr->actualState.state    = initialStrate;
  buttonStructPtr->read                 = read;
  buttonStructPtr->onPushed             = onPushed;
  buttonStructPtr->onReleased           = onReleased;
  buttonStructPtr->onPushedTimeElapsed  = onPushedTimeElapsed;
}

void buttonUpdateState(Button_t* buttonStructPtr, uint32_t currentMilli){

  PinStateTS_t actual = {
      .state     = buttonStructPtr->read(),
      .timestamp = currentMilli
  };

  if(buttonStructPtr->onPushed != NULL){
    if(checkRisingFallingState(actual.state, buttonStructPtr->previousState.state) == EDGE_FALLING){
      buttonStructPtr->onPushed();
    }
  }

  if(buttonStructPtr->onReleased != NULL){
    if(checkRisingFallingState(actual.state, buttonStructPtr->previousState.state) == EDGE_RISING){
          buttonStructPtr->onReleased();
    }
  }

  if(buttonStructPtr->onPushedTimeElapsed!= NULL){

  }
  buttonStructPtr->previousState = actual;

}
