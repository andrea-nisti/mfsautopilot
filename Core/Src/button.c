/*
 * button.c
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */
#include <stddef.h>
#include <stdbool.h>
#include "button.h"

static Button_t buttons[MAX_BUTTONS] = {};
static Button_t * freePtr = buttons;
static Button_t const * const lastPtr = &buttons[MAX_BUTTONS-1];

// Set a free element in the buttons array
static ButtonReturnFlags_t buttonAllocate(Button_t** freeButtonElement){
  static bool initDone = false;
  if (!initDone){
     for(Button_t* p = buttons; p <= lastPtr; p++){
       p->inUse = false;
     }
     initDone = true;
  }

  uint8_t scanComplete = 0;
  while(freePtr->inUse){
    if(freePtr == lastPtr || (scanComplete == MAX_BUTTONS)){
      return BUTTON_ALLOCATE_ERROR;
    }else{
      freePtr = buttons;
    }

    scanComplete++;
    freePtr++;
  }

  *freeButtonElement = freePtr;
  return BUTTON_OK;
}

Button_t const * buttonGetByUID(uint8_t uid){
  Button_t const * p = buttons;
  for(; p <= lastPtr; p++){
    if(uid == p->uid){
      return p;
    }
  }
  return NULL;
}

//Polling methods for button readings
static DriverStatus_t buttonInitStruct(uint8_t uid, Button_t* buttonStructPtr, PinState_t (*read)(void),
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
  buttonStructPtr->uid = uid;
  buttonStructPtr->inUse = true;
  return status;
}

ButtonReturnFlags_t buttonAddByUID(
    btnUID_t id, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void),
    void (*onPushedTimeElapsed)(void), uint32_t pushTime){

  Button_t * newPtr;
  if(buttonAllocate(&newPtr) == BUTTON_OK){
    buttonInitStruct(id, newPtr, read, onPushed, onReleased, onPushedTimeElapsed, pushTime);
  }else{
    return BUTTON_ALLOCATE_ERROR;
  }

  return BUTTON_OK;
}

void buttonUpdateStateByID(uint8_t uid,uint32_t currentMilli){
  Button_t const * currentBtnPtr = buttonGetByUID(uid);
  if(currentBtnPtr != NULL) buttonUpdateState(currentBtnPtr,currentMilli);
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
void buttonUpdateAllStates(uint32_t currentMilli){
  for(Button_t const * p = buttons; p <= lastPtr; p++){
    if (p->inUse){
      buttonUpdateState(p, currentMilli);
    }
  }
}
