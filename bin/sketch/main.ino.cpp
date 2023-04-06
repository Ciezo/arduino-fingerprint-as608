#include <Arduino.h>
#line 1 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
/*
    @author Cloyd Van S. Secuya
    @details Source code for setting up fingerprint AS608 sensor with Arduino 
*/
// PRE-PROCESSOR DIRECTIVES / HEADERS
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <String.h> 

// AS608 ports
SoftwareSerial mySerial(2, 3);  // TX, RX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;   // Fingerprint id
uint8_t opt; 

#line 16 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
void setup();
#line 50 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
uint8_t scan_input(void);
#line 61 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
uint8_t getFingerprintMethod(void);
#line 71 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
void loop();
#line 93 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
uint8_t getFingerprintEnroll();
#line 252 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
uint8_t getFingerprintID();
#line 324 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
int getFingerprintIDez();
#line 16 "c:\\Users\\abcd1\\Desktop\\Project\\arduino\\fingerprint\\src\\core\\arduino\\main.ino"
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
        Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    }
    else {
        Serial.println("Waiting for valid finger...");
        Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    }

    // Prompt the user to choose between enrolling and verifying fingerprints
    Serial.println("Please, choose a task");
    Serial.println("[1] Enroll a fingerprint");
    Serial.println("[2] Verify fingerprint");
    opt = getFingerprintMethod();
}

/** A method to read input as INT */
uint8_t scan_input(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

/** A method to choose between enrollng and verifying fingerprints */
uint8_t getFingerprintMethod(void) {
  uint8_t opt = 0;

  while (opt == 0) {
    while (! Serial.available());
    opt = Serial.parseInt();
  }
  return opt;
}

void loop() {
    switch (opt) {
        case 0: 
            delay(0);
            setup();
        case 1: 
            getFingerprintEnroll();
            delay(1000);
            break;
        case 2: 
            getFingerprintID();
            delay(1000);
            break; 
    }


    // while (!  getFingerprintEnroll() );
}



/** This function enrolls or saves fingerprint data into the sensor */
uint8_t getFingerprintEnroll() {
    // Prompt to Serial Monitor
    Serial.println("Loading data for Enrollment");
    // Prompt the user to enroll their fingerprint
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = scan_input();
    if (id == 0) {
        Serial.println("ID #0 is invalid!");
        Serial.println("Please, try again!");
        return;
    }

    // Begin fetching the assigned ID# 
    Serial.print("Enrolling ID #");
    Serial.println(id);

    int p = -1;
    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
        case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
        case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
        default:
        Serial.println("Unknown error");
        break;
        }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
        case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
        case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
        case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
        default:
        Serial.println("Unknown error");
        return p;
    }

    Serial.println("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
        p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
        case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
        case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
        default:
        Serial.println("Unknown error");
        break;
        }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
        case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
        case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
        case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
        default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");  Serial.println(id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
        Serial.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
        Serial.println("Fingerprints did not match");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }

    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
        Serial.println("Stored!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
        Serial.println("Could not store in that location");
        return p;
    } else if (p == FINGERPRINT_FLASHERR) {
        Serial.println("Error writing to flash");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }

    return true;
}



/** This function verifies a fingerprint data */
uint8_t getFingerprintID() {
    // Prompt on Serial Monitor
    Serial.println("Loading data for Verification");

    uint8_t p = finger.getImage();
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
        case FINGERPRINT_NOFINGER:
        Serial.println("No finger detected");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
        case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        return p;
        default:
        Serial.println("Unknown error");
        return p;
    }

    // OK success!

    p = finger.image2Tz();
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
        case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
        case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
        case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
        default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    p = finger.fingerSearch();
    if (p == FINGERPRINT_OK) {
        Serial.println("Found a print match!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }

    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    
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
