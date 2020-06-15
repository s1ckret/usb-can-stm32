/*
 * StartCanTask.c
 *
 *  Created on: Jun 13, 2020
 *      Author: S1ckret
 */


#include "StartCanTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "usbd_cdc_if.h"
#include "utlCAN.h"

extern CAN_HandleTypeDef hcan1;
extern QueueHandle_t queueToCan;

uint8_t buf[120];
void StartCanTask(void *argument)
{
  uint32_t notificationNumber = 0;
  uint32_t data = 0;
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      sprintf(buf, "Can Task: Notification %u has been taken\r\n", notificationNumber);
      while (uxQueueMessagesWaiting(queueToCan)) {
        if (xQueueReceive(queueToCan, &data, 0) == pdTRUE) {
          CAN_Send(&data);
          sprintf(buf + strlen(buf), "Can Task: Data sent: %u\r\n", data);
        }
      }
    }
    else {
      sprintf(buf, "Can Task: Shit happens... Notification %u.\r\n", notificationNumber);
    }
    CDC_Transmit_FS(buf, strlen(buf));
  }
}
