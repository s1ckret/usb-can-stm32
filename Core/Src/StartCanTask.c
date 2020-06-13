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

extern CAN_HandleTypeDef hcan1;
extern QueueHandle_t queueToCan;

uint8_t buf[120];
void StartCanTask(void *argument)
{
  uint32_t notificationNumber = 0;
  uint32_t data = 0;
  uint32_t mailbox = 0;
  CAN_TxHeaderTypeDef txHeader = {
    .DLC = 1,
    .IDE = CAN_ID_STD,
    .RTR = CAN_RTR_DATA,
    .StdId = 0x1,
  };
  for(;;)
  {
    if (xTaskNotifyWait(pdFALSE, pdTRUE, &notificationNumber, portMAX_DELAY) == pdTRUE) {
      sprintf(buf, "Can Task: Notification %u has been taken\r\n", notificationNumber);
      while (uxQueueMessagesWaiting(queueToCan)) {
        if (xQueueReceive(queueToCan, &data, 0) == pdTRUE) {
          HAL_CAN_AddTxMessage(&hcan1, &txHeader, &data, &mailbox);
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
