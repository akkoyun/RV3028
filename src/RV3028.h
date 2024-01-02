/* *******************************************************************************
 *  Copyright (C) 2014-2023 Mehmet Gunce Akkoyun Can not be copied and/or
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

	// Define RV3028C7 Address
	#ifndef __I2C_Addr_RV3028C7__
		#define __I2C_Addr_RV3028C7__		(uint8_t)0x52
	#endif

	// Include EEPROM Definitions
	#include "EEPROM.h"

	// RV3028 Class
	class RV3028 : private I2C_Functions {

		// Private Context
		private:

			// HDC2010 Sensor Variable Structure.
			struct RV3028_Struct {

				// Multiplexer Structure.
				struct Multiplexer_Struct{

					// Multiplexer Enable Variable (if set true library make multiplexer).
					bool Enable 			= false;

					// Multiplexer Channel Variable (if not defined 0 channel make).
					uint8_t Channel 		= 0;

				} Multiplexer;

			} Device;

			// BCDtoDEC -- convert binary-coded decimal (BCD) to decimal
			uint8_t BCDtoDEC(uint8_t _Value) {

				// End Function
				return ((_Value / 0x10) * 10) + (_Value % 0x10);

			}

			// BCDtoDEC -- convert decimal to binary-coded decimal (BCD)
			uint8_t DECtoBCD(uint8_t _Value) {
				
				// End Function
				return ((_Value / 10) * 0x10) + (_Value % 10);
			
			}

			// Calculate Day of Week
			uint8_t Day_of_Week(uint8_t _Day, uint8_t _Month, uint16_t _Year) {

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

			// Set 24h Clock
			void Set_24h_Clock(void) {

				// Set Register Bit
				I2C_Functions::Clear_Register_Bit(0x10, 1, true);

			}

			// Set 12h Clock
			void Set_12h_Clock(void) {

				// Clear Register Bit
				I2C_Functions::Set_Register_Bit(0x10, 1, true);

			}

			// Disable Trickle Charger
			void Disable_Trickle_Charger(void) {

				// Clear Register Bit
				I2C_Functions::Clear_Register_Bit(0x37, 5, true);

			}

			// Control for 12h Clock
			bool is_12h_Clock(void) {

				// Read Register Bit
				bool _Response = I2C_Functions::Read_Register_Bit(0x10, 1);

				// End Function
				return(_Response);

			}

			// Clear Interrupt Function
			void Clear_Interrupt(void) {

				// Clear Register
				I2C_Functions::Write_Register(0x0E, 0x00, true);

			}

			// Interrupt State Function
			void Interrupt(bool _State) {

				// Control for State
				if (_State) {

					// Set Register Bit
					I2C_Functions::Set_Register_Bit(0x10, 4, true);

				} else {

					// Clear Register Bit
					I2C_Functions::Clear_Register_Bit(0x10, 4, true);

				}

			}

			// Read Timer Interrupt Flag
			bool Read_Timer_Interrupt_Flag(void) {

				// Read Bit
				bool _Response = I2C_Functions::Read_Register_Bit(0x0E, 3);

				// End Function
				return(_Response);

			}

			// Clear Timer Interrupt Flag
			void Clear_Timer_Interrupt_Flag(void) {

				// Clear Bit
				I2C_Functions::Clear_Register_Bit(0x0E, 3, true);

			}

		// Public Context
		public:

			// Declare Time Stamp Variable
			char Time_Stamp[26];

			// Constructor
			RV3028(const bool _Multiplexer_Enable = false, const uint8_t _Multiplexer_Channel = 0) : I2C_Functions(__I2C_Addr_RV3028C7__, _Multiplexer_Enable, _Multiplexer_Channel) {

				// Set Multiplexer Variables
				this->Device.Multiplexer.Enable = _Multiplexer_Enable;
				this->Device.Multiplexer.Channel = _Multiplexer_Channel;

			}

			// Library Functions
			bool Begin(void) {

				// Start I2C
				I2C_Functions::Begin();

				// Control for Device
				if (I2C_Functions::Variables.Device.Detect) {

					// Ticke Charger Disable
					this->Disable_Trickle_Charger();

					// Set Clock Out
					this->Clock_Out(true);

					// Set 24h Format
					if (this->is_12h_Clock()) this->Set_24h_Clock();

					// Clear UNIX Time
					this->Clear_UNIX_Time();

					// End Function
					return(true);

				} else {

					// End Function
					return(false);

				}

			}

			// Get Second Functions
			uint8_t Get_Second(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x00));

				// End Function
				return (_Data);

			}

			// Get Minute Functions
			uint8_t Get_Minute(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x01));

				// End Function
				return (_Data);

			}
			
			// Get Hour Functions
			uint8_t Get_Hour(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x02));

				// End Function
				return (_Data);

			}
			
			// Get Week Day Functions
			uint8_t Get_Week_Day(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x03));

				// End Function
				return (_Data);

			}
			
			// Get Date Functions
			uint8_t Get_Date(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x04));

				// End Function
				return (_Data);

			}
			
			// Get Month Functions
			uint8_t Get_Month(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x05));

				// End Function
				return (_Data);

			}

			// Get Year Functions
			uint8_t Get_Year(void) {

				// Read Register
				uint8_t _Data = this->BCDtoDEC(I2C_Functions::Read_Register(0x06));

				// End Function
				return (_Data);

			}

			// Get Time Functions
			void Update_Time_Stamp(void) {

				// Clear TimeStamp Variable
				memset(this->Time_Stamp, '\0', 26);

				// Handle TimeStamp
				sprintf(this->Time_Stamp, "20%02hhu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu", this->Get_Year(), this->Get_Month(), this->Get_Date(), this->Get_Hour(), this->Get_Minute(), this->Get_Second());

			}

			// Set RTC Time Functions
			void Set_Time(const uint8_t _Second, const uint8_t _Minute, const uint8_t _Hour, const uint8_t _Date, const uint8_t _Month, const uint8_t _Year) {

				// Calculate Day of Week
				uint8_t _DayofWeek = this->Day_of_Week(_Date, _Month, _Year);

				// Update RTC Time
				I2C_Functions::Write_Register(0x00, this->DECtoBCD(_Second), true);
				I2C_Functions::Write_Register(0x01, this->DECtoBCD(_Minute), true);
				I2C_Functions::Write_Register(0x02, this->DECtoBCD(_Hour), true);
				I2C_Functions::Write_Register(0x03, this->DECtoBCD(_DayofWeek), true);
				I2C_Functions::Write_Register(0x04, this->DECtoBCD(_Date), true);
				I2C_Functions::Write_Register(0x05, this->DECtoBCD(_Month), true);
				I2C_Functions::Write_Register(0x06, this->DECtoBCD(_Year), true);

			}

			// Clear UNIX Time Functions
			void Clear_UNIX_Time(void) {

				// Clear UNIX Time
				I2C_Functions::Write_Register(0x1B, 0x00, true);
				I2C_Functions::Write_Register(0x1C, 0x00, true);
				I2C_Functions::Write_Register(0x1D, 0x00, true);
				I2C_Functions::Write_Register(0x1E, 0x00, true);

			}

			// Get UNIX Time Functions
			uint32_t Get_UNIX_Time(void) {

				// Declare UNIX Time Array Variable
				uint8_t _UNIX_Time_Array[4] = {0x00, 0x00, 0x00, 0x00};

				// Read UNIX Time
				I2C_Functions::Read_Multiple_Register(0x1B, _UNIX_Time_Array, 4, true);

				// Calculate UNIX Time
				uint32_t _UNIX_Time = ((uint32_t)_UNIX_Time_Array[3] << 24) | ((uint32_t)_UNIX_Time_Array[2] << 16) | ((uint32_t)_UNIX_Time_Array[1] << 8) | _UNIX_Time_Array[0];

				// End Function
				return(_UNIX_Time);

			}

			// Set Clock Output Functions
			void Clock_Out(bool _State = true) {

				// Read 35h Register
				uint8_t _Register_35h = I2C_Functions::Read_Register(0x35);

				// Control for State
				if (_State) {

					// Set CLKOE Bit
					bitSet(_Register_35h, 7);

					// Set 1Hz Output
					bitSet(_Register_35h, 2);
					bitSet(_Register_35h, 0);
					bitClear(_Register_35h, 1);

				} else {

					// Clear CLKOE Bit
					bitClear(_Register_35h, 7);

				}
			
				// Write 35h Register
				I2C_Functions::Write_Register(0x35, _Register_35h, true);

			}

			// Set Timer Functions
			void Set_Timer(const bool _Repeat, const uint16_t _Frequency, const uint16_t _Value, const bool _Interrupt, const bool _Start, const bool _Clock_Output) {

				// Disable Timer
				this->Timer(false);

				// Disable Interrupt
				this->Interrupt(false);

				// Clear Interrupt Flag
				this->Clear_Timer_Interrupt_Flag();

				// Set Timer Value
				I2C_Functions::Write_Register(0x0A, (_Value & 0xFF), true);
				I2C_Functions::Write_Register(0x0B, (_Value >> 8), true);

				// Read Control Register
				uint8_t _CONTROL1 = I2C_Functions::Read_Register(0x0F);

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
				if (_Interrupt) this->Interrupt(true);

				// Start Timer
				if (_Start) _CONTROL1 |= (1 << 2);

				// Write Register
				I2C_Functions::Write_Register(0x0F, _CONTROL1, true);
	
				// Clock Out
				if (_Clock_Output) {

					// Set Bit
					I2C_Functions::Set_Register_Bit(0x12, 1, true);

				} else {

					// Clear Bit
					I2C_Functions::Clear_Register_Bit(0x12, 1, true);

				}
				
			}

			// Enable Timer Functions
			void Timer(bool _Status) {

				// Control for Status
				if (_Status) {

					// Set Bit
					I2C_Functions::Set_Register_Bit(0x0F, 2, true);

				} else {

					// Clear Bit
					I2C_Functions::Clear_Register_Bit(0x0F, 2, true);

				}

			}

			// Write EEPROM Functions
			bool Write_EEPROM(const uint8_t _Address, const uint8_t _Value) {

				// Control For Address Limit
				if (_Address > 0x2A) return(false);

				// Wait Until Bussy
				while (I2C_Functions::Read_Register(0x0E) & 0x80);

				// Read Temp Register
				uint8_t _Temp = I2C_Functions::Read_Register(0x08);

				// Write Temp Register
				I2C_Functions::Write_Register(0x0F, _Temp | 0x08, true);

				// Write Register
				I2C_Functions::Write_Register(0x25, _Address, true);
				I2C_Functions::Write_Register(0x26, _Value, true);

				// Update Eprom
				I2C_Functions::Write_Register(0x27, 0x00, true);
				Write_Register(0x27, 0x21, true);
				
				// Wait Until Bussy
				while (I2C_Functions::Read_Register(0x0E) & 0x80);

				// Write Temp Register
				I2C_Functions::Write_Register(0x0F, _Temp, true);

				// End Function
				return(true);

			}

			// Read EEPROM Functions
			uint8_t Read_EEPROM(const uint8_t _Address) {

				// Control For Address Limit
				if (_Address > 0x2A) return(false);

				// Wait Until Bussy
				while (I2C_Functions::Read_Register(0x0E) & 0x80);

				// Read Temp Register
				uint8_t _Temp = I2C_Functions::Read_Register(0x08);

				// Write Temp Register
				I2C_Functions::Write_Register(0x0F, _Temp | 0x08, true);

				// Write Address
				I2C_Functions::Write_Register(0x25, _Address, true);

				// Update Eprom
				I2C_Functions::Write_Register(0x27, 0x00, true);
				I2C_Functions::Write_Register(0x27, 0x22, true);

				// Wait Until Bussy
				while (I2C_Functions::Read_Register(0x0E) & 0x80);

				// Read Register
				uint8_t _Register = I2C_Functions::Read_Register(0x26);

				// Write Temp Register
				I2C_Functions::Write_Register(0x0F, _Temp, true);

				// End Function
				return(_Register);

			}


			// Read PUBLISH Register from EEPROM Function
			uint32_t Read_Publish_Register(void) {
				
				// Declare Variables
				uint32_t _Publish_Register = 0x00000000;

				// Read EEPROM
				uint8_t _Publish_Register_MSB_2 = this->Read_EEPROM(__EEPROM_PUBLISH_MASK_MSB_2__);
				uint8_t _Publish_Register_MSB_1 = this->Read_EEPROM(__EEPROM_PUBLISH_MASK_MSB_1__);
				uint8_t _Publish_Register_LSB_2 = this->Read_EEPROM(__EEPROM_PUBLISH_MASK_LSB_2__);
				uint8_t _Publish_Register_LSB_1 = this->Read_EEPROM(__EEPROM_PUBLISH_MASK_LSB_1__);

				// Control for EEPROM
				if (_Publish_Register_MSB_2 == 0x00 and _Publish_Register_MSB_1 == 0x00 and _Publish_Register_LSB_2 == 0x00 and _Publish_Register_LSB_1 == 0x00) {

					// Define Default Values
					uint8_t _Publish_Register_Default_MSB_2 = ((__PUBLISH_REGISTER_DEFAULT__ >> 24) & 0xFF);
					uint8_t _Publish_Register_Default_MSB_1 = ((__PUBLISH_REGISTER_DEFAULT__ >> 16) & 0xFF);
					uint8_t _Publish_Register_Default_LSB_2 = ((__PUBLISH_REGISTER_DEFAULT__ >> 8) & 0xFF);
					uint8_t _Publish_Register_Default_LSB_1 = ((__PUBLISH_REGISTER_DEFAULT__ >> 0) & 0xFF);

					// Set Default Value
					this->Write_EEPROM(__EEPROM_PUBLISH_MASK_MSB_2__, _Publish_Register_Default_MSB_2);
					this->Write_EEPROM(__EEPROM_PUBLISH_MASK_MSB_1__, _Publish_Register_Default_MSB_1);
					this->Write_EEPROM(__EEPROM_PUBLISH_MASK_LSB_2__, _Publish_Register_Default_LSB_2);
					this->Write_EEPROM(__EEPROM_PUBLISH_MASK_LSB_1__, _Publish_Register_Default_LSB_1);

					// Set Default Value
					_Publish_Register = __PUBLISH_REGISTER_DEFAULT__;

				} else {

					// Set Default Value
					_Publish_Register = (((uint32_t)_Publish_Register_MSB_2 << 24) | ((uint32_t)_Publish_Register_MSB_1 << 16) | ((uint32_t)_Publish_Register_LSB_2 << 8) | (uint32_t)_Publish_Register_LSB_1);

				}

				// Return Value
				return _Publish_Register;

			}

			// Read STOP Register from EEPROM Function
			uint32_t Read_Stop_Register(void) {
				
				// Declare Variables
				uint32_t _Stop_Register = 0x00000000;

				// Read EEPROM
				uint8_t _Stop_Register_MSB_2 = this->Read_EEPROM(__EEPROM_STOP_MASK_MSB_2__);
				uint8_t _Stop_Register_MSB_1 = this->Read_EEPROM(__EEPROM_STOP_MASK_MSB_1__);
				uint8_t _Stop_Register_LSB_2 = this->Read_EEPROM(__EEPROM_STOP_MASK_LSB_2__);
				uint8_t _Stop_Register_LSB_1 = this->Read_EEPROM(__EEPROM_STOP_MASK_LSB_1__);

				// Control for EEPROM
				if (_Stop_Register_MSB_2 == 0x00 and _Stop_Register_MSB_1 == 0x00 and _Stop_Register_LSB_2 == 0x00 and _Stop_Register_LSB_1 == 0x00) {

					// Define Default Values
					uint8_t _Stop_Register_Default_MSB_2 = ((__STOP_REGISTER_DEFAULT__ >> 24) & 0xFF);
					uint8_t _Stop_Register_Default_MSB_1 = ((__STOP_REGISTER_DEFAULT__ >> 16) & 0xFF);
					uint8_t _Stop_Register_Default_LSB_2 = ((__STOP_REGISTER_DEFAULT__ >> 8) & 0xFF);
					uint8_t _Stop_Register_Default_LSB_1 = ((__STOP_REGISTER_DEFAULT__ >> 0) & 0xFF);

					// Set Default Value
					this->Write_EEPROM(__EEPROM_STOP_MASK_MSB_2__, _Stop_Register_Default_MSB_2);
					this->Write_EEPROM(__EEPROM_STOP_MASK_MSB_1__, _Stop_Register_Default_MSB_1);
					this->Write_EEPROM(__EEPROM_STOP_MASK_LSB_2__, _Stop_Register_Default_LSB_2);
					this->Write_EEPROM(__EEPROM_STOP_MASK_LSB_1__, _Stop_Register_Default_LSB_1);

					// Set Default Value
					_Stop_Register = __PUBLISH_REGISTER_DEFAULT__;

				} else {

					// Set Default Value
					_Stop_Register = (((uint32_t)_Stop_Register_MSB_2 << 24) | ((uint32_t)_Stop_Register_MSB_1 << 16) | ((uint32_t)_Stop_Register_LSB_2 << 8) | (uint32_t)_Stop_Register_LSB_1);

				}

				// Return Value
				return _Stop_Register;

			}


	};

#endif /* defined(__RV3028__) */
