void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Begin Serial communication at 9600 baud rate
    Serial.begin(9600);
    Serial.println("Hello! I am COM3 Port");
}

void loop() {

    // Receive byte data from Serial connection
    byte count; 
    if (Serial.available()) {
        // Read the bytes data from Serial connection
        count = Serial.read(); 

        /**
         * The in-built LED will blink dependending on the count received
         * 
         */
        for (byte i = 1; i <= count; i++) {
            digitalWrite(LED_BUILTIN, HIGH);        // ON   
            delay(100);                             
            digitalWrite(LED_BUILTIN, LOW);         // OFF
            delay(100);                       
        }
    }
}