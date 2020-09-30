/*/*
 * Modified form FreeRTOS Blinky example
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "chip.h"
#include "fsm_debounce.h"


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
QueueHandle_t colaTiempo; //creo la cola de tiempo

/* Sets up system hardware */
void prvSetupHardware(void)
{

	SystemCoreClockUpdate();
	Chip_GPIO_Init(LPC_GPIO_PORT);

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


/*Prototipo Tarea B Tecla*/
static void vTeclaTaskB(void *pvParameters);

/*Prototipo Tarea A LEDA*/
static void vLedATaskA(void *pvParameters) ;



/*****************************************************************************
 * Public functions
 ****************************************************************************/

int main(void)
{
	prvSetupHardware();

	// Creo Tarea para LEDA//
	BaseType_t resultadoA = xTaskCreate(vLedATaskA,
			"LedA",
			1000,
			NULL,
			tskIDLE_PRIORITY+1,
			&handlerLedA);
	if (resultadoA == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	// Creo Tarea para TECLA//
	BaseType_t resultadoB = xTaskCreate(vTeclaTaskB,
			"Tecla",
			1000,
			NULL,
			tskIDLE_PRIORITY+1,
			&handlerTecla);
	if (resultadoB == pdFAIL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}

	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}


/*Tarea B Tecla*/
static void vTeclaTaskB(void *pvParameters) {
	TickType_t tiempo_a_mandar = 0;
	fsmButtonInit();
	colaTiempo = xQueueCreate(2, sizeof(TickType_t));//tamano de cola y de cada dato
	if(colaTiempo == NULL)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		while(true);
	}
	while(1){
		fsmButtonUpdate(TEC1);
		tiempo_a_mandar = get_diff();
		//xQueueSend(colaTiempo, (TickType_t*) &tiempo_a_mandar, (TickType_t) 0);
		vTaskDelay(1 / portTICK_RATE_MS);
	}
}

/*Tarea A LEDA*/
static void vLedATaskA(void *pvParameters) {
	TickType_t xPeriodicity = 2*566 / portTICK_RATE_MS;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t tiempo_pulsado;

	tiempo_pulsado=566;
	while(1){
		if(colaTiempo > 0)
		{
			xQueueReceive(colaTiempo, (TickType_t*) &tiempo_pulsado, (TickType_t) 0);
		}
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], true);
		vTaskDelay(tiempo_pulsado / portTICK_RATE_MS);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], false);
		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
	}
}


