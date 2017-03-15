/*
Reading a LDR and changing the LED intensity
*/

// Pin Number constants
const int LED_1 = 3;
const int LED_2 = 5;
const int LED_3 = 6;
const int LED_4 = 9;
const int ANALOG_PIN = 0;

int ldr_value;

void
setup()
{
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);

    // Serial start for convenience
    // Serial.begin(9600);      // open the serial port at 9600 bps
    // Serial.println("STARTED");
}

void
loop()
{
    // Read in LDR Value, constrained 0 - 6
    ldr_value = constrain(analogRead(ANALOG_PIN), 0, 6);
    ldr_value = 6 - ldr_value;

    delay(100);

    // Serial.println("ldr_value");
    // Serial.println(ldr_value);

    // Set all LED's OFF before we turn on only the ones we want
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);

    // Use a switch, allowing cascading case statements, to turn on the LED's
    switch(ldr_value) {
        case 6:
        case 5:
            digitalWrite(LED_4, HIGH);
        case 4:
            digitalWrite(LED_3, HIGH);
        case 3:
            digitalWrite(LED_2, HIGH);
        case 2:
            digitalWrite(LED_1, HIGH);
    }
}
