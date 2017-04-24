//This code can be used for any purpose.

#include <Servo.h>

Servo ThrustESC;

// Button
int BUTTON_PIN = 2;
bool buttonPressed = false;

// Speed
int speed = 10;

// Potentiometer
const int ANALOG_PIN = A0;

/*
 * Recieve the interrupt, set the flag, and start the timer
 */
void
buttonInterrupt()
{
    Serial.println("Button Interrupt");
    buttonPressed = true;
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
}

void
setSpeed(int speed){
    speed = constrain(speed, 1000, 2000);
    Serial.println(speed);
    ThrustESC.writeMicroseconds(speed);
}

void
setup()
{

    // attach the ESC
    ThrustESC.attach(9);

    // Attach the button interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, LOW);

    Serial.begin(19200);
    Serial.println("Begin");

}

void
loop()
{
    speed = map(analogRead(ANALOG_PIN), 0, 1023, 1000, 2000);

    if (buttonPressed){
        buttonPressed = false;
        speed = 1000;
    }

    setSpeed(speed);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, LOW);
}
