/*
 * button.h
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "driverUtils.h"

typedef enum ReturnFlags {
  BUTTON_OK = 0,
  BUTTON_INIT_ERROR
} ReturnFlags_t;

typedef struct {
  PinState_t state;
  uint32_t   timestamp;
} PinStateTS_t;

typedef struct {
  //Config parameters (to be separated later on)
  PinState_t (*read)(void);
  void (*onPushed)(void);
  void (*onReleased)(void);
  void (*onPushedTimeElapsed)(void);
  uint32_t pushTime;

  struct {
    uint8_t pushedEvent       : 1;
    uint8_t releasedEvent     : 1;
    uint8_t pushedTimeElapsedEvent : 1;
  } Modes;

  //Button states
  PinStateTS_t previousState;
  struct {
    EdgeState_t event;
    uint32_t   timestamp;
  } LastEventTS;

} Button_t;

/*
 * Button Driver: assumes [pressed state] from HIGH (PIN_SET) to LOW (PIN_RESER).
 * Please implement this logic inside the read function
 */
DriverStatus_t buttonInitStruct(
    Button_t* buttonStructPtr, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void), void (*onPushedTimeElapsed)(void), uint32_t pushTime);

// For now, activate one detection by passing a valid pointer to func, if NULL detection is not used
void buttonUpdateState(Button_t* buttonStructPtr, uint32_t currentMilli);

#endif /* INC_BUTTON_H_ */
