#include <p32xxxx.h>
#include <inttypes.h>
#include <COMMS.h>
#include <usb.h>
#include <string.h>
#include <stdio.h>


// Circular buffers, all same size.
// uartRX is fed with UART RX DMA. We have to make sure
// that we send data fast enough, so we don't get buffer
// overrun errors.
// uartTX is transmitted via UART TX DMA,
// at a more leisurely pace.

// TODO - initialize all to 0, don't trust the compiler.
comStruct comStruct_UART_1_TX = {0};	// PC to target
comStruct comStruct_UART_1_RX = {0};	// Target to PC
comStruct comStruct_UART_2_TX = {0};	// PC to target
comStruct comStruct_UART_2_RX = {0};	// Target to PC
comStruct comStruct_UART_3_TX = {0};	// PC to target
comStruct comStruct_UART_3_RX = {0};	// Target to PC
comStruct comStruct_UART_4_TX = {0};	// PC to target
comStruct comStruct_UART_4_RX = {0};	// Target to PC






//// FUNCTIONS PROGRAMMER
//uint32_t COMMS_progOUT_addToBuf(){
//	// Called from USB, to gives us data from PC
//	comStruct* whichStruct = &progOUTstruct;	// Copy&paste error protection
//	const unsigned char *out_buf;
//	volatile size_t out_buf_len;
//
//	// Check for an empty transaction.
//	out_buf_len = usb_get_out_buffer(EP_PROG_NUM, &out_buf);
//	if ( (out_buf_len <= 0)){
//		// This should happen on edge boundaries. Nothing wrong here.
//		usb_arm_out_endpoint(EP_PROG_NUM);
//		LED_USBPROG_OUT_toggle();	// Debug toggle
//		return 0;	// Signal packet used
//	}
//	else{
//		if (COMMS_helper_addToBuf(whichStruct, (uint8_t *)out_buf, out_buf_len)){
//			return 1;	// If no space, signal, that we didn't rearm endpoint.
//		}
//	}
//
//	usb_arm_out_endpoint(EP_PROG_NUM);
//	LED_USBPROG_OUT_toggle();	// Debug toggle
//	whichStruct->timeStamp = _CP0_GET_COUNT();
//	return 0;	// 0 on success, else otherwise (no more space, PC will buffer for us :3)
//}
//
//
//uint32_t COMMS_USB_progRET_transmitBuf(){
//	// Called from USB routine, to send data from the programmer to PC
//
//	comStruct* whichStruct = &progRETstruct;	// Copy&paste error protection
//	uint32_t sizeToSend = COMMS_helper_dataLen(whichStruct);
//	uint8_t *buf;
//
//
//	if (sizeToSend>EP_PROG_NUM_LEN){
//		sizeToSend=EP_PROG_NUM_LEN;	// Limit to number of packets
//	}
//
//	whichStruct->timeStamp = _CP0_GET_COUNT();			// Save current time.
//
//
//	if (sizeToSend == 0){
//		if (whichStruct->sizeofLastSent == EP_PROG_NUM_LEN){
//			// If we landed on a boundary last time, send a zero-length packet
//			usb_send_in_buffer(EP_PROG_NUM, 0);
//			LED_USBPROG_IN_toggle();	// Toggle on each send, for nicer debugging
//			whichStruct->sizeofLastSent = 0;
//			return 0;	// Return - packet used up
//		}
//		else{
//			// Do nothing. Nothing to send, no transaction to complete.
//			return 1;	// Return - nothing to be done
//		}
//	}
//	else{
//		buf = usb_get_in_buffer(EP_PROG_NUM);			// Get buffer from endpoint
//		COMMS_helper_getData(whichStruct, sizeToSend, buf);	// Get sizeToSend data and copy into buf
//		usb_send_in_buffer(EP_PROG_NUM, sizeToSend);	// Send on endpoint EP_UART_NUM, of length sizeToSend
//		LED_USBPROG_IN_toggle();	// Toggle on each send, for nicer debugging
//		whichStruct->sizeofLastSent = sizeToSend;		// Save data size, so we can finish transaction if needed
//		return 0;		// Return - packet used up
//	}
//
//}


// HELPER FUNCTION
// helper function to add to buffer, so don't have to deal with hard coded things etc.
uint32_t COMMS_helper_addToBuf(comStruct* st, uint8_t* data, uint16_t length){
	if (COMMS_helper_spaceLeft(st) < length){
		return 1;	// Fail
	}

	uint32_t i = 0;
	for (i=0; i < length; i++){
		st->data[st->head] = data[i];
		st->head = (st->head + 1) & cyclicBufferSizeMask;
	}

	return 0; // 0 on success, else otherwise (no space available)
}

// Returns how much data is in the struct
uint32_t COMMS_helper_dataLen(comStruct* st){
	return (st->head - st->tail) & cyclicBufferSizeMask;
}

// Returns how much space is left in the struct
uint32_t COMMS_helper_spaceLeft(comStruct* st){
	return (st->tail - st->head - 1) & cyclicBufferSizeMask;
}

// Get x bytes
void COMMS_helper_getData(comStruct* st, uint8_t *buf, uint16_t length){
	if (COMMS_helper_dataLen(st) < length){
		// Don't do this please, check beforehand
		return;
	}
	uint32_t i = 0;
	for (i=0; i< length; i++){
		buf[i] = st->data[st->tail];
		st->tail = (st->tail + 1) & cyclicBufferSizeMask;
	}
}

// "Delete" any data in the buffer
void COMMS_helper_dropAll(comStruct* st){
	st->tail = st->head;
}



