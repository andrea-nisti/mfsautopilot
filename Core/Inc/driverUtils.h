/*
 * driverUtils.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_DRIVERUTILS_H_
#define INC_DRIVERUTILS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
typedef struct {
  enum {
    DRIVER_OK = 0,
    DRIVER_ERROR
  } Code;
  uint32_t flag;
} DriverStatus_t;

typedef enum {
  ALLOCATE_OK = 0,
  ALLOCATE_ERROR
} AllocateReturnFlags_t;

typedef enum
{
  PIN_RESET = 0,
  PIN_SET
}PinState_t;

typedef enum {
  EDGE_RESET = 0,
  EDGE_FALLING,
  EDGE_RISING
} EdgeState_t;

typedef struct {
  PinState_t previousRawState;
  PinState_t previousState;
  uint16_t debounceCounter;
  uint16_t debounceMaxCounter;
} DebouncedState_t;

// Stack helpers
typedef struct {
  uint8_t * actualFreePtr;
  uint8_t *  beginStackPtr;
  uint8_t *  endStackPtr;
  uint8_t  stackSize;
  uint32_t elementSize;
} StackTCB_t;

typedef struct {
  bool inUse;
  uint8_t uid;
}DriverHeader_t;

EdgeState_t checkRisingFallingState(PinState_t actual, PinState_t previous);
void initDebouncer(DebouncedState_t* stateStructPtr, uint16_t debouncerMaxCount);
PinState_t updateDebouncedSignal(DebouncedState_t* stateStructPtr, PinState_t actualRawState);
AllocateReturnFlags_t stackAllocate(uint8_t** freeReturnPtrPtr, StackTCB_t* cb, bool initDone);
#endif /* INC_DRIVERUTILS_H_ */
