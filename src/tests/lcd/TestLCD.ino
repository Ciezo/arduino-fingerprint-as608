#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Receive data from Serial Connection
byte serial_data; 

void setup()
{
	Serial.begin(9600);

	// initialize the LCD
	lcd.begin();
	lcd.backlight();
	lcd.print("Starting up....");
	lcd.setCursor(0, 1);
	lcd.print("Choose a mode");
}

void loop()
{
	serial_data = 0;
	if (Serial.available()) {
		serial_data = Serial.read();

		switch (serial_data) {
			case 1:
				lcd.clear();
				lcd.print(serial_data);
				delay(1000);
				enrollMode();
				break;
			case 2:
				lcd.clear();
				lcd.print(serial_data);
				delay(1000);
				verificationMode();
				break;
		}
	}
}

uint8_t verificationMode() {
	lcd.clear();
	lcd.print("Verify Mode");
}

uint8_t enrollMode() {
	lcd.clear();
	lcd.print("Enroll Mode");

	while (!Serial.available()) {
		// Wait until data is available on the serial connection
	}

	byte id = Serial.read();  // Read the data from the serial connection

	lcd.setCursor(0, 1);
	lcd.print("ID saved: ");
	lcd.print(id);

	// Return a value to satisfy the function's return type
	return 0;
}