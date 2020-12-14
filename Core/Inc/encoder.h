/*
 * encoder.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "driverUtils.h"
typedef enum EncoderReturnFlags {
  ENCODER_OK = 0,
  ENCODER_INIT_ERROR
} EncoderReturnFlags_t;

typedef enum {
  ENCODER_RESET = 0,
  ENCODER_CLOCKWISE,
  ENCODER_ANTICLOCKWISE
} EncoderTriggerState_t;

typedef struct Encoder{
  uint8_t encoderUID;
  PinState_t stateA;
  PinState_t stateB;
  PinState_t stateAPrev;
  PinState_t stateBPrev;
  int32_t encoderValue;
  EncoderTriggerState_t trigger;
  PinState_t (*readA)(void);
  PinState_t (*readB)(void);
} Encoder_t;

DriverStatus_t encoderInitStruct(Encoder_t* encoderPtr, uint8_t uid, PinState_t (*readA)(void), PinState_t (*readB)(void));
void encoderUpdateRawValues(Encoder_t* encoderPtr);
int32_t encoderGetValue(Encoder_t* encoderPtr);
uint8_t getEncoderUID(Encoder_t* encoderPtr);
#endif /* INC_ENCODER_H_ */
