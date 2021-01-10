#ifndef COMMS_H_96582fe1b9194228b1c0475bf8856698
#define COMMS_H_96582fe1b9194228b1c0475bf8856698

#define type_empty			0
#define type_info			1
#define type_packet			2
#define type_response_raw	3

#define status_ok			0
#define status_parsed		1
#define status_error		2
#define status_overflow		3


#define cyclicBufferSize		1024
#define cyclicBufferSizeMask	(cyclicBufferSize-1)
// Check to enforce buffer being of size 2^n
_Static_assert( ((cyclicBufferSize & cyclicBufferSizeMask) == 0), "Buffer size not equal to 2^n");


// To replace struct above!
typedef struct comStruct{
	uint8_t data[cyclicBufferSize];
	uint16_t head;
	uint16_t tail;
	uint32_t sizeofLastSent;
} comStruct;

extern comStruct comStruct_UART_1_TX;	// PC to target
extern comStruct comStruct_UART_1_RX;	// Target to PC
extern comStruct comStruct_UART_2_TX;	// PC to target
extern comStruct comStruct_UART_2_RX;	// Target to PC
extern comStruct comStruct_UART_3_TX;	// PC to target
extern comStruct comStruct_UART_3_RX;	// Target to PC
extern comStruct comStruct_UART_4_TX;	// PC to target
extern comStruct comStruct_UART_4_RX;	// Target to PC


// Newer functions
uint32_t COMMS_USB_sendToPC(comStruct * st, uint32_t ep, uint32_t maxLen);
uint32_t COMMS_USB_recvFromPC(comStruct * st, uint32_t ep);
uint32_t COMMS_helper_addToBuf(comStruct* st, uint8_t* data, uint16_t length);
uint32_t COMMS_helper_dataLen(comStruct* st);
uint32_t COMMS_helper_spaceLeft(comStruct* st);
void COMMS_helper_getData(comStruct* st, uint8_t *buf, uint16_t length);
void COMMS_helper_dropAll(comStruct* st);

#endif
