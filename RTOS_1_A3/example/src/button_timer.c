/*/*
 * Modified form FreeRTOS Blinky example
 */

#include "FreeRTOS.h"
#include "task.h"
#include "chip.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const int LED1[2] = {3, 7}; //El led1 correspond al GPIO3 bit7
const int LED2[2] = {3, 6}; //El led2 correspond al GPIO3 bit 6
const int LED3[2] = {3, 5}; //El led3 corresponde al GPIO1 bit 12
const int LEDA[2] = {2, 8}; //El led A corresponde al GPIO2 bit 8


const int TEC1[2] = {3, 12}; // Pulsador 1 corresponde a GPIO 3 bit 12
const int TEC2[2] = {3, 10}; // Pulsador 2 corresponde a GPIO 3 bit 10
const int TEC3[2] = {3, 11}; // Pulsador 3 corresponde a GPIO 3 bit 11
const int TEC4[2] = {3, 9}; // Pulsador 4 corresponde a GPIO 3 bit 9


/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* System configuration variables used by chip driver */

//Estas dos lineas se sacaron del board.h
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;

TaskHandle_t handlerLedA = NULL;
TaskHandle_t handlerLed1 = NULL;
TaskHandle_t handlerLed2 = NULL;
TaskHandle_t handlerLed3 = NULL;

/* Sets up system hardware */
static void prvSetupHardware(void)
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
}


/*Tarea D LED3*/
static void vLed3TaskD(void *pvParameters) {
	long ms_counter = 566;
	vTaskDelete(handlerLed2);
	vTaskDelete(handlerLed1);
	vTaskDelete(handlerLedA);

	while(1){
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], false);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
	}

}

/*Tarea C LED2*/
static void vLed2TaskC(void *pvParameters) {
	long ms_counter = 566;
	int i=0;
	for(i=0; i<3; i++)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], true);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], false);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
	}

	xTaskCreate(vLed3TaskD,
			"Led3",
			300,
			NULL,
			tskIDLE_PRIORITY+1,
			&handlerLed3);

	vTaskSuspend(0);
}

/*Tarea B LED1*/
static void vLed1TaskB(void *pvParameters) {
	long ms_counter = 566;
	int i=0;
	for(i=0; i<3; i++)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], true);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], false);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
	}
	xTaskCreate(vLed2TaskC,
			"Led2",
			300,
			NULL,
			tskIDLE_PRIORITY+2,
			&handlerLed2);

	vTaskSuspend(handlerLed2);
	vTaskResume(handlerLedA);
	vTaskSuspend(0);

}

/*Tarea A LEDA*/
static void vLedATaskA(void *pvParameters) {
	long ms_counter = 566;//valor para geenrar 500 mSeg de delay
	int i=0;

	xTaskCreate(vLed1TaskB,
			"Led1",
			300,
			NULL,
			tskIDLE_PRIORITY+3,
			&handlerLed1);
	vTaskSuspend(0);


	for(i=0; i<2; i++)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], true);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, LEDA[0], LEDA[1], false);
		vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
	}

	vTaskResume(handlerLed2);
	vTaskSuspend(0);
}




/*****************************************************************************
 * Public functions
 ****************************************************************************/

int main(void)
{
	prvSetupHardware();

	// Creo Tarea para LEDA//
	xTaskCreate(vLedATaskA,
			"LedA",
			300,
			NULL,
			tskIDLE_PRIORITY+4,
			&handlerLedA);

	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}


