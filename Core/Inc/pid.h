/*
 * pid.h
 *
 *  Created on: Jan 23, 2026
 *      Author: Oleg
 */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float integral;
    float last_error;

    float out_min;
    float out_max;
} PID_Controller;

void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float out_min, float out_max);

float PID_Compute(PID_Controller *pid, float setpoint, float measured_value, float dt);

void PID_Reset(PID_Controller *pid);

#endif /* INC_PID_H_ */
