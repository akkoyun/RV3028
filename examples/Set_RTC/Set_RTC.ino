#include "RV3028.h"

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

    // Start RTC
	RTC.Begin(1);

    // Set Time
    RTC.Set_Time(0,0,18,2,1,21);

}

void loop() {

	// Get Time
	Serial.print(RTC.Time_Stamp());

	// Print Day of Week
	uint8_t WeekDay = RTC.Day_of_Week(RTC.Get_Date(), RTC.Get_Month(), RTC.Get_Year());

	// Week of Day
	Serial.print(" ");
    if(WeekDay == 0) Serial.println("Saturday");
    if(WeekDay == 1) Serial.println("Sunday");
    if(WeekDay == 2) Serial.println("Monday");
    if(WeekDay == 3) Serial.println("Tuesday");
    if(WeekDay == 4) Serial.println("Wednesday");
    if(WeekDay == 5) Serial.println("Thursday");
    if(WeekDay == 6) Serial.println("Friday");

	// Loop Delay
	delay(1000);

}