#include <RH_ASK.h> // RadioHead Library
#include <SPI.h>    // Not actually used but needed to compile
RH_ASK driver;

// Button
int BUTTON_FORCE_LOW_PIN = 2;

// Speed (Value of PWM Output to ESC)
// The ESC's move with values from ~1000-2000,
// but require a value of ~10 to be present when turned on initially
// to run through their startup sequence
unsigned char thrustSpeed = 10;
unsigned char liftSpeed = 10;

// Rudder pos (Degrees)
unsigned char rudderPos = 90;

// Potentiometers (Analog Pins)
#define THRUST_KNOB A1
#define LIFT_KNOB A0
#define RUDDER_KNOB A2

uint8_t message[] = {0, 0, 0};

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
    liftSpeed = map(analogRead(LIFT_KNOB), 0, 1023, 0, 255);
    // constrain 532-0 so that only the forward push on the joystick is counted
    // map 532-0 to MIN-MAX to invert (so forward push becomes power UP)
    // The joystick does have some deadzone
    thrustSpeed = map(constrain(analogRead(THRUST_KNOB), 0, 532), 532, 0, 0, 255);
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

    message[0] = thrustSpeed;
    message[2] = liftSpeed;
    message[3] = rudderPos;

    driver.send(message, 3);
    driver.waitPacketSent();
}
