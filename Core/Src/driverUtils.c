/*
 * driverUtils.c
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#include "driverUtils.h"

EdgeState_t checkRisingFallingState(PinState_t actual,PinState_t previous){
  if(actual == previous){
    return EDGE_RESET;
  }else if((actual == PIN_SET) && (previous == PIN_RESET)){
    return EDGE_RISING;
  }else{
    return EDGE_FALLING;
  }
}

void initDebouncer(DebouncedState_t* stateStructPtr, uint16_t debouncerMaxCount){
  stateStructPtr->debounceMaxCounter = 0;
  stateStructPtr->debounceMaxCounter = debouncerMaxCount;
  stateStructPtr->previousRawState   = PIN_RESET;
}

PinState_t updateDebouncedSignal(DebouncedState_t* stateStructPtr, PinState_t actualRawState){

     if(actualRawState == stateStructPtr->previousRawState){
       stateStructPtr->debounceCounter++;
     }else{
       stateStructPtr->debounceCounter = 0;
     }
     stateStructPtr->previousRawState = actualRawState;

     if(stateStructPtr->debounceCounter >= stateStructPtr->debounceMaxCounter){
       stateStructPtr->debounceCounter = 0;
       stateStructPtr->previousRawState = actualRawState;
       return actualRawState;
     }else{
       return stateStructPtr->previousRawState;
     }
}

//Stack Utilities
// Set a free element in the buttons array
AllocateReturnFlags_t stackAllocate(uint8_t** freeReturnPtrPtr, StackTCB_t* cb, bool initDone){

  if (!initDone){
     for(uint8_t * p = cb->beginStackPtr; p <= cb->endStackPtr; p += cb->elementSize){
       *p = false;
     }
     initDone = true;
  }

  uint8_t scanComplete = 0;
  // while the block is in use
  while( *cb->actualFreePtr == true){

    if(scanComplete == cb->stackSize){
      return ALLOCATE_ERROR;
    }

    if(cb->actualFreePtr == cb->endStackPtr){
      // circle back to begin of stack, maybe someone freed the first block
      cb->actualFreePtr = cb->beginStackPtr;
      scanComplete++;
      break;
    }
    scanComplete++;
    cb->actualFreePtr += cb->elementSize;
  }

  *freeReturnPtrPtr = cb->actualFreePtr;
  return ALLOCATE_OK;
}
