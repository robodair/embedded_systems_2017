/*
Fading
Fade between two LED's, with speed controlled via a potentiometer
*/

// Max and Min Step size constants
const int MAX_STEP = 80;
const int MIN_STEP = 1;

// Delay Time and Pin Number constants
const int DELAY_TIME = 1;
const int RED_PIN = 9;
const int GREEN_PIN = 6;
const int ANALOG_PIN = 0;

// Control Variables
int i = MIN_STEP;
int i_step = MIN_STEP;
bool fadeDirection = true;

void
setup()
{
  // Initialise output pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Serial start for convenience
//  Serial.begin(9600);      // open the serial port at 9600 bps:   
//  Serial.println("STARTED");
}

void
loop()
{
  // Set LED Output
  analogWrite(RED_PIN, i);
  analogWrite(GREEN_PIN, 255-i);

  delay(DELAY_TIME);

  i_step = get_i_step();

  // Step i up or down depending on the fade direction
  if (fadeDirection) {
    i = constrain(i + i_step, 1, 255);
  } else {
    i = constrain(i - i_step, 1, 255);
  }

  // Change the fade direction if i is approaching the limits (0-254)
  if (i > 245 || i < 10){
    fadeDirection = !fadeDirection;
  }

}

/** Get the current desired value of i_step.
 *  Reads the potentiometer value and returns the step amount mapped to between 
 *  MIN_STEP and MAX_STEP as an int.
 */
int
get_i_step(){
  int step = map(analogRead(ANALOG_PIN), 0, 1023, MIN_STEP, MAX_STEP)
  return step;
}
