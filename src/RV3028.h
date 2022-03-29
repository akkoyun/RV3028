/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Linbrary
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __RV3028__
#define __RV3028__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define I2C Functions Library
#ifndef __I2C_Functions__
#include <I2C_Functions.h>
#endif

class RV3028 {

	public:

		// Library Functions
		bool Begin(void);

		// Get Time Functions
		char * Time_Stamp(void);
		uint8_t Get_Second(void);
		uint8_t Get_Minute(void);
		uint8_t Get_Hour(void);
		uint8_t Get_Week_Day(void);
		uint8_t Get_Date(void);
		uint8_t Get_Month(void);
		uint8_t Get_Year(void);

		// Get Time Functions
		void Set_Time(uint8_t _Second, uint8_t _Minute, uint8_t _Hour, uint8_t _Date, uint8_t _Month, uint8_t _Year);

		// UNIX Time Functions
		void Clear_UNIX_Time(void);
		uint32_t Get_UNIX_Time(void);

		// Timer Functions
		void Enable_Timer(void);
		void Disable_Timer(void);
		void Set_Timer(bool _Repeat, uint16_t _Frequency, uint16_t _Value, bool _Interrupt, bool _Start, bool _Clock_Output);

		// Calculate Functions
		uint8_t Day_of_Week(uint8_t _Day, uint8_t _Month, uint16_t _Year);

	private:

		//
		void Set_24h_Clock(void);
		void Set_12h_Clock(void);
		void Disable_Trickle_Charger(void);

		// Control Functions
		bool is_12h_Clock(void);

		// Interrupt Functions
		void Clear_Interrupt(void);
		void Enable_Interrupt(void);
		void Disable_Interrupt(void);
		void Read_Timer_Interrupt_Flag(void);
		void Clear_Timer_Interrupt_Flag(void);

};

extern RV3028 RTC;

#endif /* defined(__RV3028__) */
