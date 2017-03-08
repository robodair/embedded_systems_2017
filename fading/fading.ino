/*
Blink
Turns on an LED on for one second, then off for one second, repeatedly.
*/

int DELAY_TIME = 1;
int RED_PIN = 9;
int GREEN_PIN = 6;
int i = 0;
int i_step = 0;
// the setup function runs once when you press reset or power the board 
void setup()
{
  // initialize digital pins as output.
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
//  Serial.begin(9600);      // open the serial port at 9600 bps:   
//  Serial.println("STARTED");
}

// the loop function runs over and over again forever
void loop()
{
//  Serial.println("LOOP");
  while (i < 245){
    i_step = get_i_step();
    
    analogWrite(RED_PIN, i);
    analogWrite(GREEN_PIN, 255-i);
    
    delay(DELAY_TIME);
    
//    Serial.println("old i: ");
//    Serial.println(i);
    i += i_step;
    i = constrain(i, 1, 255);
//    Serial.println("new i:");
//    Serial.println(i);
    
  }
  while (i > 10){
    
    i_step = get_i_step();
    
    analogWrite(RED_PIN, i);
    analogWrite(GREEN_PIN, 255-i);
    
    delay(DELAY_TIME);
    
//    Serial.println("old i:");
//    Serial.println(i);
    i -= i_step;
    i = constrain(i, 1, 255);
//    Serial.println("new i:");
//    Serial.println(i);
  }
}

int get_i_step(){
  int sensorValue = analogRead(A0);
  int i_step = constrain(sensorValue / 64, 1, 80);
//  Serial.println(i_step);
  return i_step;
}

