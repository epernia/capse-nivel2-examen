/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "mefAscensor.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI


//Declaracion de variables externas

extern bool_t modoConfiguracion;
extern bool_t secuenciaAperturaDePuertas;
extern bool_t secuenciaCerradoDePuertas;
 delay_t fsmScheduler;

bool_t mefModoConfiguracionUpdate()
{
   return TRUE;
}

void mefAscensorInit()
{
   fsmState = YENDO_A_PLANTA_BAJA;
}



// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
void mefAscensorUpdate( void )
{
   // ---------- CONFIGURACIONES ----------------------------
   

      
    static bool_t flgConfig;
    static bool_t flgParado;
    static bool_t flgEn_planta_baja;
    static bool_t flgSubiendo;
    static bool_t flgBajando;
    static bool_t flgYendo_a_planta_baja;
    static int pisoActual;
    uint8_t pisoSiguiente;
    circularBufferStatus_t Estado;
    bool_t flgFinConfig;
    int tiempo;
      
      
       switch(fsmState)
    {
        case EN_PLANTA_BAJA:
            
            //**Entrada a la MEF**
            if (flgEn_planta_baja == FALSE)
            {
                flgEn_planta_baja = TRUE;
                Estado =  circularBufferRead(&pisosPendientes, &pisoSiguiente);
            }
            
            //**Actualizar salidas**
            secuenciaAperturaDePuertas = TRUE;
            gpioWrite(LED3, ON);
            
            //**Condiciones de trancisión**
            
            //Pasamos al modo configuracion
            if (modoConfiguracion == TRUE)
                fsmState = MODO_CONFIGURACION;
            
            //Hay peticion para subir o bajar
            if (Estado =!CIRCULAR_BUFFER_EMPTY)
            {
                if (pisoSiguiente > pisoActual)
                    fsmState = SUBIENDO;
                else if (pisoSiguiente < pisoActual)
                    fsmState = BAJANDO;
            }
            
            //**Salida del estado
            
            if (fsmState != EN_PLANTA_BAJA)
                flgEn_planta_baja == FALSE;
            
            if (fsmState !=MODO_CONFIGURACION)
            {
                secuenciaCerradoDePuertas = TRUE;
               gpioWrite(LED3, OFF);
            }
            
         
            break;
        
        
        case MODO_CONFIGURACION:
            
            //**Entrada a la MEF
            if (flgConfig == FALSE)
            {
                flgConfig == TRUE;
                mefModoConfiguracionUpdate();
               
            }
            
            //**Actualizar las salidas
            
            //**Condiciones de trnasición
            if (flgFinConfig)
            {
                fsmState = EN_PLANTA_BAJA;
            }
            
            //*Salida del estado
            if (fsmState != MODO_CONFIGURACION)
                flgConfig == FALSE;
            
            
            break;
            
        case BAJANDO:
            
            //**Entrada a la MEF
            if (flgBajando == FALSE)
            {
                tiempo = 0;
                flgBajando == TRUE;
                
            }
            
            //**Actualizar salidas
            //Vamos a simular la bajada
            //retardo 10 ms
            if( delayRead(&fsmScheduler) )
            {
                pisoActual--;
                tiempo == 0;
                
            }
        
            
            gpioWrite(LEDB, ON);
            
            //*Transición
            if (pisoActual == pisoSiguiente)
                fsmState = PARADO;
            
            //** Salida del estado
            if (fsmState =! BAJANDO)
            {
                flgBajando = FALSE;
                gpioWrite(LEDB, OFF);
                
                
            }
            
            break;
            
        case SUBIENDO:
            //**Entrada a la MEF
            if (flgSubiendo == FALSE)
            {
                tiempo = 0;
                flgSubiendo == TRUE;
                
            }
            
            //**Actualizar salidas
            //Vamos a simular la bajada
            //retardo 10 ms
            if( delayRead(&fsmScheduler) )
            {
                pisoActual++;
                tiempo == 0;
                
            }
        
            
            gpioWrite(LEDB, ON);
            
            //*Transición
            if (pisoActual == pisoSiguiente)
                fsmState = PARADO;
            
            //** Salida del estado
            if (fsmState =! SUBIENDO)
            {
                flgSubiendo = FALSE;
                gpioWrite(LEDB, OFF);
                
            }
            
            break;
            
        
        case PARADO:
            
            //**Entrada a la MEF
            if (flgParado == FALSE)
            {
                flgParado = TRUE;
                tiempo = 0;
                secuenciaAperturaDePuertas = TRUE;
                
               Estado =  circularBufferRead(&pisosPendientes, &pisoSiguiente);
            }
            
            //**Actualizar las salidas
                //Delay 10ms
            tiempo++;
            
            //**Trancisión
            //Time out parado va a planta baja
            if (tiempo > 100)
                fsmState = YENDO_A_PLANTA_BAJA;
            //Si está en planta baja se queda alli
            if (pisoActual == 0)
                fsmState = EN_PLANTA_BAJA;
            
            //Chequea si hay pisos pedidos
            if (Estado != CIRCULAR_BUFFER_EMPTY)
            {
                if (pisoSiguiente > pisoActual)
                    fsmState = SUBIENDO;
                else if(pisoSiguiente < pisoActual)
                    fsmState = BAJANDO;
            }
            
            //**Salida del estado
            if (fsmState != PARADO)
            {
                flgParado = FALSE;
                secuenciaCerradoDePuertas = TRUE;
            }
            
            break;
            
            
        case YENDO_A_PLANTA_BAJA:
            //*Ingreso al estado
            if (flgYendo_a_planta_baja == FALSE)
            { 
                flgYendo_a_planta_baja = TRUE;
                tiempo = 0;
                
            }
               
            
            //**Actualizar las salidas
            if (pisoActual < 0)
            {
               // subir
                //retardo 10 ms
                tiempo++;
                if (tiempo == 100)
                {
                    pisoActual ++;
                    tiempo = 0;
                }
            }
            
            else if (pisoActual > 0)
                //bajar
                tiempo++;
                if (tiempo == 100)
                {
                    pisoActual --;
                    tiempo = 0;
                }
            
            //**Condiciones de transición
            
           // Estado =  circularBufferRead(&buffer_circular, &pisoSiguiente);
            
            if (pisoActual == 0)
                fsmState = EN_PLANTA_BAJA;
            else if (Estado != CIRCULAR_BUFFER_EMPTY)
            {
                if (pisoActual < pisoSiguiente)
                    fsmState = SUBIENDO;
                else if (pisoActual > pisoSiguiente)
                    fsmState = BAJANDO;
            }
            
            //** Condiciones de salida
            
            if (fsmState != BAJANDO)
                flgBajando == FALSE;
                
            break;
            
        default:
           fsmState !=BAJANDO;
            
        
               
    }

   
      
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC

