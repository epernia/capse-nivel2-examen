#include "mefIngresoPiso.h"
/* circularBufferUse( pisosPendientes ); */


void mefIgresoPisoUpdate(void) {
  switch (mefIngresoPisoCurrentState) {
  case EN_ESPERA_DE_DIGITO_1: {
    // TODO
    break;
  }
  case EN_ESPERA_DE_DIGITO_2_O_LETRA: {
    // TODO
    break;
  } 
  case EN_ESPERA_DE_LETRA: {
    // TODO
    break;
  }
  case GUARDAR_PISO: {
    // TODO
    break;
  }
  default:
    break;
  }
  /* circularBufferWrite(&pisosPendientes, &piso); */
}
