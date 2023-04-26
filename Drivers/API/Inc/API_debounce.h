
#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "API_delay.h"

#define FSM_UPDATE_PERIOD 40

void debounceFSM_init();
void debounceFSM_update();


typedef enum{
	QUIETO,
	ACELERANDO,
} debounceState_t;

typedef bool bool_t;

#endif /* API_INC_API_DEBOUNCE_H_ */
