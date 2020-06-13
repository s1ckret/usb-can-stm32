/*
 * StartUsbTask.h
 *
 *  Created on: Jun 12, 2020
 *      Author: S1ckret
 */

#ifndef INC_STARTUSBTASK_H_
#define INC_STARTUSBTASK_H_

/*
 * This task receive data from @queueToUsb
 * and wakes up when notified.
 */
void StartUsbTask(void *argument);

#endif /* INC_STARTUSBTASK_H_ */
