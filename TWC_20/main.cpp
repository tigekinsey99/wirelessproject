#include "BlueNRG2_it.h"
#include "BlueNRG2_conf.h"
#include "SDK_EVAL_Config.h"
#include "sleep.h"

int main(void) {

	/* System Init */
	SystemInit();
	/* Identify BlueNRG2 platform */
	SdkEvalIdentification();
	/* Configure I/O communication channel */
	SdkEvalComUartInit(UART_BAUDRATE);
	/* BLE stack init */
	HAL_STATUS ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
		while(1);
	}
	/* Device Initialization: BLE stack GATT and GAP Init APIs.
	It could add BLE services and characteristics (if it is a GATT
	server) and initialize its state machine and other specific drivers
	(i.e. leds, buttons, sensors, ...) */
	ret = DeviceInit();
	if (ret != BLE_STATUS_SUCCESS) {
		/* Device failed to initialize
		   Turn on RED LED and hang*/
		BlueNRG->GPIO_BASE_ADDR->DATA |= BIT_DIO2; // Red
		while (1);
	}
	while(1) {
		/* BLE Stack Tick 
		   returns BLE_STATUS_SUCCESS if a user is connected */
		ret = BTLE_StackTick();
		/* Application Tick: user application where application state machine
		is handled */
		if (ret != BLE_STATUS_SUCCESS) {
			/* Turn on RED LED */
			BlueNRG->GPIO_BASE_ADDR->DATA |= BIT_DIO2; // Red
		}
		else {
		/* Application Tick: user application where application state machine
		is handled */
			APP_Tick();
		}
		/* Power Save management: enable sleep mode with wakeup on radio
		operating timings (adverting, connections intervals) */
		BlueNRG_Sleep(SLEEPMODE_WAKETIMER, 0, 0, 0);
	}/* while (1) */
} /* end main() */

/* Demo the red blue and green LEDs blinking in 1 second intervals */
void APP_Tick() {
	/* Blink RGB in order */
	BlueNRG->GPIO_BASE_ADDR->DATA |= BIT_DIO2; //Red
	HAL_DELAY(1000);
	BlueNRG->GPIO_BASE_ADDR->DATA |= BIT_DIO3; //Green
	HAL_DELAY(1000);
	BlueNRG->GPIO_BASE_ADDR->DATA |= BIT_DIO6; //Blue
	HAL_DELAY(1000);

	/* Turn off RGB in order */
	BlueNRG->GPIO_BASE_ADDR->DATA &= !BIT_DIO2; //Red
	HAL_DELAY(1000);
	BlueNRG->GPIO_BASE_ADDR->DATA &= !BIT_DIO3;//Green
	HAL_DELAY(1000);
	BlueNRG->GPIO_BASE_ADDR->DATA &= !BIT_DIO6;//Blue
	HAL_DELAY(1000);
}