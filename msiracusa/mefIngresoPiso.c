#include "mefIngresoPiso.h"
#include "board.h"
#include "sapi.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// FIXME: Al integrar con el resto de los módulos esto debería desaparecer
#define TIMEOUT 5   // FIXME: Ver arriba
#define PISO_MAX 20 // FIXME: Ver arriba
#define PISO_MIN -5 // FIXME: Ver arriba

// FIXME: Esto ya está definido en programa.c
/* circularBufferNew( tempBuff, 2, 10 ); // 10 elementos de 2 bytes */
/* circularBufferInit( tempBuff, 2, 10 ); */

void mefIngresoPisoInit() {
  /* valor de debugLevel puede ser WARN; INFO o DEBUG */
  debugLevel |= (1 << DEBUG);
  uartWriteString(UART_USB, "\n [I] Nivel debug:  ");
  char *buff[10];
  sprintf(buff, "%d", debugLevel);
  uartWriteString(UART_USB, buff);

  /* inicializo los GPIOs que se usaran */
  /* para leer desde el keypad */
  for (uint8_t i = 0; i < 4; i++) {
    gpioConfig(keypad_cols[i], GPIO_OUTPUT);
  }
  for (uint8_t i = 0; i < 4; i++) {
    gpioConfig(keypad_rows[i], GPIO_INPUT_PULLDOWN);
  }
  /* tests(); */

}

void mefIngresoPisoUpdate(void) {
  switch (mefIngresoPisoCurrentState) {
  case EN_ESPERA_DE_DIGITO_1: {
    printLCDMessage("* bienvenida*");
    piso = readKeypad();
    if (piso >= 0 && piso <= 9) {
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(UART_USB,
                        "\n [I] EN_ESPERA_DE_DIGITO_1 ::  Leido valor numerico");
        uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_1 :: Transicion a "
                                  "EN_ESPERA_DE_DIGITO_2_O_LETRA");
      }
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_2_O_LETRA;
      updateLCDDisplayOnKeyPress(/* digito */);
    } else {
      updateLCDDisplayOnKeyPress(/* valor invalido */);
    }

    break;
  }
  case EN_ESPERA_DE_DIGITO_2_O_LETRA: {
    /* actualiza el valor del timer y lo guarda en la var. global timer */
    updateTimer();

    /* si timer llega a cero, vuelve al estado anterior e imprime un mensaje de
     * timeout */
    if (!timer) {
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
      printLCDMessage("* timeout *");
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: El "
                                  "usuario no ingreso un valor. Timeout");
        uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: "
                                  "Transicion a EN_ESPERA_DE_DIGITO_1");
      }
      break;
    }

    uint8_t userInput = readKeypad(); /* correccion Chr -> Dec */
    if (userInput >= 0 && userInput <= 9) {
      piso = piso * 10 + userInput;
      mefIngresoPisoCurrentState = EN_ESPERA_DE_LETRA;
      cancelTimer();
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: "
                                  "Transicion a EN_ESPERA_DE_LETRA");
      }
    } else {
      if (userInput == 17) { /* usuario presiona 'A' */
        mefIngresoPisoCurrentState = GUARDAR_PISO;
        if (debugLevel >= (1 << INFO))
          uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: "
                                    "Transicion a GUARDAR_PISO");
      } else {
        if (userInput == 18) { /* usuario presiona 'B' */
          cancelTimer();
          mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
          if (debugLevel >= (1 << INFO)) {
            uartWriteString(UART_USB,
                            "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: Ingreso de "
                            "piso cancelado por el usuario");
            uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_DIGITO_2_O_LETRA :: "
                                      "Transicion a EN_ESPERA_DE_DIGITO_1");
          }
        } else {
          printLCDMessage("*opcion invalida*");
        }
      }
    }
    break;
  }
  case EN_ESPERA_DE_LETRA: {
    updateTimer();
    if (!timer) { // debe regresar segundos restantes; */
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_LETRA :: El usuario no "
                                  "ingreso un valor. Timeout ");
        uartWriteString(
            UART_USB,
            "\n [I] EN_ESPERA_DE_LETRA :: Transicion a EN_ESPERA_DE_DIGITO_1");
        mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
        break;
      }
    }
    uint8_t userInput = readKeypad(); /* correccion Chr -> Dec */

    if (userInput == 17) { /* usuario presiona 'A'*/
      mefIngresoPisoCurrentState = GUARDAR_PISO;
      cancelTimer();
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(
            UART_USB, "\n [I] EN_ESPERA_DE_LETRA :: Transicion a GUARDAR_PISO");
      }

    } else {
      if (userInput == 18) { /* usuario presiona 'B'*/
        mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
        cancelTimer();
        if (debugLevel >= (1 << INFO)) {
          uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_LETRA :: Ingreso de "
                                    "piso cancelado por el usuario.");
          uartWriteString(UART_USB, "\n [I] EN_ESPERA_DE_LETRA :: Transicion a "
                                    "EN_ESPERA_DE_DIGITO_1");
        }
      } else {
        printLCDMessage("opcion invalida");
      }
    }
    break;
  }
  case GUARDAR_PISO: {

    if (debugLevel >= (1 << INFO)) {
      char *buff[10];
      sprintf(buff, "%d", piso);
      uartWriteString(UART_USB,
                      "\n [I] GUARDAR_PISO :: Valor de piso seleccionado ");
      uartWriteString(UART_USB, buff);
    }

    /* piso seleccionado no es valido: imprimir mensaje de error */
    if (piso > PISO_MAX || piso < PISO_MIN) {
      printLCDMessage("Piso invalido");
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
      if (debugLevel >= (1 << WARN)) {
        uartWriteString(UART_USB,
                        "\n [W] GUARDAR_PISO :: Valor de piso invalido ");
        uartWriteString(
            UART_USB,
            "\n [I] GUARDAR_PISO :: Transicion a EN_ESPERA_DE_DIGITO_1 ");
      }

      /* piso seleccionado es valido: alamacenar en bufferCircular */
    } else {
      circularBufferWrite( &pisosPendientes, &piso);
      printLCDMessage("Piso almacenado: ");
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1;
      if (debugLevel >= (1 << INFO)) {
        uartWriteString(
            UART_USB,
            "\n [I] GUARDAR_PISO :: Transicion a EN_ESPERA_DE_DIGITO_1 ");
      }
    }
    /* // TODO procesar el buffer y ordenarlo? */
    break;
  }
  default:
    break;
  }
}

uint8_t readKeypad(void) {
  /* user_input guardara el caracter ascii a retornar */
  uint8_t user_input = {0};

  for (uint8_t i = 0; i < 4; ++i) {
    /* coloco en alto las columnas */
    gpioWrite(keypad_cols[i], ON);

    for (uint8_t j = 0; j < 4; ++j) {
      /* leo las filas y detecto cual boton esta presionado */

      if (gpioRead(keypad_rows[j])) {
        /* realizo el mapeo de [filas][columnas] a caracter ascii */
        user_input = keypad_characters[j][i];
        /* imprimo por uart si esta habilitado el debug */
        if (debugLevel >= (1 << DEBUG)) {
          char str[2] = "\0";
          str[0] = user_input;
          uartWriteString(UART_USB,
                          "\n [D] readKeypad :: Caracter presionado: ");
          uartWriteString(UART_USB, str);
        }
        /* espero a que se deje de presionar el digito */
        while (gpioRead(keypad_rows[j])) {
          delay(1); 
        }
      }
    }
    gpioWrite(keypad_cols[i], OFF);
  }

  /* correccion chr a decimal, de esta manera un '0' es igual  0. */
  return user_input - 48;
}

uint8_t updateLCDDisplayOnKeyPress() {
  // TODO
  return 0;
}

uint8_t printLCDMessage(char *msg[static 2]) {
  // TODO
  return 0;
}

uint8_t updateTimer() {
  if (context & (1 << TIMER_STARTED))
    timer--;
  else { /* inicializo el timer  */
    timer = TIMEOUT - 1;
    context |= (1 << TIMER_STARTED);
  }
  if (!timer)
    context &= (0 << TIMER_STARTED);
  if (debugLevel >= (1 << DEBUG)) {
    char *buff[10];
    uartWriteString(UART_USB, "\n [I] updateTimer :: Valor del timer: ");
    sprintf(buff, "%d", timer);
    uartWriteString(UART_USB, buff);
  }
  // FIXME : Quitar
  /* delay(199); */
  return timer;
}

uint8_t cancelTimer() {
  context &= (0 << TIMER_STARTED);
  if (debugLevel >= (1 << INFO)) {
    uartWriteString(UART_USB, "\n [I] cancelTimer :: Timer cancelado ");
  }
  return 0;
}

// FIXME: Solamente para pruebas
// FIXME: Si me queda tiempo, hacer un test con el main
void tests() {

  for (;;) {
    // prueba lectura teclado
    uint8_t myTec = readKeypad();
    if (myTec) {
      char buff[10];
      sprintf(buff, "%d", myTec);
      uartWriteString(UART_USB, "\n [I] Lectura teclado ");
      uartWriteString(UART_USB, buff);
    }
    mefIngresoPisoUpdate();
  }
}
