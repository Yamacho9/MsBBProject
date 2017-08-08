/*
 * Bluetooth.h
 *
 *  Created on: 2016/08/11
 *      Author: ryuji
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BLUETOOTH_H_
#define WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BLUETOOTH_H_
#include <stdio.h>
#include "std.h"
#include "ev3api.h"

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *btlog;

extern void console_task(intptr_t unused);
#ifdef __cplusplus
}
#endif
bool isStartSignal();


#endif /* WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BLUETOOTH_H_ */
