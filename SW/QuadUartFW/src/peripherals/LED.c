#include <p32xxxx.h>
#include <inttypes.h>
#include <LED.h>
#include <GPIODrv.h>


void LED_init(){	// All of them.
//	LEDUSER_TRISbits.LEDUSER_TRISPIN = 0;	// Set to output
//	LEDUSBINT_TRISbits.LEDUSBINT_TRISPIN = 0;
//	LEDUSBUART_OUT_TRISbits.LEDUSBUART_OUT_TRISPIN = 0;
//	LEDUSBUART_IN_TRISbits.LEDUSBUART_IN_TRISPIN = 0;
//	LEDUSBPROG_OUT_TRISbits.LEDUSBPROG_OUT_TRISPIN = 0;
//	LEDUSBPROG_IN_TRISbits.LEDUSBPROG_IN_TRISPIN = 0;
}

void LED_setState(uint8_t state){
//	if (state){
//		LEDUSER_LATbits.LEDUSER_LATPIN = 1;
//	}
//	else{
//		LEDUSER_LATbits.LEDUSER_LATPIN = 0;
//	}
}

void LED_toggle(){
//	LEDUSER_LATINV = LEDUSER_MASK;
}
