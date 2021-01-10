#include <p32xxxx.h>
#include <inttypes.h>
#include <stdbool.h>
#include <builtin.h>

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
//#if defined (__32MX270F256D__)
//	IPC7bits.USBIP = 4;
//#elif defined(__32MX440F256H__)
//	IPC11bits.USBIP = 4;
//#endif

	// Enable USB interrupt
	IPC11bits.USBIP = 4;
	IPC11bits.USBIS = 0;
	// IEC bit is set in usb_hal.h


	// Enable DMA at the end.
	//DMACONbits.ON = 1;
	//asm("nop");

	// Enable interrupts - at the end, otherwise setting priorities is ineffective ? TODO

	// set the CP0 cause IV bit high
	// -> This tells the interrupt controller to use interrupts at ebase+0x200,
	// not the general exception at 0x180
	_CP0_BIS_CAUSE(_CP0_CAUSE_IV_MASK);

	// Enable multi-vectored mode
	INTCONSET = _INTCON_MVEC_MASK;

	// set the CP0 status IE bit high to turn on interrupts
	//INTEnableInterrupts();
	asm("ei");	// Compiler handles this, but should be same as _CP0_BIS_STATUS(_CP0_STATUS_IE_MASK)

}


int main(){

	uint32_t lastTime = _CP0_GET_COUNT();
	uint32_t currTime = lastTime;

	setup();
	cdc_set_interface_list(cdc_interfaces, 4);	// 4 UART interfaces...
	usb_init();

	for(;;){

		// Handle data that is to be sent to the PC
		// Nothing yet

		#ifndef USB_USE_INTERRUPTS
		usb_service();
		#endif


		currTime = _CP0_GET_COUNT();
		// Once per ms
		if ((currTime - lastTime) > ( (80000000 / 2) / 1000)){
			lastTime = currTime;
			// Fire ALL the RX interrupts, to get data that doesn't trigger the RX interrupt
			// Note, we will get interrupted by each interrupt
			if(UART_1_STA_bits.URXDA){
				UART_1_INT_IFS_bits.UART_1_INT_IFS_RXIF = 1;
			}
			if(UART_2_STA_bits.URXDA){
				UART_2_INT_IFS_bits.UART_2_INT_IFS_RXIF = 1;
			}
			if(UART_3_STA_bits.URXDA){
				UART_3_INT_IFS_bits.UART_3_INT_IFS_RXIF = 1;
			}
			if(UART_4_STA_bits.URXDA){
				UART_4_INT_IFS_bits.UART_4_INT_IFS_RXIF = 1;
			}

		}

	}

    return(0);
    
}




/////////////////////////////////////////////////////////////7

#ifdef USB_USE_INTERRUPTS
INTERRUPT(USB1Interrupt){
	usb_service();

	// All of the cases depend on this anyway.
	if (!usb_is_configured()){
		return;
	}

	// I forget, does this happen at 1ms, or at 125us?
	// 1ms is normal time slice, but we can send many more packets than just 1/ms... TODO check

	// Highest priority should be on Target->PC transmissions, since we can only buffer so much
	// For PC->Target, the USB stack auto-buffers, so there will be no dropouts

	// Total of 4 IN endpoints (Target->PC), and 4 OUT endpoints (PC->TARGET)

	// IN endpoints
	if (!usb_in_endpoint_halted(EP_UART_1) && !usb_in_endpoint_busy(EP_UART_1)){
		COMMS_USB_sendToPC(&comStruct_UART_1_RX, EP_UART_1, EP_UART_1_LEN);
	}
	if (!usb_in_endpoint_halted(EP_UART_2) && !usb_in_endpoint_busy(EP_UART_2)){
		COMMS_USB_sendToPC(&comStruct_UART_2_RX, EP_UART_2, EP_UART_2_LEN);
	}
	if (!usb_in_endpoint_halted(EP_UART_3) && !usb_in_endpoint_busy(EP_UART_3)){
		COMMS_USB_sendToPC(&comStruct_UART_3_RX, EP_UART_3, EP_UART_3_LEN);
	}
	if (!usb_in_endpoint_halted(EP_UART_4) && !usb_in_endpoint_busy(EP_UART_4)){
		COMMS_USB_sendToPC(&comStruct_UART_4_RX, EP_UART_4, EP_UART_4_LEN);
	}

	// OUT endpoints
	// When we get data, if TX interrupt isn't running, we reenable it. Note, that will interurpt us here.
	if (!usb_out_endpoint_halted(EP_UART_1) && usb_out_endpoint_has_data(EP_UART_1) && !usb_in_endpoint_busy(EP_UART_1)) {
		if(!COMMS_USB_recvFromPC(&comStruct_UART_1_TX, EP_UART_1)){
			if (!UART_1_INT_IEC_bits.UART_1_INT_IEC_TXIE){
				UART_1_INT_IEC_bits.UART_1_INT_IEC_TXIE = 1;
			}
		}
	}
	if (!usb_out_endpoint_halted(EP_UART_2) && usb_out_endpoint_has_data(EP_UART_2) && !usb_in_endpoint_busy(EP_UART_2)) {
		if(!COMMS_USB_recvFromPC(&comStruct_UART_2_TX, EP_UART_2)){
			if (!UART_2_INT_IEC_bits.UART_2_INT_IEC_TXIE){
				UART_2_INT_IEC_bits.UART_2_INT_IEC_TXIE = 1;
			}
		}
	}
	if (!usb_out_endpoint_halted(EP_UART_3) && usb_out_endpoint_has_data(EP_UART_3) && !usb_in_endpoint_busy(EP_UART_3)) {
		if(!COMMS_USB_recvFromPC(&comStruct_UART_3_TX, EP_UART_3)){
			if (!UART_3_INT_IEC_bits.UART_3_INT_IEC_TXIE){
				UART_3_INT_IEC_bits.UART_3_INT_IEC_TXIE = 1;
			}
		}
	}
	if (!usb_out_endpoint_halted(EP_UART_4) && usb_out_endpoint_has_data(EP_UART_4) && !usb_in_endpoint_busy(EP_UART_4)) {
		if(!COMMS_USB_recvFromPC(&comStruct_UART_4_TX, EP_UART_4)){
			if (!UART_4_INT_IEC_bits.UART_4_INT_IEC_TXIE){
				UART_4_INT_IEC_bits.UART_4_INT_IEC_TXIE = 1;
			}
		}
	}

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
		if (interface == 6){
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



