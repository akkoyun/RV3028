// Include Library
#include "RV3028.h"

// Create RV3028 Object
RV3028 RTC;

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("    RTC EEPROM Access     ");
	Serial.println("--------------------------");

	// Start RTC — returns false if device not found on I2C bus
	if (!RTC.Begin()) {
		Serial.println("RV3028 not found!");
		while (1);
	}

	// Example user EEPROM address range: 0x00..0x2A
	const uint8_t address = 0x2A;
	const uint8_t writeValue = 0x5A;

	if (!RTC.Write_EEPROM(address, writeValue)) {
		Serial.println("Write_EEPROM failed");
		while (1);
	}

	uint8_t readValue = 0;
	if (!RTC.Read_EEPROM(address, readValue)) {
		Serial.println("Read_EEPROM failed");
		while (1);
	}

	Serial.print("Address 0x");
	Serial.print(address, HEX);
	Serial.print(" = 0x");
	Serial.println(readValue, HEX);

	// 16-bit example
	const uint8_t wordAddress = 0x28;
	const uint16_t writeWord = 0xBEEF;
	uint16_t readWord = 0;

	if (!RTC.Write_EEPROM_Word(wordAddress, writeWord)) {
		Serial.println("Write_EEPROM_Word failed");
		while (1);
	}

	if (!RTC.Read_EEPROM_Word(wordAddress, readWord)) {
		Serial.println("Read_EEPROM_Word failed");
		while (1);
	}

	Serial.print("Word 0x");
	Serial.print(wordAddress, HEX);
	Serial.print(" = 0x");
	Serial.println(readWord, HEX);
}

void loop() {
	delay(1000);
}
