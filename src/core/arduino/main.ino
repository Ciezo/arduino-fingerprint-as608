/*
    @author Cloyd Van S. Secuya
    @details Source code for setting up fingerprint AS608 sensor with Arduino 
*/
// PRE-PROCESSOR DIRECTIVES / HEADERS
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



// AS608 ports
SoftwareSerial mySerial(2, 3);  // TX, RX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;   // Fingerprint id
uint8_t opt; 

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buzzer 
const int buzz_pin = 8;





void setup() {

    // Begin Serial communication at 9600 baud rate
    Serial.begin(9600);
    
    // set the data rate for the sensor serial port
    /** Check if the fingerprint sensor is detected */
    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } 

    else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) { delay(1); }
    }

    finger.getTemplateCount();
    if (finger.templateCount == 0) {
        Serial.print("Sensor doesn't contain any fingerprint data.");
    }
    else {
        Serial.println("Waiting for valid finger...");
        Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    }

    // initialize the LCD
	lcd.begin();
	lcd.backlight();
    lcd.print("Starting up...");
    lcd.setCursor(0, 1);
    lcd.print("Choose mode");

    // Startup beep
    tone(buzz_pin, 500, 400);
}


void loop() {
    // Receive data from Serial Connection
	byte serial_data; 
    if (Serial.available()) {
        // Read the serial_data 
        serial_data = Serial.read();

        switch (serial_data) {
            case 1: 
                lcd.clear();
                lcd.print("Enroll mode");
                getFingerprintEnroll();
                delay(50);
                break;
            case 2: 
                lcd.clear();
                lcd.print("Verify mode");
                while(true) { getFingerprintID(); delay(50);} 
                break;  
            case 3: 
                lcd.clear();
                lcd.print("Delete all");
                deleteAllFingerprint(); 
                lcd.setCursor(0, 1);
                lcd.print("Please, reset");
                break;
        }

    }
}



/** This function enrolls or saves fingerprint data into the sensor */
uint8_t getFingerprintEnroll() {
    // Prompt the user to enroll their fingerprint
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    while (!Serial.available()) {
		// Wait until data is available on the serial connection
	}

    id = Serial.read();
    if (id == 0) {
        lcd.clear();
        lcd.print("ID invalid");
        return;
    }
    lcd.clear();
    lcd.print("Saved ID");
    lcd.setCursor(0, 1);
    lcd.print("# " + id);

    int p = -1;
    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    delay(500);
    lcd.clear();
    lcd.print("Place finger!");
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
        case FINGERPRINT_OK:
        lcd.clear();
        lcd.print("Finger captured!");
        Serial.println("Image taken");
        break;
        case FINGERPRINT_NOFINGER:
        Serial.println(".");
        lcd.clear();
        lcd.print("Waiting...");
        lcd.setCursor(0, 1);
        lcd.print("Place finger!");
        break;
        case FINGERPRINT_PACKETRECIEVEERR:

        break;
        case FINGERPRINT_IMAGEFAIL:

        break;
        default:

        break;
        }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        lcd.clear();
        lcd.print("Finger captured!");
        break;
        case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:

        return p;
        case FINGERPRINT_FEATUREFAIL:


        return p;
        case FINGERPRINT_INVALIDIMAGE:


        return p;
        default:

        return p;
    }

    Serial.println("Remove finger");
    lcd.clear();
    lcd.print("Remove finger!");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
        p = finger.getImage();
        // Write the fingerprint template data over Serial port
        Serial.write(p);
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    delay(500);
    lcd.clear();
    lcd.print("Place finger!");
    lcd.setCursor(0, 1);
    lcd.print("again...");
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        // Write the fingerprint template data over Serial port
        Serial.write(p);
        switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        lcd.clear();
        lcd.print("Finger captured!");
        break;
        case FINGERPRINT_NOFINGER:
        Serial.print(".");

        break;
        case FINGERPRINT_PACKETRECIEVEERR:

        break;
        case FINGERPRINT_IMAGEFAIL:

        break;
        default:

        break;
        }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        lcd.clear();
        lcd.print("Finger captured!");
        break;
        case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");

        return p;
        case FINGERPRINT_PACKETRECIEVEERR:

        return p;
        case FINGERPRINT_FEATUREFAIL:

        return p;
        case FINGERPRINT_INVALIDIMAGE:

        return p;
        default:

        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");  Serial.println(id);
    lcd.clear();
    lcd.print("Saving....");
    delay(1000);
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
        Serial.println("Prints matched!");
        lcd.clear();
        lcd.print("Print match!");

    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
        Serial.println("Fingerprints did not match");
        lcd.clear();
        lcd.print("Print mismatch!");
        return p;
    } else {

        return p;
    }

    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    // Write the fingerprint data model over Serial 
    Serial.write(p);
    if (p == FINGERPRINT_OK) {
        Serial.println("Stored!");
        lcd.clear();
        lcd.print("Finger saved!");
        lcd.setCursor(0, 1);
        lcd.print("Please, reset");
        tone(buzz_pin, 800, 400);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {

        return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
        Serial.println("Could not store in that location");

        return p;
    } else if (p == FINGERPRINT_FLASHERR) {
        Serial.println("Error writing to flash");

        return p;
    } else {

        return p;
    }

    return true;
}




/** This function verifies a fingerprint data */
uint8_t getFingerprintID() {

    uint8_t p = finger.getImage();
    switch (p) {
        case FINGERPRINT_OK:
        break;
        case FINGERPRINT_NOFINGER:
        lcd.clear();
        lcd.print("Verify...");
        lcd.setCursor(0, 1);
        lcd.print("Place finger!");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        return p;
        case FINGERPRINT_IMAGEFAIL:
        return p;
        default:
        return p;
    }

    // OK success!

    p = finger.image2Tz();
    switch (p) {
        case FINGERPRINT_OK:
        break;
        case FINGERPRINT_IMAGEMESS:
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        return p;
        case FINGERPRINT_FEATUREFAIL:

        return p;
        case FINGERPRINT_INVALIDIMAGE:

        return p;
        default:
        return p;
    }

    // OK converted!
    p = finger.fingerSearch();
    if (p == FINGERPRINT_OK) {
        Serial.println("Found a print match!");

    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        return p;
    } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
        lcd.clear();
        lcd.print("Invalid!");
        lcd.setCursor(0, 1);
        lcd.print("Try again!");
        // Warning beep
        tone(buzz_pin, 500, 400);
        tone(buzz_pin, 1000, 800);
        delay(200);
        tone(buzz_pin, 600, 800);
        delay(200);

        return p;
    } else {
        return p;
    }

    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    tone(buzz_pin, 800, 400);
    lcd.clear();
    lcd.print("(^-^)");
    lcd.setCursor(0, 1);
    lcd.print("Welcome, Tenant!");
    return finger.fingerID;
}



// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -1;

    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);

    return finger.fingerID;
}



/** This function deletes or removes the entire fingerprint records */
void deleteAllFingerprint() {
    Serial.println("\n\nDeleting all fingerprint templates!");
    Serial.println("Number of fingerprint data to delete: " + finger.getTemplateCount());

    finger.emptyDatabase();
}