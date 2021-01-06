#ifndef GPIODRV_H_88e635eabb0f41b7aa44dc94a480edfc
#define GPIODRV_H_88e635eabb0f41b7aa44dc94a480edfc

#include <p32xxxx.h>
#include <inttypes.h>
#include <stdbool.h>


// ICSP pins -------------------------------------------------------------------
// PGEC - analog pin
#define ICSP_PGEC_TRISbits		TRISBbits
#define ICSP_PGEC_TRISPIN		TRISB6
#define ICSP_PGEC_LATbits		LATBbits
#define ICSP_PGEC_LATPIN		LATB6
#define ICSP_PGEC_MASK			(1<<6)
#define ICSP_PGEC_ANSELbits		AD1PCFGbits
#define ICSP_PGEC_ANSELBIT		PCFG6

// PGED - analog pin
#define ICSP_PGED_TRISbits		TRISBbits
#define ICSP_PGED_TRISPIN		TRISB7
#define ICSP_PGED_LATbits		LATBbits
#define ICSP_PGED_LATPIN		LATB7
#define ICSP_PGED_MASK			(1<<7)
#define ICSP_PGED_ANSELbits		AD1PCFGbits
#define ICSP_PGED_ANSELBIT		PCFG7
// -----------------------------------------------------------------------------



// JTAG pins -------------------------------------------------------------------
// TMS - analog pin
#define JTAG_TMS_TRISbits		TRISBbits
#define JTAG_TMS_TRISPIN		TRISB10
#define JTAG_TMS_LATbits		LATBbits
#define JTAG_TMS_LATPIN			LATB10
#define JTAG_TMS_MASK			(1<<10)
#define JTAG_TMS_ANSELbits		AD1PCFGbits
#define JTAG_TMS_ANSELBIT		PCFG10

// TCK - analog pin
#define JTAG_TCK_TRISbits		TRISBbits
#define JTAG_TCK_TRISPIN		TRISB12
#define JTAG_TCK_LATbits		LATBbits
#define JTAG_TCK_LATPIN			LATB12
#define JTAG_TCK_MASK			(1<<12)
#define JTAG_TCK_ANSELbits		AD1PCFGbits
#define JTAG_TCK_ANSELBIT		PCFG12

// TDI - analog pin
#define JTAG_TDI_TRISbits		TRISBbits
#define JTAG_TDI_TRISPIN		TRISB13
#define JTAG_TDI_LATbits		LATBbits
#define JTAG_TDI_LATPIN			LATB13
#define JTAG_TDI_MASK			(1<<13)
#define JTAG_TDI_ANSELbits		AD1PCFGbits
#define JTAG_TDI_ANSELBIT		PCFG13

// TDO - analog pin
#define JTAG_TDO_TRISbits		TRISBbits
#define JTAG_TDO_TRISPIN		TRISB11
#define JTAG_TDO_LATbits		LATBbits
#define JTAG_TDO_LATPIN			LATB11
#define JTAG_TDO_MASK			(1<<11)
#define JTAG_TDO_ANSELbits		AD1PCFGbits
#define JTAG_TDO_ANSELBIT		PCFG11

// -----------------------------------------------------------------------------

// LEDs ------------------------------------------------------------------------
// Bottom side
// TX 1 - closest to connector
#define LED_1_TX_TRISbits		TRISEbits
#define LED_1_TX_TRISPIN		TRISE7
#define LED_1_TX_LATbits		LATEbits
#define LED_1_TX_LATPIN			LATE7
#define LED_1_TX_LATINV			LATEINV
#define LED_1_TX_MASK			(1<<7)

// RX 1 - 2nd closest to connector
#define LED_1_RX_TRISbits		TRISEbits
#define LED_1_RX_TRISPIN		TRISE6
#define LED_1_RX_LATbits		LATEbits
#define LED_1_RX_LATPIN			LATE6
#define LED_1_RX_LATINV			LATEINV
#define LED_1_RX_MASK			(1<<6)

// TX 2 - 3rd closest to connector
#define LED_2_TX_TRISbits		TRISEbits
#define LED_2_TX_TRISPIN		TRISE5
#define LED_2_TX_LATbits		LATEbits
#define LED_2_TX_LATPIN			LATE5
#define LED_2_TX_LATINV			LATEINV
#define LED_2_TX_MASK			(1<<5)

// RX 2 - farthest from connector
#define LED_2_RX_TRISbits		TRISEbits
#define LED_2_RX_TRISPIN		TRISE4
#define LED_2_RX_LATbits		LATEbits
#define LED_2_RX_LATPIN			LATE4
#define LED_2_RX_LATINV			LATEINV
#define LED_2_RX_MASK			(1<<4)

// Top side - these are ALL on analog ports... Disabled by writing a 1 into AD1PCFG at the ANx place
// ANx and the pin location conincide, so reuse mask
// Defined as ANSEL, as that is more commonly used
// TX 3 - closest to connector
#define LED_3_TX_TRISbits		TRISBbits
#define LED_3_TX_TRISPIN		TRISB3
#define LED_3_TX_LATbits		LATBbits
#define LED_3_TX_LATPIN			LATB3
#define LED_3_TX_LATINV			LATBINV
#define LED_3_TX_MASK			(1<<3)
#define LED_3_TX_ANSELbits		AD1PCFGbits
#define LED_3_TX_ANSELBIT		PCFG3

// RX 3 - 2nd closest to connector
#define LED_3_RX_TRISbits		TRISBbits
#define LED_3_RX_TRISPIN		TRISB2
#define LED_3_RX_LATbits		LATBbits
#define LED_3_RX_LATPIN			LATB2
#define LED_3_RX_LATINV			LATBINV
#define LED_3_RX_MASK			(1<<2)
#define LED_3_RX_ANSELbits		AD1PCFGbits
#define LED_3_RX_ANSELBIT		PCFG2

// TX 4 - 3rd closest to connector
#define LED_4_TX_TRISbits		TRISBbits
#define LED_4_TX_TRISPIN		TRISB1
#define LED_4_TX_LATbits		LATBbits
#define LED_4_TX_LATPIN			LATB1
#define LED_4_TX_LATINV			LATBINV
#define LED_4_TX_MASK			(1<<1)
#define LED_4_TX_ANSELbits		AD1PCFGbits
#define LED_4_TX_ANSELBIT		PCFG1

// RX 4 - farthest from connector
#define LED_4_RX_TRISbits		TRISBbits
#define LED_4_RX_TRISPIN		TRISB0
#define LED_4_RX_LATbits		LATBbits
#define LED_4_RX_LATPIN			LATB0
#define LED_4_RX_LATINV			LATBINV
#define LED_4_RX_MASK			(1<<0)
#define LED_4_RX_ANSELbits		AD1PCFGbits
#define LED_4_RX_ANSELBIT		PCFG0

// -----------------------------------------------------------------------------




// UARTs -----------------------------------------------------------------------
// Debug UART - UART1
#define UART_DBG_TX_TRISbits		TRISDbits
#define UART_DBG_TX_TRISPIN			TRISD3
#define UART_DBG_TX_LATbits			LATDbits
#define UART_DBG_TX_LATPIN			LATD3

#define UART_DBG_RX_TRISbits		TRISDbits
#define UART_DBG_RX_TRISPIN			TRISD2

#define UART_DBG_MODE_bits			U1MODEbits
#define UART_DBG_STA_bits			U1STAbits
#define UART_DBG_BRG_reg			U1BRG
#define UART_DBG_TX_reg				U1TXREG
#define UART_DBG_RX_reg				U1RXREG

#define UART_DBG_INT_IPC_bits			IPC6bits
#define UART_DBG_INT_IPC_PRIORITY		U1IP
#define UART_DBG_INT_IPC_SUBPRIORITY	U1IS
#define UART_DBG_INT_IEC_bits			IEC0bits
#define UART_DBG_INT_IEC_RXIE			U1RXIE
#define UART_DBG_INT_IFS_bits			IFS0bits
#define UART_DBG_INT_IFS_RXIF			U0RXIF

// From here on out, there is some lookup-table-type-thing going on
// External UART1 - PIC UART3
// External UART2 - PIC UART6
// External UART3 - PIC UART2
// External UART4 - PIC UART5

// External UART1 - PIC UART3
#define UART_1_TX_TRISbits			TRISGbits
#define UART_1_TX_TRISPIN			TRISG8
#define UART_1_TX_LATbits			LATGbits
#define UART_1_TX_LATPIN			LATG8

#define UART_1_RX_TRISbits			TRISGbits
#define UART_1_RX_TRISPIN			TRISG7

#define UART_1_DTR_TRISbits			TRISDbits
#define UART_1_DTR_TRISPIN			TRISD4
#define UART_1_DTR_LATbits			LATDbits
#define UART_1_DTR_LATPIN			LATD4

#define UART_1_MODE_bits			U3MODEbits
#define UART_1_STA_bits				U3STAbits
#define UART_1_BRG_reg				U3BRG
#define UART_1_TX_reg				U3TXREG
#define UART_1_RX_reg				U3RXREG

#define UART_1_INT_IPC_bits			IPC8bits
#define UART_1_INT_IPC_PRIORITY		U3AIP		// "Old"/pre-2019 definitions....
#define UART_1_INT_IPC_SUBPRIORITY	U3AIS		// "Old"/pre-2019 definitions....
#define UART_1_INT_IEC_bits			IEC1bits
#define UART_1_INT_IEC_RXIE			U3RXIE
#define UART_1_INT_IEC_TXIE			U3TXIE
#define UART_1_INT_IFS_bits			IFS1bits
#define UART_1_INT_IFS_RXIF			U3RXIF
#define UART_1_INT_IFS_TXIF			U3TXIF

// External UART2 - PIC UART6
#define UART_2_TX_TRISbits			TRISGbits
#define UART_2_TX_TRISPIN			TRISG6
#define UART_2_TX_LATbits			LATGbits
#define UART_2_TX_LATPIN			LATG6

#define UART_2_RX_TRISbits			TRISGbits
#define UART_2_RX_TRISPIN			TRISG9

#define UART_2_DTR_TRISbits			TRISDbits
#define UART_2_DTR_TRISPIN			TRISD5
#define UART_2_DTR_LATbits			LATDbits
#define UART_2_DTR_LATPIN			LATD5

#define UART_2_MODE_bits			U6MODEbits
#define UART_2_STA_bits				U6STAbits
#define UART_2_BRG_reg				U6BRG
#define UART_2_TX_reg				U6TXREG
#define UART_2_RX_reg				U6RXREG

#define UART_2_INT_IPC_bits			IPC12bits
#define UART_2_INT_IPC_PRIORITY		U6IP
#define UART_2_INT_IPC_SUBPRIORITY	U6IS
#define UART_2_INT_IEC_bits			IEC2bits
#define UART_2_INT_IEC_RXIE			U6RXIE
#define UART_2_INT_IEC_TXIE			U6TXIE
#define UART_2_INT_IFS_bits			IFS2bits
#define UART_2_INT_IFS_RXIF			U6RXIF
#define UART_2_INT_IFS_TXIF			U6TXIF


#define UART_3_TX_TRISbits			TRISFbits
#define UART_3_TX_TRISPIN			TRISF5
#define UART_3_TX_LATbits			LATFbits
#define UART_3_TX_LATPIN			LATF5

#define UART_3_RX_TRISbits			TRISFbits
#define UART_3_RX_TRISPIN			TRISF4

#define UART_3_DTR_TRISbits			TRISBbits
#define UART_3_DTR_TRISPIN			TRISB5
#define UART_3_DTR_LATbits			LATBbits
#define UART_3_DTR_LATPIN			LATB5
#define UART_3_DTR_MASK				(1<<5)
#define UART_3_DTR_ANSELbits		AD1PCFGbits
#define UART_3_DTR_ANSELBIT			PCFG5

#define UART_3_MODE_bits			U2MODEbits
#define UART_3_STA_bits				U2STAbits
#define UART_3_BRG_reg				U2BRG
#define UART_3_TX_reg				U2TXREG
#define UART_3_RX_reg				U2RXREG

#define UART_3_INT_IPC_bits			IPC7bits
#define UART_3_INT_IPC_PRIORITY		U2AIP		// "Old"/pre-2019 definitions....
#define UART_3_INT_IPC_SUBPRIORITY	U2AIS		// "Old"/pre-2019 definitions....
#define UART_3_INT_IEC_bits			IEC1bits
#define UART_3_INT_IEC_RXIE			U2RXIE
#define UART_3_INT_IEC_TXIE			U2TXIE
#define UART_3_INT_IFS_bits			IFS1bits
#define UART_3_INT_IFS_RXIF			U2RXIF
#define UART_3_INT_IFS_TXIF			U2TXIF


#define UART_4_TX_TRISbits			TRISBbits
#define UART_4_TX_TRISPIN			TRISB14
#define UART_4_TX_LATbits			LATBbits
#define UART_4_TX_LATPIN			LATB14
#define UART_4_TX_MASK				(1<<14)
#define UART_4_TX_ANSELbits			AD1PCFGbits
#define UART_4_TX_ANSELBIT			PCFG14

#define UART_4_RX_TRISbits			TRISBbits
#define UART_4_RX_TRISPIN			TRISB8
#define UART_4_RX_MASK				(1<<8)
#define UART_4_RX_ANSELbits			AD1PCFGbits
#define UART_4_RX_ANSELBIT			PCFG8

#define UART_4_DTR_TRISbits			TRISBbits
#define UART_4_DTR_TRISPIN			TRISB4
#define UART_4_DTR_LATbits			LATBbits
#define UART_4_DTR_LATPIN			LATB4
#define UART_4_DTR_MASK				(1<<4)
#define UART_4_DTR_ANSELbits		AD1PCFGbits
#define UART_4_DTR_ANSELBIT			PCFG4

#define UART_4_MODE_bits			U5MODEbits
#define UART_4_STA_bits				U5STAbits
#define UART_4_BRG_reg				U5BRG
#define UART_4_TX_reg				U5TXREG
#define UART_4_RX_reg				U5RXREG

#define UART_4_INT_IPC_bits			IPC12bits
#define UART_4_INT_IPC_PRIORITY		U5IP
#define UART_4_INT_IPC_SUBPRIORITY	U5IS
#define UART_4_INT_IEC_bits			IEC2bits
#define UART_4_INT_IEC_RXIE			U5RXIE
#define UART_4_INT_IEC_TXIE			U5TXIE
#define UART_4_INT_IFS_bits			IFS2bits
#define UART_4_INT_IFS_RXIF			U5RXIF
#define UART_4_INT_IFS_TXIF			U5TXIF


// Function definitons
void GPIODrv_init();

// UART things
void GPIODrv_setDTR_1(bool state);
void GPIODrv_setDTR_2(bool state);
void GPIODrv_setDTR_3(bool state);
void GPIODrv_setDTR_4(bool state);

void GPIODrv_setLED_1_TX(bool state);
void GPIODrv_setLED_1_RX(bool state);
void GPIODrv_setLED_2_TX(bool state);
void GPIODrv_setLED_2_RX(bool state);
void GPIODrv_setLED_3_TX(bool state);
void GPIODrv_setLED_3_RX(bool state);
void GPIODrv_setLED_4_TX(bool state);
void GPIODrv_setLED_5_RX(bool state);


#endif
