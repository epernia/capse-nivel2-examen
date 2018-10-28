/*=====[Datos del modulo]====================================================
 *
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Module Name: programa
 * Version: 1.0.0
 * Creation Date: 2018/10/12
 ***/

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "mefAscensor.h"
#include "sapi.h"

/*=====[Definiciones de Variables globales publicas]=========================*/

bool_t modoConfiguracion = FALSE;
bool_t secuenciaAperturaDePuertas = FALSE;
bool_t secuenciaCerradoDePuertas = FALSE;

circularBufferNew(pisosPendientes, 1, 10);
//   =
//   circularBuffer_t pisosPendientes;
//   uint8_t pisosPendientes_BufferMemory[11];



/*=====[Definiciones de Variables globales privadas]=========================*/

static int32_t varGlobalPrivada = 0;

/*=====[Prototipos de funciones privadas]====================================*/

static void cuandoBufferLleno(void *unused);

static void cuandoBufferVacio(void *unused);

/*=====[Implementaciones de funciones publicas]==============================*/

int main(void) {

  // Buffer circular de 10 elementos de 1 byte de tamaño
  boardConfig();
  uartConfig(UART_USB, 115200);
  tickConfig(1);
  /*
  // Si queremos usar los callbacks:
  circularBufferEmptyBufferCallbackSet( &pisosPendientes, cuandoBufferVacio );
  circularBufferFullBufferCallbackSet( &pisosPendientes, cuandoBufferLleno );*/

 // mefIngresoPisoInit();
  /* mefPuertasInit(); */
  /* mefModoConfiguracionInit(); */
   mefAscensorInit(); 

  // Delay no bloqueante 1s
  delay_t fsmScheduler;
  delayConfig(&fsmScheduler, 1000);

  while (TRUE) {

    if (delayRead(&fsmScheduler)) {

      // Ejecutar cada 1s
      //mefIngresoPisoUpdate();
      /* mefPuertasUpdate(); */
      /* mefModoConfiguracionUpdate(); */
       mefAscensorUpdate();
    }
  }
  return 0;
}

/*=====[Implementaciones de funciones privadas]==============================*/

/*
// Si queremos usar los callbacks:

static void cuandoBufferLleno(void* unused){
    // ...
}

static void cuandoBufferVacio(void* unused){
    // ...
}
*/