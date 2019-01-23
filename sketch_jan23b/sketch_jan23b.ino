#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


//Heart Rate Sensor
const int PulseWire = 0;
int Threshold = 550;

//LED Indicator
const int blue = 3;
const int green = 4;
const int red = 5;
const int LED13 = 13;
//GSM and GPS
static const int RXPin = 3, TXPin = 2;
static const uint32_t GPSBaud = 9600;
const int button = 4;

PulseSensorPlayground pulseSensor;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {

  Serial.begin(115200);
  ss.begin(GPSBaud);
  pinMode(button, INPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
}

void loop() {
  if (button == HIGH) {
    while (ss.available() > 0)
      if (gps.encode(ss.read()))
        displayInfo();
  }
//  if (millis() > 5000 && gps.charsProcessed() < 10)
//  {
//    Serial.println(F("No GPS detected: check wiring."));
//    while (true);
//  }
  
  int BPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.println(BPM);
    if (BPM < 80) {
      // Too low
      digitalWrite(blue, HIGH);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
    } else if (BPM < 160 && BPM > 80) {
      digitalWrite(blue, LOW);
      digitalWrite(red, LOW);
      digitalWrite(green, HIGH);
    } else if (BPM > 160) {
      digitalWrite(blue, LOW);
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);
    }
  }
  else {
//    digitalWrite(blue, LOW);
//    digitalWrite(red, LOW);
//    digitalWrite(green, LOW);
  }
  delay(100);


  
}



void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"09058382987\"\r");
    delay(1000);
    Serial.print("www.google.com.ph/maps/place/");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print("\r");
    delay(1000);
    Serial.println((char)26);
    delay(1000);
    Serial.write(0x1A);
    Serial.write(0x0D);
    Serial.write(0x0A);
    delay(1000);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}


