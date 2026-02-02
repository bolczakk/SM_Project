/*
 * sensors.c
 *
 * Created on: Jan 23, 2026
 * Author: Oleg
 */

/**
  ******************************************************************************
  * @file           : sensors.c
  * @brief          : Implementation of DS18B20 Sensor Driver.
  * Handles the 1-Wire communication protocol, sensor configuration,
  * and raw data conversion.
  * @author         : Oleg Swerblewski, Dawid Sobieska
  * @date           : 2026-01-23
  ******************************************************************************
  */

#include "sensors.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/

/** @brief DS18B20 Library Instance */
ds18b20_t ds18;

/** @brief Internal buffer for the last valid temperature reading */
static float current_temperature = 0.0f;

/* Public functions ----------------------------------------------------------*/

void Sensors_Init(TIM_HandleTypeDef* htim){
    /* --- Hardware Abstraction Layer (HAL) Configuration --- */
    /* Define which GPIO and Timer are used for 1-Wire communication */
    ow_init_t ow_init_struct;
    ow_init_struct.tim_handle = htim;       /**< Timer for timing (us) */
    ow_init_struct.gpio = GPIOA;            /**< Port: GPIOA */
    ow_init_struct.pin = GPIO_PIN_1;        /**< Pin: PA1 */
    ow_init_struct.tim_cb = NULL;           /**< No custom callback needed */
    ow_init_struct.done_cb = NULL;

    /* Initialize Low-Level 1-Wire Driver */
    ds18b20_init(&ds18, &ow_init_struct);

    /* Read ROM ID (Address) of the sensor */
    ds18b20_update_rom_id(&ds18);

    /* --- Sensor Logic Configuration --- */
    /* Set resolution and alarm thresholds */
    ds18b20_config_t ds18_conf = {
        .alarm_high = 50,                   /**< High Alarm Threshold */
        .alarm_low = -50,                   /**< Low Alarm Threshold */
        .cnv_bit = DS18B20_CNV_BIT_12       /**< 12-bit Resolution (0.0625 C step) */
    };
    ds18b20_conf(&ds18, &ds18_conf);
}

void Sensors_Process(void){
    /* 1. Start Temperature Conversion */
    ds18b20_cnv(&ds18);

    /* 2. Wait for Conversion to complete (Blocking!) */
    /* Note: In 12-bit mode, this takes up to 750ms */
    while(ds18b20_is_busy(&ds18));

    /* 3. Request Data Read */
    ds18b20_req_read(&ds18);

    /* 4. Wait for Data Transfer */
    while(ds18b20_is_busy(&ds18));

    /* 5. Read and Store Temperature */
    /* The library returns the value, we store it for the Getter */
    current_temperature = ds18b20_read_c(&ds18);
}

float Sensors_GetTemperature(void){
    /* Return scaled temperature */
    /* Note: Division by 100.0f implies the library returns fixed-point
       or an integer representation (e.g. 2500 for 25.00 C) */
    return current_temperature / 100.0f;
}
