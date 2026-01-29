/*
 * sensors.h
 *
 *  Created on: Jan 23, 2026
 *      Author: Oleg
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include "ds18b20.h"
#include "main.h"

extern ds18b20_t ds18;

void Sensors_Init(TIM_HandleTypeDef* htim);
void Sensors_Process(void);
float Sensors_GetTemperature(void);


#endif /* INC_SENSORS_H_ */
