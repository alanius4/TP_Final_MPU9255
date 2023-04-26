

#include "API_debounce.h"

static debounceState_t debounce;	// Variable interna de maquina de estados
static delay_t delayFSM;
static bool_t raising = false;		//Variable si el boton se presinó
static bool_t falling = false;		//Variable si el boton se soltó

//Inicializa la maquina de estados con boton en alto, inicializaliza el pulsador
void debounceFSM_init() {
	debounce = QUIETO;
	delayInit(&delayFSM, FSM_UPDATE_PERIOD); //inicializa el retardo no bloqueante de verifiacion de la MEF
}

//Implementacion de maquina de estados, chequea si el mpu esta acelerando o esta quieto
void debounceFSM_update(x,y,z) {
	if (delayRead(&delayFSM)) {
		switch (debounce) {
		case QUIETO:
			if ((x!=0)||(y!=0)||(z!=0)) {
				debounce = ACELERANDO;
			}
			break;
		case ACELERANDO:
			if ((x!=0)||(y!=0)||(z!=0)) {
				debounce = ACELERANDO;
			} else {
				debounce = QUIETO;
			}
			break;
		default:
			break;
		}
	}
}

