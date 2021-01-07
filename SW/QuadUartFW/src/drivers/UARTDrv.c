#include <p32xxxx.h>
#include <UARTDrv.h>
#include <GPIODrv.h>
#include <inttypes.h>
#include <interrupt.h>
#include <COMMS.h>
#include <kmem.h>
#include <usb.h>		// If usb.h isn't present, usb_cdc.h goes ballistic with errors.
#include <usb_cdc.h>

#define PBCLK	40000000
#define UART_FIFO_SIZE	8

// External UART1 - PIC UART3
// External UART2 - PIC UART6
// External UART3 - PIC UART2
// External UART4 - PIC UART5


uint32_t sizeToSendTx = 0;	// Size to increment the circular buffer after transfer is done
#if defined (__32MX440F256H__)
#define DMA_MAX_SIZE 256
#elif defined (__32MX564F128H__)
#define DMA_MAX_SIZE 65536
#else
_Static_assert(0, "Please add the proper define for DMA");
#endif

// Combined RX *and* TX interrupts. need to check which flag is set when entering
// Note, this should be ok for speeds like 115200. At 4x1Mbps, it might get bogged down

// External UART1 - PIC UART3
INTERRUPT(I2C4_SPI2_UART3_SharedInterrupt){
	uint8_t buffer[8];
	uint32_t counter;
	uint32_t txCounter;
	// We prioritize the RX interrupt
	if (UART_1_INT_IFS_bits.UART_1_INT_IFS_RXIF){
		// Readout data
		counter = 0;
		while(UART_1_STA_bits.URXDA){
			buffer[counter++] = UART_1_RX_reg;
		}
		// Save to buffer
		COMMS_helper_addToBuf(&comStruct_UART_1_RX, buffer, counter);
		// Clear interrupt flag
		UART_1_INT_IFS_bits.UART_1_INT_IFS_RXIF = 0;
	}

	// Then the TX one
	if (UART_1_INT_IFS_bits.UART_1_INT_IFS_TXIF){
		// Get as much data as we can (max 8)
		counter = COMMS_helper_dataLen(&comStruct_UART_1_TX);
		counter = (counter > UART_FIFO_SIZE) ? UART_FIFO_SIZE : counter;
		// If there is any data, send out
		if (counter){
			COMMS_helper_getData(&comStruct_UART_1_TX, buffer, counter);
			for (txCounter = 0; txCounter < counter; txCounter++){
				UART_1_TX_reg = buffer[txCounter];
			}
		}
		// Clear interrupt flag
		UART_1_INT_IFS_bits.UART_1_INT_IFS_TXIF = 0;
	}
}

// External UART2 - PIC UART6
INTERRUPT(UART6_UART2B_SharedInterrupt){
	uint8_t buffer[8];
	uint32_t counter;
	uint32_t txCounter;
	// We prioritize the RX interrupt
	if (UART_2_INT_IFS_bits.UART_2_INT_IFS_RXIF){
		// Readout data
		counter = 0;
		while(UART_2_STA_bits.URXDA){
			buffer[counter++] = UART_2_RX_reg;
		}
		// Save to buffer
		COMMS_helper_addToBuf(&comStruct_UART_2_RX, buffer, counter);
		// Clear interrupt flag
		UART_2_INT_IFS_bits.UART_2_INT_IFS_RXIF = 0;
	}

	// Then the TX one
	if (UART_2_INT_IFS_bits.UART_2_INT_IFS_TXIF){
		// Get as much data as we can (max 8)
		counter = COMMS_helper_dataLen(&comStruct_UART_2_TX);
		counter = (counter > UART_FIFO_SIZE) ? UART_FIFO_SIZE : counter;
		// If there is any data, send out
		if (counter){
			COMMS_helper_getData(&comStruct_UART_2_TX, buffer, counter);
			for (txCounter = 0; txCounter < counter; txCounter++){
				UART_2_TX_reg = buffer[txCounter];
			}
		}
		// Clear interrupt flag
		UART_2_INT_IFS_bits.UART_2_INT_IFS_TXIF = 0;
	}
}

// External UART3 - PIC UART2
INTERRUPT(I2C5_SPI4_UART2_SharedInterrupt){
	uint8_t buffer[8];
	uint32_t counter;
	uint32_t txCounter;
	// We prioritize the RX interrupt
	if (UART_3_INT_IFS_bits.UART_3_INT_IFS_RXIF){
		// Readout data
		counter = 0;
		while(UART_3_STA_bits.URXDA){
			buffer[counter++] = UART_3_RX_reg;
		}
		// Save to buffer
		COMMS_helper_addToBuf(&comStruct_UART_3_RX, buffer, counter);
		// Clear interrupt flag
		UART_3_INT_IFS_bits.UART_3_INT_IFS_RXIF = 0;
	}

	// Then the TX one
	if (UART_3_INT_IFS_bits.UART_3_INT_IFS_TXIF){
		// Get as much data as we can (max 8)
		counter = COMMS_helper_dataLen(&comStruct_UART_3_TX);
		counter = (counter > UART_FIFO_SIZE) ? UART_FIFO_SIZE : counter;
		// If there is any data, send out
		if (counter){
			COMMS_helper_getData(&comStruct_UART_3_TX, buffer, counter);
			for (txCounter = 0; txCounter < counter; txCounter++){
				UART_3_TX_reg = buffer[txCounter];
			}
		}
		// Clear interrupt flag
		UART_3_INT_IFS_bits.UART_3_INT_IFS_TXIF = 0;
	}
}

// External UART4 - PIC UART5
INTERRUPT(UART5_UART3B_SharedInterrupt){
	uint8_t buffer[8];
	uint32_t counter;
	uint32_t txCounter;
	// We prioritize the RX interrupt
	if (UART_4_INT_IFS_bits.UART_4_INT_IFS_RXIF){
		// Readout data
		counter = 0;
		while(UART_4_STA_bits.URXDA){
			buffer[counter++] = UART_4_RX_reg;
		}
		// Save to buffer
		COMMS_helper_addToBuf(&comStruct_UART_4_RX, buffer, counter);
		// Clear interrupt flag
		UART_4_INT_IFS_bits.UART_4_INT_IFS_RXIF = 0;
	}

	// Then the TX one
	if (UART_4_INT_IFS_bits.UART_4_INT_IFS_TXIF){
		// Get as much data as we can (max 8)
		counter = COMMS_helper_dataLen(&comStruct_UART_4_TX);
		counter = (counter > UART_FIFO_SIZE) ? UART_FIFO_SIZE : counter;
		// If there is any data, send out
		if (counter){
			COMMS_helper_getData(&comStruct_UART_4_TX, buffer, counter);
			for (txCounter = 0; txCounter < counter; txCounter++){
				UART_4_TX_reg = buffer[txCounter];
			}
		}
		// Clear interrupt flag
		UART_4_INT_IFS_bits.UART_4_INT_IFS_TXIF = 0;
	}
}





void UARTDrv_1_Init(struct cdc_line_coding* coding){
	UART_1_MODE_bits.ON = 0;

	UART_1_TX_TRISbits.UART_1_TX_TRISPIN = 0;	// 0 == output
	UART_1_TX_LATbits.UART_1_TX_LATPIN = 1;		// Set high, as UART is Idle High

	UART_1_RX_TRISbits.UART_1_TX_TRISPIN = 1;						// 1 == input

	UART_1_MODE_bits.SIDL = 0;		// Stop when in IDLE mode
	UART_1_MODE_bits.IREN	= 0;	// Disable IrDA
	UART_1_MODE_bits.RTSMD = 0;		// Don't care, RTS not used
	UART_1_MODE_bits.UEN = 0;		// TX & RX controlled by UART peripheral, RTC & CTS are not.
	UART_1_MODE_bits.WAKE = 0;		// Don't wake up from sleep
	UART_1_MODE_bits.LPBACK = 0;	// Loopback mode disabled
	UART_1_MODE_bits.ABAUD = 0;		// No autobauding
	UART_1_MODE_bits.RXINV = 0;		// Idle HIGH
	UART_1_MODE_bits.BRGH = 0;		// Standard speed mode - 16x baud clock
	UART_1_MODE_bits.PDSEL = 0;		// 8 bits, no parity
	if (coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS){
		UART_1_MODE_bits.STSEL = 1;	// 2 stop bits
	}
	else{
		UART_1_MODE_bits.STSEL = 0;	// 1 stop bit
	}

	UART_1_STA_bits.ADM_EN = 0;		// Don't care for auto address detection, unused
	UART_1_STA_bits.ADDR = 0;		// Don't care for auto address mark
	UART_1_STA_bits.UTXISEL = 0b10;	//TODO
	UART_1_STA_bits.UTXINV = 0;		// Idle HIGH
	UART_1_STA_bits.URXEN = 1;		// UART receiver pin enabled
	UART_1_STA_bits.UTXBRK = 0;		// Don't send breaks.
	UART_1_STA_bits.UTXEN = 1;		// Uart transmitter pin enabled
	UART_1_STA_bits.URXISEL = 0;	// Interrupt what receiver buffer not empty
	UART_1_STA_bits.ADDEN = 0;		// Address detect mode disabled (unused)
	UART_1_STA_bits.OERR = 0;		// Clear RX Overrun bit - not important at this point

	// (PBCLK/BRGH?4:16)/BAUD - 1
	UART_1_BRG_reg = (PBCLK / (U2MODEbits.BRGH ? 4 : 16)) / coding->dwDTERate - 1;

	// Setup interrupt - Split into new function fer easier ifdef-ing?
	UART_1_INT_IPC_bits.UART_1_INT_IPC_PRIORITY		= 6;	// Priority = 6, highest, above USB.
															// Once receiving is put into DMA, it can be lower.
	UART_1_INT_IPC_bits.UART_1_INT_IPC_SUBPRIORITY 	= 0;	// Subpriority = 0;
	UART_1_INT_IEC_bits.UART_1_INT_IEC_RXIE			= 1;	// Enable interrupt.

	//UARTDrv_InitDMA();	// Perform initializations of DMA.	// TODO

	UART_1_MODE_bits.ON = 1;
}


void UARTDrv_2_Init(struct cdc_line_coding* coding){	UART_1_MODE_bits.ON = 0;

	UART_2_MODE_bits.ON = 0;

	UART_2_TX_TRISbits.UART_2_TX_TRISPIN = 0;	// 0 == output
	UART_2_TX_LATbits.UART_2_TX_LATPIN = 1;		// Set high, as UART is Idle High

	UART_2_RX_TRISbits.UART_2_TX_TRISPIN = 1;						// 1 == input

	UART_2_MODE_bits.SIDL = 0;	// Stop when in IDLE mode
	UART_2_MODE_bits.IREN	= 0;	// Disable IrDA
	//UART_2_MODE_bits.RTSMD = 0;	// Don't care, RTS not used - not supported on this UART
	//UART_2_MODE_bits.UEN = 0;		// TX & RX controlled by UART peripheral, RTC & CTS are not. - not supported on this UART
	UART_2_MODE_bits.WAKE = 0;	// Don't wake up from sleep
	UART_2_MODE_bits.LPBACK = 0;	// Loopback mode disabled
	UART_2_MODE_bits.ABAUD = 0;	// No autobauding
	UART_2_MODE_bits.RXINV = 0;	// Idle HIGH
	UART_2_MODE_bits.BRGH = 0;	// Standard speed mode - 16x baud clock
	UART_2_MODE_bits.PDSEL = 0;	// 8 bits, no parity
	if (coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS){
		UART_2_MODE_bits.STSEL = 1;	// 2 stop bits
	}
	else{
		UART_2_MODE_bits.STSEL = 0;	// 1 stop bit
	}

	UART_2_STA_bits.ADM_EN = 0;	// Don't care for auto address detection, unused
	UART_2_STA_bits.ADDR = 0;		// Don't care for auto address mark
	UART_2_STA_bits.UTXISEL = 0b10;	//TODO
	UART_2_STA_bits.UTXINV = 0;	// Idle HIGH
	UART_2_STA_bits.URXEN = 1;	// UART receiver pin enabled
	UART_2_STA_bits.UTXBRK = 0;	// Don't send breaks.
	UART_2_STA_bits.UTXEN = 1;	// Uart transmitter pin enabled
	UART_2_STA_bits.URXISEL = 0;	// Interrupt what receiver buffer not empty
	UART_2_STA_bits.ADDEN = 0;	// Address detect mode disabled (unused)
	UART_2_STA_bits.OERR = 0;		// Clear RX Overrun bit - not important at this point

	// (PBCLK/BRGH?4:16)/BAUD - 1
	UART_2_BRG_reg = (PBCLK / (U2MODEbits.BRGH ? 4 : 16)) / coding->dwDTERate - 1;

	// Setup interrupt - Split into new function fer easier ifdef-ing?
	UART_2_INT_IPC_bits.UART_2_INT_IPC_PRIORITY		= 6;	// Priority = 6, highest, above USB.
															// Once receiving is put into DMA, it can be lower.
	UART_2_INT_IPC_bits.UART_2_INT_IPC_SUBPRIORITY 	= 0;	// Subpriority = 0;
	UART_2_INT_IEC_bits.UART_2_INT_IEC_RXIE			= 1;	// Enable interrupt.

	//UARTDrv_InitDMA();	// Perform initializations of DMA.	// TODO

	UART_2_MODE_bits.ON = 1;

}
void UARTDrv_3_Init(struct cdc_line_coding* coding){

	UART_3_MODE_bits.ON = 0;

	UART_3_TX_TRISbits.UART_3_TX_TRISPIN = 0;	// 0 == output
	UART_3_TX_LATbits.UART_3_TX_LATPIN = 1;		// Set high, as UART is Idle High

	UART_3_RX_TRISbits.UART_3_TX_TRISPIN = 1;						// 1 == input

	UART_3_MODE_bits.SIDL = 0;	// Stop when in IDLE mode
	UART_3_MODE_bits.IREN	= 0;	// Disable IrDA
	UART_3_MODE_bits.RTSMD = 0;	// Don't care, RTS not used
	UART_3_MODE_bits.UEN = 0;		// TX & RX controlled by UART peripheral, RTC & CTS are not.
	UART_3_MODE_bits.WAKE = 0;	// Don't wake up from sleep
	UART_3_MODE_bits.LPBACK = 0;	// Loopback mode disabled
	UART_3_MODE_bits.ABAUD = 0;	// No autobauding
	UART_3_MODE_bits.RXINV = 0;	// Idle HIGH
	UART_3_MODE_bits.BRGH = 0;	// Standard speed mode - 16x baud clock
	UART_3_MODE_bits.PDSEL = 0;	// 8 bits, no parity
	if (coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS){
		UART_3_MODE_bits.STSEL = 1;	// 2 stop bits
	}
	else{
		UART_3_MODE_bits.STSEL = 0;	// 1 stop bit
	}

	UART_3_STA_bits.ADM_EN = 0;	// Don't care for auto address detection, unused
	UART_3_STA_bits.ADDR = 0;		// Don't care for auto address mark
	UART_3_STA_bits.UTXISEL = 0b10;	//TODO
	UART_3_STA_bits.UTXINV = 0;	// Idle HIGH
	UART_3_STA_bits.URXEN = 1;	// UART receiver pin enabled
	UART_3_STA_bits.UTXBRK = 0;	// Don't send breaks.
	UART_3_STA_bits.UTXEN = 1;	// Uart transmitter pin enabled
	UART_3_STA_bits.URXISEL = 0;	// Interrupt what receiver buffer not empty
	UART_3_STA_bits.ADDEN = 0;	// Address detect mode disabled (unused)
	UART_3_STA_bits.OERR = 0;		// Clear RX Overrun bit - not important at this point

	// (PBCLK/BRGH?4:16)/BAUD - 1
	UART_3_BRG_reg = (PBCLK / (U2MODEbits.BRGH ? 4 : 16)) / coding->dwDTERate - 1;

	// Setup interrupt - Split into new function fer easier ifdef-ing?
	UART_3_INT_IPC_bits.UART_3_INT_IPC_PRIORITY		= 6;	// Priority = 6, highest, above USB.
															// Once receiving is put into DMA, it can be lower.
	UART_3_INT_IPC_bits.UART_3_INT_IPC_SUBPRIORITY 	= 0;	// Subpriority = 0;
	UART_3_INT_IEC_bits.UART_3_INT_IEC_RXIE			= 1;	// Enable interrupt.

	//UARTDrv_InitDMA();	// Perform initializations of DMA.	// TODO

	UART_3_MODE_bits.ON = 1;

}
void UARTDrv_4_Init(struct cdc_line_coding* coding){

	UART_4_MODE_bits.ON = 0;

	UART_4_TX_TRISbits.UART_4_TX_TRISPIN = 0;	// 0 == output
	UART_4_TX_LATbits.UART_4_TX_LATPIN = 1;		// Set high, as UART is Idle High

	UART_4_RX_TRISbits.UART_4_TX_TRISPIN = 1;						// 1 == input

	UART_4_MODE_bits.SIDL = 0;	// Stop when in IDLE mode
	UART_4_MODE_bits.IREN	= 0;	// Disable IrDA
	//UART_4_MODE_bits.RTSMD = 0;	// Don't care, RTS not used - not supported on this UART
	//UART_4_MODE_bits.UEN = 0;		// TX & RX controlled by UART peripheral, RTC & CTS are not. - not supported on this UART
	UART_4_MODE_bits.WAKE = 0;	// Don't wake up from sleep
	UART_4_MODE_bits.LPBACK = 0;	// Loopback mode disabled
	UART_4_MODE_bits.ABAUD = 0;	// No autobauding
	UART_4_MODE_bits.RXINV = 0;	// Idle HIGH
	UART_4_MODE_bits.BRGH = 0;	// Standard speed mode - 16x baud clock
	UART_4_MODE_bits.PDSEL = 0;	// 8 bits, no parity
	if (coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS){
		UART_4_MODE_bits.STSEL = 1;	// 2 stop bits
	}
	else{
		UART_4_MODE_bits.STSEL = 0;	// 1 stop bit
	}

	UART_4_STA_bits.ADM_EN = 0;	// Don't care for auto address detection, unused
	UART_4_STA_bits.ADDR = 0;		// Don't care for auto address mark
	UART_4_STA_bits.UTXISEL = 0b10;	//TODO
	UART_4_STA_bits.UTXINV = 0;	// Idle HIGH
	UART_4_STA_bits.URXEN = 1;	// UART receiver pin enabled
	UART_4_STA_bits.UTXBRK = 0;	// Don't send breaks.
	UART_4_STA_bits.UTXEN = 1;	// Uart transmitter pin enabled
	UART_4_STA_bits.URXISEL = 0;	// Interrupt what receiver buffer not empty
	UART_4_STA_bits.ADDEN = 0;	// Address detect mode disabled (unused)
	UART_4_STA_bits.OERR = 0;		// Clear RX Overrun bit - not important at this point

	// (PBCLK/BRGH?4:16)/BAUD - 1
	UART_4_BRG_reg = (PBCLK / (U2MODEbits.BRGH ? 4 : 16)) / coding->dwDTERate - 1;

	// Setup interrupt - Split into new function fer easier ifdef-ing?
	UART_4_INT_IPC_bits.UART_4_INT_IPC_PRIORITY		= 6;	// Priority = 6, highest, above USB.
															// Once receiving is put into DMA, it can be lower.
	UART_4_INT_IPC_bits.UART_4_INT_IPC_SUBPRIORITY 	= 0;	// Subpriority = 0;
	UART_4_INT_IEC_bits.UART_4_INT_IEC_RXIE			= 1;	// Enable interrupt.

	//UARTDrv_InitDMA();	// Perform initializations of DMA.	// TODO

	UART_4_MODE_bits.ON = 1;

}























































// DMA 0 for TX-ing, DMA1 for RXint (when added)
//INTERRUPT(DMA0Interrupt){
//
//	// Currently, when finished, just update the buffer position
//	// and clear interrupt flag
//	uartTXstruct.tail = (uartTXstruct.tail + sizeToSendTx) & cyclicBufferSizeMask;
//
//	DCH0INTCLR = _DCH0INT_CHBCIF_MASK;	// Clear the DMA channel interrupt flag (Block Transfer Done)
//	IFS1bits.DMA0IF = 0;				// Clear the DMA0 interrup flag ><

//}


//void UARTDrv_InitDMA(){
//	// Setup parts of DMA, like priorities etc.
//
//	DCH0INTbits.CHBCIE = 1;	// Enable Block Transfer Done
//
//#if defined(__32MX440F256H__)
//	IPC9bits.DMA0IP = 5;	// Priority 5 (higher than USB, so we update the size at the right time)
//	IPC9bits.DMA0IS = 0;	// Subpriority 0
//#else
//	_Static_assert(0, "Not implemented yet.");
//#endif
//
//	//DCH0CON = 0;				// Clear everything.
//	DCH0CONCLR = 0xFFFFFFFF;
//	// No CHBUSY bit, so can't check for end of transaction.
//	//DCH0CONbits.CHPRI = 0x03;	// Higher priority
//	//DCH0CONbits.CHAEN = 0;		// Auto Disable on block transfer
//	//DCH0CONbits.CHCHN = 0;		// No chaining
//	DCH0CONSET = 0x3 << _DCH0CON_CHPRI_POSITION;
//
//	//IEC1bits.DMA0IE = 1;		// Enable DMA0 interrupt
//	IEC1SET = _IEC1_DMA0IE_MASK;
//
//	// DMACON gets enabled at the end of setup in main.
//}

//void UARTDrv_Init(struct cdc_line_coding* coding){
//	UART_MODE_bits.ON = 0;
//
//	UART_TX_TRISbits.UART_TX_TRISPIN = 0;	// 0 == output
//	UART_TX_LATbits.UART_TX_LATPIN = 1;		// Set high, as UART is Idle High
//	#ifdef UART_TX_RP_REG
//		UART_TX_RP_REG = UART_TX_RP_VAL;		// Remap to proper pin
//	#endif
//
//	UART_RX_TRISbits.UART_TX_TRISPIN = 1;						// 1 == input
//	#ifdef UART_RX_PULLREG
//		UART_RX_PULLREG = UART_RX_PULLREG | UART_RX_PULLBIT;	// Enable pull-up
//	#endif
//
//	#ifdef UART_RX_REMAP_VAL
//		U2RXR = UART_RX_REMAP_VAL;									// Set to which pin
//	#endif
//
//	UART_MODE_bits.SIDL = 0;	// Stop when in IDLE mode
//	UART_MODE_bits.IREN	= 0;	// Disable IrDA
//	UART_MODE_bits.RTSMD = 0;	// Don't care, RTS not used
//	UART_MODE_bits.UEN = 0;		// TX & RX controlled by UART peripheral, RTC & CTS are not.
//	UART_MODE_bits.WAKE = 0;	// Don't wake up from sleep
//	UART_MODE_bits.LPBACK = 0;	// Loopback mode disabled
//	UART_MODE_bits.ABAUD = 0;	// No autobauding
//	UART_MODE_bits.RXINV = 0;	// Idle HIGH
//	UART_MODE_bits.BRGH = 0;	// Standard speed mode - 16x baud clock
//	UART_MODE_bits.PDSEL = 0;	// 8 bits, no parity
//	if (coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS){
//		UART_MODE_bits.STSEL = 1;	// 2 stop bits
//	}
//	else{
//		UART_MODE_bits.STSEL = 0;	// 1 stop bit
//	}
//
//	UART_STA_bits.ADM_EN = 0;	// Don't care for auto address detection, unused
//	UART_STA_bits.ADDR = 0;		// Don't care for auto address mark
//	UART_STA_bits.UTXISEL = 0b10;	//TODO
//	UART_STA_bits.UTXINV = 0;	// Idle HIGH
//	UART_STA_bits.URXEN = 1;	// UART receiver pin enabled
//	UART_STA_bits.UTXBRK = 0;	// Don't send breaks.
//	UART_STA_bits.UTXEN = 1;	// Uart transmitter pin enabled
//	UART_STA_bits.URXISEL = 0;	// Interrupt what receiver buffer not empty
//	UART_STA_bits.ADDEN = 0;	// Address detect mode disabled (unused)
//	UART_STA_bits.OERR = 0;		// Clear RX Overrun bit - not important at this point
//
//	// (PBCLK/BRGH?4:16)/BAUD - 1
//	UART_BRG_reg = (40000000 / (U2MODEbits.BRGH ? 4 : 16)) / coding->dwDTERate - 1;
//
//	// Setup interrupt - Split into new function fer easier ifdef-ing?
//	UART_INT_IPC_bits.UART_INT_IPC_PRIORITY		= 6;	// Priority = 6, highest, above USB.
//														// Once receiving is put into DMA, it can be lower.
//	UART_INT_IPC_bits.UART_INT_IPC_SUBPRIORITY 	= 0;	// Subpriority = 0;
//	UART_INT_IEC_bits.UART_INT_IEC_RXIE			= 1;	// Enable interrupt.
//
//	UARTDrv_InitDMA();	// Perform initializations of DMA.
//
//	UART_MODE_bits.ON = 1;
//}

//void UARTDrv_SendBlocking(uint8_t * buffer, uint32_t length){
//	// TODO remove.
//	uint32_t counter = 0;
//
//	for (counter = 0; counter<length; counter++){
//		while(UART_STA_bits.UTXBF){ asm("nop"); }
//		UART_TX_reg = buffer[counter];
//		asm("nop");
//	}
//}



// Can expand for modularity, if multiple DMAs
//uint32_t UARTDrv_IsTxDmaRunning(){
//	return (DCH0CONbits.CHEN);	// Don't have a CHBUSY bit. Check if DMA disabled.
//}

//void UARTDrv_RunDmaTx(){
//	// So, here's the thing. For small transfers DMA is going to have a bit of overhead.
//	// Also, if we start near the end of the buffer, there will be some performance penalty. Oh well.
//
//	// Which struct
//	comStruct* whichStruct = &uartTXstruct;
//
//	// Get size to send
//	sizeToSendTx = COMMS_helper_dataLen(whichStruct);
//
//	// Take care of end of buffer edge case. TODO DOUBLE CHECK >=
//	if ((whichStruct->tail + sizeToSendTx) > cyclicBufferSizeMask){
//		sizeToSendTx = cyclicBufferSizeMask - whichStruct->tail + 1;
//	}
//
//	// TODO, save this differently, and update in DMA via DCH0CPTR or whichever.
//	if (sizeToSendTx > DMA_MAX_SIZE){
//		sizeToSendTx = DMA_MAX_SIZE;	// FUCKING BLANKET DATASHEETS AND SPECIFIC "some bits are not available on all devices" BULLSHIT.
//	}
//
//	UART_INT_IFS_bits.UART_INT_IFS_TXIF = 0;	// Clear TX done of last transfer.
//
//	//DCH0CONbits.CHAEN = 0;						// Disable channel
//	DCH0CONCLR = _DCH0CON_CHAEN_MASK;
//	asm("nop");
//
//
//	//DCH0ECON = 0;
//	DCH0ECONCLR = 0xFFFFFFFF;
//	asm("nop");
//	//DCH0ECONbits.CHSIRQ = _UART1_TX_IRQ;		// Which interrupts enable the transfer. Interrupts from 0 to 255. U1TX = 28 on 440F
//	DCH0ECONSET = (_UART1_TX_IRQ << _DCH0ECON_CHSIRQ_POSITION) | _DCH0ECON_SIRQEN_MASK;	// Run on irq set by CHSIRQ
//	asm("nop");
//
//
//	DCH0SSA = KVA_TO_PA(&(whichStruct->data[whichStruct->tail]));	// Source address
//	DCH0DSA = KVA_TO_PA(&U1TXREG);									// Destination address
//	DCH0SSIZ = (sizeToSendTx >= DMA_MAX_SIZE) ? 0 : sizeToSendTx;	// Source is of size sizeToSendTX. 0 == 2^numBits.
//	DCH0DSIZ = 1;													// UART register is 1B large
//	DCH0CSIZ = 1;													// 1B per UART transfer
//
//	DCH0INTCLR = 0x000000FF;										// Clear all interrupt flags
//	// IE flags are already set in DMA_init
//
//
//	//DCH0CONbits.CHEN = 1;		// Enable channel
//	DCH0CONSET = _DCH0CON_CHEN_MASK;
//	asm("nop");
//	//DCH0ECONbits.CFORCE = 1;	// Force start first transfer.
//	DCH0ECONSET = _DCH0ECON_CFORCE_MASK;
//	asm("nop");
//}


