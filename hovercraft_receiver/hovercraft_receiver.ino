#include <Servo.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

// =================================================================================================
// REMEMBER TO UNCOMMENT '#define RH_ASK_ARDUINO_USE_TIMER2' on line 26 of RH_ASK.cpp in the lib
// Otherwise it will conflict with the Servo Library
// =================================================================================================

RH_ASK driver;
// RH_ASK driver(2000, 2, 4, 5); // ESP8266: do not use pin 11

// Attached Servos
Servo ThrustESC;
Servo LiftESC;
Servo RudderServo;

// Servo PWN Pins
const int THRUST_ESC_PWM_PIN = 5;
const int LIFT_ESC_PWM_PIN = 6;
const int RUDDER_SERVO_PWM_PIN = 9;

// MAX and MIN values
#define MIN_ESC_SPEED 1000
#define MAX_ESC_SPEED 2000
#define MIN_RUDDER_POS 70
#define MAX_RUDDER_POS 140

char code; // code read from RF
int value; // value read from RF

uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
uint8_t buflen;

// Speed an position with sensible defaults
int thrustSpeed = 10;
int liftSpeed = 10;
int rudderPos = 90;

// System will stop the motors if it doesn't get a signal for 0.5 seconds
// (Automatic no-signal cutout)
#define MAX_MILLIS_BEFORE_CUTOFF 500
int millisSinceLastSignal = 0;
int lastSignalMillis = 0;

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");
    if (!driver.init())
        Serial.println("init failed");
    else
        Serial.println("init success");

    ThrustESC.attach(THRUST_ESC_PWM_PIN);
    LiftESC.attach(LIFT_ESC_PWM_PIN);
    RudderServo.attach(RUDDER_SERVO_PWM_PIN);

    Serial.println("Setup Complete");
}

void loop()
{
    buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        lastSignalMillis = millis();

        // Message with a good checksum received
        thrustSpeed = buf[0]; thrustSpeed = thrustSpeed << 8; thrustSpeed += buf[1];
        liftSpeed = buf[2]; liftSpeed = liftSpeed << 8; liftSpeed += buf[3];
        rudderPos = buf[4]; rudderPos = rudderPos << 8; rudderPos += buf[5];
    }

    millisSinceLastSignal = millis() - lastSignalMillis;
    // Stop motors if we haven't received a signal
    if (millisSinceLastSignal > MAX_MILLIS_BEFORE_CUTOFF){
        thrustSpeed = MIN_ESC_SPEED;
        liftSpeed = MIN_ESC_SPEED;
    }

    // Print out what the received values are
    Serial.print(thrustSpeed);
    Serial.print(" TH    ");
    Serial.print(liftSpeed);
    Serial.print(" LI    ");
    Serial.print(rudderPos);
    Serial.print(" RD    ");
    Serial.print(millisSinceLastSignal);
    Serial.println(" millisSinceLastSignal");

    // Constrain to allowed values for the motors, and write those values
    ThrustESC.writeMicroseconds(thrustSpeed);
    LiftESC.writeMicroseconds(liftSpeed);
    RudderServo.write(rudderPos);
}
