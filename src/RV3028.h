/* *******************************************************************************
 *  Copyright (C) 2014-2025 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: RV3028C7 RTC Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#pragma once

// Include Arduino Library
#ifndef Arduino_h
	#include <Arduino.h>
#endif

// Include I2C Functions Library
#ifndef __I2C_Functions__
	#include <I2C_Functions.h>
#endif

// Include RV3028 Definitions (pulls in Registers.h)
#include "Definitions.h"

// =========================================================================
// RV3028 Class
// =========================================================================
class RV3028 : private I2C_Functions {

	public:

		// -------------------------------------------------------------------------
		// Constructor
		// -------------------------------------------------------------------------
		explicit RV3028(const bool _Mux_Enable = false, const uint8_t _Mux_Channel = 0)
			: I2C_Functions(RV3028_Cfg::I2C_ADDRESS, _Mux_Enable, _Mux_Channel) {
			// No internal timestamp buffer to keep instance RAM usage minimal.
		}

		// -------------------------------------------------------------------------
		// Initialization
		// -------------------------------------------------------------------------

		// Initialize I2C.
		// Returns true if the device is found on the bus.
		// Optional default config applies 24h mode, disables trickle charger and enables 1 Hz CLKOUT.
		bool Begin(const bool _Apply_Default_Config = false) {
			I2C_Functions::Begin();
			if (!this->Detect()) return false;
			if (_Apply_Default_Config) {
				Disable_Trickle_Charger();
				Clock_Out(true);
				Set_Clock_Type(false);
			}
			return true;
		}

		// -------------------------------------------------------------------------
		// Clock Configuration
		// -------------------------------------------------------------------------

		// Set 12h (true) or 24h (false) mode.
		void Set_Clock_Type(const bool _Type) {
			if (_Type) {
				I2C_Functions::Set_Register_Bit(RV3028_Reg::CONTROL_2, RV3028_Bit::HOUR_FORMAT, true);
			} else {
				I2C_Functions::Clear_Register_Bit(RV3028_Reg::CONTROL_2, RV3028_Bit::HOUR_FORMAT, true);
			}
		}

		bool is_12h_Clock(void) {
			return I2C_Functions::Read_Register_Bit(RV3028_Reg::CONTROL_2, RV3028_Bit::HOUR_FORMAT);
		}

		// Disable trickle charger (writes EEPROM RAM mirror — volatile until next power cycle).
		void Disable_Trickle_Charger(void) {
			I2C_Functions::Clear_Register_Bit(RV3028_Reg::EEPROM_BACKUP, RV3028_Bit::TCE, true);
		}

		// Enable or disable CLKOUT at 1 Hz (FD[2:0]=101, datasheet Table 23).
		// Writes EEPROM RAM mirror — active immediately, volatile across power cycles.
		void Clock_Out(const bool _State = true) {
			uint8_t _reg = I2C_Functions::Read_Register(RV3028_Reg::EEPROM_CLKOUT);
			if (_State) {
				bitSet(_reg,   RV3028_Bit::CLKOE);
				bitSet(_reg,   RV3028_Bit::FD2);
				bitClear(_reg, RV3028_Bit::FD1);
				bitSet(_reg,   RV3028_Bit::FD0);
			} else {
				bitClear(_reg, RV3028_Bit::CLKOE);
			}
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_CLKOUT, _reg, true);
		}

		// -------------------------------------------------------------------------
		// Interrupt Control
		// -------------------------------------------------------------------------

		// Clear all status flags (AF, TF, UF, EVF, BSF, PORF).
		void Clear_Interrupt(void) {
			I2C_Functions::Write_Register(RV3028_Reg::STATUS, 0x00, true);
		}

		// Enable or disable the timer interrupt on the INT pin.
		void Interrupt(const bool _State) {
			if (_State) {
				I2C_Functions::Set_Register_Bit(RV3028_Reg::CONTROL_2, RV3028_Bit::TIE, true);
			} else {
				I2C_Functions::Clear_Register_Bit(RV3028_Reg::CONTROL_2, RV3028_Bit::TIE, true);
			}
		}

		bool Read_Timer_Interrupt_Flag(void) {
			return I2C_Functions::Read_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::TF);
		}

		void Clear_Timer_Interrupt_Flag(void) {
			I2C_Functions::Clear_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::TF, true);
		}

		// Read raw STATUS register flags.
		uint8_t Read_Status(void) {
			return I2C_Functions::Read_Register(RV3028_Reg::STATUS);
		}

		bool Power_On_Reset_Detected(void) {
			return I2C_Functions::Read_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::PORF_FLAG);
		}

		bool Clock_Failure_Detected(void) {
			return I2C_Functions::Read_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::CLKF);
		}

		void Clear_Power_On_Reset_Flag(void) {
			I2C_Functions::Clear_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::PORF_FLAG, true);
		}

		void Clear_Clock_Failure_Flag(void) {
			I2C_Functions::Clear_Register_Bit(RV3028_Reg::STATUS, RV3028_Bit::CLKF, true);
		}

		// Datasheet note: if PORF is 1, time data are not valid until re-initialized.
		bool Is_Time_Valid(void) {
			const uint8_t _status = Read_Status();
			return ((_status & (1u << RV3028_Bit::PORF_FLAG)) == 0u) && ((_status & (1u << RV3028_Bit::CLKF)) == 0u);
		}

		// -------------------------------------------------------------------------
		// Time Read / Write
		// -------------------------------------------------------------------------

		// Return weekday register value (0–6, user-defined day mapping).
		uint8_t Get_Week_Day(void) {
			return BCDtoDEC(I2C_Functions::Read_Register(RV3028_Reg::WEEKDAY));
		}

		// Day-of-week using Sakamoto's algorithm.
		// _Day: 1–31, _Month: 1–12, _Year: full 4-digit year (e.g. 2025).
		// Returns: 0=Sunday, 1=Monday, …, 6=Saturday.
		uint8_t Day_of_Week(const uint8_t _Day, const uint8_t _Month, uint16_t _Year) {
			uint8_t _m = _Month;
			if (_m < 3) _Year--;
			uint8_t _off;
			switch (_m) {
				case  1: _off = 0; break;
				case  2: _off = 3; break;
				case  3: _off = 2; break;
				case  4: _off = 5; break;
				case  5: _off = 0; break;
				case  6: _off = 3; break;
				case  7: _off = 5; break;
				case  8: _off = 1; break;
				case  9: _off = 4; break;
				case 10: _off = 6; break;
				case 11: _off = 2; break;
				case 12: _off = 4; break;
				default: _off = 0; break;
			}
			return (uint8_t)((_Year + _Year / 4 - _Year / 100 + _Year / 400 + _off + _Day) % 7);
		}

		// Write time to all seven RTC registers.
		// _Year is 2-digit (0–99): pass 25 for year 2025.
		// Returns false if any value is out of range.
		bool Set_Time(const uint8_t _Second, const uint8_t _Minute, const uint8_t _Hour, const uint8_t _Date, const uint8_t _Month, const uint8_t _Year) {
			if (!Validate_Time_Fields(_Second, _Minute, _Hour, _Date, _Month, _Year)) return false;

			const bool _is12h = is_12h_Clock();
			const uint8_t _hour_raw = Encode_Hour_Register(_Hour, _is12h);
			const uint8_t _DoW = Day_of_Week(_Date, _Month, static_cast<uint16_t>(_Year) + 2000u);
			I2C_Functions::Write_Register(RV3028_Reg::SECOND,  DECtoBCD(_Second), true);
			I2C_Functions::Write_Register(RV3028_Reg::MINUTE,  DECtoBCD(_Minute), true);
			I2C_Functions::Write_Register(RV3028_Reg::HOUR,    _hour_raw,          true);
			I2C_Functions::Write_Register(RV3028_Reg::WEEKDAY, DECtoBCD(_DoW),    true);
			I2C_Functions::Write_Register(RV3028_Reg::DAY,     DECtoBCD(_Date),   true);
			I2C_Functions::Write_Register(RV3028_Reg::MONTH,   DECtoBCD(_Month),  true);
			I2C_Functions::Write_Register(RV3028_Reg::YEAR,    DECtoBCD(_Year),   true);
			return true;
		}

		// Burst-read all 7 time registers into a caller-supplied buffer.
		// Single I2C transaction — no second-rollover race condition.
		// _Buffer should be at least RV3028_Cfg::TIMESTAMP_SIZE (20) bytes.
		void Get_Time(char* _Buffer, const uint8_t _Size) {
			if ((_Buffer == NULL) || (_Size == 0u)) return;

			uint8_t _buf[7];
			const bool _is12h = is_12h_Clock();
			I2C_Functions::Read_Multiple_Register(RV3028_Reg::SECOND, _buf, 7, true);
			snprintf(_Buffer, _Size, "20%02u-%02u-%02u %02u:%02u:%02u",
				BCDtoDEC(_buf[6]),   // Year
				BCDtoDEC(_buf[5]),   // Month
				BCDtoDEC(_buf[4]),   // Day
				Decode_Hour_Register(_buf[2], _is12h),
				BCDtoDEC(_buf[1]),   // Minute
				BCDtoDEC(_buf[0])    // Second
			);
		}

		// -------------------------------------------------------------------------
		// UNIX Time Counter (32-bit, independent of calendar registers)
		// -------------------------------------------------------------------------

		uint32_t Get_UNIX_Time(void) {
			uint8_t _buf[4];
			I2C_Functions::Read_Multiple_Register(RV3028_Reg::UNIX_TIME_0, _buf, 4, true);
			return (static_cast<uint32_t>(_buf[3]) << 24) | (static_cast<uint32_t>(_buf[2]) << 16) | (static_cast<uint32_t>(_buf[1]) <<  8) | static_cast<uint32_t>(_buf[0]);
		}

		void Clear_UNIX_Time(void) {
			I2C_Functions::Write_Register(RV3028_Reg::UNIX_TIME_0, 0x00, true);
			I2C_Functions::Write_Register(RV3028_Reg::UNIX_TIME_1, 0x00, true);
			I2C_Functions::Write_Register(RV3028_Reg::UNIX_TIME_2, 0x00, true);
			I2C_Functions::Write_Register(RV3028_Reg::UNIX_TIME_3, 0x00, true);
		}

		// Legacy API compatibility wrapper.
		uint32_t UNIX_Time(const bool _Method) {
			if (_Method == UNIX_CLEAR) {
				Clear_UNIX_Time();
			}
			return Get_UNIX_Time();
		}

		// -------------------------------------------------------------------------
		// Timer
		// -------------------------------------------------------------------------

		// Full timer configuration.
		//   _Repeat      : true = auto-reload on expiry (TRPT bit)
		//   _Frequency   : RV3028_Timer_Freq::FREQ_xxx constant
		//   _Value       : 12-bit countdown preset (1–4095)
		//   _Interrupt   : route timer expiry to INT pin (TIE bit)
		//   _Start       : start the timer immediately
		//   _Clock_Output: route timer interrupt to CLKOUT pin (TM bit)
		bool Set_Timer(const bool _Repeat, const uint16_t _Frequency, const uint16_t _Value, const bool _Interrupt, const bool _Start, const bool _Clock_Output) {
			if ((_Value == 0u) || (_Value > 4095u)) return false;

			uint8_t _td = 0;
			switch (_Frequency) {
				case RV3028_Timer_Freq::FREQ_4096HZ: _td = 0; break;
				case RV3028_Timer_Freq::FREQ_64HZ:   _td = 1; break;
				case RV3028_Timer_Freq::FREQ_1HZ:    _td = 2; break;
				case RV3028_Timer_Freq::FREQ_1_60HZ: _td = 3; break;
				case RV3028_Timer_Freq::LEGACY_FREQ_1_60HZ: _td = 3; break;
				default: return false;
			}

			Timer(false);
			Interrupt(false);
			Clear_Timer_Interrupt_Flag();

			I2C_Functions::Write_Register(RV3028_Reg::TIMER_VALUE_0, static_cast<uint8_t>(_Value & 0xFF), true);
			I2C_Functions::Write_Register(RV3028_Reg::TIMER_VALUE_1, static_cast<uint8_t>((_Value >> 8) & 0x0F), true);

			uint8_t _ctrl = I2C_Functions::Read_Register(RV3028_Reg::CONTROL_1);
			// Clear TD[1:0] and TRPT, then set according to arguments
			_ctrl &= ~((1u << RV3028_Bit::TD1) | (1u << RV3028_Bit::TD0) | (1u << RV3028_Bit::TRPT));
			if (_Repeat) _ctrl |= (1u << RV3028_Bit::TRPT);
			_ctrl |= _td;

			if (_Interrupt) Interrupt(true);
			if (_Start) _ctrl |= (1u << RV3028_Bit::TE);
			I2C_Functions::Write_Register(RV3028_Reg::CONTROL_1, _ctrl, true);

			if (_Clock_Output) {
				I2C_Functions::Set_Register_Bit(RV3028_Reg::CLOCK_INT_MASK, RV3028_Bit::TM, true);
			} else {
				I2C_Functions::Clear_Register_Bit(RV3028_Reg::CLOCK_INT_MASK, RV3028_Bit::TM, true);
			}

			return true;
		}

		// Convenience overload: one-shot, 1 Hz, with interrupt and CLKOUT, starts immediately.
		bool Set_Timer(const uint16_t _Value) {
			return Set_Timer(false, RV3028_Timer_Freq::FREQ_1HZ, _Value, true, true, true);
		}

		void Timer(const bool _Status) {
			if (_Status) {
				I2C_Functions::Set_Register_Bit(RV3028_Reg::CONTROL_1, RV3028_Bit::TE, true);
			} else {
				I2C_Functions::Clear_Register_Bit(RV3028_Reg::CONTROL_1, RV3028_Bit::TE, true);
			}
		}

		// -------------------------------------------------------------------------
		// EEPROM Access — datasheet §7.1 compliant sequence.
		// Returns false on address-out-of-range or EEBUSY timeout.
		// -------------------------------------------------------------------------

		bool Write_EEPROM(const uint8_t _Address, const uint8_t _Value) {
			if (_Address > RV3028_Cfg::EEPROM_MAX_ADDR) return false;
			if (!EEPROM_Wait_Ready()) return false;

			const uint8_t _ctrl = I2C_Functions::Read_Register(RV3028_Reg::CONTROL_1);
			I2C_Functions::Write_Register(RV3028_Reg::CONTROL_1, _ctrl | (1u << RV3028_Bit::EERD), true);

			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_ADDRESS, _Address,                    true);
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_DATA,    _Value,                      true);
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_CMD,     0x00,                        true);
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_CMD,     RV3028_EEPROM_Cmd::WRITE,    true);

			const bool _ok = EEPROM_Wait_Ready();
			I2C_Functions::Write_Register(RV3028_Reg::CONTROL_1, _ctrl, true);
			return _ok;
		}

		// Write 16-bit word MSB-first. _Address must be ≤ 0x29 (needs two consecutive bytes).
		bool Write_EEPROM_Word(const uint8_t _Address, const uint16_t _Value) {
			if (_Address >= RV3028_Cfg::EEPROM_MAX_ADDR) return false;
			return Write_EEPROM(_Address,     static_cast<uint8_t>(_Value >> 8)) &&
				   Write_EEPROM(_Address + 1, static_cast<uint8_t>(_Value & 0xFF));
		}

		// Read one EEPROM byte. Returns false on error; value written to _Out.
		bool Read_EEPROM(const uint8_t _Address, uint8_t& _Out) {
			if (_Address > RV3028_Cfg::EEPROM_MAX_ADDR) return false;
			if (!EEPROM_Wait_Ready()) return false;

			const uint8_t _ctrl = I2C_Functions::Read_Register(RV3028_Reg::CONTROL_1);
			I2C_Functions::Write_Register(RV3028_Reg::CONTROL_1, _ctrl | (1u << RV3028_Bit::EERD), true);

			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_ADDRESS, _Address,                   true);
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_CMD,     0x00,                       true);
			I2C_Functions::Write_Register(RV3028_Reg::EEPROM_CMD,     RV3028_EEPROM_Cmd::READ,    true);

			const bool _ok = EEPROM_Wait_Ready();
			if (_ok) _Out = I2C_Functions::Read_Register(RV3028_Reg::EEPROM_DATA);
			I2C_Functions::Write_Register(RV3028_Reg::CONTROL_1, _ctrl, true);
			return _ok;
		}

		// Read 16-bit word MSB-first. _Address must be ≤ 0x29.
		bool Read_EEPROM_Word(const uint8_t _Address, uint16_t& _Out) {
			if (_Address >= RV3028_Cfg::EEPROM_MAX_ADDR) return false;
			uint8_t _msb = 0, _lsb = 0;
			if (!Read_EEPROM(_Address,     _msb)) return false;
			if (!Read_EEPROM(_Address + 1, _lsb)) return false;
			_Out = (static_cast<uint16_t>(_msb) << 8) | _lsb;
			return true;
		}

	private:

		static constexpr uint8_t HOUR_AMPM_BIT = 5;

		// BCD ↔ decimal — internal helpers, not part of the public API.
		static inline uint8_t BCDtoDEC(const uint8_t _v) {
			return (_v >> 4) * 10u + (_v & 0x0F);
		}
		static inline uint8_t DECtoBCD(const uint8_t _v) {
			return ((_v / 10u) << 4) | (_v % 10u);
		}

		static bool Is_Leap_Year(const uint16_t _year_full) {
			return ((_year_full % 4u) == 0u) && (((_year_full % 100u) != 0u) || ((_year_full % 400u) == 0u));
		}

		static uint8_t Days_In_Month(const uint8_t _month, const uint16_t _year_full) {
			switch (_month) {
				case 1:  return 31;
				case 2:  return Is_Leap_Year(_year_full) ? 29 : 28;
				case 3:  return 31;
				case 4:  return 30;
				case 5:  return 31;
				case 6:  return 30;
				case 7:  return 31;
				case 8:  return 31;
				case 9:  return 30;
				case 10: return 31;
				case 11: return 30;
				case 12: return 31;
				default: return 0;
			}
		}

		static bool Validate_Time_Fields(const uint8_t _second, const uint8_t _minute, const uint8_t _hour,
										const uint8_t _date, const uint8_t _month, const uint8_t _year) {
			if (_second > 59u) return false;
			if (_minute > 59u) return false;
			if (_hour > 23u) return false;
			if ((_month < 1u) || (_month > 12u)) return false;
			if (_year > 99u) return false;
			const uint16_t _year_full = static_cast<uint16_t>(_year) + 2000u;
			const uint8_t _max_day = Days_In_Month(_month, _year_full);
			if ((_date < 1u) || (_date > _max_day)) return false;
			return true;
		}

		static uint8_t Encode_Hour_Register(const uint8_t _hour_24, const bool _is12h) {
			if (!_is12h) return DECtoBCD(_hour_24);

			const bool _pm = (_hour_24 >= 12u);
			uint8_t _hour12 = static_cast<uint8_t>(_hour_24 % 12u);
			if (_hour12 == 0u) _hour12 = 12u;

			uint8_t _raw = DECtoBCD(_hour12);
			if (_pm) {
				_raw |= (1u << HOUR_AMPM_BIT);
			} else {
				_raw &= static_cast<uint8_t>(~(1u << HOUR_AMPM_BIT));
			}
			return _raw;
		}

		static uint8_t Decode_Hour_Register(const uint8_t _raw, const bool _is12h) {
			if (!_is12h) return BCDtoDEC(_raw & 0x3Fu);

			uint8_t _hour12 = BCDtoDEC(_raw & 0x1Fu);
			if ((_hour12 < 1u) || (_hour12 > 12u)) _hour12 = 12u;

			const bool _pm = (_raw & (1u << HOUR_AMPM_BIT)) != 0u;
			if (_pm) {
				return (_hour12 == 12u) ? 12u : static_cast<uint8_t>(_hour12 + 12u);
			}
			return (_hour12 == 12u) ? 0u : _hour12;
		}

		// Poll EEBUSY until clear or timeout expires.
		// RV3028 EEPROM access time can extend beyond a fixed loop budget.
		// Uses millis()-based timeout for deterministic behavior across MCUs.
		bool EEPROM_Wait_Ready(void) {
			const uint32_t _start = millis();
			while ((millis() - _start) < RV3028_Cfg::EEPROM_TIMEOUT_MS) {
				if ((I2C_Functions::Read_Register(RV3028_Reg::STATUS) & (1u << RV3028_Bit::EEBUSY)) == 0u) {
					return true;
				}
				delayMicroseconds(50);
			}
			return false;
		}

};
