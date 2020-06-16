/*
 * StartLcdTask.c
 *
 *  Created on: Jun 12, 2020
 *      Author: S1ckret
 */


#include "StartLcdTask.h"

#include "FreeRTOS.h"
#include "task.h"

#include "utlCAN.h"

#include "std_lcd.h"

#define N_COLUMNS 16
#define START_POS 8
#define CAN_MAX_EXT_ID 0x1FFFFFFF

static int8_t x = 8;
static int8_t y = 0;

static uint32_t DLC = 0;
static uint32_t Filter = 0;

typedef void (*process_ctrl)(void);

static void __ctrl_up(void);
static void __ctrl_down(void);
static void __ctrl_left(void);
static void __ctrl_right(void);
static void __ctrl_set(void);
static void __ctrl_filter_set(void);

static uint8_t unsaved_flag = 0;
static uint8_t is_filter_enabled = 0;
static void __ctrl_disp_unsaved(void);

static process_ctrl glueTable[LCD_CTRL_COUNT] =
{
  __ctrl_up,
  __ctrl_down,
  __ctrl_left,
  __ctrl_right,
  __ctrl_set,
  __ctrl_filter_set,
};

void StartLcdTask(void *argument)
{
  DLC = CAN_GetDLC();
  Filter = CAN_GetFilter();

  printf("DLC:    %u\n", DLC);
  printf("Fltr: 0x%08x\n", Filter);
  LCD_SetPos(x, y);

  uint32_t notificationNumber = 0;
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      glueTable[notificationNumber]();
    }
  }
}

static void __ctrl_up(void)
{
  if (x == START_POS && y == 0)
  {
    DLC++;
    if (DLC > 8) DLC = 1;
    LCD_SetPos(START_POS, 0);
    printf("%u\n", DLC);

    unsaved_flag |= 1;
  }
  else if (x > 7 && x < N_COLUMNS && y == 1) {
    uint32_t old = Filter;
    /*
    * Calculate increment for selected digit in hex number.
    * Shift bit to corresponding digit in hex number.
    */
    uint32_t diff = 0b1 << (4 * (N_COLUMNS - 1 - x));

    Filter += diff;
    if (Filter > CAN_MAX_EXT_ID) {
      /* Calculate overflow */
      Filter = diff - (CAN_MAX_EXT_ID - old) - 1;
    }

    LCD_SetPos(START_POS, 1);
    printf("%08x\n", Filter);

    unsaved_flag |= 2;
  }
  else {
    /* Change line */
    y ^= 1;
  }
  __ctrl_disp_unsaved();
  LCD_SetPos(x, y);
}

static void __ctrl_down(void)
{
  if (x == START_POS && y == 0)
  {
    DLC--;
    if (DLC < 1) DLC = 8;
    LCD_SetPos(START_POS, 0);
    printf("%u\n", DLC);

    unsaved_flag |= 1;
  }
  else if (x > START_POS - 1 && x < N_COLUMNS && y == 1) {
    uint32_t old = Filter;
    /*
     * Calculate decrement for selected digit in hex number.
     * Shift bit to corresponding digit in hex number.
     */
    uint32_t diff = 0b1 << (4 * (N_COLUMNS - 1 - x));

    Filter -= diff;
    if (Filter > CAN_MAX_EXT_ID) {
        /* Calculate overflow */
      Filter = CAN_MAX_EXT_ID + old - diff + 1;
    }

    LCD_SetPos(START_POS, 1);
    printf("%08x\n", Filter);

    unsaved_flag |= 2;
  }
  else {
    /* Change line */
    y ^= 1;
  }
  __ctrl_disp_unsaved();
  LCD_SetPos(x, y);
}

static void __ctrl_left(void)
{
  x--;
  if (x < 0) x = 15;
  LCD_SetPos(x, y);
}

static void __ctrl_right(void)
{
  x++;
  if (x > 15) x = 0;
  LCD_SetPos(x, y);
}

static void __ctrl_set(void)
{
  if (y == 0) {
    CAN_SetDLC(DLC);
    LCD_SetPos(13, 0);
    LCD_SendChar(' ');
    unsaved_flag &= 0b10;
  }
  else {
    CAN_SetFilter(Filter);
    LCD_SetPos(15, 0);
    LCD_SendChar(' ');
    unsaved_flag &= 0b01;
  }
  LCD_SetPos(x, y);
}

static void __ctrl_disp_unsaved(void)
{
  if (unsaved_flag & 0b01) {
    LCD_SetPos(13, 0);
    LCD_SendChar('U');
  }
  if (unsaved_flag & 0b10) {
    LCD_SetPos(15, 0);
    LCD_SendChar('U');
  }
  LCD_SetPos(x, y);
}

static void __ctrl_filter_set(void)
{
  is_filter_enabled ^= 1;
  if (is_filter_enabled) {
    CAN_FilterOn();
    LCD_SetPos(14, 0);
    LCD_SendChar('F');
  }
  else {
    CAN_FilterOff();
    LCD_SetPos(14, 0);
    LCD_SendChar(' ');
  }
  LCD_SetPos(x, y);
}
