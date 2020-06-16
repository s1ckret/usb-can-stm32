/*
 * StartUsbTask.c
 *
 *  Created on: Jun 12, 2020
 *      Author: S1ckret
 */


#include "StartUsbTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "std_lcd.h"

extern QueueHandle_t queueToUsb;

void StartUsbTask(void *argument)
{
  uint32_t notificationNumber = 0;
  uint8_t data[9] = {0};
  LCD_SetPos(0, 0);
  printf("Device. DLC: %u\n", CAN_GetDLC());
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      while (uxQueueMessagesWaiting(queueToUsb)) {
        if (xQueueReceive(queueToUsb, data, 0) == pdTRUE) {
          vTaskDelay(10);
          LCD_SetPos(0, 1);
          printf("%s\n", data);
        }
      }
    }
  }
}
