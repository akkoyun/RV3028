#pragma once

#include <stdint.h>
#include "Registers.h"

// =========================================================================
// RV3028-C7 Configuration Constants
// All values are compile-time constants — zero RAM cost.
// =========================================================================

namespace RV3028_Cfg {

    // I2C device address (7-bit)
    constexpr uint8_t  I2C_ADDRESS     = 0x52;

    // Clock mode selectors for Set_Clock_Type()
    constexpr bool     CLOCK_24H       = false;
    constexpr bool     CLOCK_12H       = true;

    // Timestamp buffer size including null terminator
    // Format: "20YY-MM-DD HH:MM:SS\0" = 20 bytes
    constexpr uint8_t  TIMESTAMP_SIZE  = 20;

    // EEPROM user address upper limit (inclusive)
    constexpr uint8_t  EEPROM_MAX_ADDR = 0x2A;

    // EEPROM ready timeout in milliseconds.
    // Covers startup refresh (~66 ms) and single-byte EEPROM write (~16 ms).
    constexpr uint16_t EEPROM_TIMEOUT_MS = 100;

    // Legacy loop-based timeout alias kept for backward source compatibility.
    constexpr uint16_t EEPROM_TIMEOUT    = EEPROM_TIMEOUT_MS;

}  // namespace RV3028_Cfg

// =========================================================================
// Timer Frequency Selection
// Pass to Set_Timer() _Frequency parameter.
// Value encodes the TD[1:0] field of CONTROL_1 (0=4096Hz … 3=1/60Hz).
// =========================================================================
namespace RV3028_Timer_Freq {
    constexpr uint16_t FREQ_4096HZ = 4096;
    constexpr uint16_t FREQ_64HZ   = 64;
    constexpr uint16_t FREQ_1HZ    = 1;
    constexpr uint16_t FREQ_1_60HZ = 0;   // sentinel: 0 Hz is meaningless → maps to TD=11
    // Legacy literal used by older sketches.
    constexpr uint16_t LEGACY_FREQ_1_60HZ = 60000;
}

// =========================================================================
// Legacy #define aliases — kept for backward source compatibility.
// New code should use the namespaced constants above.
// =========================================================================
#define CLOCK_12H       RV3028_Cfg::CLOCK_12H
#define CLOCK_24H       RV3028_Cfg::CLOCK_24H
#define HZ_4096         RV3028_Timer_Freq::FREQ_4096HZ
#define HZ_64           RV3028_Timer_Freq::FREQ_64HZ
#define HZ_1            RV3028_Timer_Freq::FREQ_1HZ
#define HZ_1_60         RV3028_Timer_Freq::FREQ_1_60HZ

// Legacy UNIX API aliases for backward compatibility.
#define UNIX_GET        false
#define UNIX_CLEAR      true
