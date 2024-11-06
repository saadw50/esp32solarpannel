#include <ESP32Servo.h>

#define LDR1 32
#define LDR2 33
#define error 10   // Error threshold for light differences (adjust as needed)
int Spoint = 90;    // Starting servo position

Servo myservo;
int ldr1Offset = 0; // Offset for LDR1 to compensate for the bias


void setup() {
    Serial.begin(115200); // For debugging

    myservo.attach(18); // Attach servo to GPIO 18 (or your chosen GPIO)
    myservo.write(Spoint);

    // Calculate LDR1 offset during setup:
    ldr1Offset = calculateLDR1Offset(); 
    Serial.print("LDR1 Offset: ");
    Serial.println(ldr1Offset);

    delay(1000); // Give servo time to initialize
}

// Function to calculate LDR1 offset
int calculateLDR1Offset() {
    int total = 0;
    const int numReadings = 10; // Take multiple readings for accuracy
    for (int i = 0; i < numReadings; i++) {
        total += analogRead(LDR1) - analogRead(LDR2);
        delay(10);
    }
    return total / numReadings;
}

void loop() {
    int ldr1Value = analogRead(LDR1) - ldr1Offset; // Subtract the offset
    int ldr2Value = analogRead(LDR2);

    Serial.print("LDR1 (corrected): ");
    Serial.print(ldr1Value);
    Serial.print("\tLDR2: ");
    Serial.println(ldr2Value);

    // Calculate the difference, ignoring the constant offset
    int diff = abs(ldr1Value - ldr2Value); 

    if (diff <= error) {
        // LDR readings are close enough, no need to move
    } else {
        if (ldr1Value > ldr2Value) {
            Spoint--;
        } else if (ldr1Value < ldr2Value) {
            Spoint++;
        }
    }

    Spoint = constrain(Spoint, 0, 180); 
    myservo.write(Spoint);  
    delay(90); // Adjust for servo speed
}
