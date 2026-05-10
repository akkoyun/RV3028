# RV3028 Library (v2.0.1)

Ultra-low-RAM Arduino/PlatformIO library for the Micro Crystal RV-3028-C7 RTC.

## Highlights

* Always ultra-low-RAM design: no internal timestamp buffer per instance
* Datasheet-aligned time validation helpers (PORF/CLKF)
* Safe input validation for time and timer setup
* EEPROM read/write helpers with busy handling
* 12h/24h hour conversion handled correctly

## Requirements

* Arduino framework
* I2C_Functions dependency

## Installation

### Arduino IDE

Install `RV3028` from Arduino Library Manager, then install dependency `I2C_Functions`.

### PlatformIO

```ini
lib_deps =
  akkoyun/RV3028@^2.0.1
```

## Quick Start

```cpp
#include "RV3028.h"

RV3028 RTC;
char ts[RV3028_Cfg::TIMESTAMP_SIZE];

void setup() {
  Serial.begin(115200);

  if (!RTC.Begin()) {
    while (1);
  }

  if (!RTC.Set_Time(0, 0, 12, 10, 5, 26)) {
    while (1);
  }
}

void loop() {
  RTC.Get_Time(ts, sizeof(ts));
  Serial.println(ts);
  delay(1000);
}
```

## Public API Reference

### Construction and Initialization

* `RV3028(bool muxEnable = false, uint8_t muxChannel = 0)`
  * Creates an RTC object.
* `bool Begin(bool applyDefaultConfig = false)`
  * Starts I2C and checks RTC presence.
  * If `applyDefaultConfig` is `true`, applies 24h mode, disables trickle charger, enables 1 Hz CLKOUT.

### Clock Configuration

* `void Set_Clock_Type(bool type)`
  * `true` = 12h mode, `false` = 24h mode.
* `bool is_12h_Clock()`
  * Returns current hour format.
* `void Disable_Trickle_Charger()`
  * Clears trickle charger bit in backup EEPROM RAM mirror.
* `void Clock_Out(bool state = true)`
  * Enables/disables normal CLKOUT output (configured to 1 Hz when enabled).

### Interrupt and Status

* `void Clear_Interrupt()`
  * Clears status flags.
* `void Interrupt(bool state)`
  * Enables/disables timer interrupt routing.
* `bool Read_Timer_Interrupt_Flag()`
  * Reads `TF` flag.
* `void Clear_Timer_Interrupt_Flag()`
  * Clears `TF` flag.
* `uint8_t Read_Status()`
  * Returns raw status register.
* `bool Power_On_Reset_Detected()`
  * Returns `PORF` state.
* `bool Clock_Failure_Detected()`
  * Returns `CLKF` state.
* `void Clear_Power_On_Reset_Flag()`
  * Clears `PORF`.
* `void Clear_Clock_Failure_Flag()`
  * Clears `CLKF`.
* `bool Is_Time_Valid()`
  * Returns `true` only if `PORF` and `CLKF` are both clear.

### Time Functions

* `uint8_t Get_Week_Day()`
  * Reads weekday counter value (`0..6`).
* `uint8_t Day_of_Week(uint8_t day, uint8_t month, uint16_t year)`
  * Calculates weekday for a date.
* `bool Set_Time(uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint8_t year)`
  * Writes calendar/time registers.
  * `year` is 2-digit (`0..99` -> `2000..2099`).
  * Validates ranges and leap-year day constraints.
* `void Get_Time(char* buffer, uint8_t size)`
  * Burst-reads time and formats as `20YY-MM-DD HH:MM:SS`.
  * Caller must provide buffer.

### UNIX Counter

* `uint32_t Get_UNIX_Time()`
  * Reads UNIX counter registers.
* `void Clear_UNIX_Time()`
  * Clears UNIX counter registers.
* `uint32_t UNIX_Time(bool method)`
  * Legacy compatibility wrapper.
  * `UNIX_CLEAR` clears counter and returns current value.
  * `UNIX_GET` returns current value.

### Timer

* `bool Set_Timer(bool repeat, uint16_t frequency, uint16_t value, bool interrupt, bool start, bool clockOutput)`
  * Configures countdown timer.
  * `value` valid range: `1..4095`.
  * Frequency constants:
    * `RV3028_Timer_Freq::FREQ_4096HZ`
    * `RV3028_Timer_Freq::FREQ_64HZ`
    * `RV3028_Timer_Freq::FREQ_1HZ`
    * `RV3028_Timer_Freq::FREQ_1_60HZ`
* `bool Set_Timer(uint16_t value)`
  * One-shot convenience overload at 1 Hz.
* `void Timer(bool status)`
  * Starts/stops timer by TE bit.

### EEPROM Access

* `bool Write_EEPROM(uint8_t address, uint8_t value)`
* `bool Write_EEPROM_Word(uint8_t address, uint16_t value)`
* `bool Read_EEPROM(uint8_t address, uint8_t& out)`
* `bool Read_EEPROM_Word(uint8_t address, uint16_t& out)`

Address limits follow library config (`0x00..0x2A`, word reads/writes require valid consecutive bytes).

## Examples

The library includes at least five examples:

* `examples/Set_RTC`
* `examples/Get_UNIX_Time`
* `examples/RTC_Timer`
* `examples/Status_And_Validity`
* `examples/EEPROM_Read_Write`

## RAM Analysis (ATmega2560, PlatformIO)

Measured with PlatformIO (board: `megaatmega2560`, framework: `arduino`) using this library v2.0.1.

| Example | RAM Used | RAM Total | RAM % | Flash Used |
| --- | ---: | ---: | ---: | ---: |
| Set_RTC | 534 B | 8192 B | 6.5% | 7538 B |
| Get_UNIX_Time | 482 B | 8192 B | 5.9% | 5788 B |
| RTC_Timer | 547 B | 8192 B | 6.7% | 7762 B |
| Status_And_Validity | 548 B | 8192 B | 6.7% | 6064 B |
| EEPROM_Read_Write | 594 B | 8192 B | 7.3% | 6528 B |

The table is filled from real PlatformIO build output in this repository.

## Changelog

### 2.0.1

* Unified always-ultra-low-RAM architecture
* Removed internal timestamp storage model
* Strengthened validation and status helpers
* Added additional examples and expanded documentation
