/*
 * motor.h
 *
 *  Created on: 3 Jul 2026
 *      Author: matic
 */

#ifndef INCLUDE_MOTOR_H_
#define INCLUDE_MOTOR_H_


#include "stm32g4xx_ll_gpio.h"
#include "main.h"



#define STEPS_FOR_WHOLE_CIRCUMFERENCE 1600 //1,8deg motor, driver ima 1/8 mikrokorakov s MS1 in MS2 pini na LOW



extern TIM_HandleTypeDef htim5;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);



typedef enum { EX_MOTOR, CUT_MOTOR_1, CUT_MOTOR_2, NUM_OF_MOTORS } motorsEnum;
typedef enum { DIR_LEFT, DIR_RIGHT } dirEnum;



void InitMotor(void);
void MotorDirectionRight(motorsEnum motor);
void MotorDirectionLeft(motorsEnum motor);
void MotorEnable(motorsEnum motor);
void MotorDisable(motorsEnum motor);
void MmsToMove(motorsEnum motor, uint16_t milimeters, dirEnum direction);
void CuttingSequence(uint16_t strip_len, uint16_t wire_len, uint16_t qty);
void StepEngineStart(motorsEnum motor, uint32_t steps);
void StepEngineStop(motorsEnum motor);
uint8_t StepEngineIsBusy(motorsEnum motor);

void StepEngineSetProfile(motorsEnum motor, float v_min, float v_target, float accel);
void MotionControlTick(void);

#endif /* INCLUDE_MOTOR_H_ */
