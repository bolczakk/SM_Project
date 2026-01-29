/*
 * sensors.c
 *
 *  Created on: Jan 23, 2026
 *      Author: Oleg
 */

#include "sensors.h"
#include <stdio.h>

ds18b20_t ds18;
static float current_temperature = 0.0f;

void Sensors_Init(TIM_HandleTypeDef* htim){
	ow_init_t ow_init_struct;
	ow_init_struct.tim_handle = htim;
	ow_init_struct.gpio = GPIOA;
	ow_init_struct.pin = GPIO_PIN_1;
	ow_init_struct.tim_cb = NULL;
	ow_init_struct.done_cb = NULL;

	ds18b20_init(&ds18, &ow_init_struct);
	ds18b20_update_rom_id(&ds18);

	ds18b20_config_t ds18_conf = {
		.alarm_high = 50,
		.alarm_low = -50,
		.cnv_bit = DS18B20_CNV_BIT_12
	};
	ds18b20_conf(&ds18, &ds18_conf);
}

void Sensors_Process(void){
	ds18b20_cnv(&ds18);
	while(ds18b20_is_busy(&ds18));
	ds18b20_req_read(&ds18);
	while(ds18b20_is_busy(&ds18));

	current_temperature = ds18b20_read_c(&ds18);
}

float Sensors_GetTemperature(void){
	return current_temperature/100.0f;
}
