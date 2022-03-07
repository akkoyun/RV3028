/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <RV3028.h>




uint8_t RV3028::Get_Second(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Seconds__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint8_t RV3028::Get_Minute(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Minutes__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint8_t RV3028::Get_Hour(void) {
}
uint8_t RV3028::Get_Week_Day(void) {
}
uint8_t RV3028::Get_Date(void) {
}
uint8_t RV3028::Get_Month(void) {
}
uint8_t RV3028::Get_Year(void) {
}



void RV3028::Disable_Trickle_Charger(void) {

    // Clear Register Bit
    I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_EEprom_Backup__, 5, true);

}


// Define Library Class
RV3028 RTC;

// 1903