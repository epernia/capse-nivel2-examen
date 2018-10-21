#ifndef MEFINGRESOPISO_H
#define MEFINGRESOPISO_H

#include "sapi.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* enum que contiene los posibles estados de la MEF*/
typedef enum mefIngresoPisoState {
  EN_ESPERA_DE_DIGITO_1,
  EN_ESPERA_DE_DIGITO_2_O_LETRA,
  EN_ESPERA_DE_LETRA,
  GUARDAR_PISO,
  NUM_ESTADOS,
} mefIngresoPisoState_t;

/* variables que determinan el estado del timer  */
enum mefFlags {
  TIMER_STARTED,
  NUM_FLAGS,
};
/* niveles posibles de DEBUG, se setea en la variable debugLevel */
enum debugLevel {
  WARN,
  INFO,
  DEBUG,
};
/* la variable piso contiene el valor numerico del piso seleccionado */
static uint8_t piso;
/* la variable timer contiene el valor numerico para cuentas regresivas */
static uint8_t timer;
/* almacena el valor actual de las banderas definidas en enum mefFlags */
static uint8_t context;
static uint8_t debugLevel;

/* circularBufferUse( pisosPendientes ); */
/* variables para recorrer el teclado matricial */
static const uint8_t keypad_cols[4] = {T_COL0, T_FIL2, T_FIL3, T_FIL0};
static const uint8_t keypad_rows[4] = {T_COL1, CAN_TD, CAN_RD, RS232_TXD};

/* tabla de mapeo: filaxcolumna -> carcter ascii*/
static const char keypad_characters[4][4] = {{'D', '#', '0', '*'}, // 1ra fila
                                             {'C', '9', '8', '7'}, // 2da fila
                                             {'B', '6', '5', '4'},
                                             {'A', '3', '2', '1'}};

/*contiene el estado actual de la MEF */
mefIngresoPisoState_t mefIngresoPisoCurrentState;
extern circularBuffer_t pisosPendientes;
void mefIngresoPisoInit(void);
void mefIngresoPisoUpdate(void);
uint8_t readKeypad();
uint8_t updateLCDDisplayOnKeyPress();
uint8_t printLCDMessage(char *msg[static 2]);
uint8_t updateTimer();
uint8_t cancelTimer();
void tests();
#ifdef __cplusplus
}
#endif

#endif /*INGRESOPISO.H*/
