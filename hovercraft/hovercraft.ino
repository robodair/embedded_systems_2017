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
const int MIN_ESC_SPEED = 1000;
const int MAX_ESC_SPEED = 2000;

// Rudder pos (Degrees)
int rudderPos = 90;
const int MIN_RUDDER_POS = 70;
const int MAX_RUDDER_POS = 140;

// Potentiometers (Analog Pins)
const int THRUST_KNOB = A0;
const int LIFT_KNOB = A1;
const int RUDDER_KNOB = A2;


void
setup()
{
    Serial.begin(19200);
    Serial.println("Setup");

    ThrustESC.attach(THRUST_ESC_PWM_PIN);
    LiftESC.attach(LIFT_ESC_PWM_PIN);
    RudderServo.attach(RUDDER_SERVO_PWM_PIN);
    pinMode(BUTTON_FORCE_LOW_PIN, INPUT_PULLUP);

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

    Serial.print(liftSpeed);
    Serial.print("    ");
    Serial.print(thrustSpeed);
    Serial.print("    ");
    Serial.print(buttonPressed);
    Serial.println("    ");
    Serial.println(rudderPos);

    ThrustESC.writeMicroseconds(thrustSpeed);
    LiftESC.writeMicroseconds(liftSpeed);
    RudderServo.write(rudderPos);
}
