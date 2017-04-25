#include <RH_ASK.h> // RadioHead Library
#include <SPI.h>    // Not actually used but needed to compile
RH_ASK driver;

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
#define THRUST_KNOB A1
#define LIFT_KNOB A0
#define RUDDER_KNOB A2

uint8_t thrustMessage[] = {'T', 0, 0};
uint8_t liftMessage[] = {'L', 0, 0};
uint8_t rudderMessage[] = {'R', 0, 0};

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");
    pinMode(BUTTON_FORCE_LOW_PIN, INPUT_PULLUP);

    if (!driver.init())
        Serial.println("RF init failed");
    else
        Serial.println("RF init success");

    Serial.println("Setup Complete");
}

void loop()
{
    liftSpeed = map(analogRead(LIFT_KNOB), 0, 1023, MIN_ESC_SPEED, MAX_ESC_SPEED);
    thrustSpeed = map(analogRead(THRUST_KNOB), 0, 1023, MIN_ESC_SPEED, MAX_ESC_SPEED);
    rudderPos = map(analogRead(RUDDER_KNOB), 0, 1023, MIN_RUDDER_POS, MAX_RUDDER_POS);

    if (digitalRead(BUTTON_FORCE_LOW_PIN) == LOW)
    {
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

    // Bit shift the ints into the byte array for sending
    thrustMessage[1] = (thrustSpeed >> 8);
    thrustMessage[2] = thrustSpeed;

    liftMessage[1] = (liftSpeed >> 8);
    liftMessage[2] = liftSpeed;

    rudderMessage[1] = (rudderPos >> 8);
    rudderMessage[2] = rudderPos;

    driver.send(thrustMessage, 4);
    driver.waitPacketSent();
    driver.send(liftMessage, 4);
    driver.waitPacketSent();
    driver.send(rudderMessage, 4);
    driver.waitPacketSent();
}
