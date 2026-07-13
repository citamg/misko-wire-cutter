/*
 * motor.c
 *
 *  Created on: 3 Jul 2026
 *      Author: matic
 */
#include "motor.h"


typedef struct{

	GPIO_TypeDef* 	pwm_port;
	uint32_t 		pwm_pin;

	GPIO_TypeDef* 	en_port;
	uint32_t 		en_pin;

	GPIO_TypeDef* 	dir_port;
	uint32_t 		dir_pin;

	uint32_t 		steps;
} motorHandle;

typedef struct{

	motorHandle motors[1];

}motorGroupHandle;

motorGroupHandle motorGroup;

float gear_circ = 22.8;

void InitMotor(void){

	//inicializacija pinov za 3 motorje, trenutno samo za enega -> spremeniti in narediti array strcutov motorjev

	motorGroup.motors[EX_MOTOR].pwm_port = GPIOF;
	motorGroup.motors[EX_MOTOR].pwm_pin = LL_GPIO_PIN_7;

	motorGroup.motors[EX_MOTOR].en_port = GPIOA;
	motorGroup.motors[EX_MOTOR].en_pin = LL_GPIO_PIN_1;

	motorGroup.motors[EX_MOTOR].dir_port = GPIOA;
	motorGroup.motors[EX_MOTOR].dir_pin = LL_GPIO_PIN_0;

}


void MotorDirectionRight(motorsEnum motor){

	//smer v desno je iz pogleda motorja, smer v desno porine žico naprej, DIR pin = HIGH

	LL_GPIO_SetOutputPin(motorGroup.motors[motor].dir_port, motorGroup.motors[motor].dir_pin);

}

void MotorDirectionLeft(motorsEnum motor){

	//smer v levo je iz pogleda motorja, smer v levo porine žico nazaj, DIR pin = LOW

	LL_GPIO_ResetOutputPin(motorGroup.motors[motor].dir_port, motorGroup.motors[motor].dir_pin);

}


void MotorDisable(motorsEnum motor){

	//če je EN pin high je motor ustavljen

	LL_GPIO_SetOutputPin(motorGroup.motors[motor].en_port, motorGroup.motors[motor].en_pin);

}

void MotorEnable(motorsEnum motor){

	//če je EN pin low je motor prižgan

	LL_GPIO_ResetOutputPin(motorGroup.motors[motor].en_port, motorGroup.motors[motor].en_pin);

}


void MmsToMove(motorsEnum motor, uint16_t milimeters, dirEnum direction){



	if(direction == DIR_RIGHT) MotorDirectionRight(motor);
	else MotorDirectionLeft(motor);

	uint16_t amountOfSpins = (milimeters * 1000 / gear_circ);
	uint32_t steps = (amountOfSpins * STEPS_FOR_WHOLE_CIRCUMFERENCE)/1000;
	motorGroup.motors[motor].steps = steps;
	HAL_Delay(500);
	MotorEnable(motor);
	HAL_TIM_Base_Start_IT(&htim5);
}




volatile uint32_t my_counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // check if interrupt came from TIM5
    if (htim->Instance == TIM5)
    {
        my_counter++;
        if(my_counter >= motorGroup.motors[EX_MOTOR].steps){
        	MotorDisable(EX_MOTOR);
        	my_counter = 0;
        	HAL_TIM_Base_Stop_IT(&htim5);
        }
    }
}

void CuttingSequence(void){
	MmsToMove(EX_MOTOR, 8, DIR_RIGHT);
	HAL_Delay(5000);
	MmsToMove(EX_MOTOR, 100, DIR_RIGHT);
	HAL_Delay(5000);
	MmsToMove(EX_MOTOR, 8, DIR_RIGHT);
}

