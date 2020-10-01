/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/
#include <fsm2_debounce.h>
#include "chip.h"
#include "FreeRTOS.h"
#include "queue.h"

typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} fsmButtonState_t;

extern QueueHandle_t colaTiempo;

fsmButtonState_t fsm2ButtonState;

TickType_t tiempo_down2;
TickType_t tiempo_up2;
TickType_t tiempo_diff2;

TickType_t get_diff2()
{
	return tiempo_diff2;
}

void clear_diff2()
{
	tiempo_diff2 = 0;
}


/* accion de el evento de tecla pulsada */
void button2Pressed( void )
{
	tiempo_down2 = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void button2Released( void )
{
	tiempo_up2 = xTaskGetTickCount();
	tiempo_diff2 = tiempo_up2 - tiempo_down2;
	//xQueueSend(colaTiempo, (TickType_t*) &tiempo_diff2, (TickType_t) 0);
}

void fsm2ButtonError( void )
{
    fsm2ButtonState = STATE_BUTTON_UP;
}

void fsm2ButtonInit( void )
{
    fsm2ButtonState = STATE_BUTTON_UP;  // Set initial state
}



// FSM Update Sate Function
void fsm2ButtonUpdate(const int TEC[2] )
{

    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsm2ButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1] ))
            {
                fsm2ButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( contFalling >= DEBOUNCE_TIME )
            {
                if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm2ButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    button2Pressed();//a partir de aca empieza a contar
                }
                else
                {
                    fsm2ButtonState = STATE_BUTTON_UP;
                }

                contFalling = 0;
            }

            contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
			{
				fsm2ButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( contRising >= DEBOUNCE_TIME )
            {
                if( Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm2ButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    button2Released();
                }
                else
                {
                    fsm2ButtonState = STATE_BUTTON_DOWN;
                }
                contRising = 0;
            }
            contRising++;

            /* LEAVE */
            break;

        default:
            fsm2ButtonError();
            break;
    }
}
