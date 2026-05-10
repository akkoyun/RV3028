// Include Library
#include "RV3028.h"

// Create RV3028 Object
RV3028 RTC;

// Caller-managed timestamp buffer (ultra low RAM library design)
char Time_Buffer[RV3028_Cfg::TIMESTAMP_SIZE];

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

	// Start RTC — returns false if device not found on I2C bus
	if (!RTC.Begin()) {
		Serial.println("RV3028 not found!");
		while (1);
	}

	// Set Time: second, minute, hour, date, month, year (2-digit: 25 = 2025)
	if (!RTC.Set_Time(0, 0, 18, 2, 1, 25)) {
		Serial.println("Set_Time failed (invalid value)");
		while (1);
	}

}

void loop() {

	// Read time into caller-provided buffer (burst read — race-condition safe)
	RTC.Get_Time(Time_Buffer, sizeof(Time_Buffer));

	// Print Time
	Serial.println(Time_Buffer);

	// Loop Delay
	delay(1000);

}
