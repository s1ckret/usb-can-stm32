/*
 * utlCAN.h
 *
 *  Created on: 14 июн. 2020 г.
 *      Author: S1ckret
 */

#ifndef INC_UTLCAN_H_
#define INC_UTLCAN_H_

#include "stm32f4xx_hal.h"

void CAN_Init(void);

void CAN_FilterOn();
void CAN_FilterOff();

void CAN_SetFilter(uint32_t filterValue);
void CAN_SetDLC(uint8_t dlc);

uint32_t CAN_GetFilter(void);
uint8_t CAN_GetDLC(void);

void CAN_Send(uint8_t aData[]);

#endif /* INC_UTLCAN_H_ */
