/*
 * driverUtils.h
 *
 *  Created on: 6 dic 2020
 *      Author: Andrea
 */

#ifndef INC_DRIVERUTILS_H_
#define INC_DRIVERUTILS_H_

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

EdgeState_t checkRisingFallingState(PinState_t actual,PinState_t previous);

#endif /* INC_DRIVERUTILS_H_ */
