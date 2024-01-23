// Include Library
#include "RV3028.h"

// Declare Global Variable
bool RTC_Interrupt = false;

// Create RV3028 Object
RV3028 RTC;

// Setup
void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

    // Start RTC
	RTC.Begin();

    // Set Timer
    RTC.Set_Timer(false, 1, 10, true, true, true);

	// Interrupt Definitions
	cli();
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);
	sei();

}

// Loop
void loop() {

    // Interrupt Routine
    if (RTC_Interrupt) {

        // Report
        Serial.print("Interrupt - ");

		// Update Time Stamp
		RTC.Update_Time_Stamp();

	    // Print Time
		Serial.println(RTC.Time_Stamp);

        // Start Timer
		RTC.Timer(true);

        // Set Variable
        RTC_Interrupt = false;

    }

}

// Interrupt Routine
ISR(PCINT0_vect) {

	// Control RTC Interrupt [PB0]
	if ((PINB & (1 << PINB0)) == (1 << PINB0)) {
		
		// Set Variable
		RTC_Interrupt = true;

	} else {
		
		// Set Variable
		RTC_Interrupt = false;

	}

	// Interrupt Delay
	delay(5);

}
