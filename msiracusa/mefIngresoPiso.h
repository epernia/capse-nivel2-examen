#ifndef MEFINGRESOPISO_H 
#define MEFINGRESOPISO_H 

#include "sapi.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


/* enum que contiene los posibles estados de la MEF*/
typedef enum mefIngresoPisoState{
             EN_ESPERA_DE_DIGITO_1,
             EN_ESPERA_DE_DIGITO_2_O_LETRA,
             EN_ESPERA_DE_LETRA,
             GUARDAR_PISO,
             NUM_ESTADOS,
} mefIngresoPisoState_t;

/* la variable piso contiene el valor numérico del piso */
/* seleccionado */
static uint8_t piso;
/* variables para recorrer el teclado matricial */
static const uint8_t keypad_cols[4] = {T_COL0, T_FIL2, T_FIL3, T_FIL0}; //FIXME
static const uint8_t keypad_rows[4] = {T_COL1, CAN_TD, CAN_RD, RS232_TXD}; //FIXME

/* tabla de mapeo: filaxcolumna -> carcter ascii*/
//FIXME: Cambiar a algo que tenga sentido
static const char keypad_characters[4][4] = {{'D', '#', '0', '*'}, // 1ra fila
                                             {'C', '9', '8', '7'}, // 2da fila
                                             {'B', '6', '5', '4'},
                                             {'A', '3', '2', '1'}};
 
/* esta variable contiene el estado actual de la MEF */
mefIngresoPisoState_t mefIngresoPisoCurrentState;

void mefIngresoPisoUpdate( void );
uint8_t readKeypad();
uint8_t updateLCDDisplayOnKeyPress();
uint8_t printLCDMessage(char * msg[static 2]);
uint8_t updateTimer();
uint8_t cancelTimer();



#endif /*INGRESOPISO.H*/
