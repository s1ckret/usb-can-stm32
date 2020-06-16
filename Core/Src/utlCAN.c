/*
 * utlCAN.c
 *
 *  Created on: 14 июн. 2020 г.
 *      Author: S1ckret
 */

#include "utlCAN.h"

CAN_HandleTypeDef hcan1;

static CAN_FilterTypeDef filter;
static uint32_t mailbox = 0;

CAN_TxHeaderTypeDef txHeader = {
  .DLC = 8,
  .IDE = CAN_ID_EXT,
  .RTR = CAN_RTR_DATA,
  .ExtId = 0xDEAD,
};

void CAN_Init(void) {
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 1;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

  filter.FilterActivation = ENABLE;
  filter.FilterBank = 0;
  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  filter.FilterIdHigh = (uint16_t)(0x1fbf9001>>13);
  filter.FilterIdLow = (uint16_t)(0x1fbf9001<<3) | CAN_ID_EXT;
  /*Only accept one message at once*/
  filter.FilterMaskIdHigh = (uint16_t)(0xffffffff>>13);
  filter.FilterMaskIdLow = (uint16_t)(0xffffffff<<3) | CAN_ID_EXT;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_CAN_Start(&hcan1) != HAL_OK) {
    Error_Handler();
  }
}

void CAN_FilterOn()
{
  filter.FilterMaskIdHigh = (uint16_t)(0xffffffff>>13);
  filter.FilterMaskIdLow = (uint16_t)(0xffffffff << 3) | CAN_ID_EXT;
  if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
    Error_Handler();
  }
}

void CAN_FilterOff()
{
  filter.FilterMaskIdHigh = (uint16_t)(0x0 >> 13);
  filter.FilterMaskIdLow = (uint16_t)(0x0 << 3) | CAN_ID_EXT;
  if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
    Error_Handler();
  }

}

void CAN_SetFilter(uint32_t filterValue)
{
  filter.FilterIdHigh = (uint16_t)(filterValue >> 13);
  filter.FilterIdLow = (uint16_t)(filterValue << 3) | CAN_ID_EXT;
  if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
    Error_Handler();
  }
}

void CAN_SetDLC(uint8_t dlc)
{
  txHeader.DLC = dlc;
}

uint32_t CAN_GetFilter(void)
{
  return filter.FilterIdLow + filter.FilterIdHigh << 4;
}

uint8_t CAN_GetDLC(void)
{
  return txHeader.DLC;
}

void CAN_Send(uint8_t aData[])
{
  HAL_CAN_AddTxMessage(&hcan1, &txHeader, aData, &mailbox);
}
