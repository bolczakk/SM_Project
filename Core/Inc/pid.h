/*
 * pid.h
 *
 * Created on: Jan 23, 2026
 * Author: Oleg
 */

/**
  ******************************************************************************
  * @file           : pid.h
  * @brief          : Header file for Discrete PID Controller Module.
  * Contains the configuration structure and public interface.
  * @author         : Oleg Swerblewski, Dawid Sobieska
  * @date           : 2026-01-23
  ******************************************************************************
  * @attention
  * This module implements a parallel form PID controller with Anti-Windup.
  *
  ******************************************************************************
  */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct {
    /* --- Configuration (Tuning) --- */
    float Kp;           /**< Proportional Gain (P-term weight) */
    float Ki;           /**< Integral Gain (I-term weight) */
    float Kd;           /**< Derivative Gain (D-term weight) */

    /* --- Runtime State (Internal Memory) --- */
    float integral;     /**< Accumulated Integral sum (History of error) */
    float last_error;   /**< Error value from the previous step (for Derivative) */

    /* --- Constraints (Safety limits) --- */
    float out_min;      /**< Minimum output limit (e.g. 0.0 for PWM) */
    float out_max;      /**< Maximum output limit (e.g. 100.0 for PWM) */
} PID_Controller;

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Initializes the PID Controller instance.
 * Sets the tuning parameters and clears internal state buffers.
 * @param  pid      Pointer to the PID_Controller configuration structure.
 * @param  kp       Proportional gain value.
 * @param  ki       Integral gain value.
 * @param  kd       Derivative gain value.
 * @param  out_min  Minimum possible output value (Saturation lower limit).
 * @param  out_max  Maximum possible output value (Saturation upper limit).
 * @retval None
 */

void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float out_min, float out_max);

/**
 * @brief  Computes the control output based on the PID algorithm.
 * Equation: u(t) = Kp*e(t) + Ki*Integral(e(t)) + Kd*Derivative(e(t))
 * Note: Error is calculated as (Measured - Setpoint) for Direct Action (Cooling).
 * @param  pid             Pointer to the initialized PID_Controller structure.
 * @param  setpoint        Target value (Desired setpoint).
 * @param  measured_value  Current process value (Feedback).
 * @param  dt              Time delta in seconds since the last call (Sampling time).
 * @retval Control output signal, clamped between out_min and out_max.
 */
float PID_Compute(PID_Controller *pid, float setpoint, float measured_value, float dt);

/**
 * @brief  Resets the internal state of the PID controller.
 * Clears the accumulated integral and the last error value.
 * Use this when switching from Manual to Automatic mode.
 * @param  pid  Pointer to the PID_Controller structure.
 * @retval None
 */
void PID_Reset(PID_Controller *pid);

#endif /* INC_PID_H_ */
