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

    // Start RTC
	RTC.Begin();

    // Clear UNIX
    RTC.UNIX_Time(UNIX_CLEAR);

}

void loop() {

	// Get Time
	Serial.println(RTC.UNIX_Time(UNIX_GET));

    // Wait
    delay(500);

}