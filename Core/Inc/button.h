/*
 * button.h
 *
 *  Created on: 7 dic 2020
 *      Author: Andrea
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "driverUtils.h"
#include <stdbool.h>
#define MAX_BUTTONS     2

typedef uint8_t btnUID_t;
typedef enum ButtonReturnFlags {
  BUTTON_OK = 0,
  BUTTON_INIT_ERROR,
  BUTTON_ALLOCATE_ERROR
} ButtonReturnFlags_t;

typedef struct {
  PinState_t state;
  uint32_t   timestamp;
} PinStateTS_t;
typedef struct {
  uint8_t using;
  bool inUse;
  uint8_t uid;
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
 * Button Driver: assumes [pressed state] from HIGH (PIN_SET) to LOW (PIN_RESET).
 * Please implement this logic inside the read function
 */
// Add a button by uid, BE AWARE: DO NOT ADD TWO BTNS WITH THE SAME ID
ButtonReturnFlags_t buttonAddByUID(
    uint8_t id, PinState_t (*read)(void),
    void (*onPushed)(void), void (*onReleased)(void),
    void (*onPushedTimeElapsed)(void), uint32_t pushTime);

Button_t const * buttonGetByUID(uint8_t uid);
// For now, activate one detection by passing a valid pointer to func, if NULL detection is not used
void buttonUpdateState(Button_t* buttonStructPtr, uint32_t currentMilli);
void buttonUpdateStateByID(uint8_t uid, uint32_t currentMilli);
void buttonUpdateAllStates(uint32_t currentMilli);

#endif /* INC_BUTTON_H_ */
