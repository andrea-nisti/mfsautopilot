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
