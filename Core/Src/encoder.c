/*
 * encoder.c
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#include <stdint.h>
#include "encoder.h"

void encoderInitStruct(Encoder_t* encoderPtr, uint8_t uid){
  encoderPtr->encoderValue = 0;
  encoderPtr->stateA       = PIN_SET;
  encoderPtr->stateB       = PIN_SET;
  encoderPtr->stateAPrev   = PIN_SET;
  encoderPtr->stateBPrev   = PIN_SET;
  encoderPtr->trigger      = ENCODER_RESET;
  encoderPtr->encoderUID   = uid;
}

void encoderUpdateRawValues(Encoder_t* encoderPtr,PinState_t rawStateA, PinState_t rawStateB){

  encoderPtr->stateA = rawStateA;
  encoderPtr->stateB = rawStateB;
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
      }
      break;

    case ENCODER_ANTICLOCKWISE:
      if(checkRisingFallingState(encoderPtr->stateB, encoderPtr->stateBPrev) == EDGE_RISING){
        encoderPtr->encoderValue--;
        encoderPtr->trigger = ENCODER_RESET;
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
