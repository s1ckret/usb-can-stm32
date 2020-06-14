/*
 * StartLcdTask.c
 *
 *  Created on: Jun 12, 2020
 *      Author: S1ckret
 */


#include "StartLcdTask.h"

#include "FreeRTOS.h"
#include "task.h"

#include "std_lcd.h"

static int8_t x = 8;
static int8_t y = 0;

static uint32_t DLC = 3;
static uint32_t Filter = 0x1FFFFFFE;

typedef void (*process_ctrl)(void);

static void __ctrl_up(void);
static void __ctrl_down(void);
static void __ctrl_left(void);
static void __ctrl_right(void);
static void __ctrl_set(void);

static process_ctrl glueTable[LCD_CTRL_COUNT] =
{
  __ctrl_up,
  __ctrl_down,
  __ctrl_left,
  __ctrl_right,
  __ctrl_set
};

void StartLcdTask(void *argument)
{

  vTaskDelay(1000);
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
  if (x == 8 && y == 0)
  {
    DLC++;
    if (DLC > 8) DLC = 1;
    LCD_SetPos(8, 0);
    printf("%u\n", DLC);
  }
  else if (x > 7 && x < 17 && y == 1) {
    uint32_t old = Filter;
    Filter += 0x1 << (4 * (15 - x));
    LCD_SetPos(8, 1);
    if (Filter > 0x1FFFFFFF) Filter = (0x1 << (4 * (15 - x))) - (0x1FFFFFFF - old) - 1;
    printf("%08x\n", Filter);
  }
  else {
    y ^= 1;
  }
  LCD_SetPos(x, y);
}

static void __ctrl_down(void)
{
  if (x == 8 && y == 0)
  {
    DLC--;
    if (DLC < 1) DLC = 8;
    LCD_SetPos(8, 0);
    printf("%u\n", DLC);
  }
  else if (x > 7 && x < 17 && y == 1) {
    uint32_t old = Filter;
    Filter -= 0x1 << (4 * (15 - x));
    if (Filter > 0x1FFFFFFF) Filter = 0x1FFFFFFF + old - (0x1 << (4 * (15 - x))) + 1;
    LCD_SetPos(8, 1);
    printf("%08x\n", Filter);
  }
  else {
    y ^= 1;
  }
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

}
