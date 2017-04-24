//This code can be used for any purpose.

#include <Servo.h>

int signal = 10;

const int ANALOG_PIN = A0;

void setup()
{
    Serial.begin(19200);
    pinMode(9, OUTPUT);
    analogWrite(9, signal);
    Serial.println("SET LOW");
}

void loop()
{
    signal = map(analogRead(ANALOG_PIN), 0, 1023, 140, 245);
    Serial.println(signal);
    // if (Serial.available() > 0)
    // {
    //     signal = Serial.parseInt();
    //     serialFlush();
    //     signal = constrain(signal, 10, 245);
    //     printOut(signal);
    // }
    analogWrite(9, signal);

}

void serialFlush()
{
    while (Serial.available() > 0)
    {
        Serial.readString();
    }
}

void printOut(int speed){
    Serial.print("Speed: ");
    Serial.println(speed);
}