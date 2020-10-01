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

fsmButtonState_t fsm4ButtonState;

TickType_t tiempo_down4;
TickType_t tiempo_up4;
TickType_t tiempo_diff4;

TickType_t get_diff4()
{
	return tiempo_diff4;
}

void clear_diff4()
{
	tiempo_diff4 = 0;
}


/* accion de el evento de tecla pulsada */
void button4Pressed( void )
{
	tiempo_down4 = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void button4Released( void )
{
	tiempo_up4 = xTaskGetTickCount();
	tiempo_diff4 = tiempo_up4 - tiempo_down4;
	//xQueueSend(colaTiempo, (TickType_t*) &tiempo_diff4, (TickType_t) 0);
}

void fsm4ButtonError( void )
{
    fsm4ButtonState = STATE_BUTTON_UP;
}

void fsm4ButtonInit( void )
{
    fsm4ButtonState = STATE_BUTTON_UP;  // Set initial state
}



// FSM Update Sate Function
void fsm4ButtonUpdate(const int TEC[2] )
{

    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsm4ButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1] ))
            {
                fsm4ButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( contFalling >= DEBOUNCE_TIME )
            {
                if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm4ButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    button4Pressed();//a partir de aca empieza a contar
                }
                else
                {
                    fsm4ButtonState = STATE_BUTTON_UP;
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
				fsm4ButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( contRising >= DEBOUNCE_TIME )
            {
                if( Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm4ButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    button4Released();
                }
                else
                {
                    fsm4ButtonState = STATE_BUTTON_DOWN;
                }
                contRising = 0;
            }
            contRising++;

            /* LEAVE */
            break;

        default:
            fsm4ButtonError();
            break;
    }
}
