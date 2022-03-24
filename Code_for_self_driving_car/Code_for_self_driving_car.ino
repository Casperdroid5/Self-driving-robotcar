#include <NewPing.h> // ultrasonic sensor library
#include <AFMotor.h> // motorshield library

#define ledPin1      A1  // led1 pindeclaration
#define ledPin2      A2  // led2 pindeclaration
#define buzzerPin    9   // buzzerPin pindeclaration
#define buttonPin    A0  // buttonPin pindeclaration
#define TRIGGER_PIN  A4  // TriggerPin pindeclaration
#define ECHO_PIN     A5  // EchoPin pindeclaration
#define MAX_DISTANCE 200 // Maximumdistance the ultrasonic sensor senses 


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance

AF_DCMotor motor1(1); // motorterminal for motor 1
AF_DCMotor motor2(4); // motorterminal for motor 2

bool buttonState = false;         // button is not pressed by default change to true to test the motors without a button
unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 30;         // interval at which to blink (milliseconds)
int distance = 0;

void setup()   // this code is only run once
{
  Serial.begin (9600);           // start of serial connection at a 9600 Baudrate
  pinMode(buttonPin, INPUT);     // set the buttonPin as input
  pinMode(buzzerPin, OUTPUT);    // set the buzzerPin as output
  pinMode(ledPin1, OUTPUT);      // set the ledPin1 as output
  pinMode(ledPin2, OUTPUT);      // set the ledPin1 as output

  digitalWrite(ledPin1, LOW);    // led1 disabled by default
  digitalWrite(ledPin2, LOW);    // led1 disabled by default

  motor1.setSpeed(150);          // motor1 speed set
  motor1.run(RELEASE);           // motor1 stopped
  motor2.setSpeed (150);         // motor2 speed set
  motor2.run(RELEASE);           // motor2 stopped

  Serial.println ("Hello world"); // Hello Word! Just to show the user that the system booted
}

void loop()    // this code is run in a loop
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) // if the specified time has passed
  {
    distance = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    previousMillis = currentMillis;
  }

  if (digitalRead(buttonPin) == true) // read the button state and see if it is pressed
  {
    (buzzerPin, 800, 5);         // give a tone on the buzzerPin, at x hz for, x ms
    buttonState = !buttonState; // toggle the button state
    delay(150);
    Serial.println ("button was pressed");
    Serial.println (buttonState); // print button state to terminal
  }

  if (buttonState == true)  // if buttonState is true than do the following code:
  {

    if ((distance > 0) && (distance < 25))              // if the distance is between 1 and 25 cm then:
    {
      digitalWrite(ledPin2, HIGH);  // turn led 2 on
      digitalWrite(ledPin1, LOW);    // turn led 1 off
      motor1.run(BACKWARD);         // drive motor 1 backward
      motor2.run(FORWARD);          // drive motor 2 forward
      //Serial.println ("ROTATING");
    }
    else // drive forward!
    {
      motor1.run(FORWARD);  // drive motor 1 forward
      motor2.run(FORWARD);  // drive motor 2 forward
      digitalWrite(ledPin1, HIGH);    // turn led 1 on
      digitalWrite(ledPin2, LOW);     // turn led 2 off
      //Serial.println ("GOING FORWARD");
    }
  }

  else // if buttonState is not true
  {
    //Serial.println ("not pressed");
    motor1.run(RELEASE);            // motor1 stopped
    motor2.run(RELEASE);            // motor2 stopped
  }
}
