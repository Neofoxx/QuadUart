#include <p32xxxx.h>
#include <inttypes.h>
#include <configBits.h>

const uint32_t __attribute__((section (".SECTION_DEVCFG3"))) temp3 =
//	0x38F00000
	0x38F30000
	| (0b0 << _DEVCFG3_FVBUSONIO_POSITION)	// USBVBUSON controlled by PORT function
	| (0b0 << _DEVCFG3_FUSBIDIO_POSITION)	// USBID controlled by PORT function
	| (0b1 << _DEVCFG3_FCANIO_POSITION)		// Default CAN I/O pins
//	| (0b1 << _DEVCFG3_FETHIO_POSITION)		// Default Ethernet I/O pins
//	| (0b1 << _DEVCFG3_FMIIEN_POSITION)		// MII is enabled
	| (0b111 << _DEVCFG3_FSRSSEL_POSITION)	// TODO check - Assign Interrupt Priority 7 to a shadow set	
	| (0xB5B5 << _DEVCFG3_USERID_POSITION);	// UserID is B5B5

const uint32_t __attribute__((section (".SECTION_DEVCFG2"))) temp2 =
	0xFFF87888
	| (0b000 << _DEVCFG2_FPLLODIV_POSITION)		// PLL output divided by 1 (80MHz/2 = 80Mhz)
	| (0b0 << _DEVCFG2_UPLLEN_POSITION)			// USB PLL Enabled
	| (0b001 << _DEVCFG2_UPLLIDIV_POSITION)		// USB PLL input divided by 2 (8MHz/2 = 4MHz)
	| (0b101 << _DEVCFG2_FPLLMUL_POSITION)		// PLL Multiplier is 20 (4MHz*20 = 80MHz)
	| (0b001 << _DEVCFG2_FPLLIDIV_POSITION);	// PLL input divided by 2 (8MHz/2 = 4MHz)

const uint32_t __attribute__((section (".SECTION_DEVCFG1"))) temp1 =
	0xFF600858
	| (0b0 << _DEVCFG1_FWDTEN_POSITION)		// Watchdog timer disabled, can be enabled in software
	| (0b00000 << _DEVCFG1_WDTPS_POSITION)	// Watchdog timer postscale is 1
	| (0b01 << _DEVCFG1_FCKSM_POSITION)		// Clock switching enabled, Fail-Safe Clock Monitoring DISABLED
	| (0b01 << _DEVCFG1_FPBDIV_POSITION)	// PBCLK is SYSCLK / 2
	| (0b1 << _DEVCFG1_OSCIOFNC_POSITION)	// CLOCK output disabled
	| (0b01 << _DEVCFG1_POSCMOD_POSITION)	// XT oscillator mode
	| (0b0 << _DEVCFG1_IESO_POSITION)		// Internal-External switchover disabled (Two-speed start-up disabled)
	| (0b0 << _DEVCFG1_FSOSCEN_POSITION)	// Disable secondary oscillator
	| (0b011 << _DEVCFG1_FNOSC_POSITION);	// POSC (XT) + PLL selected

const uint32_t __attribute__((section (".SECTION_DEVCFG0"))) temp0 =
	0x6EF00FF4									// Don't forget about that one 0
	| (0b1 << _DEVCFG0_CP_POSITION)				// Code Protection disabled
	| (0b1 << _DEVCFG0_BWP_POSITION)			// Boot Flash is Writeable during code execution
	| (0b1111111111 << _DEVCFG0_PWP_POSITION)	// Memory is NOT write-protected
	| (0b01 << _DEVCFG0_ICESEL_POSITION)		// PGEC2/PGED2 is used
#ifdef DEBUG_BUILD								// Defined with Makefile
	| (0b11<<_DEVCFG0_DEBUG_POSITION);			// Debugger is DISABLED. Apparently the MX1/MX2 family need this disabled, for JTAG to work
												// Note, application will run automatically, might want to add a delay at the beginning.
#else
	| (0b11<<_DEVCFG0_DEBUG_POSITION);			// Debugger is DISABLED (DEBUG bit) - DEBUG NEEDS TO BE DISABLED, IF NO DEBUGGER PRESENT! Otherwise code doesn't run.
#endif




