/*
 * encoder.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_
/*
  uint8_t countA = 0;
  uint8_t countB = 0;

  EncoderTriggerState_t trigger = ENCODER_RESET;
  int32_t encoderValue = 0;
  while (1)
  {
    // Read ENC1 Raw States
    stateA_raw = HAL_GPIO_ReadPin(GPIO_ENC1_A_GPIO_Port, GPIO_ENC1_A_Pin);
    stateB_raw = HAL_GPIO_ReadPin(GPIO_ENC1_B_GPIO_Port, GPIO_ENC1_B_Pin);
    // Calculate estimated states with debounce
    if(stateA_raw == stateAPrev_raw) countA++;
    if(stateB_raw == stateBPrev_raw) countB++;
    if(countA >= ENCODER_DEBOUNCE_COUNT){
      stateA = stateA_raw;
      countA = 0;
    }
    if(countB >= ENCODER_DEBOUNCE_COUNT){
      stateB = stateB_raw;
      countB = 0;
    }
*/

#include "driverUtils.h"
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
} Encoder_t;

void initEncoderStruct(uint8_t uid,Encoder_t* encoderPtr);
void updateRawEncoderValues(Encoder_t* encoderPtr,PinState_t rawStateA, PinState_t rawStateB);
int32_t getEncoderValue(Encoder_t* encoderPtr);
uint8_t getEncoderUID(Encoder_t* encoderPtr);
#endif /* INC_ENCODER_H_ */
