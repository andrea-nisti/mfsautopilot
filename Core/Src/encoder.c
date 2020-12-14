/*
 * encoder.c
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#include <stdint.h>
#include <stddef.h>
#include "encoder.h"

DriverStatus_t encoderInitStruct(Encoder_t* encoderPtr, uint8_t uid, PinState_t (*readA)(void), PinState_t (*readB)(void)){
  DriverStatus_t status = {
      .Code = DRIVER_ERROR,
      .flag = ENCODER_INIT_ERROR
  };

  if(readA == NULL || readB == NULL){
    return status;
  }


  encoderPtr->readA = readA;
  encoderPtr->readB = readB;
  PinState_t stateA = readA();
  PinState_t stateB = readB();

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
