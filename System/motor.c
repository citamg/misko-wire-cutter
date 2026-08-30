/*
 * motor.c
 *
 *  Created on: 3 Jul 2026
 *      Author: matic
 */
#include "motor.h"


typedef enum { STEP_IDLE, STEP_RUNNING } stepState_t;

typedef struct{

	GPIO_TypeDef* 		pwm_port;
	uint32_t 			pwm_pin;

	GPIO_TypeDef* 		en_port;
	uint32_t 			en_pin;

	GPIO_TypeDef* 		dir_port;
	uint32_t 			dir_pin;

	volatile uint32_t 	steps_target;
	volatile uint32_t	steps_done;
	volatile stepState_t state;

} motorHandle;

static motorHandle motors[NUM_OF_MOTORS];

static const float gear_circ = 22.8f;

void InitMotor(void){

	//inicializacija pinov za 3 motorje, trenutno samo za enega -> spremeniti in narediti array strcutov motorjev

	motors[EX_MOTOR].pwm_port = GPIOF;
	motors[EX_MOTOR].pwm_pin = LL_GPIO_PIN_7;

	motors[EX_MOTOR].en_port = GPIOA;
	motors[EX_MOTOR].en_pin = LL_GPIO_PIN_1;

	motors[EX_MOTOR].dir_port = GPIOA;
	motors[EX_MOTOR].dir_pin = LL_GPIO_PIN_0;

	motors[EX_MOTOR].state = STEP_IDLE;
	motors[EX_MOTOR].state = STEP_IDLE;
	motors[EX_MOTOR].steps_target = 0;
	motors[EX_MOTOR].steps_done = 0;

	//nastavljeno 50% duty cycle
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, (htim5.Init.Period + 1) / 2);

}


void MotorDirectionRight(motorsEnum motor){

	//smer v desno je iz pogleda motorja, smer v desno porine žico naprej, DIR pin = HIGH

	LL_GPIO_SetOutputPin(motors[motor].dir_port, motors[motor].dir_pin);

}

void MotorDirectionLeft(motorsEnum motor){

	//smer v levo je iz pogleda motorja, smer v levo porine žico nazaj, DIR pin = LOW

	LL_GPIO_ResetOutputPin(motors[motor].dir_port, motors[motor].dir_pin);

}


void MotorDisable(motorsEnum motor){

	//če je EN pin high je motor ustavljen

	LL_GPIO_SetOutputPin(motors[motor].en_port, motors[motor].en_pin);

}

void MotorEnable(motorsEnum motor){

	//če je EN pin low je motor prižgan

	LL_GPIO_ResetOutputPin(motors[motor].en_port, motors[motor].en_pin);

}





void StepEngineStart(motorsEnum motor, uint32_t steps){

	motors[motor].steps_target = steps;
	motors[motor].steps_done = 0;
	motors[motor].state = STEP_RUNNING;

	__HAL_TIM_SET_COUNTER(&htim5, 0);
	__HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
}

void StepEngineStop(motorsEnum motor){
	__HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);
	HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_2);
	motors[motor].state = STEP_IDLE;
}

uint8_t StepEngineIsBusy(motorsEnum motor){
	return (motors[motor].state == STEP_RUNNING);
}




void MmsToMove(motorsEnum motor, uint16_t milimeters, dirEnum direction){


	if(direction == DIR_RIGHT) 	MotorDirectionRight(motor);
	else 						MotorDirectionLeft(motor);

	uint32_t amountOfSpins = (uint32_t)((float)milimeters * 1000.0f  / gear_circ);
	uint32_t steps = (amountOfSpins * STEPS_FOR_WHOLE_CIRCUMFERENCE) / 1000;

	MotorEnable(motor);
	StepEngineStart(motor, steps);
}

void CuttingSequence(uint16_t strip_len, uint16_t wire_len, uint16_t qty){

	for(uint16_t i = 0; i < 0; i++){
		MmsToMove(EX_MOTOR, strip_len, DIR_RIGHT);
		while (StepEngineIsBusy(EX_MOTOR)) { }
		MmsToMove(EX_MOTOR, wire_len - 2*strip_len, DIR_RIGHT);
		while (StepEngineIsBusy(EX_MOTOR)) { }
		MmsToMove(EX_MOTOR, strip_len, DIR_RIGHT);
		while (StepEngineIsBusy(EX_MOTOR)) { }
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // check if interrupt came from TIM5
	if (htim->Instance == TIM5) {
		motorHandle *m = &motors[EX_MOTOR];
		if (m->state != STEP_RUNNING) return;

		m->steps_done++;
		if (m->steps_done >= m->steps_target) {
			StepEngineStop(EX_MOTOR);
			MotorDisable(EX_MOTOR);
		}
	}
}








