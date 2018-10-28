/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/
 #include "sapi.h"

//#ifndef _APP_H_
//#define _APP_H_

#ifndef MEFASCENSOR_H
#define MEFASCENSOR_H

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[tipos de datos declarados por el usuario]===============*/

typedef enum {
    EN_PLANTA_BAJA,
    MODO_CONFIGURACION,
    BAJANDO,
    SUBIENDO,
    PARADO,
    YENDO_A_PLANTA_BAJA,
} fsm_state_t;

/*==================[declaraciones de datos externos]========================*/



//Variables globales



fsm_state_t fsmState;
int piso_actual;
bool_t flgFinConfig;


//declaracion Buffer circular
circularBufferNew(pisosPendientes, 1 , 10);


/*==================[declaraciones de funciones externas]====================*/
void mefAscensorInit();
void mefAscensorUpdate();
bool_t mefModoConfiguracionUpdate();

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _APP_H_ */