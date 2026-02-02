/*
 * pid.c
 *
 * Created on: Jan 23, 2026
 * Author: Oleg
 */

/**
  ******************************************************************************
  * @file           : pid.c
  * @brief          : Implementation of Discrete PID Controller functionality.
  * Implements the core logic for error calculation,
  * integral clamping (anti-windup), and output saturation.
  * @author         : Oleg Swerblewski, Dawid Sobieska
  * @date           : 2026-01-23
  ******************************************************************************
  */

#include "pid.h"

void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float out_min, float out_max) {
    /* Set Configuration Parameters */
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->out_min = out_min;
    pid->out_max = out_max;

    /* Clear Internal State */
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}

float PID_Compute(PID_Controller *pid, float setpoint, float measured_value, float dt) {
    /* 1. Calculate Error */
    /* Note: For Cooling (Fan), Error = Measured - Setpoint.
       If Temp > Setpoint, Error is positive, Fan speeds up. */
    float error = measured_value - setpoint;

    /* 2. Proportional Term */
    float P_out = pid->Kp * error;

    /* 3. Integral Term with Anti-Windup */
    pid->integral += error * dt;

    /* Anti-Windup: Clamp the integral sum based on output limits */
    /* Logic: If Integral part alone exceeds max output, cap it. */
    if (pid->integral * pid->Ki > pid->out_max) {
        pid->integral = pid->out_max / pid->Ki;
    }
    if (pid->integral * pid->Ki < pid->out_min) {
        pid->integral = pid->out_min / pid->Ki;
    }

    float I_out = pid->Ki * pid->integral;

    /* 4. Derivative Term */
    float derivative = (error - pid->last_error) / dt;
    float D_out = pid->Kd * derivative;

    /* 5. Total Output Calculation */
    float output = P_out + I_out + D_out;

    /* 6. Output Saturation */
    if (output > pid->out_max) {
        output = pid->out_max;
    } else if (output < pid->out_min) {
        output = pid->out_min;
    }

    /* 7. Save state for next iteration */
    pid->last_error = error;

    return output;
}

void PID_Reset(PID_Controller *pid) {
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}
