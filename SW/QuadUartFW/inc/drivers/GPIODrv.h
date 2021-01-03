#ifndef GPIODRV_H_88e635eabb0f41b7aa44dc94a480edfc
#define GPIODRV_H_88e635eabb0f41b7aa44dc94a480edfc

#include <p32xxxx.h>
#include <inttypes.h>

#define GPIO_mode_output_low	0
#define GPIO_mode_output_high	1
#define GPIO_mode_input			2

#define GPIO_LOW				0
#define GPIO_HIGH				1


// LEDs
#define LED_1_TRISbits		TRISEbits
#define LED_1_TRISPIN		TRISE7
#define LED_1_LATbits		LATEbits
#define LED_1_LATPIN		LATE7
#define LED_1_LATINV		LATEINV
#define LED_1_MASK			(1<<7)


// UARTs
// Debug UART - UART1
#define UART_DBG_TX_TRISbits		TRISDbits
#define UART_DBG_TX_TRISPIN			TRISD3
#define UART_DBG_TX_LATbits			LATDbits
#define UART_DBG_TX_LATPIN			LATD3

#define UART_DBG_RX_TRISbits		TRISDbits
#define UART_DBG_RX_TRISPIN			TRISD2

#define UART_DBG_MODE_bits			U2MODEbits
#define UART_DBG_STA_bits			U2STAbits
#define UART_DBG_BRG_reg			U2BRG
#define UART_DBG_TX_reg				U2TXREG
#define UART_DBG_RX_reg				U2RXREG

#define UART_DBG_INT_IPC_bits			IPC6bits
#define UART_DBG_INT_IPC_PRIORITY		U1IP
#define UART_DBG_INT_IPC_SUBPRIORITY	U1IS
#define UART_DBG_INT_IEC_bits			IEC0bits
#define UART_DBG_INT_IEC_RXIE			U1RXIE
#define UART_DBG_INT_IFS_bits			IFS0bits
#define UART_DBG_INT_IFS_RXIF			U0RXIF

/*
// UART RTS and DTR
// RTS on CON1-8
#define UART_RTS_TRISbits		TRISEbits
#define UART_RTS_TRISPIN		TRISE5
#define UART_RTS_LATbits		LATEbits
#define UART_RTS_LATPIN			LATE5
#define UART_RTS_LATINV			LATEINV
#define UART_RTS_LATSET			LATESET
#define UART_RTS_LATCLR			LATECLR
#define UART_RTS_MASK			(1<<5)

// DTR on CON1-9
#define UART_DTR_TRISbits		TRISEbits
#define UART_DTR_TRISPIN		TRISE6
#define UART_DTR_LATbits		LATEbits
#define UART_DTR_LATPIN			LATE6
#define UART_DTR_LATINV			LATEINV
#define UART_DTR_LATSET			LATESET
#define UART_DTR_LATCLR			LATECLR
#define UART_DTR_MASK			(1<<6)
*/



// Function definitons
void GPIODrv_init();

// UART things
void GPIODrv_setRTS(uint32_t state);
void GPIODrv_setDTR(uint32_t state);


#endif
