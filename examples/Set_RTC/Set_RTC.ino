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

    // Set Time
    RTC.Set_Time(0,0,18,2,1,21);

}

void loop() {

	// Update Time Stamp
	RTC.Update_Time_Stamp();

	// Print Time
	Serial.println(RTC.Time_Stamp);

	// Loop Delay
	delay(1000);

}