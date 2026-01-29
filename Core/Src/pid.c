/*
 * pid.c
 *
 *  Created on: Jan 23, 2026
 *      Author: Oleg
 */

#include "pid.h"

void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float out_min, float out_max) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}

float PID_Compute(PID_Controller *pid, float setpoint, float measured_value, float dt) {
    float error = measured_value - setpoint;

    float P_out = pid->Kp * error;

    // Człon Całkujący
    pid->integral += error * dt;

    // Prosty Anti-Windup
    if (pid->integral * pid->Ki > pid->out_max) pid->integral = pid->out_max / pid->Ki;
    if (pid->integral * pid->Ki < pid->out_min) pid->integral = pid->out_min / pid->Ki;

    float I_out = pid->Ki * pid->integral;

    float derivative = (error - pid->last_error) / dt;
    float D_out = pid->Kd * derivative;

    // Suma sygnałów
    float output = P_out + I_out + D_out;

    // Saturacja wyjścia (0 - 100% PWM)
    if (output > pid->out_max) output = pid->out_max;
    else if (output < pid->out_min) output = pid->out_min;

    pid->last_error = error;

    return output;
}

void PID_Reset(PID_Controller *pid) {
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}
