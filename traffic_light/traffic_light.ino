/*
Traffic Light
Controlling 5 LED's with a push button as input
*/

volatile bool walkButtonPressed = false;

unsigned long currentMillis;

unsigned int interval = 5000;   // 5 seconds
unsigned long prevMillis;       // Global to store button state

unsigned int flashInterval = 250;
unsigned long flashPrevMillis; // Store the flash interval
bool flashOn = true;

// LED states
int LED_TRAFFIC_RED = 12;
int LED_TRAFFIC_YELLOW = 11;
int LED_TRAFFIC_GREEN = 10;

int LED_WALK_RED = 7;
int LED_WALK_GREEN = 6;

// Button
int BUTTON_PIN = 2;

// States
typedef enum {WALK_GO, WALK_FLASH, WALK_STOP, TRAFFIC_GO, TRAFFIC_SLOW, TRAFFIC_STOP} state;
state lights = TRAFFIC_GO;

/*
 * Recieve the interrupt, set the flag, and start the timer
 */
void
buttonInterrupt()
{
    // Detach this interrupt
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));

    Serial.println("Interrupt");
    // Set up the timer
    walkButtonPressed = true;
    prevMillis = millis();
    interval = 5000; // 5 seconds
}

/*
 * Tick the timer and do the crossing routine if interval is reached
 */
void
timerTick()
{
    // Get snapshot of time
    currentMillis = millis();

    if (((unsigned long) currentMillis - prevMillis) >= interval){

        // Set up a walk button timer if one has been requested by a button press
        if (walkButtonPressed && lights == TRAFFIC_GO){
            Serial.println("Walk pressed, in timer");
            walkButtonPressed = false;
            // Timer reached, and the walking button has been pressed, set the traffic to SLOW
            // The switch statement will take care of the rest
            lights = TRAFFIC_SLOW;
            Serial.println("Switch to TRAFFIC_SLOW");
            prevMillis = millis();
            interval = 1500; // 1 secs
        } else {
            // Set the next state based on the previous state
            // And for how long that state should remain the active state
            switch (lights){

                case WALK_GO:
                    lights = WALK_FLASH;
                    Serial.println("Switch to WALK_FLASH");
                    prevMillis = millis();
                    interval = 3000;
                    break;
                case WALK_FLASH:
                    lights = WALK_STOP;
                    Serial.println("Switch to WALK_STOP");
                    prevMillis = millis();
                    interval = 2000;
                    break;
                case WALK_STOP:
                    lights = TRAFFIC_GO;
                    Serial.println("Switch to TRAFFIC_GO");
                    //interval = 2000; // No interval for TRAFFIC_GO because it doesn't time out
                    prevMillis = millis();
                    break;
                case TRAFFIC_STOP:
                    lights = WALK_GO;
                    Serial.println("Switch to WALK_GO");
                    interval = 5000;
                    prevMillis = millis();
                    break;
                case TRAFFIC_SLOW:
                    lights = TRAFFIC_STOP;
                    Serial.println("Switch to TRAFFIC_STOP");
                    interval = 2000;
                    prevMillis = millis();
                    break;
                default:
                    // No change, TRAFFIC_GO remains until the button is pressed
                    break;
            }
        }
    }
}

void
setup()
{
    // Serial start for convenience
    Serial.begin(19200);
    Serial.println("STARTED");

    // Setup the LED pins
    pinMode(LED_TRAFFIC_RED, OUTPUT);
    pinMode(LED_TRAFFIC_YELLOW, OUTPUT);
    pinMode(LED_TRAFFIC_GREEN, OUTPUT);

    pinMode(LED_WALK_RED, OUTPUT);
    pinMode(LED_WALK_GREEN, OUTPUT);

    // Attach the button interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, FALLING);
}

void
loop()
{
    timerTick();

    // Set up the lights based on the state enum
    switch (lights){

        case WALK_GO:
            digitalWrite(LED_TRAFFIC_RED, HIGH);
            digitalWrite(LED_TRAFFIC_YELLOW, LOW);
            digitalWrite(LED_TRAFFIC_GREEN, LOW);
            digitalWrite(LED_WALK_RED, LOW);
            digitalWrite(LED_WALK_GREEN, HIGH);
            break;
        case WALK_FLASH:
            digitalWrite(LED_TRAFFIC_RED, HIGH);
            digitalWrite(LED_TRAFFIC_YELLOW, LOW);
            digitalWrite(LED_TRAFFIC_GREEN, LOW);
            digitalWrite(LED_WALK_GREEN, LOW);

            if (((unsigned long) currentMillis - flashPrevMillis) >= flashInterval){
                flashOn = !flashOn;
                // Decide whether walk red LED should be high or low
                flashPrevMillis = millis();
            }
            if (flashOn){
                Serial.println("FLASH ON");
                digitalWrite(LED_WALK_RED, HIGH);
            } else {
                Serial.println("FLASH OFF");
                digitalWrite(LED_WALK_RED, LOW);
            }
            break;
        case WALK_STOP:
        case TRAFFIC_STOP:
            digitalWrite(LED_TRAFFIC_RED, HIGH);
            digitalWrite(LED_TRAFFIC_YELLOW, LOW);
            digitalWrite(LED_TRAFFIC_GREEN, LOW);
            digitalWrite(LED_WALK_RED, HIGH);
            digitalWrite(LED_WALK_GREEN, LOW);
            break;
        case TRAFFIC_SLOW:
            digitalWrite(LED_TRAFFIC_RED, LOW);
            digitalWrite(LED_TRAFFIC_YELLOW, HIGH);
            digitalWrite(LED_TRAFFIC_GREEN, LOW);
            digitalWrite(LED_WALK_RED, HIGH);
            digitalWrite(LED_WALK_GREEN, LOW);
            break;
        case TRAFFIC_GO:
            // Attach the interrupt
            attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, FALLING);
            digitalWrite(LED_TRAFFIC_RED, LOW);
            digitalWrite(LED_TRAFFIC_YELLOW, LOW);
            digitalWrite(LED_TRAFFIC_GREEN, HIGH);
            digitalWrite(LED_WALK_RED, HIGH);
            digitalWrite(LED_WALK_GREEN, LOW);
            break;
        default:
            // Shouldn't ever get here
            Serial.println("What the hell did you do?");
            break;
    }

}
