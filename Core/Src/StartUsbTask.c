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

uint8_t buf[120];
void StartUsbTask(void *argument)
{
  uint32_t notificationNumber = 0;
  uint32_t data = 0;
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      sprintf(buf, "Usb Task: Notification %u has been taken\r\n", notificationNumber);

      if (xQueueReceive(queueToUsb, &data, 0) == pdTRUE) {
        sprintf(buf + strlen(buf), "Usb Task: Data received: %u\r\n", data);
      }
    }
    else {
      sprintf(buf, "Usb Task: Shit happens... Notification %u.\r\n", notificationNumber);
    }
    CDC_Transmit_FS(buf, strlen(buf));
  }
}
