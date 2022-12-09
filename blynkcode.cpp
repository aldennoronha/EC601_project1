#include <Wire.h>
#include "MAX30100_PulseOximeter.h" 
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Wire.h"


#define REPORTING_PERIOD_MS 1200


char auth[] = "9KJVe0BlbZKsR6lxQs3Buqj9sKUR02yW";
// You should get Auth Token in the Blynk App.
char ssid[] = "IOT";		 // Your WiFi credentials. 
char pass[] = "MYPASSWORD";

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0 
PulseOximeter pox;

float BPM, SpO2; 
uint32_t tsLastReport = 0;

const int PulseSensorPurplePin = A0; // Pulse Sensor PURPLE WIRE connected to ANALOG
PIN 0
const int buttonPin = 12;

// variables will change:
int buttonState = 0; 		// variable for reading the pushbutton status
int Signal; 			// holds the incoming raw data. Signal value can range from 0-1024
void onBeatDetected() 
{
Serial.println("Beat Detected!"); 
}
void setup() 
{
   Serial.begin(115200);
pinMode(16, OUTPUT); 
pinMode(buttonPin, INPUT); 
Blynk.begin(auth, ssid, pass);

Serial.print("Initializing Pulse Oximeter..");
if (!pox.begin())
 {
Serial.println("FAILED");
for(;;); 
}
else
{
Serial.println("SUCCESS");
pox.setOnBeatDetectedCallback(onBeatDetected); 
}
// The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
//pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); 
}

void loop() 
{
buttonState = digitalRead(buttonPin);

// check if the pushbutton is pressed. If it is, the buttonState is HIGH: 
if (buttonState == HIGH) {
// turn LED on: 
pox.update(); 
Blynk.run();
BPM = pox.getHeartRate();
SpO2 = pox.getSpO2(); 
//Blynk.virtualWrite(V7, BPM); 
//Blynk.virtualWrite(V8, SpO2);

if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
{
Serial.print("Heart rate:"); 
Serial.print(BPM); 
Serial.print(" bpm / SpO2:"); 
Serial.print(SpO2); 
Serial.println(" %");

Blynk.virtualWrite(V7, BPM); 
Blynk.virtualWrite(V8, SpO2);

tsLastReport = millis(); 
}
}
else 
{
// turn LED off:
Signal = analogRead(PulseSensorPurplePin); // Read the PulseSensor's value. 
Serial.println(Signal); // Send the Signal value to Serial Plotter. 
Blynk.virtualWrite(V0, Signal);
} 

}
