#include <p32xxxx.h>
#include <GPIODrv.h>
#include <inttypes.h>

void GPIODrv_init(){
//	GPIODrv_setupPinTMS(GPIO_mode_input);
//	GPIODrv_setupPinTCK(GPIO_mode_input);
//	GPIODrv_setupPinTDI(GPIO_mode_input);
//	GPIODrv_setupPinTDO(GPIO_mode_input);
//	GPIODrv_setupPinMCLR(GPIO_mode_input);
}

////////////
// Pin setup
////////////


// UART things
void GPIODrv_setRTS(uint32_t state){
//	if (state == GPIO_HIGH){
//		UART_RTS_LATSET = UART_RTS_MASK;
//	}
//	else{
//		UART_RTS_LATCLR = UART_RTS_MASK;
//	}
}

void GPIODrv_setDTR(uint32_t state){
//	if (state == GPIO_HIGH){
//		UART_DTR_LATSET = UART_DTR_MASK;
//	}
//	else{
//		UART_DTR_LATCLR = UART_DTR_MASK;
//	}
}
