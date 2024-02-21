/* *******************************************************************************
 *  Copyright (C) 2014-2023 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __RV3028__
#define __RV3028__

	// Include Arduino Library
	#ifndef Arduino_h
		#include <Arduino.h>
	#endif

	// Include I2C Functions Library
	#ifndef __I2C_Functions__
		#include <I2C_Functions.h>
	#endif

	// Include RV3028 Definitions
	#include "Definitions.h"

	// RV3028 Class
	class RV3028 : private I2C_Functions {

		// Public Context
		public:

			// Constructor
			explicit RV3028(const bool _Multiplexer_Enable = false, const uint8_t _Multiplexer_Channel = 0) : I2C_Functions(__I2C_Addr_RV3028C7__, _Multiplexer_Enable, _Multiplexer_Channel) {


			}

			// Library Functions
			bool Begin(void) {

				// Start I2C
				I2C_Functions::Begin();

				// Control for Device
				if (I2C_Functions::Detect()) {

					// Ticke Charger Disable
					this->Disable_Trickle_Charger();

					// Set Clock Out
					this->Clock_Out(true);

					// Set 24h Format
					this->Set_Clock_Type(CLOCK_24H);

					// Clear UNIX Time
					this->UNIX_Time(UNIX_CLEAR);

					// End Function
					return(true);

				} else {

					// End Function
					return(false);

				}

			}

			// BCDtoDEC -- convert binary-coded decimal (BCD) to decimal
			inline uint8_t BCDtoDEC(const uint8_t _Value) {

				// End Function
				return (_Value >> 4) * 10 + (_Value & 0x0F);

			}

			// BCDtoDEC -- convert decimal to binary-coded decimal (BCD)
			inline uint8_t DECtoBCD(const uint8_t _Value) {

				// Calculate and Return
				return ((_Value / 10) << 4) | (_Value % 10);

			}

			// Get Week Day Functions
			uint8_t Get_Week_Day(void) {

				// Read Register
				return BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_WEEKDAY));

			}

			// Calculate Day of Week
			uint8_t Day_of_Week(uint8_t _Day, uint8_t _Month, uint16_t _Year) {

				// Declare Variables
				uint8_t _mTable = 0;
				int _SummedDate = 0;
				int _DoW = 0;
				int _Leap = 0;

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

			// Set Clock Type
			void Set_Clock_Type(const bool _Type) {

				// Control for Type
				if (_Type) {

					// Set Register Bit
					I2C_Functions::Set_Register_Bit(RV3028_REGISTER_CONTROL_2, RV3028_BITMASK_12_24, true);

				} else {

					// Clear Register Bit
					I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_CONTROL_2, RV3028_BITMASK_12_24, true);

				}

			}

			// Disable Trickle Charger
			void Disable_Trickle_Charger(void) {

				// Clear Register Bit
				I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_EEPROM_BACKUP, RV3028_BITMASK_TCE, true);

			}

			// Control for 12h Clock
			bool is_12h_Clock(void) {

				// Read Register Bit
				return(I2C_Functions::Read_Register_Bit(RV3028_REGISTER_CONTROL_2, RV3028_BITMASK_12_24));

			}

			// Clear Interrupt Function
			void Clear_Interrupt(void) {

				// Clear Register
				I2C_Functions::Write_Register(RV3028_REGISTER_STATUS, 0x00, true);

			}

			// Interrupt State Function
			void Interrupt(bool _State) {

				// Control for State
				if (_State) {

					// Set Register Bit
					I2C_Functions::Set_Register_Bit(RV3028_REGISTER_CONTROL_2, RV3028_BITMASK_TIE, true);

				} else {

					// Clear Register Bit
					I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_CONTROL_2, RV3028_BITMASK_TIE, true);

				}

			}

			// Read Timer Interrupt Flag
			bool Read_Timer_Interrupt_Flag(void) {

				// Read Bit
				return(I2C_Functions::Read_Register_Bit(RV3028_REGISTER_STATUS, RV3028_BITMASK_TF));

			}

			// Clear Timer Interrupt Flag
			void Clear_Timer_Interrupt_Flag(void) {

				// Clear Bit
				I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_STATUS, RV3028_BITMASK_TF, true);

			}

			// Get Time Functions
			void Time_Stamp(char * _Time_Stamp = NULL) {

				// Handle TimeStamp
				sprintf(_Time_Stamp, "20%02hhu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu", 
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_YEAR)), 	// Get Year
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_MONTH)), 	// Get Month
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_DAY)), 		// Get Date
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_HOUR)),		// Get Hour
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_MINUTE)), 	// Get Minute
					this->BCDtoDEC(I2C_Functions::Read_Register(RV3028_REGISTER_SECOND))	// Get Second
				);

			}

			// Set RTC Time Functions
			void Set_Time(const uint8_t _Second, const uint8_t _Minute, const uint8_t _Hour, const uint8_t _Date, const uint8_t _Month, const uint8_t _Year) {

				// Calculate Day of Week
				uint8_t _DayofWeek = this->Day_of_Week(_Date, _Month, _Year);

				// Update RTC Time
				I2C_Functions::Write_Register(RV3028_REGISTER_SECOND, this->DECtoBCD(_Second), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_MINUTE, this->DECtoBCD(_Minute), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_HOUR, this->DECtoBCD(_Hour), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_WEEKDAY, this->DECtoBCD(_DayofWeek), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_DAY, this->DECtoBCD(_Date), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_MONTH, this->DECtoBCD(_Month), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_YEAR, this->DECtoBCD(_Year), true);

			}

			// UNIX Time Functions
			uint32_t UNIX_Time(const bool _Method) {

				// Declare UNIX Time Array Variable
				uint8_t _UNIX_Time_Array[4] = {0x00, 0x00, 0x00, 0x00};

				// Control for Method
				if (_Method == UNIX_CLEAR) {

					// Clear UNIX Time
					I2C_Functions::Write_Register(RV3028_REGISTER_UNIX_TIME_0, _UNIX_Time_Array[0], true);
					I2C_Functions::Write_Register(RV3028_REGISTER_UNIX_TIME_1, _UNIX_Time_Array[1], true);
					I2C_Functions::Write_Register(RV3028_REGISTER_UNIX_TIME_2, _UNIX_Time_Array[2], true);
					I2C_Functions::Write_Register(RV3028_REGISTER_UNIX_TIME_3, _UNIX_Time_Array[3], true);

				} else {

					// Read UNIX Time
					I2C_Functions::Read_Multiple_Register(RV3028_REGISTER_UNIX_TIME, _UNIX_Time_Array, 4, true);

				}

				// Calculate UNIX Time
				uint32_t _UNIX_Time = ((uint32_t)_UNIX_Time_Array[3] << 24) | ((uint32_t)_UNIX_Time_Array[2] << 16) | ((uint32_t)_UNIX_Time_Array[1] << 8) | _UNIX_Time_Array[0];

				// End Function
				return(_UNIX_Time);

			}

			// Set Clock Output Functions
			void Clock_Out(bool _State = true) {

				// Read 35h Register
				uint8_t _REGISTER_EEPROM_CLKOUT = I2C_Functions::Read_Register(RV3028_REGISTER_EEPROM_CLKOUT);

				// Control for State
				if (_State) {

					// Set CLKOE Bit
					bitSet(_REGISTER_EEPROM_CLKOUT, RV3028_BITMASK_CLKOE);

					// Set 1Hz Output
					bitSet(_REGISTER_EEPROM_CLKOUT, RV3028_BITMASK_FD0);
					bitClear(_REGISTER_EEPROM_CLKOUT, RV3028_BITMASK_FD1);
					bitSet(_REGISTER_EEPROM_CLKOUT, RV3028_BITMASK_FD2);

				} else {

					// Clear CLKOE Bit
					bitClear(_REGISTER_EEPROM_CLKOUT, RV3028_BITMASK_CLKOE);

				}
			
				// Write 35h Register
				I2C_Functions::Write_Register(RV3028_REGISTER_EEPROM_CLKOUT, _REGISTER_EEPROM_CLKOUT, true);

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
				I2C_Functions::Write_Register(RV3028_REGISTER_TIMER_VALUE_0, (_Value & 0xFF), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_TIMER_VALUE_1, (_Value >> 8), true);

				// Read Control Register
				uint8_t _CONTROL1 = I2C_Functions::Read_Register(RV3028_REGISTER_CONTROL_1);

				// Set Timer Repeat Bit
				if (_Repeat) {bitSet(_CONTROL1, RV3028_BITMASK_TRPT);} else {bitClear(_CONTROL1, RV3028_BITMASK_TRPT);}

				// Set Timer Frequency
				switch (_Frequency) {

					// 4096 Hz
					case HZ_4096: {

						// Clear Bits
						_CONTROL1 &= ~3;

						// End Switch
						break;

					}

					// 64 Hz
					case HZ_64: {

						// Clear Bits
						_CONTROL1 &= ~3;

						// Set Bit
						_CONTROL1 |= 1;

						// End Switch
						break;

					}

					// 1 Hz
					case HZ_1: {

						// Clear Bits
						_CONTROL1 &= ~3;

						// Set Bit
						_CONTROL1 |= 2;

						// End Switch
						break;

					}

					// 1/60 Hz
					case HZ_1_60: {

						// Set Bits
						_CONTROL1 |= 3;

						// End Switch
						break;

					}

				}

				// Set Interrupt
				if (_Interrupt) this->Interrupt(true);

				// Start Timer
				if (_Start) bitSet(_CONTROL1, RV3028_BITMASK_TE); 

				// Write Register
				I2C_Functions::Write_Register(0x0F, _CONTROL1, true);
	
				// Clock Out
				if (_Clock_Output) {

					// Set Bit
					I2C_Functions::Set_Register_Bit(RV3028_REGISTER_CLOCK_INT_MASK, 1, true);

				} else {

					// Clear Bit
					I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_CLOCK_INT_MASK, 1, true);

				}
				
			}
			void Set_Timer(const uint16_t _Value) {

				// Disable Timer
				this->Timer(false);

				// Disable Interrupt
				this->Interrupt(false);

				// Clear Interrupt Flag
				this->Clear_Timer_Interrupt_Flag();

				// Set Timer Value
				I2C_Functions::Write_Register(RV3028_REGISTER_TIMER_VALUE_0, (_Value & 0xFF), true);
				I2C_Functions::Write_Register(RV3028_REGISTER_TIMER_VALUE_1, (_Value >> 8), true);

				// Read Control Register
				uint8_t _CONTROL1 = I2C_Functions::Read_Register(RV3028_REGISTER_CONTROL_1);

				// Disable Timer Repeat Bit
				bitClear(_CONTROL1, RV3028_BITMASK_TRPT);

				// Set Timer Frequency
				_CONTROL1 &= ~3;
				_CONTROL1 |= 2;

				// Set Interrupt
				this->Interrupt(true);

				// Start Timer
				bitSet(_CONTROL1, RV3028_BITMASK_TE); 

				// Write Register
				I2C_Functions::Write_Register(0x0F, _CONTROL1, true);
	
				// Clock Out
				I2C_Functions::Set_Register_Bit(RV3028_REGISTER_CLOCK_INT_MASK, 1, true);
				
			}

			// Enable Timer Functions
			void Timer(bool _Status) {

				// Control for Status
				if (_Status) {

					// Set Bit
					I2C_Functions::Set_Register_Bit(RV3028_REGISTER_CONTROL_1, RV3028_BITMASK_TE, true);

				} else {

					// Clear Bit
					I2C_Functions::Clear_Register_Bit(RV3028_REGISTER_CONTROL_1, RV3028_BITMASK_TE, true);

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

			// Write EEPROM Word Functions
			bool Write_EEPROM_Word(const uint8_t _Address, const uint16_t _Value) {

				// Control For Address Limit
				if (_Address > 0x2A) return(false);

				// Write MSB Register
				this->Write_EEPROM(_Address, (_Value >> 8));

				// Write LSB Register
				this->Write_EEPROM(_Address + 1, (_Value & 0xFF));

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

			// Read EEPROM Word Functions
			uint16_t Read_EEPROM_Word(const uint8_t _Address) {

				// Control For Address Limit
				if (_Address > 0x2A) return(false);

				// Read MSB Register
				uint8_t _MSB = this->Read_EEPROM(_Address);

				// Read LSB Register
				uint8_t _LSB = this->Read_EEPROM(_Address + 1);

				// Return Word
				return((_MSB << 8) | _LSB);

			}

	};

#endif /* defined(__RV3028__) */
