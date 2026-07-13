

#include "periodic_services.h"
#include "timing_utils.h"
#include "stm32g4xx_ll_tim.h"
//#include "kbd.h"


typedef struct{
	TIM_TypeDef* timer;
}periodic_services_handle_t;


periodic_services_handle_t periodic_services;

void PSERV_init(void){
	periodic_services.timer = TIM6;
	LL_TIM_EnableCounter(periodic_services.timer);
}


void PSERV_enable(void){
	LL_TIM_EnableIT_UPDATE(periodic_services.timer);

}

void PSERV_disable(void){
	LL_TIM_DisableIT_UPDATE(periodic_services.timer);
}


void PSERV_run_services_Callback(void){
	//KBD_scan();
	//KBD_demo_toggle_LEDs_if_buttons_pressed();
}
