#include "RV3028.h"

// Declare Global Variable
bool RTC_Interrupt = false;

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("       RTC Functions      ");
	Serial.println("--------------------------");

    // Start RTC
	RTC.Begin(1);

    // Set Timer
    RTC.Set_Timer(false, 1, 10, true, true, true);

	// Interupt Definations
	cli();
	PCICR	|= 0b00000111;	// Set All Interrupt
	PCMSK0	|= 0b00010000;	// Timer Interupt Definations
	sei();

}

void loop() {

	// Loop Delay
	delay(1000);

    // Interrupt Routine
    if (RTC_Interrupt) {
        
        // Report
        Serial.print("Interrupt - ");

	    // Get Time
    	Serial.println(RTC.Time_Stamp());

        // Set Timer
        RTC.Set_Timer(false, 1, 10, true, true, true);

        // Set Variable
        RTC_Interrupt = false;
        
    }
    

}

ISR(PCINT0_vect) {

	// Control RTC Interrupt [PB4]
	if ((PINB & (1 << PINB4)) == (1 << PINB4)) {
		
		// Set Variable
		RTC_Interrupt = true;

	} else {
		
		// Set Variable
		RTC_Interrupt = false;

	}

	// Interrupt Delay
	delay(50);

}
