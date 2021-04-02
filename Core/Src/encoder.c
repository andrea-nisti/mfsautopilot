/*
 * encoder.c
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#include <stdint.h>
#include <stddef.h>
#include "encoder.h"

static Encoder_t encoders[MAX_ENCODERS] = {};
static Encoder_t * freePtr = encoders;
static Encoder_t const * const lastPtr = &encoders[MAX_ENCODERS-1];
/*
static EncoderReturnFlags_t buttonAllocate(Encoder_t** freeButtonElement){
  auto si = sizeof(**freeButtonElement);
  static bool initDone = false;
  if (!initDone){
     for(Button_t* p = buttons; p <= lastPtr; p++){
       p->inUse = false;
     }
     initDone = true;
  }

  uint8_t scanComplete = 0;
  while(freePtr->inUse){
    if(freePtr == lastPtr || (scanComplete == MAX_ENCODERS)){
      return BUTTON_ALLOCATE_ERROR;
    }else{
      freePtr = buttons;
    }

    scanComplete++;
    freePtr++;
  }

  *freeButtonElement = freePtr;
  return ENCODER_OK;
}
*/

DriverStatus_t encoderInitStruct(Encoder_t* encoderPtr, uint8_t uid, PinState_t (*readA)(void), PinState_t (*readB)(void),void (*onTick)(Encoder_t const * const enc)){
  DriverStatus_t status = {
      .Code = DRIVER_ERROR,
      .flag = ENCODER_INIT_ERROR
  };

  if(readA == NULL || readB == NULL){
    return status;
  }


  encoderPtr->readA  = readA;
  encoderPtr->readB  = readB;
  encoderPtr->onTick = onTick;
  PinState_t stateA  = readA();
  PinState_t stateB  = readB();

  encoderPtr->encoderValue = 0;
  encoderPtr->stateA       = stateA;
  encoderPtr->stateB       = stateB;
  encoderPtr->stateAPrev   = stateA;
  encoderPtr->stateBPrev   = stateB;
  encoderPtr->trigger      = ENCODER_RESET;
  encoderPtr->encoderUID   = uid;

  return status;
}

void encoderUpdateRawValues(Encoder_t* encoderPtr){

  encoderPtr->stateA=encoderPtr->readA();
  encoderPtr->stateB=encoderPtr->readB();

  // Decode signals, detect A falling edge
  if(checkRisingFallingState(encoderPtr->stateA, encoderPtr->stateAPrev) == EDGE_FALLING){

    if(encoderPtr->stateB == PIN_SET) encoderPtr->trigger = ENCODER_CLOCKWISE;
    else encoderPtr->trigger = ENCODER_ANTICLOCKWISE;

  }

  switch(encoderPtr->trigger){
    case ENCODER_CLOCKWISE:
      if(checkRisingFallingState(encoderPtr->stateB, encoderPtr->stateBPrev) == EDGE_FALLING){
        encoderPtr->encoderValue++;
        encoderPtr->trigger = ENCODER_RESET;
        encoderPtr->onTick(encoderPtr);
      }
      break;

    case ENCODER_ANTICLOCKWISE:
      if(checkRisingFallingState(encoderPtr->stateB, encoderPtr->stateBPrev) == EDGE_RISING){
        encoderPtr->encoderValue--;
        encoderPtr->trigger = ENCODER_RESET;
        encoderPtr->onTick(encoderPtr);
      }
      break;

    case ENCODER_RESET:
          break;
  }

  encoderPtr->stateAPrev = encoderPtr->stateA;
  encoderPtr->stateBPrev = encoderPtr->stateB;
}

int32_t encoderGetValue(Encoder_t* encoderPtr){
  return encoderPtr->encoderValue;
}
uint8_t encoderGetUID(Encoder_t* encoderPtr){
  return encoderPtr->encoderUID;
}
