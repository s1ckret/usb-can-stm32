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

uint32_t DLC = 3;
uint32_t Filter = 0xBEE;

void StartLcdTask(void *argument)
{

  vTaskDelay(1000);
  printf("DLC:    %u\n", DLC);
  printf("Fltr: 0x%05x\n", Filter);

  uint32_t notificationNumber = 0;
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {

    }
  }
}

