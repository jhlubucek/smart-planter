#include "WiFi.h"
#include "PubSubClient.h"
#include "sensors/AnalogSensor.h"
#include "DHT.h"
#include "Connection.h"

class Planter
{
private:
    DHT dht;
    AnalogSensor lightSensor;
    Connection conn;
public:
    Planter();
    void connect();
    void loopClient();
    void publish();
    float getData();
};

Planter::Planter() : 
  dht(DHTPIN, DHTTYPE), 
  lightSensor(LIGHT_SENSOR_PIN, LIGHT_SENSOR_MIN_VALUE, LIGHT_SENSOR_MAX_VALUE),
  conn(WIFI_PASSWD)
{
    //lightSensor.setValues(LIGHT_SENSOR_PIN, LIGHT_SENSOR_MIN_VALUE, LIGHT_SENSOR_MAX_VALUE);
    dht.begin();
    conn.setTopicPrefix(PLANTER_NAME);
    conn.subscribe("inTopic");
    Serial.println("subscribed");
    delay(1000);
}



void Planter::loopClient(){ 
  if (conn.connected()){
    uint32_t loopStart = millis();
    while (millis() - loopStart < 3000) 
    {
      conn.loop();
    }
  }
  Topic *topic = conn.getTopic("inTopic");
  if (topic != nullptr)
  {
    if (topic->getLength() == 1 && (char)topic->getValue()[0] == '1')
    {
      digitalWrite(BUILTIN_LED, HIGH);
      unsigned long time_now = millis();
      while((unsigned long)(millis() - time_now) < WATER_FOR_MILLIS){
        //wait
      }
      digitalWrite(BUILTIN_LED, LOW);
      conn.publish("inTopic", (float)2, true);
    }else {
      digitalWrite(BUILTIN_LED, LOW);
    }
  }else{
    Serial.println("topic not found");
  }

  Serial.println();
}


void Planter::publish(){
  conn.publish("airTemperature", dht.readTemperature());
  conn.publish("airHumidity", dht.readHumidity());
  conn.publish("light", lightSensor.getValue());
}

float Planter::getData(){
  float t = dht.readTemperature();
  return t;
}

void Planter::connect(){
  conn.connect();
}

