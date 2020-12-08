/*
 * button.c
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */
#include <stddef.h>
#include "button.h"

//Polling methods for button readings
DriverStatus_t buttonInitStruct(
    Button_t* buttonStructPtr, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void), void (*onPushedTimeElapsed)(void), uint32_t pushTime){

  DriverStatus_t status = {
      .Code = DRIVER_ERROR,
      .flag = BUTTON_INIT_ERROR
  };

  if(read == NULL) return status;
  PinState_t initialState = read();

  buttonStructPtr->Modes.pushedEvent       = 0;
  buttonStructPtr->Modes.releasedEvent     = 0;
  buttonStructPtr->Modes.pushedTimeElapsedEvent = 0;

  buttonStructPtr->previousState.state  = initialState;
  buttonStructPtr->read                 = read;
  buttonStructPtr->onPushed             = onPushed;
  buttonStructPtr->onReleased           = onReleased;
  buttonStructPtr->onPushedTimeElapsed  = onPushedTimeElapsed;
  buttonStructPtr->pushTime             = pushTime;

  if(onPushed != NULL){
    status.Code = DRIVER_OK;
    status.flag = BUTTON_OK;
    buttonStructPtr->Modes.pushedEvent = 1;
  }

  if(onReleased != NULL){
    status.Code = DRIVER_OK;
    status.flag = BUTTON_OK;
    buttonStructPtr->Modes.releasedEvent = 1;
  }

  if(onPushedTimeElapsed != NULL){
    status.Code = DRIVER_OK;
    status.flag = BUTTON_OK;
    buttonStructPtr->Modes.pushedTimeElapsedEvent = 1;
  }

  return status;
}

void buttonUpdateState(Button_t* buttonStructPtr, uint32_t currentMilli){

  PinStateTS_t actual = {
      .state     = buttonStructPtr->read(),
      .timestamp = currentMilli
  };

  if(checkRisingFallingState(actual.state, buttonStructPtr->previousState.state) == EDGE_FALLING){
    buttonStructPtr->LastEventTS.event = EDGE_FALLING;
    buttonStructPtr->LastEventTS.timestamp = currentMilli;
    if(buttonStructPtr->Modes.pushedEvent) buttonStructPtr->onPushed();
  }

  if(checkRisingFallingState(actual.state, buttonStructPtr->previousState.state) == EDGE_RISING){
    buttonStructPtr->LastEventTS.event = EDGE_RISING;
    buttonStructPtr->LastEventTS.timestamp = currentMilli;
    if(buttonStructPtr->Modes.releasedEvent) buttonStructPtr->onReleased();
  }

  if(buttonStructPtr->Modes.pushedTimeElapsedEvent){
    if(actual.state == PIN_RESET && buttonStructPtr->LastEventTS.event == EDGE_FALLING){
      uint32_t timediff = actual.timestamp - buttonStructPtr->LastEventTS.timestamp;
      if (timediff >= buttonStructPtr->pushTime){
        buttonStructPtr->LastEventTS.event = EDGE_RESET;
        buttonStructPtr->LastEventTS.timestamp = currentMilli;
        buttonStructPtr->onPushedTimeElapsed();
      }
    }
  }

  buttonStructPtr->previousState = actual;
}
