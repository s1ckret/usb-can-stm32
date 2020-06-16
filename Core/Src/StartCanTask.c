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

void StartCanTask(void *argument)
{
  uint32_t notificationNumber = 0;
  uint8_t data[8] = {0};
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      while (uxQueueMessagesWaiting(queueToCan)) {
        if (xQueueReceive(queueToCan, data, 0) == pdTRUE) {
          CAN_Send(&data);
        }
      }
    }
  }
}
