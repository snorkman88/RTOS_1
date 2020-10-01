/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/
#include <fsm1_debounce.h>
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

fsmButtonState_t fsm1ButtonState;

TickType_t tiempo_down;
TickType_t tiempo_up;
TickType_t tiempo_diff;

TickType_t get_diff1()
{
	return tiempo_diff;
}

void clear_diff()
{
	tiempo_diff = 0;
}


/* accion de el evento de tecla pulsada */
void button1Pressed( void )
{
	tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void button1Released( void )
{
	tiempo_up = xTaskGetTickCount();
	tiempo_diff = tiempo_up - tiempo_down;
}

void fsmButtonError( void )
{
    fsm1ButtonState = STATE_BUTTON_UP;
}

void fsm1ButtonInit( void )
{
    fsm1ButtonState = STATE_BUTTON_UP;  // Set initial state
}



// FSM Update Sate Function
void fsm1ButtonUpdate(const int TEC[2] )
{

    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsm1ButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1] ))
            {
                fsm1ButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( contFalling >= DEBOUNCE_TIME )
            {
                if( !Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm1ButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    button1Pressed();//a partir de aca empieza a contar
                }
                else
                {
                    fsm1ButtonState = STATE_BUTTON_UP;
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
				fsm1ButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( contRising >= DEBOUNCE_TIME )
            {
                if( Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC[0], TEC[1]) )
                {
                    fsm1ButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    button1Released();
                }
                else
                {
                    fsm1ButtonState = STATE_BUTTON_DOWN;
                }
                contRising = 0;
            }
            contRising++;

            /* LEAVE */
            break;

        default:
            fsmButtonError();
            break;
    }
}
