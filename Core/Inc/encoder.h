/*
 * encoder.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "driverUtils.h"
#define MAX_ENCODERS     1


typedef enum EncoderReturnFlags {
  ENCODER_OK = 0,
  ENCODER_INIT_ERROR
} EncoderReturnFlags_t;

typedef enum {
  ENCODER_RESET = 0,
  ENCODER_CLOCKWISE,
  ENCODER_ANTICLOCKWISE
} EncoderTriggerState_t;

typedef struct EncoderStruct{
  DriverHeader_t header;
  uint8_t encoderUID;
  PinState_t stateA;
  PinState_t stateB;
  PinState_t stateAPrev;
  PinState_t stateBPrev;
  int32_t encoderValue;
  EncoderTriggerState_t trigger;
  PinState_t (*readA)(void);
  PinState_t (*readB)(void);
  void (*onTick)(struct EncoderStruct const * const enc);
} Encoder_t;

DriverStatus_t encoderInitStruct(Encoder_t* encoderPtr, uint8_t uid, PinState_t (*readA)(void), PinState_t (*readB)(void),void (*onTick)(Encoder_t const * const enc));
void encoderUpdateRawValues(Encoder_t* encoderPtr);
int32_t encoderGetValue(Encoder_t* encoderPtr);
uint8_t getEncoderUID(Encoder_t* encoderPtr);

//New APIs (work in progress)
EncoderReturnFlags_t encoderAddByUID(
    uint8_t id, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void),
    void (*onPushedTimeElapsed)(void), uint32_t pushTime);

Encoder_t const * encoderGetByUID(uint8_t uid);
void encoderUpdateState(Encoder_t* encoderStructPtr, uint32_t currentMilli);
void encoderUpdateStateByID(uint8_t uid, uint32_t currentMilli);
void encoderUpdateAllStates(uint32_t currentMilli);


#endif /* INC_ENCODER_H_ */
