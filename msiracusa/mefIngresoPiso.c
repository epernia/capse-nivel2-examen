#include "mefIngresoPiso.h"
/* circularBufferUse( pisosPendientes ); */

void mefIgresoPisoUpdate(void) {
  switch (mefIngresoPisoCurrentState) {
  case EN_ESPERA_DE_DIGITO_1: {
    /* inicializo a cero el vector piso */
    piso = 0;
    if (readKeyboard()) {
      piso = readKeyboard();
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_2_O_LETRA;
      updateLCDDisplayOnKeyPress();
    } else
      return;
    break;
  }
  case EN_ESPERA_DE_DIGITO_2_O_LETRA: {
    if (!updateTimer()) // debe regresar segundos restantes;
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
    if (readKeyboard()) { // debe regresar 0 si es letra no A o B
      if (readKeyboard == numero)
        piso = piso * 10 + readKeyboard();
      if (readKeyboard == A) {
        mefIngresoPisoCurrentState = GUARDAR_PISO;
      }
      if (readKeyboard == B)
        mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
      1();
    }
    break;
  }
  case EN_ESPERA_DE_LETRA: {
    if (!updateTimer()) // debe regresar segundos restantes;
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
    if (readKeyboard == A) {
      mefIngresoPisoCurrentState = GUARDAR_PISO;
    }
    if (readKeyboard == B) {
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
      cancelTimer();
    }
    break;
  }
  case GUARDAR_PISO: {
    circularBufferWrite(&pisosPendientes, &piso);
    // TODO procesar el buffer y ordenarlo?
    printLCDMessage();
    mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
    break;
  }
  default:
    break;
  }
}

uint8_t readKeyboard(){
  //TODO
}

uint8_t updateLCDDisplayOnKeyPress(){
  //TODO
}

uint8_t printLCDMessage(char * msg[static 2]){
  //TODO
}

uint8_t updateTimer(char * msg[static 2]){
  //TODO
}

uint8_t cancelTimer(char * msg[static 2]){
  //TODO
}
