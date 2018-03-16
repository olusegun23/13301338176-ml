#include <SoftwareSerial.h>

#include <SoftwareSerial.h>

/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */


// #define echoPin A1 // Echo Pin
// #define trigPin A0 // Trigger Pin

#define echoPin4 10 // Echo Pin
#define trigPin4 9 // Trigger Pin

#define echoPin3 8 // Echo Pin
#define trigPin3 7 // Trigger Pin

#define echoPin2 6 // Echo Pin
#define trigPin2 5 // Trigger Pin

#define echoPin1 4 // Echo Pin
#define trigPin1 3 // Trigger Pin

#define LEDPin 13 // Onboard LED

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration1, distance1,duration2, distance2,duration3, distance3,duration4, distance4 ; // Duration used to calculate distance

void setup() {
 Serial.begin (9600);
 pinMode(trigPin1, OUTPUT);
 pinMode(echoPin1, INPUT);

 pinMode(trigPin2, OUTPUT);
 pinMode(echoPin2, INPUT);

 pinMode(trigPin3, OUTPUT);
 pinMode(echoPin3, INPUT);

 pinMode(trigPin4, OUTPUT);
 pinMode(echoPin4, INPUT);


 
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}

void loop() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin1, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance1 = duration1/58.2;
 
 // if (distance >= maximumRange || distance <= minimumRange){
 
 if (0) {
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 // digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
// Serial.println(distance1);
 // digitalWrite(LEDPin, LOW); 
 }
 
 //Delay 50ms before next reading.
 delay(50);


 digitalWrite(trigPin2, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 //Calculate the distance (in cm) based on the speed of sound.
 distance2 = duration2/58.2;
 // if (distance >= maximumRange || distance <= minimumRange){
 if (0) {
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 // digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 //Serial.println(distance2);
 // digitalWrite(LEDPin, LOW); 
 }
 //Delay 50ms before next reading.
 delay(50);



digitalWrite(trigPin3, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin3, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin3, LOW);
 duration3 = pulseIn(echoPin3, HIGH);
 //Calculate the distance (in cm) based on the speed of sound.
 distance3 = duration3/58.2;
 // if (distance >= maximumRange || distance <= minimumRange){
 if (0) {
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 // digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
// Serial.println(distance3);
 // digitalWrite(LEDPin, LOW); 
 }
 //Delay 50ms before next reading.
 delay(50);



digitalWrite(trigPin4, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin4, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin4, LOW);
 duration4 = pulseIn(echoPin4, HIGH);
 //Calculate the distance (in cm) based on the speed of sound.
 distance4 = duration4/58.2;
 // if (distance >= maximumRange || distance <= minimumRange){
 if (0) {
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 // digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.println(distance4);
 // digitalWrite(LEDPin, LOW); 
 }
 //Delay 50ms before next reading.
 delay(50);



 
}
