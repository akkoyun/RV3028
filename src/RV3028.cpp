/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include "RV3028.h"

// Library Functions
void RV3028::Begin(uint8_t _Mux_Channel) {

	// Multiplexer Set
	if (_Mux_Channel != 0) {
		I2C.Set_Multiplexer(I2C.TCA9548.I2C_Address, _Mux_Channel);
		_I2C_Multiplexer_Channel = _Mux_Channel;
	}

	// Ticke Charger Disable
	Disable_Trickle_Charger();

	// Set 24h Format
	if (is_12h_Clock()) Set_24h_Clock();

}

// Get Time Variables
String RV3028::Time_Stamp(void) {

	// Multiplexer Set
	if (_I2C_Multiplexer_Channel != 0) I2C.Set_Multiplexer(I2C.TCA9548.I2C_Address, _I2C_Multiplexer_Channel);

	// Declare Timestamp Variable
	char _Time_Stamp[25];	// dd-mm-yyyy hh.mm.ss

	// Handle TimeStamp
	sprintf(_Time_Stamp, "%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", Get_Date(), Get_Month(), Get_Year(), Get_Hour(), Get_Minute(), Get_Second());

	// End Function
	return(_Time_Stamp);

}
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

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Hours__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint8_t RV3028::Get_Week_Day(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Week_Day__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint8_t RV3028::Get_Date(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Date__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint8_t RV3028::Get_Month(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Month__);

	// End Function
	return (I2C.BCDtoDEC(_Raw_Data));

}
uint16_t RV3028::Get_Year(void) {

	// Read Register
	uint8_t _Raw_Data = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Year__);

	// End Function
	return (2000 + I2C.BCDtoDEC(_Raw_Data));

}

// Set Time Variables
void RV3028::Set_Time(uint8_t _Second, uint8_t _Minute, uint8_t _Hour, uint8_t _Date, uint8_t _Month, uint8_t _Year) {

	// Set Year
	Set_Year(_Year);

	// Set Month
	Set_Month(_Month);

	// Set Day
	Set_Date(_Date);

	// Calculate Day of Week
	uint8_t _DayofWeek = Day_of_Week(_Date, _Month, _Year);

	// Set Day of Week
	Set_Week_Day(_DayofWeek);

	// Set Hour
	Set_Hour(_Hour);

	// Set Minute
	Set_Minute(_Minute);

	// Set Second
	Set_Second(_Second);

}
void RV3028::Set_Second(uint8_t _Second) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Seconds__, I2C.DECtoBCD(_Second), true);

}
void RV3028::Set_Minute(uint8_t _Minute) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Minutes__, I2C.DECtoBCD(_Minute), true);

}
void RV3028::Set_Hour(uint8_t _Hour) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Hours__, I2C.DECtoBCD(_Hour), true);

}
void RV3028::Set_Week_Day(uint8_t _Week_Day) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Week_Day__, I2C.DECtoBCD(_Week_Day), true);

}
void RV3028::Set_Date(uint8_t _Date) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Date__, I2C.DECtoBCD(_Date), true);

}
void RV3028::Set_Month(uint8_t _Month) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Month__, I2C.DECtoBCD(_Month), true);

}
void RV3028::Set_Year(uint8_t _Year) {

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Year__, I2C.DECtoBCD(_Year), true);

}

// UNIX Time Functions
void RV3028::Clear_UNIX_Time(void) {

	// Write Variable
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_UNIX_Time0__, 0x00, true);
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_UNIX_Time1__, 0x00, true);
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_UNIX_Time2__, 0x00, true);
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_UNIX_Time3__, 0x00, true);

}
uint32_t RV3028::Get_UNIX_Time(void) {
	
	// Declare Variable
	uint8_t _Raw_Array[4];

	// Read Variable
	I2C.Read_Multiple_Register(I2C.RV3028C7.I2C_Address, __RV3028_UNIX_Time0__, _Raw_Array, 4, true);

	// Calculate Time
	uint32_t _UNIX_Time = ((uint32_t)_Raw_Array[3] << 24) | ((uint32_t)_Raw_Array[2] << 16) | ((uint32_t)_Raw_Array[1] << 8) | _Raw_Array[0];

	// End Function
	return(_UNIX_Time);

}

// Misc Functions
void RV3028::Disable_Trickle_Charger(void) {

    // Clear Register Bit
    I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_EEprom_Backup__, 5, true);

}
void RV3028::Set_24h_Clock(void) {

    // Clear Register Bit
    I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Control2__, 1, true);

}
void RV3028::Set_12h_Clock(void) {

    // Clear Register Bit
	I2C.Set_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Control2__, 1, true);

}

// Calculate Functions
uint8_t RV3028::Day_of_Week(uint8_t _Day, uint8_t _Month, uint16_t _Year) {

	// Declare Variables
	int _mTable;
	int _SummedDate;
	int _DoW;
	int _Leap;

	// Leap Year Calculation
	if((fmod(_Year,4) == 0 && fmod(_Year,100) != 0) || (fmod(_Year,400) == 0)) {_Leap = 1;} else {_Leap = 0;}

    // Limit results to year 1900-2299 (to save memory)
    while(_Year > 2299) {_Year = _Year - 400;}
    while(_Year < 1900) {_Year = _Year + 400;}

    // Calculating century
    int _Century = _Year / 100;
    int _Century_Table;

    // Calculating two digit year
    uint8_t _Year_Short = fmod(_Year, 100);

    // Century value based on Table
    if(_Century == 19) _Century_Table = 1;
    if(_Century == 20) _Century_Table = 0;
    if(_Century == 21) _Century_Table = 5;
    if(_Century == 22) _Century_Table = 3;

    // Jan and Feb calculations affected by leap years
    if(_Month == 1) { if(_Leap == 1) { _mTable = 6;} else { _mTable = 0; }}
    if(_Month == 2) { if(_Leap == 1) { _mTable = 2;} else { _mTable = 3; }}

    // Other months not affected and have set values
    if(_Month == 10) _mTable = 0;
    if(_Month == 8) _mTable = 2;
    if(_Month == 3 || _Month == 11) _mTable = 3;
    if(_Month == 4 || _Month == 7) _mTable = 6;
    if(_Month == 5) _mTable = 1;
    if(_Month == 6) _mTable = 4;
    if(_Month == 9 || _Month == 12) _mTable = 5;

    // Enter the data into the formula
    _SummedDate = _Day + _mTable + _Year_Short + (_Year_Short / 4) + _Century_Table;
    
    // Find remainder
    _DoW = fmod(_SummedDate,7);  

	// End Function
	return(_DoW);

}

// Control Functions
bool RV3028::is_12h_Clock(void) {

	// Read Register Bit
	bool _Response = I2C.Read_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Control2__, 1);

	// End Function
	return(_Response);

}

// Interrupt Functions
void RV3028::Clear_Interrupt(void) {

	// Reset Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Status__, 0x00, true);

}
void RV3028::Enable_Interrupt(void) {

	// Reset Register
	I2C.Set_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Status__, 4, true);

}
void RV3028::Disable_Interrupt(void) {

	// Reset Register
	I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Status__, 4, true);

}
void RV3028::Read_Timer_Interrupt_Flag(void) {

	// Read Bit
	I2C.Read_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Status__, 3);

}
void RV3028::Clear_Timer_Interrupt_Flag(void) {

	// Clear Bit
	I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Status__, 3, true);

}

// Timer Functions
void RV3028::Enable_Timer(void) {

	// Clear Bit
	I2C.Set_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Control1__, 2, true);

}
void RV3028::Disable_Timer(void) {

	// Clear Bit
	I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_Control1__, 2, true);

}
void RV3028::Set_Timer(bool _Repeat, uint16_t _Frequency, uint16_t _Value, bool _Interrupt, bool _Start, bool _Clock_Output) {

	// Multiplexer Set
	if (_I2C_Multiplexer_Channel != 0) I2C.Set_Multiplexer(I2C.TCA9548.I2C_Address, _I2C_Multiplexer_Channel);

	// Disable Timer
	Disable_Timer();

	// Disable Interrupt
	Disable_Interrupt();

	// Clear Flag
	Clear_Timer_Interrupt_Flag();

	// Set Timer Value
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Timer_Value0__, (_Value & 0xFF), true);
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Timer_Value1__, (_Value >> 8), true);

	// Read Control Register
	uint8_t _CONTROL1 = I2C.Read_Register(I2C.RV3028C7.I2C_Address, __RV3028_Control1__);

	// Set Timer Repeat Bit
	if (_Repeat) {_CONTROL1 |= 1 << 7;} else {_CONTROL1 &= ~(1 << 7);}

	// Set Timer Frequency
	switch (_Frequency) {
	
		case 4096:
			_CONTROL1 &= ~3;
			break;

		case 64:
			_CONTROL1 &= ~3;
			_CONTROL1 |= 1;
			break;

		case 1:
			_CONTROL1 &= ~3;
			_CONTROL1 |= 2;
			break;

		case 60000:
			_CONTROL1 |= 3;
			break;

	}

	// Set Interrupt
	if (_Interrupt) Enable_Interrupt();

	// Start Timer
	if (_Start) _CONTROL1 |= (1 << 2);

	// Write Register
	I2C.Write_Register(I2C.RV3028C7.I2C_Address, __RV3028_Control1__, _CONTROL1, true);

	// Clock Out
	if (_Clock_Output) I2C.Set_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_INT_Mask__, 1, true);
	if (_Clock_Output) I2C.Clear_Register_Bit(I2C.RV3028C7.I2C_Address, __RV3028_INT_Mask__, 1, true);

}

// Define Library Class
RV3028 RTC;

// 1903