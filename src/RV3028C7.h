/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Linbrary
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __RV3028C7__
#define __RV3028C7__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define I2C Functions Library
#ifndef __I2C_Functions__
#include <I2C_Functions.h>
#endif

class RV3028C7 {

	public:

		bool Function(void);

	private:
	
};

extern RV3028C7 RTC;

#endif /* defined(__RV3028C7__) */
