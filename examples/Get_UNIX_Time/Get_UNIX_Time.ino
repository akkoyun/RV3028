// Include Library
#include "RV3028.h"

// Create RV3028 Object
RV3028 RTC;

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

	// Clear UNIX counter (call explicitly when needed — Begin() no longer auto-clears)
	RTC.Clear_UNIX_Time();

}

void loop() {

	// Get and print UNIX time counter
	Serial.println(RTC.Get_UNIX_Time());

	// Wait
	delay(500);

}
