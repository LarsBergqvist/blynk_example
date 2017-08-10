#include <Arduino.h>          // Arduino basic stuff
#include <WiFi.h>             // WiFi-library needed by Blynk
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // Include Blynk library
#include "credentials.h"      // Keep WiFi and Blynk credentials in a separate file


#define BEEP_PIN 17

//
// Called when the value of V1 is written to the Blynk server
// V1 is connected to a button. When the button is pressed, the V1=1
// and when it is released V1=0
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial.print("V1 value is: ");
  Serial.println(pinValue);
  if (pinValue == 1)
  {
    digitalWrite(BEEP_PIN, HIGH);
  }
  else
  {
    digitalWrite(BEEP_PIN, LOW);
  }
}

//
// DHT Sensor setup
//
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN_INDOOR 16
#define DHTTYPE_INDOOR DHT11
DHT dht(DHTPIN_INDOOR, DHTTYPE_INDOOR);

//
// Use a timer to trigger reading of the sensor every 5 seconds
// Write the value to V2 to send it to the Blynk server
//
BlynkTimer timer;
#define TIMER_INTERVAL 5000
void timerEvent()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (!isnan(temperature))
  {
    Serial.println(temperature);
    Blynk.virtualWrite(V2, temperature);
  }
}


void setup()
{
  Serial.begin(9600);

  pinMode(BEEP_PIN, OUTPUT);

  dht.begin();

  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,16),8442);

  timer.setInterval(TIMER_INTERVAL, timerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
