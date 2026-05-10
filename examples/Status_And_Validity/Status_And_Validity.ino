// Include Library
#include "RV3028.h"

// Create RV3028 Object
RV3028 RTC;

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("   RTC Status & Validity   ");
	Serial.println("--------------------------");

	// Start RTC — returns false if device not found on I2C bus
	if (!RTC.Begin()) {
		Serial.println("RV3028 not found!");
		while (1);
	}

	// Read and print status register
	const uint8_t status = RTC.Read_Status();
	Serial.print("STATUS = 0x");
	Serial.println(status, HEX);

	// Print status helpers
	Serial.print("Time valid: ");
	Serial.println(RTC.Is_Time_Valid() ? "yes" : "no");

	Serial.print("POR flag: ");
	Serial.println(RTC.Power_On_Reset_Detected() ? "set" : "clear");

	Serial.print("CLKF flag: ");
	Serial.println(RTC.Clock_Failure_Detected() ? "set" : "clear");

	// Optional: clear flags after reading
	if (RTC.Power_On_Reset_Detected()) {
		RTC.Clear_Power_On_Reset_Flag();
	}
	if (RTC.Clock_Failure_Detected()) {
		RTC.Clear_Clock_Failure_Flag();
	}
}

void loop() {
	delay(1000);
}
