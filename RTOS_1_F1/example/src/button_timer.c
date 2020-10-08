/*/*
 * Modified form FreeRTOS Blinky example
 */

#include "FreeRTOS.h"
//#include "task.h"
#include "chip.h"
#include "keys.h"


/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* System configuration variables used by chip driver */

//Estas dos lineas se sacaron del board.h
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;

const int LED1[2] = {3, 7}; //El led1 correspond al GPIO3 bit7
const int LED2[2] = {3, 6}; //El led2 correspond al GPIO3 bit 6
const int LED3[2] = {3, 5}; //El led3 corresponde al GPIO1 bit 12
const int LEDA[2] = {2, 8}; //El led A corresponde al GPIO2 bit 8


const int TEC1[2] = {3, 12}; // Pulsador 1 corresponde a GPIO 3 bit 12
const int TEC2[2] = {3, 10}; // Pulsador 2 corresponde a GPIO 3 bit 10
const int TEC3[2] = {3, 11}; // Pulsador 3 corresponde a GPIO 3 bit 11
const int TEC4[2] = {3, 9}; // Pulsador 4 corresponde a GPIO 3 bit 9

TaskHandle_t handlerLedA = NULL;
TaskHandle_t handlerTecla = NULL;


/* Sets up system hardware */
void prvSetupHardware(void)
{

	SystemCoreClockUpdate();
	Chip_GPIO_Init( LPC_GPIO_PORT );

	Chip_SCU_PinMuxSet(6,  9, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);
	Chip_SCU_PinMuxSet(6, 10, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);
	Chip_SCU_PinMuxSet(6, 11, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);//LED1
	Chip_SCU_PinMuxSet(6, 12, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);//LEDA

	Chip_SCU_PinMuxSet(7, 4,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecA joystick
	Chip_SCU_PinMuxSet(7, 3,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecB joystick
	Chip_SCU_PinMuxSet(7, 2,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecC joystick
	Chip_SCU_PinMuxSet(7, 1,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecD joystick

	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LEDA[0], LEDA[1]);//Configuro el GPIO como salida para los leds
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED1[0], LED1[1]);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED2[0], LED2[1]);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED3[0], LED3[1]);

	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC1[0], TEC1[1]); //Configuro el GPIO como entrada para pulsadores
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC3[0], TEC3[1]);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC2[0], TEC2[1]);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC4[0], TEC4[1]);

	Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], false);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], false);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], false);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], false);

}


void task_ledA( void* taskParmPtr );
void task_ledB( void* taskParmPtr );
void task_ledC( void* taskParmPtr );
void task_ledD( void* taskParmPtr );

/*****************************************************************************
 * Public functions
 ****************************************************************************/

int main(void)
{
	prvSetupHardware();

	BaseType_t res = xTaskCreate (
              	  task_ledA,					// Funcion de la tarea a ejecutar
				  ( const char * )"task_ledA",	// Nombre de la tarea como String amigable para el usuario
				  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				  0,							// Parametros de tarea
				  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				  0							// Puntero a la tarea creada en el sistema
          		  );
	if (res == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	res = xTaskCreate (
              	  task_ledB,					// Funcion de la tarea a ejecutar
				  ( const char * )"task_ledB",	// Nombre de la tarea como String amigable para el usuario
				  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				  0,							// Parametros de tarea
				  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				  0							// Puntero a la tarea creada en el sistema
          		  );
	if (res == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	res = xTaskCreate (
              	  task_ledC,					// Funcion de la tarea a ejecutar
				  ( const char * )"task_ledC",	// Nombre de la tarea como String amigable para el usuario
				  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				  0,							// Parametros de tarea
				  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				  0							// Puntero a la tarea creada en el sistema
          		  );
	if (res == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	res = xTaskCreate (
              	  task_ledD,					// Funcion de la tarea a ejecutar
				  ( const char * )"task_ledD",	// Nombre de la tarea como String amigable para el usuario
				  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				  0,							// Parametros de tarea
				  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				  0							// Puntero a la tarea creada en el sistema
          		  );
	if (res == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	keys_Init();
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}



void task_ledA( void* taskParmPtr )
{
    TickType_t dif =   566 / portTICK_RATE_MS;
    TickType_t xPeriodicity = 2*566 / portTICK_RATE_MS; // Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( TEC1_INDEX ) )
        {
            dif = get_diff(TEC1_INDEX);
        }
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], true);
        vTaskDelay( dif / portTICK_RATE_MS);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], false);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}

void task_ledB( void* taskParmPtr )
{
    TickType_t dif =   566 / portTICK_RATE_MS;
    TickType_t xPeriodicity = 2*566 / portTICK_RATE_MS; // Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( TEC2_INDEX ) )
        {
            dif = get_diff(TEC2_INDEX);
        }
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], true);
        vTaskDelay( dif / portTICK_RATE_MS);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], false);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}


void task_ledC( void* taskParmPtr )
{
    TickType_t dif =   566 / portTICK_RATE_MS;
    TickType_t xPeriodicity = 2*566 / portTICK_RATE_MS; // Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( TEC3_INDEX ) )
        {
            dif = get_diff(TEC3_INDEX);
        }
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], true);
        vTaskDelay( dif / portTICK_RATE_MS);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], false);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}

void task_ledD( void* taskParmPtr )
{
    TickType_t dif =   566 / portTICK_RATE_MS;
    TickType_t xPeriodicity = 2*566 / portTICK_RATE_MS; // Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( TEC4_INDEX ) )
        {
            dif = get_diff(TEC4_INDEX);
        }
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
        vTaskDelay( dif / portTICK_RATE_MS);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], false);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}
