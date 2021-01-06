#include <p32xxxx.h>
#include <GPIODrv.h>
#include <inttypes.h>

// TRIS bits - 1 is input, 0 is output
// "ANSEL" / AD1PCFGbits bits - 0 is analog, 1 is digital. Default is analog!

void GPIODrv_init(){
	// Prepare ALL GPIO inputs, even if then redefined/reused in other modules


	// JTAG & ICSP pins
	// When in DEBUG, can/should leave unconfigured
	// When not in DEBUG, either configure as analog inputs, or as digital outputs
	// -> Except TMS, which can be an input due to an external pull-up resistor
	// For JTAG, might need to check how to disable JTAG pins... (TODO)
#if !defined(DEBUG)
	// ICSP
	ICSP_PGEC_ANSELbits.ICSP_PGEC_ANSELBIT = 1;		// Digital pin
	ICSP_PGEC_TRISbits.ICSP_PGEC_TRISPIN = 0;		// Digital output
	ICSP_PGEC_LATbits.ICSP_PGEC_LATPIN = 0;			// Set to 0

	ICSP_PGED_ANSELbits.ICSP_PGED_ANSELBIT = 1;		// Digital pin
	ICSP_PGED_TRISbits.ICSP_PGED_TRISPIN = 0;		// Digital output
	ICSP_PGED_LATbits.ICSP_PGED_LATPIN = 0;			// Set to 0

	// JTAG
	JTAG_TMS_ANSELbits.JTAG_TMS_ANSELBIT = 1;		// Digital pin
	JTAG_TMS_TRISbits.JTAG_TMS_TRISPIN = 1;			// Digital output

	JTAG_TCK_ANSELbits.JTAG_TCK_ANSELBIT = 1;		// Digital pin
	JTAG_TCK_TRISbits.JTAG_TCK_TRISPIN = 0;			// Digital output
	JTAG_TCK_LATbits.JTAG_TCK_LATPIN = 0;			// Set to 0

	JTAG_TDI_ANSELbits.JTAG_TDI_ANSELBIT = 1;		// Digital pin
	JTAG_TDI_TRISbits.JTAG_TDI_TRISPIN = 0;			// Digital output
	JTAG_TDI_LATbits.JTAG_TDI_LATPIN = 0;			// Set to 0

	JTAG_TDO_ANSELbits.JTAG_TDO_ANSELBIT = 1;		// Digital pin
	JTAG_TDO_TRISbits.JTAG_TDO_TRISPIN = 0;			// Digital output
	JTAG_TDO_LATbits.JTAG_TDO_LATPIN = 0;			// Set to 0
#endif



	// LEDs
	// Setup as outputs and turn off
	LED_1_TX_TRISbits.LED_1_TX_TRISPIN = 0;			// Digital output
	LED_1_TX_LATbits.LED_1_TX_LATPIN = 0;			// Set to 0

	LED_1_RX_TRISbits.LED_1_RX_TRISPIN = 0;			// Digital output
	LED_1_RX_LATbits.LED_1_RX_LATPIN = 0;			// Set to 0

	LED_2_TX_TRISbits.LED_2_TX_TRISPIN = 0;			// Digital output
	LED_2_TX_LATbits.LED_2_TX_LATPIN = 0;			// Set to 0

	LED_2_RX_TRISbits.LED_2_RX_TRISPIN = 0;			// Digital output
	LED_2_RX_LATbits.LED_2_RX_LATPIN = 0;			// Set to 0

	LED_3_TX_ANSELbits.LED_3_TX_ANSELBIT = 1;		// Digital pin
	LED_3_TX_TRISbits.LED_3_TX_TRISPIN = 0;			// Digital output
	LED_3_TX_LATbits.LED_3_TX_LATPIN = 0;			// Set to 0

	LED_3_RX_ANSELbits.LED_3_RX_ANSELBIT = 1;		// Digital pin
	LED_3_RX_TRISbits.LED_3_RX_TRISPIN = 0;			// Digital output
	LED_3_RX_LATbits.LED_3_RX_LATPIN = 0;			// Set to 0

	LED_4_TX_ANSELbits.LED_4_TX_ANSELBIT = 1;		// Digital pin
	LED_4_TX_TRISbits.LED_4_TX_TRISPIN = 0;			// Digital output
	LED_4_TX_LATbits.LED_4_TX_LATPIN = 0;			// Set to 0

	LED_4_RX_ANSELbits.LED_4_RX_ANSELBIT = 1;		// Digital pin
	LED_4_RX_TRISbits.LED_4_RX_TRISPIN = 0;			// Digital output
	LED_4_RX_LATbits.LED_4_RX_LATPIN = 0;			// Set to 0


	// UARTs
	// Setup as outpus and high (TX, DTR), or as inputs (RX)
	UART_1_TX_TRISbits.UART_1_TX_TRISPIN = 0;		// Digital output
	UART_1_TX_LATbits.UART_1_TX_LATPIN = 1;			// Set to 1

	UART_1_RX_TRISbits.UART_1_RX_TRISPIN = 1;		// Set to input

	UART_1_DTR_TRISbits.UART_1_DTR_TRISPIN = 0;		// Digital output
	UART_1_DTR_LATbits.UART_1_DTR_LATPIN = 1;		// Set to 1 (probably)


	UART_2_TX_TRISbits.UART_2_TX_TRISPIN = 0;		// Digital output
	UART_2_TX_LATbits.UART_2_TX_LATPIN = 1;			// Set to 1

	UART_2_RX_TRISbits.UART_2_RX_TRISPIN = 1;		// Set to input

	UART_2_DTR_TRISbits.UART_2_DTR_TRISPIN = 0;		// Digital output
	UART_2_DTR_LATbits.UART_2_DTR_LATPIN = 1;		// Set to 1 (probably)


	UART_3_TX_TRISbits.UART_3_TX_TRISPIN = 0;		// Digital output
	UART_3_TX_LATbits.UART_3_TX_LATPIN = 1;			// Set to 1

	UART_3_RX_TRISbits.UART_3_RX_TRISPIN = 1;		// Set to input

	UART_3_DTR_ANSELbits.UART_3_DTR_ANSELBIT = 1;	// Digital pin
	UART_3_DTR_TRISbits.UART_3_DTR_TRISPIN = 0;		// Digital output
	UART_3_DTR_LATbits.UART_3_DTR_LATPIN = 1;		// Set to 1 (probably)


	UART_4_TX_TRISbits.UART_4_TX_TRISPIN = 0;		// Digital output
	UART_4_TX_LATbits.UART_4_TX_LATPIN = 1;			// Set to 1

	UART_4_RX_TRISbits.UART_4_RX_TRISPIN = 1;		// Set to input

	UART_4_DTR_ANSELbits.UART_4_DTR_ANSELBIT = 1;	// Digital pin
	UART_4_DTR_TRISbits.UART_4_DTR_TRISPIN = 0;		// Digital output
	UART_4_DTR_LATbits.UART_4_DTR_LATPIN = 1;		// Set to 1 (probably)



}


void GPIODrv_setDTR_1(bool state){
	UART_1_DTR_LATbits.UART_1_DTR_LATPIN = state;
}

void GPIODrv_setDTR_2(bool state){
	UART_2_DTR_LATbits.UART_2_DTR_LATPIN = state;
}

void GPIODrv_setDTR_3(bool state){
	UART_3_DTR_LATbits.UART_3_DTR_LATPIN = state;
}

void GPIODrv_setDTR_4(bool state){
	UART_4_DTR_LATbits.UART_4_DTR_LATPIN = state;
}

void GPIODrv_setLED_1_TX(bool state){
	LED_1_TX_LATbits.LED_1_TX_LATPIN = state;
}
void GPIODrv_setLED_1_RX(bool state){
	LED_1_RX_LATbits.LED_1_RX_LATPIN = state;
}
void GPIODrv_setLED_2_TX(bool state){
	LED_2_TX_LATbits.LED_2_TX_LATPIN = state;
}
void GPIODrv_setLED_2_RX(bool state){
	LED_2_RX_LATbits.LED_2_RX_LATPIN = state;
}
void GPIODrv_setLED_3_TX(bool state){
	LED_3_TX_LATbits.LED_3_TX_LATPIN = state;
}
void GPIODrv_setLED_3_RX(bool state){
	LED_3_RX_LATbits.LED_3_RX_LATPIN = state;
}
void GPIODrv_setLED_4_TX(bool state){
	LED_4_TX_LATbits.LED_4_TX_LATPIN = state;
}
void GPIODrv_setLED_5_RX(bool state){
	LED_4_RX_LATbits.LED_4_RX_LATPIN = state;
}
