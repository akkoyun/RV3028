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

		uint8_t Get_Second(void);
		uint8_t Get_Minute(void);
		uint8_t Get_Hour(void);
		uint8_t Get_Week_Day(void);
		uint8_t Get_Date(void);
		uint8_t Get_Month(void);
		uint8_t Get_Year(void);

	private:

		void Disable_Trickle_Charger(void);
};

extern RV3028 RTC;

#endif /* defined(__RV3028C7__) */
