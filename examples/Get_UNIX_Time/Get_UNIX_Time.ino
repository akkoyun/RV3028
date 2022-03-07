#include "RV3028.h"

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

    // Start RTC
	RTC.Begin(1);

    // Clear UNIX
    RTC.Clear_UNIX_Time();

}

void loop() {

	// Get Time
	Serial.println(RTC.Get_UNIX_Time());

    // Wait
    delay(1);

}