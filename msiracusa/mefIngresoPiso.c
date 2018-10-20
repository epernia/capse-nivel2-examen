#include "board.h"
#include "sapi.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mefIngresoPiso.h"
/* circularBufferUse( pisosPendientes ); */
#define DEBUG_ENABLED 1

void mefIngresoPisoUpdate(void) {
  switch (mefIngresoPisoCurrentState) {
  case EN_ESPERA_DE_DIGITO_1: {

    /* inicializo a cero el vector piso */
    piso = 0;

    if (readKeypad()) {
      piso = readKeypad();
      mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_2_O_LETRA;
      updateLCDDisplayOnKeyPress();
    } 
     
    break;
  }
  case EN_ESPERA_DE_DIGITO_2_O_LETRA: {
    /* if (!updateTimer()) // debe regresar segundos restantes; */
    /*   mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1; */
    /* if (readKeypad()) { // debe regresar 0 si es letra no A o B */
    /*   if (readKeypad == numero) */
    /*     piso = piso * 10 + readKeypad(); */
    /*   if (readKeypad == A) { */
    /*     mefIngresoPisoCurrentState = GUARDAR_PISO; */
    /*   } */
    /*   if (readKeypad == B) */
    /*     mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1; */
    /*   if (readKeypad != anteriores) */
    /*     printError */
    /* } */
    break;
  }
  case EN_ESPERA_DE_LETRA: {
    /* if (!updateTimer()) // debe regresar segundos restantes; */
    /*   mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1; */
    /* if (readKeypad == A) { */
    /*   mefIngresoPisoCurrentState = GUARDAR_PISO; */
    /* } */
    /* if (readKeypad == B) { */
    /*   mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1; */
    /*   cancelTimer(); */
    /* } */
    break;
  }
  case GUARDAR_PISO: {
    /* circularBufferWrite(&pisosPendientes, &piso); */
    /* // TODO procesar el buffer y ordenarlo? */
    /* printLCDMessage(); */
    /* mefIngresoPisoCurrentState = EN_ESPERA_DE_DIGITO_1; */
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
        if (DEBUG_ENABLED) {
          char str[2] = "\0";
          str[0] = user_input;
          uartWriteString(UART_USB, "\n [I] Caracter presionado: ");
          uartWriteString(UART_USB, str);
        }
        /* espero a que se deje de presionar el digito */
        while (gpioRead(keypad_rows[j])) {
          delay(10/1000); // delay acepta segundos como parametros
        }
      }
    }
    gpioWrite(keypad_cols[i], OFF);
  }
  return user_input;
}

uint8_t updateLCDDisplayOnKeyPress() {
  //TODO
  return 0;
}

uint8_t printLCDMessage(char * msg[static 2]){
  //TODO
  return 0;
}

uint8_t updateTimer(){
  //TODO
  return 0;
}

uint8_t cancelTimer(){
  //TODO
  return 0;
}


// Solamente para pruebas
int main(){
  boardConfig();
  uartConfig(UART_USB, 115200);
  /* inicializo los GPIOs que se usaran */
    /* para leer desde el keypad */
  for (uint8_t i = 0; i < 4; i++) {
    gpioConfig(keypad_cols[i], GPIO_OUTPUT);
  }
  for (uint8_t i = 0; i < 4; i++) {
    gpioConfig(keypad_rows[i], GPIO_INPUT_PULLDOWN);
  }


  for(;;){
    // prueba lectura teclado
    uint8_t myTec = readKeypad();
    if (myTec){
      char buff[10];
      sprintf(buff, "%d", myTec);
      uartWriteString(UART_USB, "\n [I] Lectura teclado ");
      
      uartWriteString(UART_USB, buff);
    }
    /* mefIngresoPisoUpdate(); */
    
    }
  return 0;
}
