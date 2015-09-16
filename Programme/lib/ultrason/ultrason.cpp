#include "ultrason.h"

long capteurDistance(int capteur) {
    // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(capteur, OUTPUT);
  digitalWrite(capteur, LOW);
  delayMicroseconds(2);
  digitalWrite(capteur, HIGH);
  delayMicroseconds(5);
  digitalWrite(capteur, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(capteur, INPUT);
  duration = pulseIn(capteur, HIGH);

  // convert the time into a distance
  if ( duration > 0 ) {
     inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
  }
  else {
    inches = -1;
    cm = -1;
  }
 
 
  delayMicroseconds(200);

  return cm;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

// en theorie timeout 18.5ms ici 20ms devrait être bon mais on peut gagner 1.5ms par boucle => 1.5*120 = 180ms de gagner 
unsigned long pulseIn(uint8_t pin, uint8_t state) {

    unsigned long timeout = millis();

    // While the pin is *not* in the target state we make sure the timeout hasn't been reached.
    while ((digitalRead(pin)) != state) {
        if ( (millis()-timeout) > 20 ) {
            return -1;
        }
    }

    timeout = millis();
    unsigned long time = micros();

    // When the pin *is* in the target state we bump the counter while still keeping track of the timeout.
    while ((digitalRead(pin)) == state) {
        if ( (millis() - timeout) > 20 ) {
            return -1;
        }
    }

    // Return the pulse time in microsecond!
    return (micros() - time);
}