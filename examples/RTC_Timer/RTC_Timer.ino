// Include Library
#include "RV3028.h"

// volatile: prevents compiler from optimizing away the ISR→loop handshake
volatile bool RTC_Interrupt = false;

// Create RV3028 Object
RV3028 RTC;

// Caller-managed timestamp buffer (ultra low RAM library design)
char Time_Buffer[RV3028_Cfg::TIMESTAMP_SIZE];

// Setup
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

	// Configure timer: one-shot, 1 Hz, 10-count (10 s), interrupt + CLKOUT enabled
	if (!RTC.Set_Timer(false, RV3028_Timer_Freq::FREQ_1HZ, 10, true, true, true)) {
		Serial.println("Set_Timer failed");
		while (1);
	}

	// Enable pin-change interrupt on PB0 (CLKOUT → Arduino pin 53 on Mega)
	cli();
	PCICR  |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);
	sei();

}

// Loop
void loop() {

	if (RTC_Interrupt) {

		// Read time via burst (race-condition safe)
		RTC.Get_Time(Time_Buffer, sizeof(Time_Buffer));

		Serial.print("Interrupt - ");
		Serial.println(Time_Buffer);

		// Clear TF flag then restart one-shot timer
		RTC.Clear_Timer_Interrupt_Flag();
		RTC.Timer(true);

		RTC_Interrupt = false;

	}

}

// Pin-change ISR — keep minimal: set flag only, no I2C, no delay()
ISR(PCINT0_vect) {
	RTC_Interrupt = (bool)(PINB & (1 << PINB0));
}
