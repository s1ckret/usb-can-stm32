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

#include "usbd_cdc_if.h"

extern QueueHandle_t queueToUsb;

void StartUsbTask(void *argument)
{
  uint8_t buf[60];
  uint32_t notificationNumber = 0;
  uint8_t data[8] = {0};
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      memset(buf, 0, 60);
      while (uxQueueMessagesWaiting(queueToUsb)) {
        if (xQueueReceive(queueToUsb, data, 0) == pdTRUE) {
          sprintf(buf + strlen(buf), "%s", data);
        }
      }
    }
    else {
      sprintf(buf, "UER!N=%u.\r\n", notificationNumber);
    }
    CDC_Transmit_FS(buf, strlen(buf));
  }
}
