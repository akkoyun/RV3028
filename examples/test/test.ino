#include <RV3028.h>

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

}

void loop() {

	// Get Minute
  Serial.print("Minute : "); Serial.println(RTC.Get_Minute());
  Serial.print("Second : "); Serial.println(RTC.Get_Second());


	// Loop Delay
	delay(1000);

}