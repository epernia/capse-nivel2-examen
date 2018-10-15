#ifndef MEFINGRESOPISO_H 
#define MEFINGRESOPISO_H 

#include "sapi.h"

/* enum que contiene los posibles estados de la MEF*/
typedef enum{
             EN_ESPERA_DE_DIGITO_1,
             EN_ESPERA_DE_DIGITO_2_O_LETRA,
             EN_ESPERA_DE_LETRA,
             GUARDAR_PISO,
             NUM_ESTADOS,
} mefIngresoPisoState_t;

/* la variable piso contiene el valor numérico del piso */
/* seleccionado */
static uint8_t piso;


/* esta variable contiene el estado actual de la MEF */
mefIngresoPisoState_t mefIngresoPisoCurrentState;

void mefIngresoPisoUpdate( void );


#endif /*INGRESOPISO.H*/
