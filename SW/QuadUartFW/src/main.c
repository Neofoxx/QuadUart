#include <p32xxxx.h>
#include <inttypes.h>
#include <stdbool.h>

#include <string.h>
#include <stdio.h>
#include <newlib.h>
#include <errno.h>

// Config bits
#include <configBits.h>

// Drivers for HW
#include <GPIODrv.h>
#include <UARTDrv.h>
//#include <LED.h>
// USB
#include <usb.h>
#include <usb_config.h>
#include <usb_ch9.h>
#include <usb_cdc.h>
// COMMS - usb and UART
#include <COMMS.h>
// Interrupts
#include <interrupt.h>



#ifdef MULTI_CLASS_DEVICE
// Interfaces 0, 2, 4 and 6. Each begins at the IAD, and then goes on for 2 interfaces (0 and 1, 2 and 3, 4 and 5, 6 and 7).
static uint8_t cdc_interfaces[] = { 0, 2, 4, 6 };
#endif

static struct cdc_line_coding line_coding_1 =
{
	115200,
	CDC_CHAR_FORMAT_1_STOP_BIT,
	CDC_PARITY_NONE,
	8,
};

static struct cdc_line_coding line_coding_2 =
{
	115200,
	CDC_CHAR_FORMAT_1_STOP_BIT,
	CDC_PARITY_NONE,
	8,
};

static struct cdc_line_coding line_coding_3 =
{
	115200,
	CDC_CHAR_FORMAT_1_STOP_BIT,
	CDC_PARITY_NONE,
	8,
};

static struct cdc_line_coding line_coding_4 =
{
	115200,
	CDC_CHAR_FORMAT_1_STOP_BIT,
	CDC_PARITY_NONE,
	8,
};



volatile uint32_t packetCounter = 0;	// Reset by SOF. May be useful


// TODO - run the timer, like in MX440 example (SysTick style)
void simpleDelay(unsigned int noOfLoops){
    unsigned int i = 0;
    while (i<noOfLoops){
        i++;
        asm("nop");
    }
}

void setup(){
	// What is the equivalent of SYSTEMConfigPerformance?
	// -> Setting up the system for the required System Clock
	// -> Seting up the Wait States
	// -> Setting up PBCLK
	// -> Setting up Cache module (not presenf on MX1/2, but is on MX4)
	// Also of interest: https://microchipdeveloper.com/32bit:mx-arch-exceptions-processor-initialization
	// See Pic32 reference manual, for CP0 info http://ww1.microchip.com/downloads/en/devicedoc/61113e.pdf

	// DO NOT setup KSEG0 (cacheable area) on MX1/MX2, debugging will NOT work

	unsigned int val;
	val = _CP0_GET_CONFIG();	// Macro present in cp0defs.h
	val |= 0x00000003;	// Sets to KSEG0/cachable region, as per Table 2-11 in ref. manual
	_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, val);	// No macro, do it as suggested in first link.

	BMXCONbits.BMXWSDRM = 0;	// Set wait-states to 0 for Data Memory from SRAM

	CHECONbits.PFMWS = 2;		// Flash requires minimum 2 wait states at 61-80MHz

	CHECONbits.PREFEN = 1;		// Enable prefetch for cacheable PFM instructions

	GPIODrv_init();

	UARTDrv_1_Init(&line_coding_1);
	UARTDrv_2_Init(&line_coding_2);
	UARTDrv_3_Init(&line_coding_3);
	UARTDrv_4_Init(&line_coding_4);

	// Copied for USB, from hardware.c
	// TODO, make generic, make proper.
#if defined (__32MX270F256D__)
	IPC7bits.USBIP = 4;
#elif defined(__32MX440F256H__)
	IPC11bits.USBIP = 4;
#endif

	// Enable DMA at the end.
	//DMACONbits.ON = 1;
	//asm("nop");

	// Enable interrupts - at the end, otherwise setting priorities is ineffective ? TODO
//	INTEnableSystemMultiVectoredInt();



}


int main(){


	setup();
	cdc_set_interface_list(cdc_interfaces, 4);	// 4 UART interfaces...
	usb_init();

	for(;;){

		// Handle data that is to be sent to the PC
		// Nothing yet

//		COMMS_handleIncomingProg();

//		#ifndef USB_USE_INTERRUPTS
		usb_service();
//		#endif

	}

    return(0);
    
}




/////////////////////////////////////////////////////////////7

#ifdef USB_USE_INTERRUPTS
INTERRUPT(USB1Interrupt){
	usb_service();
	//COMMS_addToInputBuffer();

	// All of the cases depend on this anyway.
	if (!usb_is_configured()){
		return;
	}

	// Priorities!

	// 1. We have one IN (us->PC) endpoint for USB-UART. This one must have high throughput, but can have bigger latency
	// 4. The OUT (PC->us) endpoint for USB-UART has got the short straw, when it happens it happens.

	// 3. We have one IN (us->PC) endpoint for the programmer - low-ish throughput, medium latency?
	// 2. We have one OUT (PC->us) endpoint for the programmer - medium throughput, low latency desired

	// So, uh, we can send MORE than 64B in one 1ms time slot.
	// The trick is to limit the number of packets to <19, or something like that.
	// Let's limit it to 8 packets (8*64 = 512B), which should be quite a lot.
	// -> Scratch that, the USB engine will not do stupid things if we send too late in the frame.
	// Also, let's focus on making this functional, rather then 3Mb/s UARt-> PC fast. 115200 will be plenty, frig it.
	// After everything is said and done, optimize the UART for faster speeds.

	////////////////////////////////////////////////////////////
	// 1. Push data to EP4 IN (UART, us to PC)
	////////////////////////////////////////////////////////////

	// Post data to EP4 IN (USB-UART, us to PC)
//	if (!usb_in_endpoint_halted(EP_UART_NUM) && !usb_in_endpoint_busy(EP_UART_NUM)){
//		if (!COMMS_USB_uartRX_transmitBuf()){
//			packetCounter++;
//		}

		// TODO - add DMA that automatically reads data from UART into a buffer.
		// TODO - then in the ISR copy into circular buffer (if space), and rearm
		// TODO - expand with timeSinceLastModified && dataInBuffer -> abort DMA.
		// TODO - ISR then hadles normal (as above) & abort in the same way = speed.
//	}

	////////////////////////////////////////////////////////////
	// 2. Get data from EP2 OUT (programmer, PC to us)
	////////////////////////////////////////////////////////////
//	if (!usb_out_endpoint_halted(EP_PROG_NUM) && usb_out_endpoint_has_data(EP_PROG_NUM) && !usb_in_endpoint_busy(EP_PROG_NUM)) {
//		if (!COMMS_progOUT_addToBuf()){
//		}
//		packetCounter++; // I think packet is consumed regardless
//	}

	////////////////////////////////////////////////////////////
	// 3. Push data to EP2 IN (programmer, us to PC)
	////////////////////////////////////////////////////////////

	// Post data to EP2 IN (programmer, us to PC)
//	if (!usb_in_endpoint_halted(EP_PROG_NUM) && !usb_in_endpoint_busy(EP_PROG_NUM)){
//		if (!COMMS_USB_progRET_transmitBuf()){
//			packetCounter++;
//		}
//	}

	////////////////////////////////////////////////////////////
	// 4. Get data from EP4 OUT (UART, PC to us)
	////////////////////////////////////////////////////////////

	// Get data from EP4 OUT (USB-UART, PC to us)
//	if (!usb_out_endpoint_halted(EP_UART_NUM) && usb_out_endpoint_has_data(EP_UART_NUM) && !usb_in_endpoint_busy(EP_UART_NUM)) {
//		if (!COMMS_uartTX_addToBuf()){
//		}
//		packetCounter++;
//	}


	////////////////////
	// At this point, also check for data in the uartTX buffer, and if the DMA isn't running.

//	if (!UARTDrv_IsTxDmaRunning() && (COMMS_helper_dataLen(&uartTXstruct) > 0) && UART_STA_bits.TRMT){	// TRMT because sometimes.. it's weird.
//		UARTDrv_RunDmaTx();
//		LED_toggle();
//	}

}
#endif


/* Callbacks. These function names are set in usb_config.h. */
void app_set_configuration_callback(uint8_t configuration)
{

}

uint16_t app_get_device_status_callback()
{
	return 0x0000;
}

void app_endpoint_halt_callback(uint8_t endpoint, bool halted){
	// This is a notification only.  There is no way to reject this request.
}

int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting){
	// The callback should return 0 if the new alternate setting can be set or -1 if
	// it cannot. This callback is completely unnecessary if you only have one
	// alternate setting (alternate setting zero) for each interface.
	return 0;
}

int8_t app_get_interface_callback(uint8_t interface){
	// The application should return the interface's current alternate setting
	return 0;
}

void app_out_transaction_callback(uint8_t endpoint){
	// If we wanted, we can use this function to trigger when the
	// OUT transaction on the endpoint is finished (PC->us), and read
	// data out fast that way. Unnecessary for now.
}

void app_in_transaction_complete_callback(uint8_t endpoint){
	// If we wanted, we can use this function to trigger when the
	// IN transaction on the endpoint is finished (us->PC), and rearm/send
	// data fast that way. Unnecessary for now.
}

int8_t app_unknown_setup_request_callback(const struct setup_packet *setup){
	/* To use the CDC device class, have a handler for unknown setup
	 * requests and call process_cdc_setup_request() (as shown here),
	 * which will check if the setup request is CDC-related, and will
	 * call the CDC application callbacks defined in usb_cdc.h. For
	 * composite devices containing other device classes, make sure
	 * MULTI_CLASS_DEVICE is defined in usb_config.h and call all
	 * appropriate device class setup request functions here.
	 */
	// Since we only use (dual) CDC, cdc_setup_request is enough, it gets called twise.
	return process_cdc_setup_request(setup);
}

int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor){
	// Called when a GET_DESCRIPTOR request is received from
	// the host for a descriptor which is unrecognized by the USB stack.
	return -1;
}

void app_start_of_frame_callback(void){
	// Interrupt at SOF
	packetCounter = 0;
}

void app_usb_reset_callback(void){
	// Possibly reset USB buffer, or setup DMAs or something.
	// Unnecessary for now.
}

/* CDC Callbacks. See usb_cdc.h for documentation. */

int8_t app_send_encapsulated_command(uint8_t interface, uint16_t length){
	// Unsure what to support here.
	return -1;
}

int16_t app_get_encapsulated_response(uint8_t interface,
                                      uint16_t length, const void **report,
                                      usb_ep0_data_stage_callback *callback,
                                      void **context){
	// Unsure what to support here.
	return -1;
}

int8_t app_set_comm_feature_callback(uint8_t interface,
                                     bool idle_setting,
                                     bool data_multiplexed_state)
{
	return -1;
}

int8_t app_clear_comm_feature_callback(uint8_t interface,
                                       bool idle_setting,
                                       bool data_multiplexed_state)
{
	return -1;
}

int8_t app_get_comm_feature_callback(uint8_t interface,
                                     bool *idle_setting,
                                     bool *data_multiplexed_state)
{
	return -1;
}

int8_t app_set_line_coding_callback(uint8_t interface,
                                    const struct cdc_line_coding *coding)
{
	// Check if proper interface (should just enable it on the one that's supported...)
	if (interface != 2){
		return 0;
	}


	// Check if values are in ranges we support
	if (coding->dwDTERate <= 1000000
			&& (coding->bCharFormat == CDC_CHAR_FORMAT_1_STOP_BIT || coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS)
			&& coding->bParityType == CDC_PARITY_NONE
			&& coding->bDataBits == 8){

		if (interface == 0){
			line_coding_1 = *coding;
			UARTDrv_1_Init(&line_coding_1);
		}
		if (interface == 2){
			line_coding_2 = *coding;
			UARTDrv_2_Init(&line_coding_2);
		}
		if (interface == 4){
			line_coding_3 = *coding;
			UARTDrv_3_Init(&line_coding_3);
		}
		if (interface == 5){
			line_coding_4 = *coding;
			UARTDrv_4_Init(&line_coding_4);
		}

	}

	return 0;	// No way to deny. Must return 0, otherwise it will stall.
}

int8_t app_get_line_coding_callback(uint8_t interface,
                                    struct cdc_line_coding *coding)
{
	// Return what sort of line_coding we are using.
	if (interface == 0){
		*coding = line_coding_1;
	}
	else if (interface == 2){
		*coding = line_coding_2;
	}
	else if (interface == 4){
		*coding = line_coding_3;
	}
	else if (interface == 6){
		*coding = line_coding_4;
	}

	return 0;
}

int8_t app_set_control_line_state_callback(uint8_t interface,
                                           bool dtr, bool rts)
{
	// Set DTR and RTS lines, according to demands.
	// Only interface 2 (UART) supports this

	if (interface == 0){
		GPIODrv_setDTR_1(dtr);
	}
	else if (interface == 2){
		GPIODrv_setDTR_2(dtr);
	}
	else if (interface == 4){
		GPIODrv_setDTR_3(dtr);
	}
	else if (interface == 6){
		GPIODrv_setDTR_4(dtr);
	}


	return 0;
}

int8_t app_send_break_callback(uint8_t interface, uint16_t duration)
{
	return 0;
}



