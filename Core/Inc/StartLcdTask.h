/*
 * StartUsbTask.h
 *
 *  Created on: Jun 12, 2020
 *      Author: S1ckret
 */

#ifndef INC_STARTUSBTASK_H_
#define INC_STARTUSBTASK_H_


/* For glue table */
enum LCD_CTRL {
  LCD_CTRL_UP = 0,
  LCD_CTRL_DOWN,
  LCD_CTRL_LEFT,
  LCD_CTRL_RIGHT,
  LCD_CTRL_SET,
  LCD_CTRL_COUNT
};

void StartLcdTask(void *argument);

#endif /* INC_STARTUSBTASK_H_ */
