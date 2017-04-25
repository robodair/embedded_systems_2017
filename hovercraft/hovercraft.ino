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

uint8_t message[] = {0, 0, 0, 0, 0, 0};

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
    // constrain 532-0 so that only the forward push on the joystick is counted
    // map 532-0 to MIN-MAX to invert (so forward push becomes power UP)
    // The joystick does have some deadzone
    thrustSpeed = map(constrain(analogRead(THRUST_KNOB), 0, 532),
                      532, 0, MIN_ESC_SPEED, MAX_ESC_SPEED);
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
    message[0] = (thrustSpeed >> 8);
    message[1] = thrustSpeed;
    message[2] = (liftSpeed >> 8);
    message[3] = liftSpeed;
    message[4] = (rudderPos >> 8);
    message[5] = rudderPos;

    driver.send(message, 6);
    driver.waitPacketSent();
}
