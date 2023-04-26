
#include "API_delay.h"
#include "stm32f4xx_hal.h"  		/* Necesario para corregir un warning al usar HAL_GetTick*/

// Iniciliza estructura de delay
void delayInit(delay_t *delay, tick_t duration) {
	delay->duration = duration;
	delay->running = false;
}

//Devuelve 1 si se cumplio la duración del retardo, en caso de no estar corriendo cambia el estado de delay->running a true
bool_t delayRead(delay_t *delay) {
	if(delay->duration == 0)
		return false;	// Check de parámetros
	if (!delay->running) {
		delay->running = true;
		delay->startTime = HAL_GetTick();
		return false;
	} else {
		if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
			delay->running = false;
			return true;
		} else
			return false;
	}
}

// Permite cambiar el valor de la duración del retardo
void delayWrite(delay_t *delay, tick_t duration) {
	delay->duration = duration;
}

