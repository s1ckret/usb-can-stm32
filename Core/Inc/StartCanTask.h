/*
 * StartCanTask.h
 *
 *  Created on: Jun 13, 2020
 *      Author: S1ckret
 */

#ifndef INC_STARTCANTASK_H_
#define INC_STARTCANTASK_H_

/*
 * This task receive data from @queueToCan
 * and wakes up when notified.
 */
void StartCanTask(void *argument);

#endif /* INC_STARTCANTASK_H_ */
