#pragma once

#include <stdint.h>

// =========================================================================
// RV3028-C7 Register Map and Bit Positions
// All values are compile-time constants — zero RAM cost.
// =========================================================================

namespace RV3028_Reg {

    // -------------------------------------------------------------------------
    // RTC Time Registers (0x00–0x06)
    // -------------------------------------------------------------------------
    constexpr uint8_t SECOND          = 0x00;
    constexpr uint8_t MINUTE          = 0x01;
    constexpr uint8_t HOUR            = 0x02;
    constexpr uint8_t WEEKDAY         = 0x03;
    constexpr uint8_t DAY             = 0x04;
    constexpr uint8_t MONTH           = 0x05;
    constexpr uint8_t YEAR            = 0x06;

    // -------------------------------------------------------------------------
    // Alarm Registers (0x07–0x09)
    // -------------------------------------------------------------------------
    constexpr uint8_t ALARM_MINUTES   = 0x07;
    constexpr uint8_t ALARM_HOURS     = 0x08;
    constexpr uint8_t ALARM_WEEKDAY   = 0x09;

    // -------------------------------------------------------------------------
    // Timer Registers (0x0A–0x0B)
    // -------------------------------------------------------------------------
    constexpr uint8_t TIMER_VALUE_0   = 0x0A;
    constexpr uint8_t TIMER_VALUE_1   = 0x0B;

    // -------------------------------------------------------------------------
    // Status and Control Registers (0x0E–0x10)
    // -------------------------------------------------------------------------
    constexpr uint8_t STATUS          = 0x0E;
    constexpr uint8_t CONTROL_1       = 0x0F;
    constexpr uint8_t CONTROL_2       = 0x10;

    // -------------------------------------------------------------------------
    // Clock Interrupt Mask Register (0x12)
    // TM bit routes timer interrupt to CLKOUT pin
    // -------------------------------------------------------------------------
    constexpr uint8_t CLOCK_INT_MASK  = 0x12;

    // -------------------------------------------------------------------------
    // UNIX Time Registers (0x1B–0x1E) — little-endian 32-bit counter
    // -------------------------------------------------------------------------
    constexpr uint8_t UNIX_TIME_0     = 0x1B;
    constexpr uint8_t UNIX_TIME_1     = 0x1C;
    constexpr uint8_t UNIX_TIME_2     = 0x1D;
    constexpr uint8_t UNIX_TIME_3     = 0x1E;

    // -------------------------------------------------------------------------
    // EEPROM Control Registers (0x25–0x27)
    // Must be accessed via the two-step EEPROM command sequence (datasheet §7.1)
    // -------------------------------------------------------------------------
    constexpr uint8_t EEPROM_ADDRESS  = 0x25;
    constexpr uint8_t EEPROM_DATA     = 0x26;
    constexpr uint8_t EEPROM_CMD      = 0x27;

    // -------------------------------------------------------------------------
    // EEPROM Mirror Registers (0x35–0x37)
    // Writes here are volatile (RAM mirror only). Settings survive reset only
    // if also written to EEPROM via Write_EEPROM() targeting the same address.
    // -------------------------------------------------------------------------
    constexpr uint8_t EEPROM_CLKOUT   = 0x35;
    constexpr uint8_t EEPROM_OFFSET   = 0x36;
    constexpr uint8_t EEPROM_BACKUP   = 0x37;

}  // namespace RV3028_Reg

// =========================================================================
// EEPROM Command Bytes (datasheet Table 20)
// =========================================================================
namespace RV3028_EEPROM_Cmd {
    constexpr uint8_t WRITE = 0x21;
    constexpr uint8_t READ  = 0x22;
}

// =========================================================================
// Bit Positions — used with Arduino bitSet() / bitClear() / bitRead()
// =========================================================================
namespace RV3028_Bit {

    // Status Register (0x0E)
    constexpr uint8_t EEBUSY      = 7;
    constexpr uint8_t CLKF        = 6;
    constexpr uint8_t BSF         = 5;
    constexpr uint8_t UF          = 4;
    constexpr uint8_t TF          = 3;
    constexpr uint8_t AF          = 2;
    constexpr uint8_t EVF         = 1;
    constexpr uint8_t PORF_FLAG   = 0;

    // Control 1 Register (0x0F)
    constexpr uint8_t TRPT        = 7;
    constexpr uint8_t WADA        = 5;
    constexpr uint8_t USEL        = 4;
    constexpr uint8_t EERD        = 3;
    constexpr uint8_t TE          = 2;
    constexpr uint8_t TD1         = 1;
    constexpr uint8_t TD0         = 0;

    // Control 2 Register (0x10)
    constexpr uint8_t TSE         = 7;
    constexpr uint8_t CLKIE       = 6;
    constexpr uint8_t UIE         = 5;
    constexpr uint8_t TIE         = 4;
    constexpr uint8_t AIE         = 3;
    constexpr uint8_t EIE         = 2;
    constexpr uint8_t HOUR_FORMAT = 1;  // 0 = 24h, 1 = 12h
    constexpr uint8_t RESET       = 0;

    // EEPROM CLKOUT Register (0x35)
    // FD[2:0]: 000=32768Hz 001=8192Hz 010=1024Hz 011=64Hz 100=32Hz 101=1Hz 110=1/60Hz
    constexpr uint8_t CLKOE       = 7;
    constexpr uint8_t CLKSY       = 6;
    constexpr uint8_t PORIE       = 3;
    constexpr uint8_t FD2         = 2;
    constexpr uint8_t FD1         = 1;
    constexpr uint8_t FD0         = 0;

    // EEPROM BACKUP Register (0x37)
    constexpr uint8_t EEOFFSET_0  = 7;
    constexpr uint8_t BSIE        = 6;
    constexpr uint8_t TCE         = 5;
    constexpr uint8_t FEDE        = 4;
    constexpr uint8_t BSM_1       = 3;
    constexpr uint8_t BSM_0       = 2;
    constexpr uint8_t TCR_1       = 1;
    constexpr uint8_t TCR_0       = 0;

    // Clock Interrupt Mask Register (0x12)
    constexpr uint8_t TM          = 1;

    // Alarm Registers (0x07–0x09) — bit 7 of each register
    // 0 = this field participates in comparison, 1 = don't care
    constexpr uint8_t AE          = 7;

}  // namespace RV3028_Bit
