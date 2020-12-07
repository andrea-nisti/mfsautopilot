/*
 * button.h
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "driverUtils.h"

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
  uint32_t pushTimeTrigger;

  //Button states
  PinStateTS_t actualState;
  PinStateTS_t previousState;

} Button_t;

void buttonInitStruct(
    Button_t* buttonStructPtr, PinState_t initialStrate, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void), void (*onPushedTimeElapsed)(void));

// For now, activate one detection by passing a valid pointer to func, if NULL detection is not used
void buttonUpdateState(Button_t* buttonStructPtr, uint32_t currentMilli);

#endif /* INC_BUTTON_H_ */
