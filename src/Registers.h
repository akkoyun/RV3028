#ifndef __RV3028_Registers__
#define __RV3028_Registers__

	// RV3028 Register Addresses
	#define RV3028_REGISTER_SECOND			0x00
	#define RV3028_REGISTER_MINUTE			0x01
	#define RV3028_REGISTER_HOUR			0x02
	#define RV3028_REGISTER_WEEKDAY			0x03
	#define RV3028_REGISTER_DAY				0x04
	#define RV3028_REGISTER_MONTH			0x05
	#define RV3028_REGISTER_YEAR			0x06

	// RV3028 UNIX Time Registers
	#define RV3028_REGISTER_UNIX_TIME		0x1B
	#define RV3028_REGISTER_UNIX_TIME_0		0x1B
	#define RV3028_REGISTER_UNIX_TIME_1		0x1C
	#define RV3028_REGISTER_UNIX_TIME_2		0x1D
	#define RV3028_REGISTER_UNIX_TIME_3		0x1E

	// RV3028 Configuration Registers
	#define RV3028_REGISTER_EEPROM_CLKOUT	0x35
	#define RV3028_REGISTER_EEPROM_BACKUP	0x37

	// RV3028 Register Bit EEPROM CLKOUT
	#define RV3028_BITMASK_CLKOE			0x07
	#define RV3028_BITMASK_CLKSY			0x06
	#define RV3028_BITMASK_PORIE			0x03
	#define RV3028_BITMASK_FD2				0x02
	#define RV3028_BITMASK_FD1				0x01
	#define RV3028_BITMASK_FD0				0x00

	// RV3028 Register Bit EEPROM BACKUP
	#define RV3028_BITMASK_EEOFFSET_0		0x07
	#define RV3028_BITMASK_BSIE				0x06
	#define RV3028_BITMASK_TCE				0x05
	#define RV3028_BITMASK_FEDE				0x04
	#define RV3028_BITMASK_BSM_1			0x03
	#define RV3028_BITMASK_BSM_0			0x02
	#define RV3028_BITMASK_TCR_1			0x01
	#define RV3028_BITMASK_TCR_0			0x00

	// RV3028 Control Register
	#define RV3028_REGISTER_STATUS			0x0E
	#define RV3028_REGISTER_CONTROL_1		0x0F
	#define RV3028_REGISTER_CONTROL_2		0x10

	// RV3028 Register Bit Status
	#define RV3028_BITMASK_EEBUSY			0x07
	#define RV3028_BITMASK_CLKF				0x06
	#define RV3028_BITMASK_BSF				0x05
	#define RV3028_BITMASK_UF				0x04
	#define RV3028_BITMASK_TF				0x03
	#define RV3028_BITMASK_AF				0x02
	#define RV3028_BITMASK_EVF				0x01
	#define RV3028_BITMASK_PORF				0x00

	// RV3028 Register Bit Control 1
	#define RV3028_BITMASK_TRPT				0x07
	#define RV3028_BITMASK_WADA				0x05
	#define RV3028_BITMASK_USEL				0x04
	#define RV3028_BITMASK_EERD				0x03
	#define RV3028_BITMASK_TE				0x02
	#define RV3028_BITMASK_TD1				0x01
	#define RV3028_BITMASK_TD0				0x00

	// RV3028 Register Bit Control 2
	#define RV3028_BITMASK_TSE				0x07
	#define RV3028_BITMASK_CLKIE			0x06
	#define RV3028_BITMASK_UIE				0x05
	#define RV3028_BITMASK_TIE				0x04
	#define RV3028_BITMASK_AIE				0x03
	#define RV3028_BITMASK_EIE				0x02
	#define RV3028_BITMASK_12_24			0x01
	#define RV3028_BITMASK_RESET			0x00

	// RV3028 Timer Registers
	#define RV3028_REGISTER_TIMER_VALUE_0	0x0A
	#define RV3028_REGISTER_TIMER_VALUE_1	0x0B
	#define RV3028_REGISTER_CLOCK_INT_MASK	0x12


#endif /* defined(__RV3028_Registers__) */
