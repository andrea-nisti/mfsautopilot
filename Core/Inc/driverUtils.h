/*
 * driverUtils.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_DRIVERUTILS_H_
#define INC_DRIVERUTILS_H_

#include <stdint.h>

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

EdgeState_t checkRisingFallingState(PinState_t actual, PinState_t previous);
void initDebouncer(DebouncedState_t* stateStructPtr, uint16_t debouncerMaxCount);
PinState_t updateDebouncedSignal(DebouncedState_t* stateStructPtr, PinState_t actualRawState);

#endif /* INC_DRIVERUTILS_H_ */
