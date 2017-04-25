#include <Servo.h>

// Attached Servos
Servo ThrustESC;
Servo LiftESC;
Servo RudderServo;

// Servo PWN Pins
const int THRUST_ESC_PWM_PIN = 9;
const int LIFT_ESC_PWM_PIN = 10;
const int RUDDER_SERVO_PWM_PIN = 3;

// Button
int BUTTON_FORCE_LOW_PIN = 2;

// Speed (Value of PWM Output to ESC)
// The ESC's move with values from ~1000-2000,
// but require a value of ~10 to be present when turned on initially
// to run through their startup sequence
int thrustSpeed = 10;
int liftSpeed = 10;
#define MIN_ESC_SPEED 1000
#define MAX_ESC_SPEED 2000

// Rudder pos (Degrees)
int rudderPos = 90;
#define MIN_RUDDER_POS 70
#define MAX_RUDDER_POS 140

// Potentiometers (Analog Pins)
#define THRUST_KNOB A0
#define LIFT_KNOB A1
#define RUDDER_KNOB A2

#define RF_TRANSMIT_PIN 4
#define LED_PIN 13


void
setup()
{
    Serial.begin(19200);
    Serial.println("Setup");

    ThrustESC.attach(THRUST_ESC_PWM_PIN);
    LiftESC.attach(LIFT_ESC_PWM_PIN);
    RudderServo.attach(RUDDER_SERVO_PWM_PIN);
    pinMode(BUTTON_FORCE_LOW_PIN, INPUT_PULLUP);

    pinMode(RF_TRANSMIT_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Setup Complete");
}


void
loop()
{
    liftSpeed = map(analogRead(LIFT_KNOB), 0, 1023, MIN_ESC_SPEED, MAX_ESC_SPEED);
    thrustSpeed = map(analogRead(THRUST_KNOB), 0, 1023, MIN_ESC_SPEED, MAX_ESC_SPEED);
    rudderPos = map(analogRead(RUDDER_KNOB), 0, 1023, MIN_RUDDER_POS, MAX_RUDDER_POS);

    if (digitalRead(BUTTON_FORCE_LOW_PIN) == LOW){
        liftSpeed = 10;
        thrustSpeed = 10;
        rudderPos = 90;
    }

    Serial.print(thrustSpeed);
    Serial.print(" TH    ");
    Serial.print(liftSpeed);
    Serial.print(" LI    ");
    Serial.print(rudderPos);
    Serial.println(" RD");

    ThrustESC.writeMicroseconds(thrustSpeed);
    LiftESC.writeMicroseconds(liftSpeed);
    RudderServo.write(rudderPos);

    digitalWrite(RF_TRANSMIT_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("HIGH");
    delay(3000);

    digitalWrite(RF_TRANSMIT_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("LOW");
    delay(3000);
}
